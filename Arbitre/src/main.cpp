#include <iostream>
#include "../../Commun/interface.h"
#include "../../Commun/library.h"
#include <vector>
#include <time.h>
#include "MapLoader.h"
#include "../../Commun/interface_gui.h"
#include "fonctions.h"
#include "generation.h"
#include <thread>
#include <chrono>

#define GETFUNCTION(handler,name) \
	if ((name = (p##name)GETPROC(hLib, #name)) == nullptr)\
	{\
		std::cerr << "Impossible de trouver la fonction '"#name"' dans la dll" << std::endl;\
		return(-1);\
	}


void LoadMapPerso(Regions &regions, MapTerritoire map) {
	for (auto iterator : map) {
		std::set<Coordinates> coor = iterator.second;
		std::vector<std::pair<unsigned int,unsigned int>> monVector;
		for (auto it2 : coor) {

			monVector.push_back(std::make_pair(it2.first, it2.second));
		}
		
		regions.push_back(monVector);
	}
}

void LoadMapTest(Regions &regions) {
	regions.push_back({ {1,1}, {1,2} });
	regions.push_back({ {2,1} });
	regions.push_back({ {2,3}, {2,4}, {3,4} });
	regions.push_back({ {4,5} });
	regions.push_back({ {5,4}, {6,5} });
	regions.push_back({ {6,6} });
	regions.push_back({ {7,4} });
	regions.push_back({ {3,1}, {3,2}, {3,3} });
	regions.push_back({ {4,6}, {5,6} });
	regions.push_back({ {4,7} });
}


int main(int argc, char *argv[])
{
	srand(time(NULL));
	const int nbPlayers = (argc - 1);
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " libfile" << std::endl;
		return(-1);
	}

	for (int i = 1; i < argc; i++)
		std::cout << "Argument de la commande : '" << argv[i] << "'" << std::endl;

	pInitGame InitGame;
	pPlayTurn PlayTurn;
	pEndGame EndGame;

	pInitGame* tab_InitGame = (pInitGame*)malloc(sizeof(pInitGame)*nbPlayers);
	pPlayTurn* tab_PlayTurn = (pPlayTurn*)malloc(sizeof(pPlayTurn)*nbPlayers);
	pEndGame* tab_EndGame = (pEndGame*)malloc(sizeof(pEndGame)*nbPlayers);

	HLIB hLib = {};
	for (int i = 0; i < nbPlayers; i++)
	{
		if ((hLib = LOADLIB(argv[i + 1])) == nullptr)
		{
			std::cerr << "Impossible de charger la librairie '" << argv[i + 1] << "'" << std::endl;
			return(-1);
		}

		else {

			GETFUNCTION(hLib, InitGame);
			tab_InitGame[i] = InitGame;

			GETFUNCTION(hLib, PlayTurn);
			tab_PlayTurn[i] = PlayTurn;

			GETFUNCTION(hLib, EndGame);
			tab_EndGame[i] = EndGame;
		}
	}



	/*
	Utilisation de la macro GETFUNCTION pour éviter de réécrire le même genre de code (attention à adapter dans votre projet en fonction de votre contexte !)
	Génère le code suivant pour 'GETFUNCTION(hLib, InitGame)' :

	if ((InitGame = (pInitGame)GETPROC(hLib, "InitGame")) == nullptr)
	{
	std::cerr << "Impossible de trouver la fonction 'InitGame' dans la dll" << std::endl;
	return(-1);
	}

	*/

	
	SMap map;
	SGameState state;
	std::vector<SPlayerInfo> player;
	STurn turn;
	std::vector<void *> ctx;
	//std::map<int, std::set<Coordinates>> maMap;
	//maMap = initialisationMap();
	MapTerritoire maMap = InitMap(&map, 10, 10, 10, nbPlayers);
	InitGameState(&map, &state, nbPlayers);

	
	//InitMap(&map);
	//InitGameState(&map, &state, nbPlayers);

	

	void *ctxGUI;
	SGameTurn sGameTurn;
	

	Regions regions;							// vector de vector de pair, donc la grille, à relier à la génération de SMap
	//LoadDefaultMap(regions);
	//LoadMapTest(regions);
	LoadMapPerso(regions, maMap);
	
	SRegions *mapCells = ConvertMap(regions);	// Convert des std::vector< std::vector<std::pair<unsigned int, unsigned int>> > en SRegions*
	ctxGUI = InitGUI(nbPlayers, mapCells);		
	DeleteMap(mapCells);						// Après InitGUI

	int a;
	std::cin >> a;

	for (unsigned int idxStrat = 0; idxStrat < nbPlayers; idxStrat++)
	{
		
		player[idxStrat].name[0] = '\0';

		for (unsigned int i = 0; i < NbMembers; ++i) {
			player[idxStrat].members[i][0] = '\0';
		}
		
		ctx[idxStrat] = tab_InitGame[idxStrat](idxStrat, nbPlayers, &map, &player[idxStrat]);
		
		SetPlayerInfo(ctxGUI, idxStrat, &player[idxStrat]);		// A placer à chaque chargement de librairie de joueur.

		std::cout << "Nom de la strategie : '" << player[idxStrat].name << "'" << std::endl;



		for (unsigned int i = 0; i < NbMembers; ++i)
			std::cout << "Nom du membre #" << (i + 1) << " : '" << player[idxStrat].members[i] << "'" << std::endl;
	}

	unsigned int idTurn = 0;
	
	for (unsigned int i = 0; i < 8; ++i)
		for (unsigned int j = 0; j < 2; ++j)
			sGameTurn.dices[j][i] = 0;
	updatePoints(nbPlayers, &state, &map);
	SetGameState(ctxGUI, idTurn, &state);			// A placer au début du jeu, et à chaque tour 
	
	int b;
	std::cin >> b;
	int fin = 0;
	int gameTurn = 1;
	bool win = false;
	int winnerPlayer = -1;

	idTurn++;
	do {
		// Pour chaque joueurs 
		//mettre i à 1 si on veut tester que la 2eme stratégie
		for (int i = 0; i < nbPlayers; i++) {
			
			fin = 0;
			gameTurn = i;
			// Tant que le joueur fait un coup valide ou que le joueur a fini son tour
			do {
				fin = tab_PlayTurn[i](gameTurn, ctx[i], &state, &turn);
				
				if (!fin) {
					if (ValidAttack(&turn, &map, &state, i))								// Attaque valide
					{
						Confrontation(&turn, &state, &sGameTurn, i);
						updatePoints(nbPlayers, &state, &map);
						UpdateGameState(ctxGUI, ++idTurn, &sGameTurn, &state);
						
						/*int a;
						std::cin >> a;*/
						std::this_thread::sleep_for(std::chrono::seconds(3));
					}		
					else {
						gameTurn++;
						break;
					}		
				}
				win = isWin(i, &state);
			} while (!fin && !win);

			if (gameTurn != i)																	// Si le tour du joueur a échoué, on retablit les paramètres
				RetablirEtat(&map, &state);
			else {																			// Sinon on valide les paramètres	
				ValiderEtat(&map, &state);
				updatePoints(nbPlayers, &state, &map);
				int nbDes = state.points[i];
				std::cout << nbDes << std::endl;
				distributionDes(i, nbDes, &state, &map);
			}
			SetGameState(ctxGUI, idTurn, &state);
			if (win)
				winnerPlayer = i;
				break;
			
		}
	} while (!win);

	std::cout << "Nb tours " << idTurn << std::endl;

	for (unsigned int i = 0; i < nbPlayers; ++i)
		tab_EndGame[i](ctx[i], winnerPlayer);

	free(tab_PlayTurn);
	free(tab_InitGame);
	free(tab_EndGame);

	std::cout << "avant" << std::endl;
	UninitGUI(ctxGUI);
	std::cout << "avant2" << std::endl;
	CLOSELIB(hLib);
	std::cout << "après" << std::endl;
	return(0);
}




