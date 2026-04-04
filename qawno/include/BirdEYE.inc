
/*

                    oooooooooo.   oooooooooooo oooooooooooo ooooo ooooo      ooo oooooooooooo ooooooooo.    .oooooo..o 
                    `888'   `Y8b  `888'     `8 `888'     `8 `888' `888b.     `8' `888'     `8 `888   `Y88. d8P'    `Y8 
                    888      888  888          888          888   8 `88b.    8   888          888   .d88' Y88bo.      
                    888      888  888oooo8     888oooo8     888   8   `88b.  8   888oooo8     888ooo88P'   `"Y8888o.  
                    888      888  888    "     888    "     888   8     `88b.8   888    "     888`88b.         `"Y88b 
                    888     d88'  888       o  888          888   8       `888   888       o  888  `88b.  oo     .d8P 
                    o888bood8P'   o888ooooood8 o888o        o888o o8o        `8  o888ooooood8 o888o  o888o 8""88888P'  

*/

#define				BIRDEYE_NOTIFICACAO		    (0)
#define				BIRDEYE_EXPULSAO			(1)
#define				BIRDEYE_BANIMENTO		    (2)



/*

                    oooooooooooo   .oooooo.   ooooooooo.   oooooo   oooooo     oooo       .o.       ooooooooo.   oooooooooo.   
                    `888'     `8  d8P'  `Y8b  `888   `Y88.  `888.    `888.     .8'       .888.      `888   `Y88. `888'   `Y8b  
                    888         888      888  888   .d88'   `888.   .8888.   .8'       .8"888.      888   .d88'  888      888 
                    888oooo8    888      888  888ooo88P'     `888  .8'`888. .8'       .8' `888.     888ooo88P'   888      888 
                    888    "    888      888  888`88b.        `888.8'  `888.8'       .88ooo8888.    888`88b.     888      888 
                    888         `88b    d88'  888  `88b.       `888'    `888'       .8'     `888.   888  `88b.   888     d88' 
                    o888o         `Y8bood8P'  o888o  o888o       `8'      `8'       o88o     o8888o o888o  o888o o888bood8P'   

*/

forward OnBirdEYECheat(playerid, type, const feature[]);

forward OnBirdConnectionInclude(index);
public OnBirdConnectionInclude(index) {return index;}



/*

                    oooooooooooo ooooo     ooo ooooo      ooo   .oooooo.   ooooooooooooo ooooo   .oooooo.   ooooo      ooo  .oooooo..o        .o  .o8                     .             o.   
                    `888'     `8 `888'     `8' `888b.     `8'  d8P'  `Y8b  8'   888   `8 `888'  d8P'  `Y8b  `888b.     `8' d8P'    `Y8       .8' "888                   .o8             `8.  
                    888          888       8   8 `88b.    8  888               888       888  888      888  8 `88b.    8  Y88bo.           .8'   888oooo.   .ooooo.  .o888oo  .oooo.    `8. 
                    888oooo8     888       8   8   `88b.  8  888               888       888  888      888  8   `88b.  8   `"Y8888o.       88    d88' `88b d88' `88b   888   `P  )88b    88 
                    888    "     888       8   8     `88b.8  888               888       888  888      888  8     `88b.8       `"Y88b      88    888   888 888ooo888   888    .oP"888    88 
                    888          `88.    .8'   8       `888  `88b    ooo       888       888  `88b    d88'  8       `888  oo     .d8P      `8.   888   888 888    .o   888 . d8(  888   .8' 
                    o888o           `YbodP'    o8o        `8   `Y8bood8P'      o888o     o888o  `Y8bood8P'  o8o        `8  8""88888P'        `8.  `Y8bod8P' `Y8bod8P'   "888" `Y888""8o .8'  

*/

/*
*   Descri  o:
*   - Sincronizar as logs com o website
*
*   Observa  o:
*   - H  um delay de 4 minutos entre as chamadas
*
*   Returns:
*   - false: Ainda n o pode sincronizar
*   - true: Sincronizado com o sucesso
*/
stock bool:BirdEYE_SyncLogs()
    (CallRemoteFunction("Bird@LogRequest", "d", 0));



