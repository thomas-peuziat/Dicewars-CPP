#include "interface_lib.h"
#include <cstring>
#include <iostream>
#include <time.h>

#include "Dijkstra.h"
#include "CalculConnexite.h"


#ifdef _MSC_VER
#pragma warning(disable:4996)	// disable _CRT_SECURE_NO_WARNINGS
#endif


struct SContext
{
	int id;
	int nbPlayers;
	SPlayerInfo* infos;
	const SMap* map;
	//unsigned int gameTurn;//v�rifier si toujours �gal au coup pr�c�dent
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
	std::cout << "PlayTurn DIJKSTRA" << std::endl;
	SContext* contexte = static_cast<SContext*>(ctx);
	SCell *territories = static_cast<SContext*>(ctx)->map->cells;
	const SMap *map = static_cast<SContext*>(ctx)->map;
	const int nbCells = map->nbCells;

	//si le coup pr�c�dent est valide
	if (gameTurn) {
		//int cellFrom = 1, cellTo = 1;
		
		//state->nbCells donne le nombre de cell dans la map
		//std::cout << "Appel InitDijkstra" << std::endl;
		//initDijkstra(contexte->map, state->nbCells, 0);

		//std::cout << "Fin initDijkstra" << std::endl;

		//getMaxConnexite
		//trouver les territoires les plus proches en partant de chaque endroit de la plus grande composante connexe => map??
		
		//l'indice correspond � l'id - la valeur correspond � sa composante connexe
		std::vector<int> vConnexite = calculateConnexite(0, map, state, nbCells);
		
		for (const int& it : vConnexite) {
			std::cout << it << std::endl;
		}
		std::cout << "FIN vConnexite" << std::endl;

		//map contenant le num de la composante en cl� et le nombre d'occurences en valeur
		std::map<unsigned int, unsigned int> nbColor = getMapWithColors(vConnexite);
		//for (auto &it : nbColor) {
		//	std::cout << "couleur : " << it.first << " occurence : " << it.second << std::endl;
		//}
		
		//nombre maximal de territoire dans une seule composante connexe
		int maxiConnexite = getMaxConnexite(nbColor);
		//std::cout << "MAX Connexite : " << maxiConnexite << std::endl;

		//correspond � la couleur contenant le maximum de territoire dans une seule composante connexe
		int colorToFind = -1;
		for (auto &it : nbColor)
		{
			if (it.second == maxiConnexite)
				colorToFind = it.first;
		}

		std::vector<int> vIndexeCellFrom; // id des cellules que l'on poss�de faisant partie de la plus grande composante connexe - potentielle attaquante
		std::vector<int> vecTMP;
		
		std::map<unsigned int, std::vector<int>> cellToJoin; //cl�=la couleur ; valeur= vector d'id de cellules

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
		

		//construction d'une structure repertoriant les cellules attaquantes, les cellules vis�es et la distance les s�parant


		std::map<unsigned int, std::map<int,int>> mFromToDist;
		//cl�=cellule attaquante ; valeur= map [avec cl�=id de cellules vis�es et valeur=distance jusqu'a une cellule d'arriv�e]


		//utilis� pour les tests et dans le cas sp�cifique ou la distance minimale n'apparait qu'une seule fois
		/*
			Exemple :
			distance entre 2 cell --> dep : 0 - Arr : 6 => 7
			distance entre 2 cell --> dep : 0 - Arr : 8 => 4
			distance entre 2 cell --> dep : 1 - Arr : 6 => 10
			distance entre 2 cell --> dep : 1 - Arr : 8 => 7
			distance entre 2 cell --> dep : 7 - Arr : 6 => 7
			distance entre 2 cell --> dep : 7 - Arr : 8 => 4

			Ici, on peut attaquer la cellule 8 depuis la cellule 0 ou 7 mais on veut toujours avoir le plus de chance donc il nous faut savoir a quel moment on a le plus de d�s pour y parvenir
		*/
		int distanceMinimale = INT_MAX;
		int cellToATKWithMiniDistance = -1;
		int cellFrom = -1;
		int nbApparition = 1; //Calcul le nombre d'apparition de la distance minimale -> pour essayer d'optimiser l'algo et �viter les boucles inutiles

		




		for (const int& it : vIndexeCellFrom) {
			//ajout de la cellule attaquante
			int cellDepart = it;
			mFromToDist.insert({ cellDepart,std::map<int,int>() });
			for (auto iteratorOnCellToJoin : cellToJoin) {
				//pour toutes les cellules que l'on peut attaquer
				for (int i = 0; i < iteratorOnCellToJoin.second.size(); i++) {
					int cellToATK = iteratorOnCellToJoin.second[i];
					//calcul de la distance qui les s�pare
					int distanceBetweenCell = initDijkstra(map, nbCells,cellDepart, cellToATK);
					std::cout << "distance entre 2 cell --> dep : " << cellDepart << " - Arr : " << cellToATK << " => " << distanceBetweenCell << std::endl;
					mFromToDist[cellDepart].insert({ cellToATK, distanceBetweenCell });

					if (distanceMinimale > distanceBetweenCell) { //r�cup�re quelle cellule attaque laquelle avec quelle distance
						distanceMinimale = distanceBetweenCell;
						cellToATKWithMiniDistance = cellToATK;
						cellFrom = cellDepart;
						std::cout << "REINIT nbApparition" << std::endl;
						nbApparition = 1; // la distance mini a chang� on r�initialise le compteur
					}
					else {
						if (distanceMinimale == distanceBetweenCell) {
							std::cout << "INCREMENTE nbApparition" << std::endl;
							nbApparition++;
						}
					}
				}
			}
		}

		/*
			si nbApparition est sup�rieur � 1, cela veut dire qu'il nous faut construire les vector contenant toutes les pairs de cellules {ATK, vis�es}
			afin de d�cider en fonction du nombre de d�s
		*/

		//Si le cas sp�cifique se pr�sente, on peut utiliser des vector (les distances seront toutes les m�mes donc pas besoin de les stocker)
		if (nbApparition > 1) {
			std::vector<std::pair<int, int>> vecCellAtkDef;

			for (auto itMap : mFromToDist) {
				itMap.first;
				for (auto itMapInterne : itMap.second) {
					itMapInterne.first;
					if (distanceMinimale == itMapInterne.second) {
						vecCellAtkDef.push_back({ itMap.first, itMapInterne.first });//ajout de la cellule attaquante et celle vis�e
					}
				}
			}

			int cellATKWithMaxiDices = -1;
			int cellDEFWithMaxiDices = -1;

			int maxiDice = -1;
			for (auto itVec : vecCellAtkDef) {
				if (map->cells[itVec.first].infos.nbDices > maxiDice) {
					maxiDice = map->cells[itVec.first].infos.nbDices;
					cellATKWithMaxiDices = itVec.first;
					cellDEFWithMaxiDices = itVec.second;
				}
			}
			std::cout << std::endl << std::endl;
			std::cout << "cellDep => " << cellATKWithMaxiDices << " cellArr => " << cellDEFWithMaxiDices << " nbDice => " << maxiDice << std::endl;
		}
		else { //si 1 seule apparition
			/*turn->cellFrom = cellFrom;
			turn->cellTo = cellToATKWithMiniDistance;*/
		}

		//if (cellToATKWithMiniDistance!=-1){
		//	std::cout << std::endl << std::endl;
		//	std::cout << "cellDepart? => " << cellFrom << "cellArrivee? => " << cellToATKWithMiniDistance << " distance ? => " << distanceMinimale << std::endl;
		//}
	


		//attaque avec la plus grande diff�rence de d�s

		/*
			Pour tous nos territoires (ou que la moiti� puisqu'on va essayer de tous les rejoindre ? a v�rifier)
				R�cup�rer un territoire de d�part
				Calculer le plus court chemin avec tous les autres disponibles (m�me s'ils appartiennent � la m�me composante connexe)

			Dans le cas ou les plus courts chemins sont �gaux
				S�lectionner le territoire attaquant avec la plus grosse diff�rence de d�s par rapport au 1er d�fenseur
				attaquer avec le territoire ayant le plus petit nombre de d�s
					CAR - 2 d�s contre 1 : environ 74% de chance de gagner
						- 3 d�s contre 2 : environ 69% de chance de gagner
		*/

		//NB : Penser au cas o� il ne reste que 2 joueurs avec chacun 1 et 1 seule composante connexe => dijkstra ne sert a rien et il faut quand meme attaquer
	}
	return 0;
}

API_EXPORT void EndGame(void *ctx, unsigned int idWinner)
{
	std::cout << "EndGame" << std::endl;
}
