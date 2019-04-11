#include "interface_lib.h"
#include <cstring>
#include <iostream>
#include <time.h>

#ifdef _MSC_VER
#pragma warning(disable:4996)	// disable _CRT_SECURE_NO_WARNINGS
#endif


struct SContext
{
	int id;
	int nbPlayers;
	SPlayerInfo* infos;
	const SMap* map;
	unsigned int gameTurn;//v�rifier si toujours �gal au coup pr�c�dent
};

API_EXPORT void* InitGame(unsigned int id, unsigned int nbPlayer, const SMap *map, SPlayerInfo *info)
{
	std::cout << "InitGame" << std::endl;
	SContext *ctx = new SContext();

	strcpy(info->name, "Strat�gie Dijkstra");
	strcpy(info->members[0], "COUTY Killian");
	strcpy(info->members[1], "DANIEL Florian");
	strcpy(info->members[2], "GAUDUCHEAU Cl�ment");
	strcpy(info->members[3], "PEUZIAT Thomas");

	ctx->id = id;
	ctx->nbPlayers = nbPlayer;
	ctx->infos = info;
	ctx->map = map;

	return(ctx);
}

// 0 tour termin� 
// 1 coup termin�
API_EXPORT int PlayTurn(unsigned int gameTurn, void *ctx, const SGameState *state, STurn *turn)
{
	std::cout << "PlayTurn" << std::endl;

	SCell *territories = static_cast<SContext*>(ctx)->map->cells;


	//si le coup pr�c�dent est valide
	if (gameTurn == 0) {
		int cellFrom = -1, cellTo = -1;
		
		//state->nbCells donne le nombre de cell dans la map
	}
	return 0;
}

API_EXPORT void EndGame(void *ctx, unsigned int idWinner)
{
	std::cout << "EndGame" << std::endl;
}
