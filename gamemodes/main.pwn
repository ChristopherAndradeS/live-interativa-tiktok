#include <open.mp>
#include <sscanf2>
#include <requests>

#define LIVE_API_ENDPOINT           "http://127.0.0.1:3000"
#define LIVE_API_EVENTS_PATH        "/events"
#define LIVE_API_POLL_INTERVAL_MS   (1000)

#define LIVE_MAX_ID_LEN             (64)
#define LIVE_MAX_NAME_LEN           (64)
#define LIVE_MAX_USERID_LEN         (32)
#define LIVE_MAX_MSG_LEN            (144)
#define LIVE_MAX_GIFT_NAME_LEN      (64)
#define LIVE_MAX_TYPE_LEN           (24)


new const gVehicleNames[][] = 
{
    "Landstalker", "Bravura", "Buffalo", "Linerunner", "Perrenial", "Sentinel", "Dumper", "Firetruck", "Trashmaster",
    "Stretch", "Manana", "Infernus", "Voodoo", "Pony", "Mule", "Cheetah", "Ambulance", "Leviathan", "Moonbeam",
    "Esperanto", "Taxi", "Washington", "Bobcat", "Whoopee", "BF Injection", "Hunter", "Premier", "Enforcer",
    "Securicar", "Banshee", "Predator", "Bus", "Rhino", "Barracks", "Hotknife", "Trailer", "Previon", "Coach",
    "Cabbie", "Stallion", "Rumpo", "RC Bandit", "Romero", "Packer", "Monster", "Admiral", "Squalo", "Seasparrow",
    "Pizzaboy", "Tram", "Trailer", "Turismo", "Speeder", "Reefer", "Tropic", "Flatbed", "Yankee", "Caddy", "Solair",
    "Berkley's RC Van", "Skimmer", "PCJ-600", "Faggio", "Freeway", "RC Baron", "RC Raider", "Glendale", "Oceanic",
    "Sanchez", "Sparrow", "Patriot", "Quad", "Coastguard", "Dinghy", "Hermes", "Sabre", "Rustler", "ZR-350", "Walton",
    "Regina", "Comet", "BMX", "Burrito", "Camper", "Marquis", "Baggage", "Dozer", "Maverick", "News Chopper", "Rancher",
    "FBI Rancher", "Virgo", "Greenwood", "Jetmax", "Hotring", "Sandking", "Blista Compact", "Police Maverick",
    "Boxville", "Benson", "Mesa", "RC Goblin", "Hotring Racer A", "Hotring Racer B", "Bloodring Banger", "Rancher",
    "Super GT", "Elegant", "Journey", "Bike", "Mountain Bike", "Beagle", "Cropduster", "Stunt", "Tanker", "Roadtrain",
    "Nebula", "Majestic", "Buccaneer", "Shamal", "Hydra", "FCR-900", "NRG-500", "HPV1000", "Cement Truck", "Tow Truck",
    "Fortune", "Cadrona", "SWAT Truck", "Willard", "Forklift", "Tractor", "Combine", "Feltzer", "Remington", "Slamvan",
    "Blade", "Streak", "Freight", "Vortex", "Vincent", "Bullet", "Clover", "Sadler", "Firetruck", "Hustler", "Intruder",
    "Primo", "Cargobob", "Tampa", "Sunrise", "Merit", "Utility", "Nevada", "Yosemite", "Windsor", "Monster", "Monster",
    "Uranus", "Jester", "Sultan", "Stratum", "Elegy", "Raindance", "RC Tiger", "Flash", "Tahoma", "Savanna", "Bandito",
    "Freight Flat", "Streak Carriage", "Kart", "Mower", "Dune", "Sweeper", "Broadway", "Tornado", "AT-400", "DFT-30",
    "Huntley", "Stafford", "BF-400", "News Van", "Tug", "Trailer", "Emperor", "Wayfarer", "Euros", "Hotdog", "Club",
    "Freight Box", "Trailer", "Andromada", "Dodo", "RC Cam", "Launch", "LSPD Car", "SFPD Car", "LVPD Car",
    "Police Rancher", "Picador", "S.W.A.T", "Alpha", "Phoenix", "Glendale", "Sadler", "Luggage", "Luggage", "Stairs",
    "Boxville", "Tiller", "Utility Trailer"
}; 


forward FetchLiveEvents();
forward OnLiveEventsResponse(Request:req, E_HTTP_STATUS:status, Node:node);

