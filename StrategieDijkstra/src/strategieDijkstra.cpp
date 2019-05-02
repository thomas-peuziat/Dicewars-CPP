#include "interface_lib.h"
#include <cstring>
#include <iostream>
#include <time.h>

#include "Dijkstra.h"
#include "fonctionsStratDijkstra.h"


#ifdef _MSC_VER
#pragma warning(disable:4996)	// disable _CRT_SECURE_NO_WARNINGS
#endif


struct SContext
{
	int id;
	int nbPlayers;
	SPlayerInfo* infos;
	const SMap* map;
	std::vector<int> wayToFollow; //chemin de cellules à suivre pour atteindre l'autres composantes connexes
};

API_EXPORT void* InitGame(unsigned int id, unsigned int nbPlayer, const SMap *map, SPlayerInfo *info)
{
	std::cout << "InitGame" << std::endl;
	SContext *ctx = new SContext();

	strcpy(info->name, "Strategie Dijkstra");
	strcpy(info->members[0], "COUTY Killian");
	strcpy(info->members[1], "DANIEL Florian");
	strcpy(info->members[2], "GAUDUCHEAU Clement");
	strcpy(info->members[3], "PEUZIAT Thomas");

	ctx->id = id;
	ctx->nbPlayers = nbPlayer;
	ctx->infos = info;
	ctx->map = map;
	ctx->wayToFollow = {};

	return(ctx);
}

