#include <iostream>
#include "../../Commun/interface.h"
#include "../../Commun/library.h"
#include <vector>
#include<time.h>
#include "MapLoader.h"
#include "../../Commun/interface_gui.h"

#define GETFUNCTION(handler,name) \
	if ((name = (p##name)GETPROC(hLib, #name)) == nullptr)\
	{\
		std::cerr << "Impossible de trouver la fonction '"#name"' dans la dll" << std::endl;\
		return(-1);\
	}

const int NB_CELL = 10;

void InitMap(SMap *map);
void RetablirEtat(const SMap *map, SGameState *state);
void ValiderEtat(SMap *map, const SGameState*state);
bool ValidAttack(const STurn *turn, const SMap *map, const SGameState *state, int playerID);
void InitGameState(const SMap *map, SGameState *state);
void Confrontation(const STurn *turn, SGameState *state, /*--SGameTurn *sGameTurn, --*/int idPlayer);
int getNbTerritories(int IDPlayer, SGameState *state);
//void PlayGame(const int nbPlayers, void* ctx[], const SGameState *state, const SMap *map, pInitGame* tab_InitGame, pPlayTurn* tab_PlayTurn , pEndGame* tab_InitGame);


int main(int argc, char *argv[])
{
	srand(time(NULL));
	const int nbPlayers = 5;
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " libfile" << std::endl;
		return(-1);
	}

	std::cout << "Argument de la commande : '" << argv[1] << "'" << std::endl;

	pInitGame InitGame;
	pPlayTurn PlayTurn;
	pEndGame EndGame;

	pInitGame* tab_InitGame = (pInitGame*)malloc(sizeof(pInitGame)*NB_CELL);
	pPlayTurn* tab_PlayTurn = (pPlayTurn*)malloc(sizeof(pPlayTurn)*NB_CELL);
	pEndGame* tab_EndGame = (pEndGame*)malloc(sizeof(pEndGame)*NB_CELL);

	HLIB hLib;
	for (int i = 0; i < nbPlayers; i++)
	{
		if ((hLib = LOADLIB(argv[1])) == nullptr)
		{
			std::cerr << "Impossible de charger la librairie '" << argv[1] << "'" << std::endl;
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
	SPlayerInfo player;
	STurn turn;
	void *ctx[nbPlayers];
	//--void *ctxGUI;
	//--SGameTurn sGameTurn;
	//--unsigned int idTurn = 0;

	//--for (unsigned int i = 0; i < 8; ++i)
	//--	for (unsigned int j = 0; j < 2; ++j)
	//--		sGameTurn.dices[j][i] = 0;

	//--Regions regions;							// vector de vector de pair, donc la grille, à relier à la génération de SMap
	//--LoadDefaultMap(regions);

	//--SRegions *mapCells = ConvertMap(regions);	// Convert des std::vector< std::vector<std::pair<unsigned int, unsigned int>> > en SRegions*
	//--ctxGUI = InitGUI(nbPlayers, mapCells);		
	//--DeleteMap(mapCells);						// Après InitGUI

	InitMap(&map);
	InitGameState(&map, &state);

	//--SetGameState(ctxGUI, idTurn, &state);			// A placer au début du jeu, et à chaque tour 

	player.name[0] = '\0';

	for (unsigned int i = 0; i < NbMembers; ++i) {
		player.members[i][0] = '\0';
	}
	
	for (int i = 0; i < nbPlayers; i++)
	{
		ctx[i] = tab_InitGame[i](i, nbPlayers, &map, &player);
		//--SetPlayerInfo(ctxGUI, 1, &player);		// A placer à chaque chargement de librairie de joueur.
	}
	
	std::cout << "Nom de la stratégie : '" << player.name << "'" << std::endl;
	
		

	for (unsigned int i = 0; i < NbMembers; ++i)
		std::cout << "Nom du membre #" << (i + 1) << " : '" << player.members[i] << "'" << std::endl;


	// Interblocage lorsque tout le monde ne possède plus qu'un dé sur son territoire
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
				//win = isWin(i, &state);
			} while (fin == 1);

			if (!gameTurn)																	// Si le tour du joueur a échoué, on retablit les paramètres
				RetablirEtat(&map, &state);
			else																			// Sinon on valide les paramètres
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

void RetablirEtat(const SMap *map, SGameState *state)
{
	for (int i = 0; i < map->nbCells; i++)
		state->cells[i] = map->cells[i].infos;
}

void ValiderEtat(SMap *map, const SGameState*state)
{
	for (int i = 0; i < map->nbCells; i++)
		map->cells[i].infos = state->cells[i];
}

void InitMap(SMap *map)
{
	map->cells = (SCell*)malloc(sizeof(SCell)*NB_CELL);
	SCell cell[NB_CELL];
	std::vector<SCell*> ptcell;
	for (auto i = 0; i < NB_CELL; i++)
	{
		SCell c;
		c.infos.id = i;
		c.infos.owner = rand() % 5;
		c.infos.nbDices = rand() % 6 + 1;
		cell[i] = c;


	}


	for (auto i = 0; i < NB_CELL; i++)
	{
		map->cells[i] = cell[i];
	}
	map->nbCells = NB_CELL;


	SCell* ptcell0 = &map->cells[0];
	SCell* ptcell1 = &map->cells[1];
	SCell* ptcell2 = &map->cells[2];
	SCell* ptcell3 = &map->cells[3];
	SCell* ptcell4 = &map->cells[4];
	SCell* ptcell5 = &map->cells[5];
	SCell* ptcell6 = &map->cells[6];
	SCell* ptcell7 = &map->cells[7];
	SCell* ptcell8 = &map->cells[8];
	SCell* ptcell9 = &map->cells[9];


	std::vector<SCell*> v1 = { ptcell1, ptcell2 };
	map->cells[0].nbNeighbors = 2;
	map->cells[0].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[0].nbNeighbors);
	for (auto i = 0; i < map->cells[0].nbNeighbors; i++)
	{
		map->cells[0].neighbors[i] = v1[i];
	}
	std::vector<SCell*> v2 = { ptcell0, ptcell7 };

	map->cells[1].nbNeighbors = 2;
	map->cells[1].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[1].nbNeighbors);
	for (auto i = 0; i < map->cells[1].nbNeighbors; i++)
	{
		map->cells[1].neighbors[i] = v2[i];
	}

	std::vector<SCell*> v3 = { ptcell0,ptcell7, ptcell3 };

	map->cells[2].nbNeighbors = 3;
	map->cells[2].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[2].nbNeighbors);
	for (auto i = 0; i < map->cells[2].nbNeighbors; i++)
	{
		map->cells[2].neighbors[i] = v3[i];
	}

	std::vector<SCell*> v4 = { ptcell2,ptcell4, ptcell8 };

	map->cells[3].nbNeighbors = 3;
	map->cells[3].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[3].nbNeighbors);
	for (auto i = 0; i < map->cells[3].nbNeighbors; i++)
	{
		map->cells[3].neighbors[i] = v4[i];
	}

	std::vector<SCell*> v5 = { ptcell6, ptcell5, ptcell3 };

	map->cells[4].nbNeighbors = 3;
	map->cells[4].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[4].nbNeighbors);
	for (auto i = 0; i < map->cells[4].nbNeighbors; i++)
	{
		map->cells[4].neighbors[i] = v5[i];
	}

	std::vector<SCell*> v6 = { ptcell4, ptcell8 };

	map->cells[5].nbNeighbors = 2;
	map->cells[5].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[5].nbNeighbors);
	for (auto i = 0; i < map->cells[5].nbNeighbors; i++)
	{
		map->cells[5].neighbors[i] = v6[i];
	}

	std::vector<SCell*> v7 = { ptcell4 };

	map->cells[6].nbNeighbors = 1;
	map->cells[6].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[6].nbNeighbors);
	for (auto i = 0; i < map->cells[6].nbNeighbors; i++)
	{
		map->cells[6].neighbors[i] = v7[i];
	}

	std::vector<SCell*> v8 = { ptcell2, ptcell1 };

	map->cells[7].nbNeighbors = 2;
	map->cells[7].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[7].nbNeighbors);
	for (auto i = 0; i < map->cells[7].nbNeighbors; i++)
	{
		map->cells[7].neighbors[i] = v8[i];
	}

	std::vector<SCell*> v9 = { ptcell3, ptcell5, ptcell9 };


	map->cells[8].nbNeighbors = 3;
	map->cells[8].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[8].nbNeighbors);
	for (auto i = 0; i < map->cells[8].nbNeighbors; i++)
	{
		map->cells[8].neighbors[i] = v9[i];
	}

	std::vector<SCell*> v10 = { ptcell8 };

	map->cells[9].nbNeighbors = 1;
	map->cells[9].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[9].nbNeighbors);
	for (auto i = 0; i < map->cells[9].nbNeighbors; i++)
	{
		map->cells[9].neighbors[i] = v10[i];
	}
}