forward OnLiveCommandReceive(const uid[], const nick[], const userid[], bool:is_moder, bool:is_newgifter, bool:is_sub, const cmdtext[]);
forward OnLiveMessageReceive(const uid[], const nick[], const userid[], bool:is_moder, bool:is_newgifter, bool:is_sub, const message[]);
forward OnLiveGiftReceive(const uid[], const nick[], const gift_name[], repeat_count, diamond_count);
forward OnLiveGiftEndReceive(const uid[], const nick[], const gift_name[], repeat_count, diamond_count);
forward OnLiveLikeReceive(const uid[], const nick[], like_count, total_likecount);
forward OnLiveFollowReceive(const uid[], const nick[]);
forward OnLiveMemberJoin(const uid[], const nick[], userid[]);
forward OnLiveRoomInfo(viewer_count, total_likecount);
forward OnLiveStreamEnd();
forward OnLiveSubscribe(const uid[], const nick[]);
forward OnUserCommandText(const uid[], const nick[], const cmd[], const params[]);

new RequestsClient:gLiveClient = RequestsClient:-1;
new gLiveTotalDiamonds = 0;

enum E_LIVE_GIFT_DIAMONDS
{
    GIFT_NAME[LIVE_MAX_GIFT_NAME_LEN],
    GIFT_DIAMONDS
}

new const gLiveGiftDiamondTable[][E_LIVE_GIFT_DIAMONDS] =
{
    {"Rose", 1},
    {"TikTok", 1},
    {"Finger Heart", 5},
    {"Perfume", 20},
    {"Doughnut", 30},
    {"Money Gun", 500},
    {"Galaxy", 1000},
    {"Lion", 29999},
    {"Universe", 34999}
};

main() {}

public OnGameModeInit()
{
    gLiveClient = RequestsClient(LIVE_API_ENDPOINT);

    if(!IsValidRequestsClient(gLiveClient))
    {
        printf("[ TIKTOK ] Falha ao criar RequestsClient para %s. Client ID: %d", 
        LIVE_API_ENDPOINT, _:gLiveClient);
        return 1;
    }

    printf("[ TIKTOK ] RequestsClient conectado em %s (id = %d)", 
    LIVE_API_ENDPOINT, _:gLiveClient);

    SetTimer("FetchLiveEvents", LIVE_API_POLL_INTERVAL_MS, true);
    
    print("[ TIKTOK ] Polling de eventos iniciado.");

    return 1;
}

public OnUserCommandText(const uid[], const nick[], const cmd[], const params[])
{
    if(cmd[0] == '\0') return 1;

    printf("%s | %s", cmd, params);

    new playerid = 0;

    if(!strcmp(cmd, "veh", true))
    {
        new modelid, veh_name[32], color1, color2;

        if(sscanf(params, "iii", modelid, color1, color2)) 
        {
            if(sscanf(params, "s[32]ii", veh_name, color1, color2)) 
                return SendClientMessage(playerid, -1, "{ff3333}[ CMD ] {ffffff}Use: /veh {ff3333}[ MODELID ou NOME]");
            
            modelid = GetVehicleModelByName(veh_name);
        }

        if(modelid < 400 || modelid > 605) 
            return SendClientMessage(playerid, -1, "{ff3333}[ CMD ] {ffffff}Parâmetro {ff3333}[ MODELID ou NOME ] {ffffff}Inválido!");
        
        new Float:pX, Float:pY, Float:pZ, Float:pA;
        GetPlayerPos(playerid, pX, pY, pZ);
        GetPlayerFacingAngle(playerid, pA);

        new vehicleid = CreateVehicle(modelid, pX, pY, pZ, pA, color1, color2, -1);

        PutPlayerInVehicle(playerid, vehicleid, 0);
    }

    return 1;
}

public OnRequestFailure(Request:id, errorCode, errorMessage[], len)
{
    printf("[ TIKTOK ] Request falhou | request = %d | code = %d | message = %s", 
    _:id, errorCode, errorMessage);
    return 1;
}

public FetchLiveEvents()
{
    if(!IsValidRequestsClient(gLiveClient))
    {
        print("[ TIKTOK ] RequestsClient invalido durante FetchLiveEvents.");
        return 1;
    }

    new Request:req = RequestJSON(
        gLiveClient,
        LIVE_API_EVENTS_PATH,
        HTTP_METHOD_GET,
        "OnLiveEventsResponse"
    );

    if(!IsValidRequest(req))
    {
        print("[ TIKTOK ] Falha ao enviar request para /events.");
    }

    return 1;
}

stock LiveGetGiftDiamondValue(const giftName[])
{
    for (new i = 0; i < sizeof(gLiveGiftDiamondTable); i++)
    {
        if (!strcmp(giftName, gLiveGiftDiamondTable[i][GIFT_NAME], true))
        {
            return gLiveGiftDiamondTable[i][GIFT_DIAMONDS];
        }
    }

    return 0;
}