// 0 tour terminé 
// 1 coup terminé
API_EXPORT int PlayTurn(unsigned int gameTurn, void *ctx, const SGameState *state, STurn *turn)
{
	std::cout << "PlayTurn DIJKSTRA" << std::endl;

	SContext* contexte = static_cast<SContext*>(ctx);
	const SMap *map = contexte->map;
	const int nbCells = map->nbCells;
	std::vector<int> Predecesseurs = {};

	//si le coup précédent est valide
	if (gameTurn == contexte->id) {


		/*
		
		TODO : CAS OU VCONNEXITE EST VIDE => return 1 => ca evite de boucler pour rien
				ligne 248 -> erreur ??
		
		
		*/



		//l'indice correspond à l'id - la valeur correspond à sa composante connexe
		std::vector<int> vConnexite = calculateConnexite(contexte->id, map, state, nbCells);
		
		//a supprimer à la fin 
		for (const int& it : vConnexite) {
			std::cout << it << std::endl;
		}
		std::cout << "FIN vConnexite" << std::endl;
		//  --


		//map contenant le num de la composante en clé et le nombre d'occurences en valeur
		std::map<unsigned int, unsigned int> nbColor = getMapWithColors(vConnexite);
		

		//nombre maximal de territoire dans une seule composante connexe
		int maxiConnexite = getMaxConnexite(nbColor);

		//correspond à la couleur contenant le maximum de territoire dans une seule composante connexe
		int colorToFind = -1;
		for (auto &it : nbColor)
		{
			if (it.second == maxiConnexite)
				colorToFind = it.first;
		}

		std::vector<int> vIndexeCellFrom; // id des cellules que l'on possède faisant partie de la plus grande composante connexe - potentielle attaquante
		
		std::map<unsigned int, std::vector<int>> cellToJoin; //clé=la couleur ; valeur= vector d'id de cellules qui appartiennent à une composante connexe

		//remplissage de la map avec les informations précédemment récoltées
		int indexe = 0;
		for (const int& it : vConnexite) {
			
			if (it == colorToFind) {
				vIndexeCellFrom.push_back(indexe);
			}
			else {
				if (it != 0) { //on ne s'occupe pas des cellules qui ne nous appartiennent pas
					auto search = cellToJoin.find(it);
					if (search == cellToJoin.end()) {
						cellToJoin.insert(std::pair<int, std::vector<int> >(it, std::vector<int>()));
						cellToJoin[it].push_back(indexe);
					}
					else {
						cellToJoin[it].push_back(indexe);
					}

				}
			}
			indexe++;
		}


		//a supprimer à la fin 
		std::cout << "CELL FROM" << std::endl;
		for (const int& it : vIndexeCellFrom) {
			std::cout << it << std::endl;
		}
		std::cout << "CELL TO JOIN" << std::endl;
		for (auto it : cellToJoin) {
			std::cout << it.first << "--> {";

			for (int i = 0; i < it.second.size(); i++) {
				std::cout << it.second[i] << " - ";
			}
			std::cout << "}" << std::endl;

		}
		// --

		if (cellToJoin.size() > 0) {


			//construction d'une structure repertoriant les cellules attaquantes, les cellules visées et la distance les séparant
			std::map<unsigned int, std::map<int, int>> mFromToDist;
			//clé=cellule attaquante ; valeur= map [avec clé=id de cellules visées et valeur=distance jusqu'a une cellule d'arrivée]


			/*
				Problème :
				distance entre 2 cell --> dep : 0 - Arr : 6 => 7
				distance entre 2 cell --> dep : 0 - Arr : 8 => 4
				distance entre 2 cell --> dep : 1 - Arr : 6 => 10
				distance entre 2 cell --> dep : 1 - Arr : 8 => 7
				distance entre 2 cell --> dep : 7 - Arr : 6 => 7
				distance entre 2 cell --> dep : 7 - Arr : 8 => 4

				Ici, on peut attaquer la cellule 8 depuis la cellule 0 ou 7 mais on veut toujours avoir le plus de chance donc il nous faut savoir a quel moment on a le plus de dés pour y parvenir
			*/
			int distanceMinimale = INT_MAX;
			int cellToATKWithMiniDistance = -1;
			int cellDepartWithMiniDistance = -1;
			int nbApparition = 1; //Calcul le nombre d'apparition de la distance minimale -> pour essayer d'optimiser l'algo et éviter les boucles inutiles



			for (const int& it : vIndexeCellFrom) {
				int cellDepart = it;
				//ajout de la cellule attaquante
				mFromToDist.insert({ cellDepart,std::map<int,int>() });

				for (auto iteratorOnCellToJoin : cellToJoin) {//pour toutes les cellules que l'on peut attaquer (parcourt de la map)
					for (int i = 0; i < iteratorOnCellToJoin.second.size(); i++) { //parcourt du vector qui est clé de la map
						int cellToATK = iteratorOnCellToJoin.second[i];
						//calcul de la distance qui les sépare
						int distanceBetweenCell = initDijkstra(map, nbCells, cellDepart, cellToATK, Predecesseurs);
						//a supprimer à la fin
						//std::cout << "distance entre 2 cell --> dep : " << cellDepart << " - Arr : " << cellToATK << " => " << distanceBetweenCell << std::endl;
						// --
						mFromToDist[cellDepart].insert({ cellToATK, distanceBetweenCell });

						if (distanceMinimale > distanceBetweenCell) { //récupère quelle cellule attaque laquelle avec quelle distance
							distanceMinimale = distanceBetweenCell;
							cellToATKWithMiniDistance = cellToATK;
							cellDepartWithMiniDistance = cellDepart;
							nbApparition = 1; // la distance mini a changé on réinitialise le compteur
						}
						else {
							if (distanceMinimale == distanceBetweenCell) {
								nbApparition++;
							}
						}
					}
				}
			}

			/*
				si nbApparition est supérieur à 1, cela veut dire qu'il nous faut construire les vector contenant toutes les pairs de cellules {ATK, visées}
				afin de décider en fonction du nombre de dés qui pourra attaquer (les distances seront toutes les mêmes donc pas besoin de les stocker)
			*/
			if (nbApparition > 1) {
				std::vector<std::pair<int, int>> vecCellAtkDef;

				for (auto itMap : mFromToDist) {
					itMap.first;
					for (auto itMapInterne : itMap.second) {
						itMapInterne.first;
						if (distanceMinimale == itMapInterne.second) {
							vecCellAtkDef.push_back({ itMap.first, itMapInterne.first });//ajout de la cellule attaquante et celle visée
						}
					}
				}

				int cellDepWithMaxiDices = -1;
				int cellToATKWithMaxiDices = -1;

				int maxiDice = -1;
				for (auto itVec : vecCellAtkDef) {
					if (map->cells[itVec.first].infos.nbDices > maxiDice) {
						maxiDice = map->cells[itVec.first].infos.nbDices;
						cellDepWithMaxiDices = itVec.first;
						cellToATKWithMaxiDices = itVec.second;
					}
				}
				std::cout << std::endl << std::endl;
				std::cout << "AVEC VECTOR - distance Mini " << distanceMinimale << " DES :" << maxiDice << std::endl;
				turn->cellFrom = cellDepWithMaxiDices;

				//construction du chemin à suivre
				constructWayToFollow(contexte->wayToFollow, Predecesseurs, cellDepWithMaxiDices, cellToATKWithMaxiDices);

				turn->cellTo = contexte->wayToFollow[contexte->wayToFollow.size() - 1];
				
				if (maxiDice + 1 > map->cells[turn->cellTo].infos.nbDices) {//si on a au moins 1 dé de plus que l'adversaire
					//on attaque
					contexte->wayToFollow.pop_back();
					std::cout << "cellDep => " << cellDepWithMaxiDices << " cellArr => " << cellToATKWithMaxiDices << " nbDice => " << maxiDice << std::endl;
					return 0;
				}
				else {
					//on attaque en fonction du nbDes
					std::cout << "J'ATTAQUE PAS DEPUIS DIJKSTRA MAIS NBDES AVEC VECTOR" << std::endl;
					attaquerEnFonctionNbDes(vIndexeCellFrom, map, turn);
					contexte->wayToFollow.clear();
					return 1;
				}

			}
			else { //si 1 seule apparition, on évite de reboucler
				std::cout << "SANS VECTOR - distance MINI : " << distanceMinimale << " DES :" << map->cells[cellDepartWithMiniDistance].infos.nbDices << std::endl;
				std::cout << "cellDep => " << cellDepartWithMiniDistance << " cellArr => " << cellToATKWithMiniDistance << " nbDice => " << map->cells[cellDepartWithMiniDistance].infos.nbDices << std::endl;

				//construction du chemin à suivre
				constructWayToFollow(contexte->wayToFollow, Predecesseurs, cellDepartWithMiniDistance, cellToATKWithMiniDistance);


				int nbDiceAttaquant = map->cells[cellDepartWithMiniDistance].infos.nbDices;
				int nbDiceAttaque = map->cells[contexte->wayToFollow[contexte->wayToFollow.size() - 1]].infos.nbDices;
				
				if (nbDiceAttaquant + 1 > nbDiceAttaque) {
					//on attaque
					turn->cellFrom = cellDepartWithMiniDistance;
					turn->cellTo = contexte->wayToFollow[contexte->wayToFollow.size() - 1];
					contexte->wayToFollow.pop_back();
					return 0;
				}
				else {
					//on attaque en fonction du nombre de des
					std::cout << "J'ATTAQUE PAS DEPUIS DIJKSTRA MAIS NBDES SANS VECTOR" << std::endl;
					attaquerEnFonctionNbDes(vIndexeCellFrom, map, turn);
					contexte->wayToFollow.clear();
					return 1;
				}

			}
		}
		else {
			//si cellToJoin == 0
			//Cas où cellToJoin est vide => dijkstra ne sert a rien et il faut quand meme attaquer

			std::cout << "ATTAQUE NBDES CAR 1 seule composante connexe " << std::endl;

			attaquerEnFonctionNbDes(vIndexeCellFrom, map, turn);

			//std::cout << "dep : " << turn->cellFrom << " arr : " << turn->cellTo << " diff des => " << diffMaximale << std::endl;
		}
	
	}
	contexte->wayToFollow.clear();
	return 1;
}

API_EXPORT void EndGame(void *ctx, unsigned int idWinner)
{
	std::cout << "EndGame" << std::endl;
}
