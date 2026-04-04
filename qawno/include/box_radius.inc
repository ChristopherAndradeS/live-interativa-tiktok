/*

                    ooooo      ooo       .o.       ooooooooooooo ooooo oooooo     oooo       .o.        .oooooo..o 
                    `888b.     `8'      .888.      8'   888   `8 `888'  `888.     .8'       .888.      d8P'    `Y8 
                    8 `88b.    8      .8"888.          888       888    `888.   .8'       .8"888.     Y88bo.      
                    8   `88b.  8     .8' `888.         888       888     `888. .8'       .8' `888.     `"Y8888o.  
                    8     `88b.8    .88ooo8888.        888       888      `888.8'       .88ooo8888.        `"Y88b 
                    8       `888   .8'     `888.       888       888       `888'       .8'     `888.  oo     .d8P 
                    o8o        `8  o88o     o8888o     o888o     o888o       `8'       o88o     o8888o 8""88888P'  


        native CreatePlayerBoxRadius(playerid, Float:pos_x, Float:pos_y, color = 0x1E90FFFF, bgcolor = 0x1C1C1CFF, Float:size = 5.0);
        native UpdatePlayerBoxRadiusProgress(playerid, id, Float:value);
        native DestroyPlayerBoxRadiusProgress(playerid, id);
        native DestroyPlayerBoxRadiusAll(playerid);
                                                                                          
*/

#if defined _box_radius_included
	#endinput
#endif
#define _box_radius_included

            // Definers
#define                 MAX_BOXR         (5)
#define                 MAX_BOXRDRAW     (12)


enum BOX_RADIUS_ENUM {

         // Config
    Float:E_BOXR_POSITION[2],
    Float:E_BOXR_SIZE,
    Float:E_BOXR_VALUE,

        // Colors
    E_BOXR_COLOR,
    E_BOXR_BACKGROUND,

        // Draws
    bool:E_BOXRDRAW_CREATED,
	PlayerText:E_BOXR_TEXT[MAX_BOXRDRAW + 1],

    bool:E_BOXR_USED
};

static e_BoxRadiusDraw[MAX_PLAYERS][MAX_BOXR][BOX_RADIUS_ENUM];



/*

                    .oooooo..o ooooooooooooo   .oooooo.     .oooooo.   oooo    oooo  .oooooo..o
                    d8P'    `Y8 8'   888   `8  d8P'  `Y8b   d8P'  `Y8b  `888   .8P'  d8P'    `Y8
                    Y88bo.           888      888      888 888           888  d8'    Y88bo.     
                    `"Y8888o.       888      888      888 888           88888[       `"Y8888o. 
                        `"Y88b      888      888      888 888           888`88b.         `"Y88b
                    oo     .d8P      888      `88b    d88' `88b    ooo   888  `88b.  oo     .d8P
                    8""88888P'      o888o      `Y8bood8P'   `Y8bood8P'  o888o  o888o 8""88888P' 

*/

stock CreatePlayerBoxRadius(playerid, Float:pos_x, Float:pos_y, color = 0x1E90FFFF, bgcolor = 0x1C1C1CFF, Float:size = 5.0) {

    new index = -1;
    for(new i; i < MAX_BOXR; i++) {
        if(!e_BoxRadiusDraw[playerid][i][E_BOXR_USED]) {
            index = i;
            break;
        }
    }
    if(index == -1) return -1; // Sem espaço disponivel

    e_BoxRadiusDraw[playerid][index][E_BOXR_USED] = true;

    /*      Config       */
    e_BoxRadiusDraw[playerid][index][E_BOXR_POSITION][0] = pos_x;
    e_BoxRadiusDraw[playerid][index][E_BOXR_POSITION][1] = pos_y;
    e_BoxRadiusDraw[playerid][index][E_BOXR_COLOR] = color;
    e_BoxRadiusDraw[playerid][index][E_BOXR_BACKGROUND] = bgcolor;
    e_BoxRadiusDraw[playerid][index][E_BOXR_SIZE] = size;
    e_BoxRadiusDraw[playerid][index][E_BOXR_VALUE] = -1.0;   // Prevent Bugs

    UpdatePlayerBoxRadiusProgress(playerid, index, 100);
    return index;
}

