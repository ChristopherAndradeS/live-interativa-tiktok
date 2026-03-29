CMD:convidar(playerid, params[])
{
	new tmp[128], idx;
	tmp = strtok(params, idx);
	if(!strlen(tmp))return SendClientMessage(playerid, BRANCO, "USO: /convidar [NomePlayer/ID]");
	new playa;
	playa = ReturnUser(tmp);
	/*if(PlayerInfo[playa][pMembro] > 0)return SendClientMessage(playa,BRANCO,"Este player já possui uma Organização.");*/
	if(playa == INVALID_PLAYER_ID) return SendClientMessage(playerid, BRANCO, "Jogador não existe.");
	GetPlayerName(playa, giveplayer, sizeof(giveplayer));
	GetPlayerName(playerid, name, sizeof(name));
	new ftext[30];
	if(PlayerInfo[playerid][pLider] == 1) ftext = "LSPD";
	format(string, sizeof(string), "[ATENÇÃO %s]:%s lhe mandou um convite para entrar para a Organização: [%s].",giveplayer,name,ftext);
	SendClientMessage(playa,VERMELHO,string);
	SendClientMessage(playa,VERMELHO,"Você aceita este convite?");
	ShowPlayerDialog(playa, 28, DIALOG_STYLE_LIST, "Convite", "Aceito.\nNão Aceito.", "Confirmar","Cancelar"); //mostrará o dialog
	InviteJob[playa] = PlayerInfo[playerid][pLider]; InviteOffer[playa] = playerid;//Detalhe - irá setar a variavel o ID da org/qm mandou convite
	SendClientMessage(playerid, BRANCO, "Você enviou o convite.");
	return 1;
}