bool ValidAttack(const STurn *turn, const SMap *map, const SGameState *state, int playerID) {
	const SCell& cellFrom = map->cells[turn->cellFrom];
	const SCellInfo& cellInfoFrom = state->cells[cellFrom.infos.id];
	const SCell& cellTo = map->cells[turn->cellTo];
	const SCellInfo& cellInfoTo = state->cells[cellTo.infos.id];

	if (cellInfoTo.owner == playerID ||		// Si on s'attaque soi-même
		cellInfoFrom.owner != playerID ||	// Si on ne possède pas la cellule
		cellInfoFrom.nbDices <= 1)			// Si on ne possède pas assez de dés
		return false;

	bool isNeighbor = false;
	for (int i = 0; i < cellFrom.nbNeighbors; i++) {
		if (cellFrom.neighbors[i]->infos.id == cellTo.infos.id) {
			isNeighbor = true;
			break;
		}
	}

	if (!isNeighbor)
		return false;		// La cellule n'est pas voisine

	return true;	// Le coup est valide
}

void InitGameState(const SMap *map, SGameState *state)
{
	state->cells = (SCellInfo*)malloc(sizeof(SCellInfo)*NB_CELL);
	for (int i = 0; i < map->nbCells; i++)
		state->cells[i] = map->cells[i].infos;

	state->nbCells = NB_CELL;
}

