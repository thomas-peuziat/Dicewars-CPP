#include <iostream>
#include "../../Commun/interface.h"
#include "../../Commun/library.h"
#include <vector>
#include<time.h>
#include "MapLoader.h"
#include "../../Commun/interface_gui.h"
#include "fonctions.h"

#define GETFUNCTION(handler,name) \
	if ((name = (p##name)GETPROC(hLib, #name)) == nullptr)\
	{\
		std::cerr << "Impossible de trouver la fonction '"#name"' dans la dll" << std::endl;\
		return(-1);\
	}



int main(int argc, char *argv[])
{
	srand(time(NULL));
	const int nbPlayers = 2;
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " libfile" << std::endl;
		return(-1);
	}

	std::cout << "Argument de la commande : '" << argv[1] << "'" << std::endl;
	std::cout << "Argument de la commande : '" << argv[2] << "'" << std::endl;

	pInitGame InitGame;
	pPlayTurn PlayTurn;
	pEndGame EndGame;

	pInitGame* tab_InitGame = (pInitGame*)malloc(sizeof(pInitGame)*NB_CELL);
	pPlayTurn* tab_PlayTurn = (pPlayTurn*)malloc(sizeof(pPlayTurn)*NB_CELL);
	pEndGame* tab_EndGame = (pEndGame*)malloc(sizeof(pEndGame)*NB_CELL);

	HLIB hLib;
	for (int i = 0; i < nbPlayers; i++)
	{
		if ((hLib = LOADLIB(argv[i+1])) == nullptr)
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
	Utilisation de la macro GETFUNCTION pour �viter de r��crire le m�me genre de code (attention � adapter dans votre projet en fonction de votre contexte !)
	G�n�re le code suivant pour 'GETFUNCTION(hLib, InitGame)' :

	if ((InitGame = (pInitGame)GETPROC(hLib, "InitGame")) == nullptr)
	{
	std::cerr << "Impossible de trouver la fonction 'InitGame' dans la dll" << std::endl;
	return(-1);
	}

	*/


	SMap map;
	SGameState state;
	SPlayerInfo player[nbPlayers];
	STurn turn;
	void *ctx[nbPlayers];
	//--void *ctxGUI;
	//--SGameTurn sGameTurn;
	//--unsigned int idTurn = 0;

	//--for (unsigned int i = 0; i < 8; ++i)
	//--	for (unsigned int j = 0; j < 2; ++j)
	//--		sGameTurn.dices[j][i] = 0;

	//--Regions regions;							// vector de vector de pair, donc la grille, � relier � la g�n�ration de SMap
	//--LoadDefaultMap(regions);

	//--SRegions *mapCells = ConvertMap(regions);	// Convert des std::vector< std::vector<std::pair<unsigned int, unsigned int>> > en SRegions*
	//--ctxGUI = InitGUI(nbPlayers, mapCells);		
	//--DeleteMap(mapCells);						// Apr�s InitGUI

	InitMap(&map);
	InitGameState(&map, &state);

	//--SetGameState(ctxGUI, idTurn, &state);			// A placer au d�but du jeu, et � chaque tour 
	for (unsigned int idxStrat = 0; idxStrat < nbPlayers; idxStrat++)
	{
		player[idxStrat].name[0] = '\0';

		for (unsigned int i = 0; i < NbMembers; ++i) {
			player[idxStrat].members[i][0] = '\0';
		}

		ctx[idxStrat] = tab_InitGame[idxStrat](idxStrat, nbPlayers, &map, &player[idxStrat]);
		//--SetPlayerInfo(ctxGUI, 1, &player[idxStrat]);		// A placer � chaque chargement de librairie de joueur.

		std::cout << "Nom de la strat�gie : '" << player[idxStrat].name << "'" << std::endl;



		for (unsigned int i = 0; i < NbMembers; ++i)
			std::cout << "Nom du membre #" << (i + 1) << " : '" << player[idxStrat].members[i] << "'" << std::endl;
	}
	


	// Interblocage lorsque tout le monde ne poss�de plus qu'un d� sur son territoire
	int fin = 0;
	int gameTurn = 1;
	bool win = false;

	do {
		// Pour chaque joueurs
		for (int i = 0; i < nbPlayers; i++) {
			fin = 0;
			gameTurn = 1;

			// Tant que le joueur fait un coup valide ou que le joueur a fini son tour
			do {
				fin = tab_PlayTurn[i](gameTurn, ctx[i], &state, &turn);
				if (!fin) {
					if (ValidAttack(&turn, &map, &state, i))								// Attaque valide
					{
						Confrontation(&turn, &state, /*--&sGameTurn, --*/i);
						//--UpdateGameState(ctxGUI, ++idTurn, &sGameTurn, &state);
					}		
					else {
						break;
					}		
				}
				win = isWin(i, &state);
			} while (fin == 1);

			if (!gameTurn)																	// Si le tour du joueur a �chou�, on retablit les param�tres
				RetablirEtat(&map, &state);
			else																			// Sinon on valide les param�tres
				ValiderEtat(&map, &state);

			if (win)
				break;
		}
	} while (!win);
	// TODO : Penser au fait qu'on utilise un tableau de ctx, un par joueur
	

	EndGame(ctx[0], 1);

	free(tab_PlayTurn);
	free(tab_InitGame);
	free(tab_EndGame);

	//--UninitGUI(ctxGUI);

	CLOSELIB(hLib);

	return(0);
}

