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

	std::vector<pInitGame> tab_InitGame(nbPlayers);
	std::vector<pPlayTurn> tab_PlayTurn(nbPlayers);
	std::vector<pEndGame> tab_EndGame(nbPlayers);

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
	std::vector<SPlayerInfo> player(nbPlayers);
	STurn turn;
	std::vector<void *> ctx(nbPlayers);
	MapTerritoire maMap = InitMap(&map, 25, 30, 30, nbPlayers);
	InitGameState(&map, &state, nbPlayers);

	void *ctxGUI;
	SGameTurn sGameTurn;
	

	Regions regions;							// vector de vector de pair, donc la grille, à relier à la génération de SMap
	LoadMapPerso(regions, maMap);
	
	SRegions *mapCells = ConvertMap(regions);	// Convert des std::vector< std::vector<std::pair<unsigned int, unsigned int>> > en SRegions*
	ctxGUI = InitGUI(nbPlayers, mapCells);		
	DeleteMap(mapCells);						// Après InitGUI

	std::cout << "Pour lancer le jeu, pensez a actualiser localhost:5678 puis appuyez sur une touche et appuyez sur Entree." << std::endl;
	int waiting;
	std::cin >> waiting;

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
	
	int fin = 0;
	int gameTurn = 1;
	bool win = false;
	int winnerPlayer = -1;

	std::cout << "========== JEU LANCE ===========" << std::endl;
	std::cout << "(Verifiez localhost:5678)" << std::endl;

	idTurn++;
	do {
		
		// Pour chaque joueurs 
		// mettre i à 1 si on veut tester que la 2eme stratégie
		for (int p = 0; p < nbPlayers; p++) {
			
			fin = 0;
			gameTurn = p;
			// Tant que le joueur fait un coup valide ou que le joueur a fini son tour
			do {
				fin = tab_PlayTurn[p](gameTurn, ctx[p], &state, &turn);
				
				if (!fin) {
					if (ValidAttack(&turn, &map, &state, p))					// Attaque valide
					{
						Confrontation(&turn, &state, &sGameTurn, p);
						updatePoints(nbPlayers, &state, &map);
						UpdateGameState(ctxGUI, ++idTurn, &sGameTurn, &state);

						// Permet d'attendre pour bien voir l'affichage d'un coup
						std::this_thread::sleep_for(std::chrono::seconds(2));
					}		
					else {
						gameTurn++;
						break;
					}		
				}
				win = isWin(p, &state);
			} while (!fin && !win);

			if (gameTurn != p)					// Si le tour du joueur a échoué, on retablit les paramètres
				RetablirEtat(&map, &state);
			else {								// Sinon on valide les paramètres	
				ValiderEtat(&map, &state);
				updatePoints(nbPlayers, &state, &map);
				int nbDes = state.points[p];
				distributionDes(p, nbDes, &state, &map);
			}
			SetGameState(ctxGUI, idTurn, &state);
			if (win) {
				winnerPlayer = p;
				break;
			}
		}
	} while (!win);

	std::cout << "Partie finie en " << idTurn << " tours." << std::endl;
	std::cout << "==========================" << std::endl;


	for (unsigned int i = 0; i < nbPlayers; ++i)
		tab_EndGame[i](ctx[i], winnerPlayer);


	UninitGUI(ctxGUI);
	CLOSELIB(hLib);
	return(0);
}
