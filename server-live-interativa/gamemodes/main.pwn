#include <open.mp>
#include <requests>

#define LIVE_API_ENDPOINT "http://127.0.0.1:3000"
#define LIVE_API_EVENTS_PATH "/events"
#define LIVE_API_POLL_INTERVAL_MS (1000)

#define LIVE_MAX_ID_LEN (64)
#define LIVE_MAX_NAME_LEN (64)
#define LIVE_MAX_USERID_LEN (32)
#define LIVE_MAX_MSG_LEN (144)
#define LIVE_MAX_GIFT_NAME_LEN (64)
#define LIVE_MAX_TYPE_LEN (24)

forward FetchLiveEvents();
forward OnLiveEventsResponse(Request:req, E_HTTP_STATUS:status, Node:node);

forward OnLiveMessageReceive(uniqueId[], nickname[], userId[], isModerator, isNewGifter, isSubscriber, message[]);
forward OnLiveGiftReceive(uniqueId[], nickname[], giftName[], repeatCount, diamondCount);
forward OnLiveGiftEndReceive(uniqueId[], nickname[], giftName[], repeatCount, diamondCount);
forward OnLiveLikeReceive(uniqueId[], nickname[], likeCount, totalLikeCount);
forward OnLiveFollowReceive(uniqueId[], nickname[]);
forward OnLiveMemberJoin(uniqueId[], nickname[], userId[]);
forward OnLiveRoomInfo(viewerCount, likeCount);
forward OnLiveStreamEnd();
forward OnLiveSubscribe(uniqueId[], nickname[]);

new RequestsClient:gLiveClient = RequestsClient:-1;
new gLiveTotalDiamonds = 0;

enum E_LIVE_GIFT_DIAMONDS
{
    GiftName[LIVE_MAX_GIFT_NAME_LEN],
    GiftDiamonds
};

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

    if (!IsValidRequestsClient(gLiveClient))
    {
        printf("[TIKTOK] Falha ao criar RequestsClient para %s. Client ID: %d", LIVE_API_ENDPOINT, _:gLiveClient);
        return 1;
    }

    printf("[TIKTOK] RequestsClient conectado em %s (id=%d)", LIVE_API_ENDPOINT, _:gLiveClient);
    SetTimer("FetchLiveEvents", LIVE_API_POLL_INTERVAL_MS, true);
    print("[TIKTOK] Polling de eventos iniciado.");

    return 1;
}

public OnRequestFailure(Request:id, errorCode, errorMessage[], len)
{
    printf("[TIKTOK] Request failure | request=%d | code=%d | message=%s", _:id, errorCode, errorMessage);
    return 1;
}

public FetchLiveEvents()
{
    if (!IsValidRequestsClient(gLiveClient))
    {
        print("[TIKTOK] RequestsClient invalido durante FetchLiveEvents.");
        return 1;
    }

    new Request:req = RequestJSON(
        gLiveClient,
        LIVE_API_EVENTS_PATH,
        HTTP_METHOD_GET,
        "OnLiveEventsResponse"
    );

    if (!IsValidRequest(req))
    {
        print("[TIKTOK] Falha ao enviar request para /events.");
    }

    return 1;
}

stock LiveSanitizeString(const input[], output[], outputSize)
{
    new outIdx = 0;

    for (new i = 0; input[i] != '\0' && outIdx < (outputSize - 1); i++)
    {
        new c = input[i];

        if (c < 32 || c > 126) continue;
        if (c == '"' || c == '\\') continue;

        output[outIdx++] = c;
    }

    output[outIdx] = '\0';
    return 1;
}

stock LiveJsonGetSafeString(Node:node, const key[], output[], outputSize)
{
    new buffer[256];
    buffer[0] = '\0';

    JsonGetString(node, key, buffer, sizeof(buffer));
    LiveSanitizeString(buffer, output, outputSize);

    return 1;
}