stock UpdatePlayerBoxRadiusProgress(playerid, id, Float:value) {
    if(e_BoxRadiusDraw[playerid][id][E_BOXR_VALUE] == value || value > 100 || value < 0) 
        return true;
    
    value = 100 - value;
    new 
        Float:x = e_BoxRadiusDraw[playerid][id][E_BOXR_POSITION][0], 
        Float:y = e_BoxRadiusDraw[playerid][id][E_BOXR_POSITION][1],
        bgcolor = e_BoxRadiusDraw[playerid][id][E_BOXR_BACKGROUND],
        color = e_BoxRadiusDraw[playerid][id][E_BOXR_COLOR],
        Float:size = e_BoxRadiusDraw[playerid][id][E_BOXR_SIZE],
        Float:size_ldbeat = 10 + size;

    if(!e_BoxRadiusDraw[playerid][id][E_BOXRDRAW_CREATED]) {

        e_BoxRadiusDraw[playerid][id][E_BOXRDRAW_CREATED] = true;

        e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][0] = CreatePlayerTextDraw(playerid, x + (0.426 * size), y + 33.2 + (3.52 * size), "LD_BUM:blkdot"); 
        PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][0], 18.499 + 2.71 * size, -25.6 - 3.49 * size);
        PlayerTextDrawAlignment(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][0], 1);
        PlayerTextDrawColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][0], bgcolor);
        PlayerTextDrawSetShadow(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][0], 0);
        PlayerTextDrawSetOutline(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][0], 0);
        PlayerTextDrawBackgroundColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][0], 255);
        PlayerTextDrawFont(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][0], 4);
        PlayerTextDrawSetProportional(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][0], 1);

        e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][1] = CreatePlayerTextDraw(playerid, x - 4.2 - (0.167 * size), y + 6 - (0.154 * size), "LD_BEAT:chit");
        PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][1], size_ldbeat, size_ldbeat);
        PlayerTextDrawAlignment(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][1], 1);
        PlayerTextDrawColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][1], bgcolor);
        PlayerTextDrawSetShadow(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][1], 0);
        PlayerTextDrawSetOutline(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][1], 0);
        PlayerTextDrawBackgroundColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][1], 255);
        PlayerTextDrawFont(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][1], 4);
        PlayerTextDrawSetProportional(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][1], 1);

        e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][2] = CreatePlayerTextDraw(playerid, x + 13 + (2.746 * size), y + 6 - (0.154 * size), "LD_BEAT:chit");
        PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][2], size_ldbeat, size_ldbeat);
        PlayerTextDrawAlignment(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][2], 1);
        PlayerTextDrawColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][2], bgcolor);
        PlayerTextDrawSetShadow(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][2], 0);
        PlayerTextDrawSetOutline(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][2], 0);
        PlayerTextDrawBackgroundColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][2], 255);
        PlayerTextDrawFont(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][2], 4);
        PlayerTextDrawSetProportional(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][2], 1);

        e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][3] = CreatePlayerTextDraw(playerid, x - 4.2 - (0.167 * size), y + 25 + (2.6666 * size), "LD_BEAT:chit");
        PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][3], size_ldbeat, size_ldbeat);
        PlayerTextDrawAlignment(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][3], 1);
        PlayerTextDrawColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][3], bgcolor);
        PlayerTextDrawSetShadow(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][3], 0);
        PlayerTextDrawSetOutline(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][3], 0);
        PlayerTextDrawBackgroundColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][3], 255);
        PlayerTextDrawFont(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][3], 4);
        PlayerTextDrawSetProportional(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][3], 1);

        e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][4] = CreatePlayerTextDraw(playerid, x + 13 + (2.746 * size), y + 25 + (2.6666 * size), "LD_BEAT:chit");
        PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][4], size_ldbeat, size_ldbeat);
        PlayerTextDrawAlignment(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][4], 1);
        PlayerTextDrawColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][4], bgcolor);
        PlayerTextDrawSetShadow(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][4], 0);
        PlayerTextDrawSetOutline(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][4], 0);
        PlayerTextDrawBackgroundColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][4], 255);
        PlayerTextDrawFont(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][4], 4);
        PlayerTextDrawSetProportional(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][4], 1);

        e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][5] = CreatePlayerTextDraw(playerid, x - 2.7 + (0.005 * size), y + 31.2 + (3.07 * size), "LD_BUM:blkdot"); 
        PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][5], 24.100 + 3.566 * size, -20.500 - 2.746 * size);
        PlayerTextDrawAlignment(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][5], 1);
        PlayerTextDrawColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][5], bgcolor);
        PlayerTextDrawSetShadow(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][5], 0);
        PlayerTextDrawSetOutline(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][5], 0);
        PlayerTextDrawBackgroundColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][5], 255);
        PlayerTextDrawFont(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][5], 4);
        PlayerTextDrawSetProportional(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][5], 1);

        e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][6] = CreatePlayerTextDraw(playerid, x + (0.426 * size), y + 33.2 + (3.52 * size), "LD_BUM:blkdot");
        PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][6], 18.499 + 2.71 * size, -25.6 - 3.49 * size);
        PlayerTextDrawAlignment(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][6], 1);
        PlayerTextDrawColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][6], color);
        PlayerTextDrawSetShadow(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][6], 0);
        PlayerTextDrawSetOutline(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][6], 0);
        PlayerTextDrawBackgroundColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][6], 255);
        PlayerTextDrawFont(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][6], 4);
        PlayerTextDrawSetProportional(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][6], 1);

        e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][7] = CreatePlayerTextDraw(playerid, x - 4.2 - (0.167 * size), y + 6 - (0.154 * size), "LD_BEAT:chit");
        PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][7], size_ldbeat, size_ldbeat);
        PlayerTextDrawAlignment(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][7], 1);
        PlayerTextDrawColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][7], color);
        PlayerTextDrawSetShadow(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][7], 0);
        PlayerTextDrawSetOutline(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][7], 0);
        PlayerTextDrawBackgroundColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][7], 255);
        PlayerTextDrawFont(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][7], 4);
        PlayerTextDrawSetProportional(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][7], 1);

        e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][8] = CreatePlayerTextDraw(playerid, x + 13 + (2.746 * size), y + 6 - (0.154 * size), "LD_BEAT:chit"); 
        PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][8], size_ldbeat, size_ldbeat);
        PlayerTextDrawAlignment(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][8], 1);
        PlayerTextDrawColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][8], color);
        PlayerTextDrawSetShadow(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][8], 0);
        PlayerTextDrawSetOutline(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][8], 0);
        PlayerTextDrawBackgroundColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][8], 255);
        PlayerTextDrawFont(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][8], 4);
        PlayerTextDrawSetProportional(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][8], 1);

        e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][9] = CreatePlayerTextDraw(playerid, x - 2.7 + (0.005 * size), y + 31.2 + (3.07 * size), "LD_BUM:blkdot"); 
        PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][9], 24.100 + 3.566 * size, -20.500 - 2.746 * size);
        PlayerTextDrawAlignment(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][9], 1);
        PlayerTextDrawColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][9], color);
        PlayerTextDrawSetShadow(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][9], 0);
        PlayerTextDrawSetOutline(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][9], 0);
        PlayerTextDrawBackgroundColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][9], 255);
        PlayerTextDrawFont(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][9], 4);
        PlayerTextDrawSetProportional(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][9], 1);

        e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][10] = CreatePlayerTextDraw(playerid, x - 4.2 - (0.167 * size), y + 25 + (2.6666 * size), "LD_BEAT:chit");
        PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][10], size_ldbeat, size_ldbeat);
        PlayerTextDrawAlignment(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][10], 1);
        PlayerTextDrawColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][10], color);
        PlayerTextDrawSetShadow(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][10], 0);
        PlayerTextDrawSetOutline(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][10], 0);
        PlayerTextDrawBackgroundColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][10], 255);
        PlayerTextDrawFont(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][10], 4);
        PlayerTextDrawSetProportional(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][10], 1);

        e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][11] = CreatePlayerTextDraw(playerid, x + 13 + (2.746 * size), y + 25 + (2.6666 * size), "LD_BEAT:chit");
        PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][11], size_ldbeat, size_ldbeat);
        PlayerTextDrawAlignment(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][11], 1);
        PlayerTextDrawColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][11], color);
        PlayerTextDrawSetShadow(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][11], 0);
        PlayerTextDrawSetOutline(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][11], 0);
        PlayerTextDrawBackgroundColor(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][11], 255);
        PlayerTextDrawFont(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][11], 4);
        PlayerTextDrawSetProportional(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][11], 1);

    } else {
        new 
            Float:boxr_size_1 = -25.6 - 3.49 * size,
            Float:boxr_size_2 = -20.500 - 2.746 * size,

            Float:boxr_posx_1 = x - 4.2 - (0.167 * size),
            Float:boxr_posx_2 = x + 13 + (2.746 * size),

            Float:boxr_posy_final = y + 25 + (2.6666 * size),
            Float:boxr_posy_inicio = y + 6 - (0.154 * size),

            Float:new_boxr_size = boxr_size_1 - (-0.665 * size_ldbeat),
            Float:porcentagem = (new_boxr_size / boxr_size_1) * 100;

        if(value <= porcentagem)
        {
            value = value / porcentagem;

            new Float:new_boxr_posy = boxr_posy_inicio + ((boxr_posy_final - boxr_posy_inicio) * value);
                                                                                          
            PlayerTextBoxSetPos(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][7], boxr_posx_1, new_boxr_posy, color); 
            PlayerTextBoxSetPos(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][8], boxr_posx_2, new_boxr_posy, color);

            PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][7], size_ldbeat, size_ldbeat);
            PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][8], size_ldbeat, size_ldbeat);
                                                                                  
            PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][6], 18.499 + 2.71 * size, boxr_size_1 - (value * new_boxr_size));
            PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][9], 24.100 + 3.566 * size, boxr_size_2 - (boxr_size_2 * value));

        } else {

            value = value - porcentagem;
            value = value / (100 - porcentagem);

            PlayerTextBoxSetPos(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][7], boxr_posx_1 + (value * (0.5  * size_ldbeat)), boxr_posy_final + (value * (0.84  * size_ldbeat)), color); 
            PlayerTextBoxSetPos(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][8], boxr_posx_2 + (value * (0.4  * size_ldbeat)), boxr_posy_final + (value * (0.84  * size_ldbeat)), color); 

            new Float:new_size_ldbeat = size_ldbeat - (value * size_ldbeat);
            PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][7], new_size_ldbeat, new_size_ldbeat);
            PlayerTextDrawTextSize(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][8], new_size_ldbeat, new_size_ldbeat); 

            new Float:new_value = -0.665 * size_ldbeat;
            PlayerTextDrawTextSize(playerid,e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][6], 18.499 + 2.71 * size, new_value - (value * new_value));
            PlayerTextDrawTextSize(playerid,e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][9], 0, 0);

            PlayerTextDrawHide(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][10]);
            PlayerTextDrawHide(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][11]);

            for(new i; i < 10; i++)
                PlayerTextDrawShow(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][i]);

            return true;
        }
    }

    for(new i; i < 12; i++)
        PlayerTextDrawShow(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][i]);
    return true;
}

