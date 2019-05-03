#include "interface_lib.h"

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

	return(ctx);
}

// 0 tour termin� 
// 1 coup termin�
API_EXPORT int PlayTurn(unsigned int gameTurn, void *ctx, const SGameState *state, STurn *turn)
{
	//std::cout << "PlayTurn DIJKSTRA" << std::endl;

	SContext* contexte = static_cast<SContext*>(ctx);
	const SMap *map = contexte->map;
	const int nbCells = map->nbCells;
	std::vector<int> Predecesseurs(state->nbCells,-1); //contiendra les pr�d�cesseurs en fonction d'une cellule de d�part et d'arriv�e
	std::vector<int> wayToFollow = {};    //chemin de cellules � suivre pour atteindre l'autres composantes connexes
	
	//si le coup pr�c�dent est valide
	if (gameTurn == contexte->id) {


		//l'indice correspond � l'id de la cellule - la valeur correspond � sa composante connexe
		std::vector<int> vConnexite = calculateConnexite(contexte->id, map, state, nbCells);
		
		//si on poss�de au moins 1 cellule
		if (vConnexite.size() > 0) {


			//map contenant le num de la composante en cl� et le nombre d'occurences en valeur
			std::map<unsigned int, unsigned int> nbColor = getMapWithColors(vConnexite);


			//nombre maximal de territoire dans une seule composante connexe
			int maxiConnexite = getMaxConnexite(nbColor);

			//correspond � la couleur contenant le maximum de territoire dans une seule composante connexe
			int colorToFind = -1;
			for (auto &it : nbColor)
			{
				if (it.second == maxiConnexite)
					colorToFind = it.first;
			}

			std::vector<int> vIndexeCellFrom; // id des cellules que l'on poss�de faisant partie de la plus grande composante connexe - potentielle attaquante

			std::map<unsigned int, std::vector<int>> cellToJoin; //cl�=la couleur ; valeur= vector d'id de cellules qui appartiennent � une composante connexe

			//remplissage de la map avec les informations pr�c�demment r�colt�es
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


			//si au moins une cellule � rejoindre
			if (cellToJoin.size() > 0) {

				/*
					Probl�me :
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
				int cellDepartWithMiniDistance = -1;
				int nbApparition = 1; //Calcul le nombre d'apparition de la distance minimale -> pour essayer d'optimiser l'algo et �viter les boucles inutiles


				std::map<unsigned int, std::map<int, int>> mFromToDist;
				//cl�=cellule attaquante ; valeur= map [avec cl�=id de cellules vis�es et valeur=distance jusqu'a une cellule d'arriv�e]

				//construction d'une structure repertoriant les cellules attaquantes, les cellules vis�es et la distance les s�parant
				for (const int& it : vIndexeCellFrom) {
					int cellDepart = it;
					//ajout de la cellule attaquante
					mFromToDist.insert({ cellDepart,std::map<int,int>() });

					for (auto iteratorOnCellToJoin : cellToJoin) {//pour toutes les cellules que l'on peut attaquer (parcourt de la map)
						for (int i = 0; i < iteratorOnCellToJoin.second.size(); i++) { //parcourt du vector qui est cl� de la map
							int cellToATK = iteratorOnCellToJoin.second[i];
							//calcul de la distance qui les s�pare
							int distanceBetweenCell = initDijkstra(map, nbCells, cellDepart, cellToATK, Predecesseurs);
							mFromToDist[cellDepart].insert({ cellToATK, distanceBetweenCell });

							if (distanceMinimale > distanceBetweenCell) { //r�cup�re quelle cellule attaque laquelle avec quelle distance
								distanceMinimale = distanceBetweenCell;
								cellToATKWithMiniDistance = cellToATK;
								cellDepartWithMiniDistance = cellDepart;
								nbApparition = 1; // la distance mini a chang� on r�initialise le compteur
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
					si nbApparition est sup�rieur � 1, cela veut dire qu'il nous faut construire les vector contenant toutes les pairs de cellules {ATK, vis�es}
					afin de d�cider en fonction du nombre de d�s qui pourra attaquer (les distances seront toutes les m�mes donc pas besoin de les stocker)
				*/
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

					int cellDepWithMaxiDices = -1;
					int cellToATKWithMaxiDices = -1;

					int maxiDice = -1;
					for (auto itVec : vecCellAtkDef) {
						if (state->cells[itVec.first].nbDices > maxiDice) {
							maxiDice = state->cells[itVec.first].nbDices;
							cellDepWithMaxiDices = itVec.first;
							cellToATKWithMaxiDices = itVec.second;
						}
					}

					if (cellDepWithMaxiDices != -1 && cellToATKWithMaxiDices != 1) {
						//construction du chemin � suivre
						initDijkstra(map, nbCells, cellDepWithMaxiDices, cellToATKWithMaxiDices, Predecesseurs);//permet de red�finir le vector de Predecesseurs
						constructWayToFollow(wayToFollow, Predecesseurs, cellDepWithMaxiDices, cellToATKWithMaxiDices);

							
						if ((maxiDice + 1 > state->cells[wayToFollow[wayToFollow.size() - 1]].nbDices) && (maxiDice > 1)) {//si on a au moins 1 d� de plus que l'adversaire
							//on attaque si la cellule attaqu�e ne nous appartient pas d�j�
							if (state->cells[cellDepWithMaxiDices].owner != state->cells[wayToFollow[wayToFollow.size() - 1]].owner) {
								turn->cellFrom = cellDepWithMaxiDices;
								turn->cellTo = wayToFollow[wayToFollow.size() - 1];

								wayToFollow.clear();
								return 0;
							}
							else {
								return 1;
							}
						}
						else {
							//on attaque en fonction du nbDes
							if (AttaqueCouteQueCoute(nbCells, contexte->id, state, map, turn)) {
								wayToFollow.clear();
								return 0;
							}
							else {
								return 1;
							}
						}
					}
					else {
						//on attaque en fonction du nbDes
						if (AttaqueCouteQueCoute(nbCells, contexte->id, state, map, turn)) {
							wayToFollow.clear();
							return 0;
						}
						else {
							return 1;
						}
					}

				}//end nbApparition>1
				else { //si 1 seule apparition, on �vite de reboucler

					if (cellDepartWithMiniDistance != -1 && cellToATKWithMiniDistance != -1) {
						//construction du chemin � suivre
						initDijkstra(map, nbCells, cellDepartWithMiniDistance, cellToATKWithMiniDistance, Predecesseurs);//permet de red�finir le vector de Predecesseurs
						constructWayToFollow(wayToFollow, Predecesseurs, cellDepartWithMiniDistance, cellToATKWithMiniDistance);


						int nbDiceAttaquant = state->cells[cellDepartWithMiniDistance].nbDices;
						int nbDiceAttaque = state->cells[wayToFollow[wayToFollow.size() - 1]].nbDices;

						if ((nbDiceAttaquant + 1 > nbDiceAttaque) && (nbDiceAttaquant > 1)) {
							if (state->cells[cellDepartWithMiniDistance].owner != state->cells[wayToFollow[wayToFollow.size() - 1]].owner) {
								//on attaque
								turn->cellFrom = cellDepartWithMiniDistance;
								turn->cellTo = wayToFollow[wayToFollow.size() - 1];
								wayToFollow.clear();
								return 0;
							}
							else {
								return 1;
							}
						}
						else {
							//on attaque en fonction du nbDes
							if (AttaqueCouteQueCoute(nbCells, contexte->id, state, map, turn)) {
								wayToFollow.clear();
								return 0;
							}
							else {
								return 1;
							}
						}
						
					}
					else {
						//on attaque en fonction du nombre de des
						if (AttaqueCouteQueCoute(nbCells, contexte->id, state, map, turn)) {
							wayToFollow.clear();
							return 0;
						}
						else {
							return 1;
						}
					}
					
				}
			}//end cellToJoin.size()>0
			else {
				//si cellToJoin == 0
				//Cas o� cellToJoin est vide (1 seule composante connexe)=> dijkstra ne sert a rien et il faut quand meme attaquer
				//on attaque en fonction du nbDes
				if (AttaqueCouteQueCoute(nbCells, contexte->id, state, map, turn)) {
					wayToFollow.clear();
					return 0;
				}
				else {
					return 1;
				}
			}
		}//end vConnexite > 0
		else {
			std::cout << "Aucun territoire" << std::endl;
		}
	}//end gameTurn == id

	wayToFollow.clear();
	return 1;
}

API_EXPORT void EndGame(void *ctx, unsigned int idWinner)
{
	SContext* contexte = static_cast<SContext*>(ctx);
	if (idWinner == contexte->id)
		std::cout << "J'ai gagne !" << std::endl;
	else
		std::cout << "J'ai perdu !" << std::endl;

	delete ctx;
}