void Confrontation(const STurn *turn, SGameState *state, /*--SGameTurn* sGameTurn, --*/int idPlayer)
{
	int NbDicesFrom = state->cells[turn->cellFrom].nbDices;
	int NbDicesTo = state->cells[turn->cellTo].nbDices;

	int TotalFrom = 0;
	int TotalTo = 0;

	int scoreDes;

	for (int i = 0; i < NbDicesFrom; i++) {
		scoreDes = (rand() % 6) + 1;
		TotalFrom += scoreDes;
		//--sGameTurn->dices[0][i] = scoreDes;
	}
		
	for (int i = 0; i < NbDicesTo; i++) {
		scoreDes = (rand() % 6) + 1;
		TotalTo += scoreDes;
		//--sGameTurn->dices[1][i] = scoreDes;
	}

	if (TotalFrom > TotalTo)				// si l'attaquant a gagné
	{
		state->cells[turn->cellTo].owner = state->cells[turn->cellFrom].owner;
		state->cells[turn->cellTo].nbDices = state->cells[turn->cellFrom].nbDices - 1;
		state->cells[turn->cellFrom].nbDices = 1;

	}
	else									// si l'attaquant a perdu 
	{
		state->cells[turn->cellFrom].nbDices = 1;
	}
	//--sGameTurn->idPlayer = idPlayer;
	//--sGameTurn->turn = *(turn);
}

int getNbTerritories(int IDPlayer, SGameState *state) {
	int nbTerr = 0;
	for (int i = 0; i < state->nbCells; i++) 
	{
		if (state->cells[i].owner == IDPlayer)
			nbTerr++;
	}
	return nbTerr;
}