stock LiveAccumulateGiftRevenue(const giftName[], repeatCount, diamondCount)
{
    if (repeatCount < 1) repeatCount = 1;
    if (diamondCount < 1) diamondCount = LiveGetGiftDiamondValue(giftName);

    gLiveTotalDiamonds += (diamondCount * repeatCount);
    return gLiveTotalDiamonds;
}

stock LiveGetTotalDiamonds()
{
    return gLiveTotalDiamonds;
}

stock LiveDispatchEvent(const type[], Node:dataNode)
{
    new 
        uniqueId[LIVE_MAX_ID_LEN],
        nickname[LIVE_MAX_NAME_LEN], 
        userId[LIVE_MAX_USERID_LEN],
        message[LIVE_MAX_MSG_LEN], 
        giftName[LIVE_MAX_GIFT_NAME_LEN]
    ;

    if(!strcmp(type, "chat", true))
    {
        new 
            bool:isModerator    = false, 
            bool:isNewGifter    = false, 
            bool:isSubscriber   = false
        ;

        JsonGetString(dataNode, "uniqueId", uniqueId, sizeof(uniqueId));
        JsonGetString(dataNode, "nickname", nickname, sizeof(nickname));
        JsonGetString(dataNode, "userId", userId, sizeof(userId));
        JsonGetString(dataNode, "comment", message, sizeof(message));

        JsonGetBool(dataNode, "isModerator", isModerator);
        JsonGetBool(dataNode, "isNewGifter", isNewGifter);
        JsonGetBool(dataNode, "isSubscriber", isSubscriber);

        if(message[0] != '\0' && uniqueId[0] != '\0')
        {
            if(message[0] == '!')
            {
                strdel(message[0], 0, 1);
                CallLocalFunction("OnLiveCommandReceive", "sssiiis", uniqueId, nickname, userId, _:isModerator, _:isNewGifter, _:isSubscriber, message);
        
            }

            else    
                CallLocalFunction("OnLiveMessageReceive", "sssiiis", uniqueId, nickname, userId, _:isModerator, _:isNewGifter, _:isSubscriber, message);
        }

        return 1;
    }

    if(!strcmp(type, "like", true))
    {
        new likeCount = 0, totalLikeCount = 0;

        JsonGetString(dataNode, "uniqueId", uniqueId, sizeof(uniqueId));
        JsonGetString(dataNode, "nickname", nickname, sizeof(nickname));
        JsonGetInt(dataNode, "likeCount", likeCount);
        JsonGetInt(dataNode, "totalLikeCount", totalLikeCount);

        CallLocalFunction("OnLiveLikeReceive", "ssii", uniqueId, nickname, likeCount, totalLikeCount);
        return 1;
    }

    if(!strcmp(type, "follow", true))
    {
        JsonGetString(dataNode, "uniqueId", uniqueId, sizeof(uniqueId));
        JsonGetString(dataNode, "nickname", nickname, sizeof(nickname));
        
        CallLocalFunction("OnLiveFollowReceive", "ss", uniqueId, nickname);
        
        return 1;
    }

    if (!strcmp(type, "member", true))
    {
        JsonGetString(dataNode, "uniqueId", uniqueId, sizeof(uniqueId));
        JsonGetString(dataNode, "nickname", nickname, sizeof(nickname));
        JsonGetString(dataNode, "userId", userId, sizeof(userId));
        CallLocalFunction("OnLiveMemberJoin", "sss", uniqueId, nickname, userId);
        return 1;
    }

    if(!strcmp(type, "gift", true) || !strcmp(type, "giftEnd", true))
    {
        new repeatCount = 1, diamondCount = 0;

        JsonGetString(dataNode, "uniqueId", uniqueId, sizeof(uniqueId));
        JsonGetString(dataNode, "nickname", nickname, sizeof(nickname));
        JsonGetString(dataNode, "giftName", giftName, sizeof(giftName));
        JsonGetInt(dataNode, "repeatCount", repeatCount);
        JsonGetInt(dataNode, "diamondCount", diamondCount);

        if(!strcmp(type, "giftEnd", true))
        {
            LiveAccumulateGiftRevenue(giftName, repeatCount, diamondCount);
            CallLocalFunction("OnLiveGiftEndReceive", "sssii", uniqueId, nickname, giftName, repeatCount, diamondCount);
        }
        else
            CallLocalFunction("OnLiveGiftReceive", "sssii", uniqueId, nickname, giftName, repeatCount, diamondCount);
        
        return 1;
    }

    if(!strcmp(type, "roomUser", true))
    {
        new viewerCount = 0, likeCount = 0;

        JsonGetInt(dataNode, "viewerCount", viewerCount);
        JsonGetInt(dataNode, "likeCount", likeCount);

        CallLocalFunction("OnLiveRoomInfo", "ii", viewerCount, likeCount);
        return 1;
    }

    if(!strcmp(type, "streamEnd", true))
    {
        CallLocalFunction("OnLiveStreamEnd", "");
        return 1;
    }

    if(!strcmp(type, "subscribe", true))
    {
        JsonGetString(dataNode, "uniqueId", uniqueId, sizeof(uniqueId));
        JsonGetString(dataNode, "nickname", nickname, sizeof(nickname));
        
        CallLocalFunction("OnLiveSubscribe", "ss", uniqueId, nickname);
    
        return 1;
    }

    return 1;
}