/*

                        .o.       ooooo         .oooooo..o 
                        .888.      `888'        d8P'    `Y8 
                        .8"888.      888         Y88bo.      
                    .8' `888.     888          `"Y8888o.  
                    .88ooo8888.    888              `"Y88b 
                    .8'     `888.   888       o oo     .d8P 
                    o88o     o8888o o888ooooood8 8""88888P'  

*/

#if defined _ALS_SetPlayerPos
	#undef SetPlayerPos
#else
	#define _ALS_SetPlayerPos
#endif

#if defined _ALS_SetVehiclePos
    #undef SetVehiclePos
#else
    #define _ALS_SetVehiclePos
#endif

#if defined _ALS_SpawnPlayer
    #undef SpawnPlayer
#else
    #define _ALS_SpawnPlayer
#endif

#if defined _ALS_PutPlayerInVehicle
    #undef PutPlayerInVehicle
#else
    #define _ALS_PutPlayerInVehicle
#endif

#if defined _ALS_SetPlayerSkin
    #undef SetPlayerSkin
#else
    #define _ALS_SetPlayerSkin
#endif

#if defined _ALS_CreateVehicle
    #undef CreateVehicle
#else
    #define _ALS_CreateVehicle
#endif

#if defined _ALS_AddStaticVehicle
    #undef AddStaticVehicle
#else
    #define _ALS_AddStaticVehicle
#endif

#if defined _ALS_AddStaticVehicleEx
    #undef AddStaticVehicleEx
#else
    #define _ALS_AddStaticVehicleEx
#endif

#if defined _ALS_DestroyVehicle
    #undef DestroyVehicle
#else
    #define _ALS_DestroyVehicle
#endif

#if defined _ALS_SetVehicleHealth
    #undef SetVehicleHealth
#else
    #define _ALS_SetVehicleHealth
#endif

#if defined _ALS_RepairVehicle
    #undef RepairVehicle
#else
    #define _ALS_RepairVehicle
#endif

#if defined _ALS_GivePlayerWeapon
    #undef GivePlayerWeapon
#else
    #define _ALS_GivePlayerWeapon
#endif

#if defined _ALS_SetPlayerAmmo
    #undef SetPlayerAmmo
#else
    #define _ALS_SetPlayerAmmo
#endif

#if defined _ALS_ResetPlayerWeapons
    #undef ResetPlayerWeapons
#else
    #define _ALS_ResetPlayerWeapons
#endif

#if defined _ALS_ResetPlayerMoney
    #undef ResetPlayerMoney
#else
    #define _ALS_ResetPlayerMoney
#endif

#if defined _ALS_GivePlayerMoney
    #undef GivePlayerMoney
#else
    #define _ALS_GivePlayerMoney
#endif

#if defined _ALS_GetPlayerMoney
    #undef GetPlayerMoney
#else
    #define _ALS_GetPlayerMoney
#endif

#if defined _ALS_ChangeVehicleColor
    #undef ChangeVehicleColor
#else
    #define _ALS_ChangeVehicleColor
#endif

#if defined _ALS_SetPlayerHealth
    #undef SetPlayerHealth
#else
    #define _ALS_SetPlayerHealth
#endif

#if defined _ALS_TogglePlayerSpectating
    #undef TogglePlayerSpectating
#else
    #define _ALS_TogglePlayerSpectating
#endif

#if defined _ALS_SetPlayerScore
    #undef SetPlayerScore
#else
    #define _ALS_SetPlayerScore
#endif

#if defined _ALS_SetSpawnInfo
    #undef SetSpawnInfo
#else
    #define _ALS_SetSpawnInfo
#endif



/*

                    .oooooo..o ooooooooooooo   .oooooo.     .oooooo.   oooo    oooo  .oooooo..o 
                    d8P'    `Y8 8'   888   `8  d8P'  `Y8b   d8P'  `Y8b  `888   .8P'  d8P'    `Y8 
                    Y88bo.           888      888      888 888           888  d8'    Y88bo.      
                    `"Y8888o.       888      888      888 888           88888[       `"Y8888o.  
                        `"Y88b      888      888      888 888           888`88b.         `"Y88b 
                    oo     .d8P      888      `88b    d88' `88b    ooo   888  `88b.  oo     .d8P 
                    8""88888P'      o888o      `Y8bood8P'   `Y8bood8P'  o888o  o888o 8""88888P'  

*/

