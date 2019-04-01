#include "interface_lib.h"
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
#pragma warning(disable:4996)	// disable _CRT_SECURE_NO_WARNINGS
#endif


struct SContext
{
	int id;
	int nbPlayers;
	SPlayerInfo* infos;
};

API_EXPORT void* InitGame(unsigned int id, unsigned int nbPlayer, const SMap *map, SPlayerInfo *info)
{
	std::cout << "InitGame" << std::endl;
	SContext *ctx = new SContext();

	strcpy(info->name, "Test de librairie");
	strcpy(info->members[0], "COUTY Killian");
	strcpy(info->members[1], "DANIEL Florian");
	strcpy(info->members[2], "GAUDUCHEAU Clément");
	strcpy(info->members[3], "PEUZIAT Thomas");
	
	ctx->id = id;
	ctx->nbPlayers = nbPlayer;
	ctx->infos = info;

	return(ctx);
}

API_EXPORT int PlayTurn(unsigned int gameTurn, void *ctx, const SGameState *state, STurn *turn)
{
	std::cout << "PlayTurn" << std::endl;

	return(0);
}

API_EXPORT void EndGame(void *ctx, unsigned int idWinner)
{
	std::cout << "EndGame" << std::endl;
}
