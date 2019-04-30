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
	unsigned int gameTurn;//vérifier si toujours égal au coup précédent
};

API_EXPORT void* InitGame(unsigned int id, unsigned int nbPlayer, const SMap *map, SPlayerInfo *info)
{
	std::cout << "InitGame" << std::endl;
	SContext *ctx = new SContext();

	strcpy(info->name, "Strategie 2");
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

// 0 tour terminé 
// 1 coup terminé
API_EXPORT int PlayTurn(unsigned int gameTurn, void *ctx, const SGameState *state, STurn *turn)
{
	//std::cout << "PlayTurn Strategie 2" << std::endl;
	SContext* contexte = static_cast<SContext*>(ctx);
	SCell *territories = static_cast<SContext*>(ctx)->map->cells;

	int cellFrom = -1;
	int cellTo = -1;
	int diffDes = -1;
	int diffDesMax = -1;

	SCellInfo myCell;
	SCellInfo neighborCell;

	if (gameTurn == contexte->id)							// Le coup précédent est correct				
	{

		for (int i = 0; i < contexte->map->nbCells; i++) {
			if (state->cells[i].owner == contexte->id) {
				// Si la cellule est celle du joueur
				myCell = state->cells[i];
				if (myCell.nbDices > 1)
				{


					for (int j = 0; j < territories[myCell.id].nbNeighbors; j++) {
						// Pour chaque voisin de la celulle en cours

						int neigId = territories[myCell.id].neighbors[j]->infos.id;
						if (state->cells[neigId].owner != (contexte->id)) {
							// La cellule n'est pas au joueur
							neighborCell = state->cells[neigId];
							diffDes = myCell.nbDices - neighborCell.nbDices;

							if (diffDes >= 0 && diffDes >= diffDesMax) {
								cellFrom = myCell.id;
								cellTo = neighborCell.id;
								diffDesMax = diffDes;
							}
						}
					}
				}

			}
		}

		if (cellFrom != -1 && cellTo != -1) {
			turn->cellFrom = cellFrom;
			turn->cellTo = cellTo;
			return 0;
		}
		else {
			return 1;
		}
	}

	return 1;
}

API_EXPORT void EndGame(void *ctx, unsigned int idWinner)
{
	std::cout << "EndGame" << std::endl;
	delete ctx;
}