public OnLiveEventsResponse(Request:req, E_HTTP_STATUS:status, Node:node)
{
    if(status != HTTP_STATUS_OK)
    {
        printf("[ TIKTOK ] HTTP inesperado em /events: %d", _:status);
        return 1;
    }

    if(JsonNodeType(node) != JSON_NODE_ARRAY)
    {
        print("[ TIKTOK ] /events nao retornou um JSON array.");
        return 1;
    }

    new length;
    JsonArrayLength(node, length);

    if(length <= 0) return 1;
    
    new type[LIVE_MAX_TYPE_LEN];

    for(new i = 0; i < length; i++)
    {
        new Node:eventNode, Node:dataNode;
        JsonArrayObject(node, i, eventNode);

        if(JsonNodeType(eventNode) != JSON_NODE_OBJECT) continue;
        
        type[0] = '\0';
        JsonGetString(eventNode, "type", type, sizeof(type));

        if(type[0] == '\0') continue;
     
        JsonGetObject(eventNode, "data", dataNode);

        LiveDispatchEvent(type, dataNode);
    }

    return 1;
}

public OnLiveCommandReceive(const uid[], const nick[], const userid[], bool:is_moder, bool:is_newgifter, bool:is_sub, const cmdtext[])
{
    //printf("[ CMD ] %s: %s", nick, cmdtext);

    new end = strfind(cmdtext, " ");

    new cmd[64], params[144];

    strmid(cmd, cmdtext, 0, end);
    strmid(params, cmdtext, end + 1, strlen(cmdtext));

    CallLocalFunction("OnUserCommandText", "ssss", uid, nick, cmd, params);

    return 1;
}

public OnLiveMessageReceive(const uid[], const nick[], const userid[], bool:is_moder, bool:is_newgifter, bool:is_sub, const message[])
{
    //printf("[ CHAT ] %s: %s", nick, message);
    return 1;
}

public OnLiveGiftReceive(const uid[], const nick[], const gift_name[], repeat_count, diamond_count)
{
    //printf("[ GIFT ] %s (%s) enviou %d x %s (diamonds = %d)", nick, uid, repeat_count, gift_name, diamond_count);
    return 1;
}

public OnLiveGiftEndReceive(const uid[], const nick[], const gift_name[], repeat_count, diamond_count)
{
    //printf("[ GIFT-END ] %s (%s) finalizou %d x %s (diamonds = %d) | totalLive = %d", nick, uid, repeat_count, gift_name, diamond_count, LiveGetTotalDiamonds());
    return 1;
}

public OnLiveLikeReceive(const uid[], const nick[], like_count, total_likecount)
{
    //printf("[ LIKE ] %s (%s): +%d (total = %d)", nick, uid, like_count, total_likecount);
    return 1;
}

public OnLiveFollowReceive(const uid[], const nick[])
{
    //printf("[ FOLLOW ] %s (%s) seguiu.", nick, uid);
    return 1;
}

public OnLiveMemberJoin(const uid[], const nick[], userid[])
{
    //printf("[ JOIN ] %s (%s) entrou na live.", nick, uid);
    return 1;
}

public OnLiveRoomInfo(viewer_count, total_likecount)
{
    //printf("[ ROOM ] viewers= %d likes = %d", viewer_count, total_likecount);
    return 1;
}

public OnLiveStreamEnd()
{
    //print("[ LIVE ] Stream encerrada.");
    return 1;
}

public OnLiveSubscribe(const uid[], const nick[])
{
    //printf("[ SUB ] %s (%s) se inscreveu.", nick, uid);
    return 1;
}


stock GetVehicleModelByName(const name[]) 
{
    if(name[0] == '\0') return 400;

    for (new i = 0; i < sizeof(gVehicleNames); i++) 
    {
        if (strfind(gVehicleNames[i], name, true) != -1) {
            return i + 400;
        }
    }

    return 400;
}