stock birdEYE_SetPlayerPos(playerid, Float:x, Float:y, Float:z) {
	CallRemoteFunction("BirdSetPlayerPos", "ifff", playerid, x, y, z);
	return SetPlayerPos(playerid, x, y, z);
}

stock birdEYE_SetVehiclePos(vehicleid, Float:x, Float:y, Float:z) {
	CallRemoteFunction("BirdSetVehiclePos", "ifff", vehicleid, x, y, z);
	return SetVehiclePos(vehicleid, x, y, z);
}

stock birdEYE_SpawnPlayer(playerid) {
	if(!CallRemoteFunction("BirdSpawnPlayer", "i", playerid)) return false;
	return SpawnPlayer(playerid);
}

stock birdEYE_PutPlayerInVehicle(playerid, vehicleid, seatid) {
	if(!CallRemoteFunction("BirdPutPlayerInVehicle", "iii", playerid, vehicleid, seatid)) return false;
	return PutPlayerInVehicle(playerid, vehicleid, seatid);
}

stock birdEYE_SetPlayerSkin(playerid, skinid) {
	if(!CallRemoteFunction("BirdSetPlayerSkin", "ii", playerid, skinid)) return false;
	return SetPlayerSkin(playerid, skinid);
}

stock birdEYE_CreateVehicle(vehicletype, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay, addsiren = false) {
	new id = CreateVehicle(vehicletype, x, y, z, rotation, color1, color2, respawn_delay, addsiren);
	if(id == INVALID_VEHICLE_ID) return INVALID_VEHICLE_ID;
	if(!CallRemoteFunction("BirdCreateVehicle", "iiffffiiii", id, vehicletype, x, y, z, rotation, color1, color2, respawn_delay, addsiren)) return id;
	return id;
}

stock birdEYE_AddStaticVehicle(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2) {
	new id = AddStaticVehicle(modelid, spawn_x, spawn_y, spawn_z, z_angle, color1, color2);
	if(id == INVALID_VEHICLE_ID) return INVALID_VEHICLE_ID;
	if(!CallRemoteFunction("BirdCreateVehicle", "iiffffiiii", id, modelid, spawn_x, spawn_y, spawn_z, z_angle, color1, color2, 0, 0)) return id;
	return id;
}

stock birdEYE_AddStaticVehicleEx(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2, respawn_delay, addsiren = false) {
	new id = AddStaticVehicleEx(modelid, spawn_x, spawn_y, spawn_z, z_angle, color1, color2, respawn_delay, addsiren);
	if(id == INVALID_VEHICLE_ID) return INVALID_VEHICLE_ID;
	if(!CallRemoteFunction("BirdCreateVehicle", "iiffffiiii", id, modelid, spawn_x, spawn_y, spawn_z, z_angle, color1, color2, respawn_delay, addsiren)) return id;
	return id;
}

stock birdEYE_DestroyVehicle(vehicleid) {
    if(!CallRemoteFunction("BirdDestroyVehicle", "i", vehicleid)) return false;   
    return DestroyVehicle(vehicleid);
}

stock birdEYE_SetVehicleHealth(vehicleid, Float:health) {
    if(!CallRemoteFunction("BirdSetVehicleHealth", "if", vehicleid, health)) return false;   
    return SetVehicleHealth(vehicleid, health);
}

stock birdEYE_RepairVehicle(vehicleid) {
    if(!CallRemoteFunction("BirdRepairVehicle", "i", vehicleid)) return false;
    return RepairVehicle(vehicleid);
}

stock birdEYE_GivePlayerWeapon(playerid, weaponid, ammo) {
    if(!CallRemoteFunction("BirdGivePlayerWeapon", "iii", playerid, weaponid, ammo)) return false;
    return GivePlayerWeapon(playerid, weaponid, ammo);
}

stock birdEYE_SetPlayerAmmo(playerid, weaponid, ammo) {
    if(!CallRemoteFunction("BirdSetPlayerAmmo", "iii", playerid, weaponid, ammo)) return false;
    return SetPlayerAmmo(playerid, weaponid, ammo);
}

stock birdEYE_ResetPlayerWeapons(playerid) {
    if(!CallRemoteFunction("BirdResetPlayerWeapons", "i", playerid)) return false;
    return ResetPlayerWeapons(playerid);
}

