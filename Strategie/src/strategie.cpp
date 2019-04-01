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
	ctx->map = map;

	return(ctx);
}

API_EXPORT int PlayTurn(unsigned int gameTurn, void *ctx, const SGameState *state, STurn *turn)
{
	std::cout << "PlayTurn" << std::endl;


	SCell *territories = static_cast<SContext*>(ctx)->map->cells;

	srand(time(NULL));


	
	if (gameTurn == 0)							// Le coup précédent est correct				
	{
		int cellFrom = -1;
		int cellTo = -1;
		int *tab_own = new int();
		int *tab_adj = new int();
		int idx_own = 0;
		int idx_adj = 0;

		int ret = rand() % 2;					// Tirage aléatoire pour déterminer si le joueur va faire un coup
		if (ret == 1)
		{
			// Parcours de toute les celulles de la map
			for (int i = 0; i < static_cast<SContext*>(ctx)->map->nbCells; i++)
			{
				// On ne garde que celle dont on est le propriétaire
				if (territories[i].infos.owner == static_cast<SContext*>(ctx)->id && territories[i].infos.nbDices > 1)
				{
					tab_own[idx_own] = i;
					i++;
				}
			}


			if (idx_own > 0)								// Si au moins 1 des celulle est jouable
			{
				int nbVoisins = territories[cellFrom].nbNeighbors;

				int cellFrom = tab_own[rand() % idx_own];
				int cellTo = territories[cellFrom].neighbors[rand() % nbVoisins]->infos.id;

				turn->cellFrom = cellFrom;
				turn->cellTo = cellTo;
				return 1;
			}
		}

	}

	return 0;
}

API_EXPORT void EndGame(void *ctx, unsigned int idWinner)
{
	std::cout << "EndGame" << std::endl;
}
