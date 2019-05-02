#include "interface_lib.h"
#include "Utility.h"
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
	SContext *ctx = new SContext();

	strcpy(info->name, "Strategie Bourgeoise");
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

// 0 coup terminé 
// 1 tour terminé
API_EXPORT int PlayTurn(unsigned int gameTurn, void *ctx, const SGameState *state, STurn *turn)
{
	//std::cout << "PlayTurn Strategie 2" << std::endl;
	SContext* contexte = static_cast<SContext*>(ctx);
	SCell *territories = static_cast<SContext*>(ctx)->map->cells;

	int cellFrom = -1;
	int cellTo = -1;
	int diffDes = -1;
	int diffDesMax = 0;
	int nbDesAtk = 10;
	int nbDesAtkMin = 10;

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
							// La cellule n'est pas au joueur à moi
							neighborCell = state->cells[neigId];
							diffDes = myCell.nbDices - neighborCell.nbDices;
							nbDesAtk = myCell.nbDices;

							// Si la différence de dés est la plus grande et si le joueur attaque avec moins de dés
							if (diffDes >= diffDesMax && nbDesAtk < nbDesAtkMin ) {

								// Si la différence de dés est supérieure à 0
								if (diffDes > 0) {
									cellFrom = myCell.id;
									cellTo = neighborCell.id;
									diffDesMax = diffDes;
									nbDesAtk = nbDesAtk;
								}

								// Si la différence de dés est égale à zéro
								// Le but est d'attaquer un territoire possédant le même nombre de dés seulement si la prise de risque peut être compensé par la récupération d'un nombre de dés max
								else if(diffDes == 0){

									int nbTerritories = getNbTerritories(contexte->id, state);;
									int nbDices = getNbDices(contexte->id, state);
									// Calcul du nombre maximum de dés sur l'ensemble des territoires
									int nbDicesMax = nbTerritories * 8;

									int nbDicesStock = state->diceStock[contexte->id];

									// Calcul du besoin de dés pour avoir tous mes territoires à 8 dés après une attaque
									int nbDiceNeed = (((nbDicesMax - nbDices) + myCell.nbDices) - 1);

									// Si la compensation est suffisante, on peut attaquer
									if (nbDiceNeed < (nbDicesStock + state->points[contexte->id])) {
										cellFrom = myCell.id;
										cellTo = neighborCell.id;
										diffDesMax = diffDes;
										nbDesAtk = nbDesAtk;
									}
								}
								
								
							}
						}
					}
				}

			}
		}

		// Mise à jour du turn
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
	SContext* contexte = static_cast<SContext*>(ctx);
	if (idWinner == contexte->id)
		std::cout << "J'ai gagné !" << std::endl;

	delete ctx;
}
