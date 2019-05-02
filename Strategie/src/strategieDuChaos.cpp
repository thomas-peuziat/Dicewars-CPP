#include "interface_lib.h"
#include <cstring>
#include <iostream>
#include <time.h>
#include <vector>

#ifdef _MSC_VER
#pragma warning(disable:4996)	// disable _CRT_SECURE_NO_WARNINGS
#endif


struct SContext
{
	int id;
	int nbPlayers;
	SPlayerInfo* infos;
	const SMap* map;
};

API_EXPORT void* InitGame(unsigned int id, unsigned int nbPlayer, const SMap *map, SPlayerInfo *info)
{
	SContext *ctx = new SContext();

	strcpy(info->name, "Stratégie du chaos");
	strcpy(info->members[0], "COUTY Killian");
	strcpy(info->members[1], "DANIEL Florian");
	strcpy(info->members[2], "GAUDUCHEAU Clement");
	strcpy(info->members[3], "PEUZIAT Thomas");
	
	ctx->id = id;
	ctx->nbPlayers = nbPlayer;
	ctx->infos = info;
	ctx->map = map;
	
	return(ctx);
}

API_EXPORT int PlayTurn(unsigned int gameTurn, void *ctx, const SGameState *state, STurn *turn)
{

	SCell *territories = static_cast<SContext*>(ctx)->map->cells;
	SContext *contexte = static_cast<SContext*>(ctx);


	if (gameTurn == contexte->id)							// Le coup précédent est correct				
	{
		int cellFrom = -1;
		int cellTo = -1;
		std::vector<int> tab_own(6);
		int idx_own = 0;
		int idx_adj = 0;

		int ret = rand() % 2;					// Tirage aléatoire pour déterminer si le joueur va faire un coup
		if (ret != 0)
		{
			// Parcours de toutes les celulles de la map
			for (int i = 0; i < state->nbCells; i++)
			{
				// On ne garde que celles dont on est le propriétaire
				if (state->cells[i].owner == contexte->id && state->cells[i].nbDices > 1)
				{
					tab_own[idx_own] = i;
					idx_own += 1;
				}
			}


			if (idx_own > 0)								// Si au moins 1 des celulles est jouable
			{
				int cellFrom = tab_own[rand() % idx_own];

				int nbVoisins = territories[cellFrom].nbNeighbors;
				int cellTo = territories[cellFrom].neighbors[rand() % nbVoisins]->infos.id;

				// Mise à jour du turn
				turn->cellFrom = cellFrom;
				turn->cellTo = cellTo;
				return 0;
			}
		}

	}

	return 1;
}

API_EXPORT void EndGame(void *ctx, unsigned int idWinner)
{

	SContext* contexte = static_cast<SContext*>(ctx);
	if (idWinner == contexte->id)
		std::cout << "J'ai gagné !" << std::endl;

	delete ctx;

}