stock LiveGetGiftDiamondValue(const giftName[])
{
    for (new i = 0; i < sizeof(gLiveGiftDiamondTable); i++)
    {
        if (!strcmp(giftName, gLiveGiftDiamondTable[i][GiftName], true))
        {
            return gLiveGiftDiamondTable[i][GiftDiamonds];
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
    new uniqueId[LIVE_MAX_ID_LEN], nickname[LIVE_MAX_NAME_LEN], userId[LIVE_MAX_USERID_LEN];
    new message[LIVE_MAX_MSG_LEN], giftName[LIVE_MAX_GIFT_NAME_LEN];

    uniqueId[0] = '\0';
    nickname[0] = '\0';
    userId[0] = '\0';
    message[0] = '\0';
    giftName[0] = '\0';

    LiveJsonGetSafeString(dataNode, "uniqueId", uniqueId, sizeof(uniqueId));
    LiveJsonGetSafeString(dataNode, "nickname", nickname, sizeof(nickname));

    if (!strcmp(type, "chat", true))
    {
        new bool:isModerator = false, bool:isNewGifter = false, bool:isSubscriber = false;

        LiveJsonGetSafeString(dataNode, "userId", userId, sizeof(userId));
        LiveJsonGetSafeString(dataNode, "comment", message, sizeof(message));

        JsonGetBool(dataNode, "isModerator", isModerator);
        JsonGetBool(dataNode, "isNewGifter", isNewGifter);
        JsonGetBool(dataNode, "isSubscriber", isSubscriber);

        if (message[0] != '\0' && uniqueId[0] != '\0')
        {
            CallLocalFunction("OnLiveMessageReceive", "sssiiis", uniqueId, nickname, userId, _:isModerator, _:isNewGifter, _:isSubscriber, message);
        }
        return 1;
    }

    if (!strcmp(type, "gift", true) || !strcmp(type, "giftEnd", true))
    {
        new repeatCount = 1, diamondCount = 0;

        LiveJsonGetSafeString(dataNode, "giftName", giftName, sizeof(giftName));
        JsonGetInt(dataNode, "repeatCount", repeatCount);
        JsonGetInt(dataNode, "diamondCount", diamondCount);

        if (giftName[0] == '\0' || uniqueId[0] == '\0')
        {
            return 1;
        }

        if (!strcmp(type, "giftEnd", true))
        {
            // Acumula monetização no fechamento do combo para evitar duplicidade.
            LiveAccumulateGiftRevenue(giftName, repeatCount, diamondCount);
            CallLocalFunction("OnLiveGiftEndReceive", "sssii", uniqueId, nickname, giftName, repeatCount, diamondCount);
        }
        else
        {
            CallLocalFunction("OnLiveGiftReceive", "sssii", uniqueId, nickname, giftName, repeatCount, diamondCount);
        }

        return 1;
    }

    if (!strcmp(type, "like", true))
    {
        new likeCount = 0, totalLikeCount = 0;

        JsonGetInt(dataNode, "likeCount", likeCount);
        JsonGetInt(dataNode, "totalLikeCount", totalLikeCount);

        CallLocalFunction("OnLiveLikeReceive", "ssii", uniqueId, nickname, likeCount, totalLikeCount);
        return 1;
    }

    if (!strcmp(type, "follow", true))
    {
        if (uniqueId[0] != '\0')
        {
            CallLocalFunction("OnLiveFollowReceive", "ss", uniqueId, nickname);
        }
        return 1;
    }

    if (!strcmp(type, "member", true))
    {
        LiveJsonGetSafeString(dataNode, "userId", userId, sizeof(userId));
        CallLocalFunction("OnLiveMemberJoin", "sss", uniqueId, nickname, userId);
        return 1;
    }

    if (!strcmp(type, "roomUser", true))
    {
        new viewerCount = 0, likeCount = 0;

        JsonGetInt(dataNode, "viewerCount", viewerCount);
        JsonGetInt(dataNode, "likeCount", likeCount);

        CallLocalFunction("OnLiveRoomInfo", "ii", viewerCount, likeCount);
        return 1;
    }

    if (!strcmp(type, "streamEnd", true))
    {
        CallLocalFunction("OnLiveStreamEnd", "");
        return 1;
    }

    if (!strcmp(type, "subscribe", true))
    {
        if (uniqueId[0] != '\0')
        {
            CallLocalFunction("OnLiveSubscribe", "ss", uniqueId, nickname);
        }
        return 1;
    }

    return 1;
}

public OnLiveEventsResponse(Request:req, E_HTTP_STATUS:status, Node:node)
{
    if (status != HTTP_STATUS_OK)
    {
        printf("[TIKTOK] HTTP inesperado em /events: %d", _:status);
        return 1;
    }

    if (JsonNodeType(node) != JSON_NODE_ARRAY)
    {
        print("[TIKTOK] /events nao retornou um JSON array.");
        return 1;
    }

    new length;
    JsonArrayLength(node, length);

    if (length <= 0)
    {
        return 1;
    }

    new type[LIVE_MAX_TYPE_LEN];

    for (new i = 0; i < length; i++)
    {
        new Node:eventNode, Node:dataNode;
        JsonArrayObject(node, i, eventNode);

        if (JsonNodeType(eventNode) != JSON_NODE_OBJECT)
        {
            continue;
        }

        type[0] = '\0';
        JsonGetString(eventNode, "type", type, sizeof(type));

        if (type[0] == '\0')
        {
            continue;
        }

        if (!JsonGetObject(eventNode, "data", dataNode) || JsonNodeType(dataNode) != JSON_NODE_OBJECT)
        {
            // Compatibilidade com payload legado (campos no root do evento).
            dataNode = eventNode;
        }

        LiveDispatchEvent(type, dataNode);
    }

    return 1;
}

public OnLiveMessageReceive(uniqueId[], nickname[], userId[], isModerator, isNewGifter, isSubscriber, message[])
{
    printf("[LIVE CHAT] (%s/%s) %s: %s | mod=%d newGifter=%d sub=%d", uniqueId, userId, nickname, message, isModerator, isNewGifter, isSubscriber);
    return 1;
}

public OnLiveGiftReceive(uniqueId[], nickname[], giftName[], repeatCount, diamondCount)
{
    printf("[LIVE GIFT] %s (%s) enviou %d x %s (diamonds=%d)", nickname, uniqueId, repeatCount, giftName, diamondCount);
    return 1;
}

public OnLiveGiftEndReceive(uniqueId[], nickname[], giftName[], repeatCount, diamondCount)
{
    printf("[LIVE GIFT-END] %s (%s) finalizou %d x %s (diamonds=%d) | totalLive=%d", nickname, uniqueId, repeatCount, giftName, diamondCount, LiveGetTotalDiamonds());
    return 1;
}

public OnLiveLikeReceive(uniqueId[], nickname[], likeCount, totalLikeCount)
{
    printf("[LIVE LIKE] %s (%s): +%d (total=%d)", nickname, uniqueId, likeCount, totalLikeCount);
    return 1;
}

public OnLiveFollowReceive(uniqueId[], nickname[])
{
    printf("[LIVE FOLLOW] %s (%s) seguiu.", nickname, uniqueId);
    return 1;
}

public OnLiveMemberJoin(uniqueId[], nickname[], userId[])
{
    printf("[LIVE JOIN] %s (%s/%s) entrou na live.", nickname, uniqueId, userId);
    return 1;
}

public OnLiveRoomInfo(viewerCount, likeCount)
{
    printf("[LIVE ROOM] viewers=%d likes=%d", viewerCount, likeCount);
    return 1;
}

public OnLiveStreamEnd()
{
    print("[LIVE] Stream encerrada.");
    return 1;
}

public OnLiveSubscribe(uniqueId[], nickname[])
{
    printf("[LIVE SUB] %s (%s) se inscreveu.", nickname, uniqueId);
    return 1;
}