stock DestroyPlayerBoxRadiusAll(playerid) {

    for(new i; i < MAX_BOXR; i++) {
        if(e_BoxRadiusDraw[playerid][i][E_BOXR_USED]) {
            DestroyPlayerBoxRadiusProgress(playerid, i);
        }
    }

    return true;
}

stock DestroyPlayerBoxRadiusProgress(playerid, id) {
    
    if(id < 0 || id >= MAX_BOXR) return false;
    if(!e_BoxRadiusDraw[playerid][id][E_BOXR_USED]) return false;

    for(new i; i < MAX_BOXRDRAW; i++) {
        PlayerTextDrawDestroy(playerid, e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][i]);
        e_BoxRadiusDraw[playerid][id][E_BOXR_TEXT][i] = PlayerText:INVALID_TEXT_DRAW;
    }
    e_BoxRadiusDraw[playerid][id][E_BOXRDRAW_CREATED] = false;

    e_BoxRadiusDraw[playerid][id][E_BOXR_USED] = false;

    new dsadsa[BOX_RADIUS_ENUM];
    e_BoxRadiusDraw[playerid][id] = dsadsa;
	return true;
}

stock PlayerTextBoxSetPos(playerid, PlayerText:text, Float:x, Float:y, color)
{
    PlayerTextDrawDestroy(playerid, text);

    text = CreatePlayerTextDraw(playerid, x, y, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, text, 0, 0);
    PlayerTextDrawAlignment(playerid, text, 1);
    PlayerTextDrawColor(playerid, text, color);
    PlayerTextDrawSetShadow(playerid, text, 0);
    PlayerTextDrawSetOutline(playerid, text, 0);
    PlayerTextDrawBackgroundColor(playerid, text, 255);
    PlayerTextDrawFont(playerid, text, 4);
    PlayerTextDrawSetProportional(playerid, text, 1);
    PlayerTextDrawShow(playerid, text);
    return 1;
}