stock birdEYE_ResetPlayerMoney(playerid) {
    if(!CallRemoteFunction("BirdResetPlayerMoney", "i", playerid)) return false;
    return ResetPlayerMoney(playerid);
}

stock birdEYE_GivePlayerMoney(playerid, money) {
    if(!CallRemoteFunction("BirdGivePlayerMoney", "ii", playerid, money)) return false;
    return GivePlayerMoney(playerid, money);
}

stock birdEYE_GetPlayerMoney(playerid)
    return (CallRemoteFunction("BirdGetPlayerMoney", "i", playerid));

stock birdEYE_ChangeVehicleColor(vehicleid, color1, color2) {
    if(!CallRemoteFunction("BirdChangeVehicleColor", "iii", vehicleid, color1, color2)) return false;
    return ChangeVehicleColor(vehicleid, color1, color2);
}

stock birdEYE_SetPlayerHealth(playerid, Float:health) {
    if(!CallRemoteFunction("BirdSetPlayerHealth", "if", playerid, health)) return false;
    return SetPlayerHealth(playerid, health);
}

stock birdEYE_TogglePlayerSpectating(playerid, toggle) {
    if(!CallRemoteFunction("BirdTogglePlayerSpect", "ii", playerid, toggle)) return false;
    return TogglePlayerSpectating(playerid, toggle);
}

stock birdEYE_SetPlayerScore(playerid, score) {
    if(!CallRemoteFunction("BirdSetPlayerScore", "ii", playerid, score)) return false;
    return SetPlayerScore(playerid, score);
}

stock birdEYE_SetSpawnInfo(playerid, team, skin, Float:x, Float:y, Float:z, Float:rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo) {
    if(!CallRemoteFunction("BirdSetSpawnInfo", "iiiffffiiiiii", playerid, team, skin, x, y, z, rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)) return false;
    return SetSpawnInfo(playerid, team, skin, x, y, z, rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
}



/*

                    ooooooooo.   oooooooooooo oooooooooo.   oooooooooooo oooooooooooo ooooo ooooo      ooo oooooooooooo 
                    `888   `Y88. `888'     `8 `888'   `Y8b  `888'     `8 `888'     `8 `888' `888b.     `8' `888'     `8 
                    888   .d88'  888          888      888  888          888          888   8 `88b.    8   888         
                    888ooo88P'   888oooo8     888      888  888oooo8     888oooo8     888   8   `88b.  8   888oooo8    
                    888`88b.     888    "     888      888  888    "     888    "     888   8     `88b.8   888    "    
                    888  `88b.   888       o  888     d88'  888       o  888          888   8       `888   888       o 
                    o888o  o888o o888ooooood8 o888bood8P'   o888ooooood8 o888o        o888o o8o        `8  o888ooooood8 

*/

#define SetPlayerPos                birdEYE_SetPlayerPos
#define SetVehiclePos				birdEYE_SetVehiclePos
#define SpawnPlayer				    birdEYE_SpawnPlayer
#define PutPlayerInVehicle			birdEYE_PutPlayerInVehicle
#define SetPlayerSkin			    birdEYE_SetPlayerSkin
#define CreateVehicle			    birdEYE_CreateVehicle
#define AddStaticVehicle			birdEYE_AddStaticVehicle
#define AddStaticVehicleEx			birdEYE_AddStaticVehicleEx
#define DestroyVehicle			    birdEYE_DestroyVehicle
#define SetVehicleHealth			birdEYE_SetVehicleHealth
#define RepairVehicle			    birdEYE_RepairVehicle
#define GivePlayerWeapon			birdEYE_GivePlayerWeapon
#define SetPlayerAmmo			    birdEYE_SetPlayerAmmo
#define ResetPlayerWeapons			birdEYE_ResetPlayerWeapons
#define ResetPlayerMoney			birdEYE_ResetPlayerMoney
#define GivePlayerMoney			    birdEYE_GivePlayerMoney
#define GetPlayerMoney			    birdEYE_GetPlayerMoney
#define ChangeVehicleColor			birdEYE_ChangeVehicleColor
#define SetPlayerHealth			    birdEYE_SetPlayerHealth
#define TogglePlayerSpectating		birdEYE_TogglePlayerSpectating
#define SetPlayerScore		        birdEYE_SetPlayerScore
#define SetSpawnInfo		        birdEYE_SetSpawnInfo