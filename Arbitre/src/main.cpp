#include <iostream>
#include "../../Commun/interface.h"
#include "../../Commun/library.h"
#include <vector>

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


int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " libfile" << std::endl;
		return(-1);
	}

	std::cout << "Argument de la commande : '" << argv[1] << "'" << std::endl;

	HLIB hLib;
	if ((hLib = LOADLIB(argv[1])) == nullptr)
	{
		std::cerr << "Impossible de charger la librairie '" << argv[1] << "'" << std::endl;
		return(-1);
	}

	pInitGame InitGame;
	pPlayTurn PlayTurn;
	pEndGame EndGame;

	/*
	Utilisation de la macro GETFUNCTION pour �viter de r��crire le m�me genre de code (attention � adapter dans votre projet en fonction de votre contexte !)
	G�n�re le code suivant pour 'GETFUNCTION(hLib, InitGame)' :

	if ((InitGame = (pInitGame)GETPROC(hLib, "InitGame")) == nullptr)
	{
	std::cerr << "Impossible de trouver la fonction 'InitGame' dans la dll" << std::endl;
	return(-1);
	}

	*/
	GETFUNCTION(hLib, InitGame);
	GETFUNCTION(hLib, PlayTurn);
	GETFUNCTION(hLib, EndGame);

	SMap map;
	SGameState state;
	SPlayerInfo player;
	STurn turn;
	void *ctx[1];

	InitMap(&map);

	for (unsigned int i = 0; i < NbMembers; ++i) {
		player.members[i][0] = '\0';
	}
	std::cout << "Nom de la strat�gie : '" << player.name << "'" << std::endl;

	ctx[0] = InitGame(0, 3, &map, &player);

	for (unsigned int i = 0; i < NbMembers; ++i)
		std::cout << "Nom du membre #" << (i + 1) << " : '" << player.members[i] << "'" << std::endl;
	int res = 0;
	int gameTurn = 0;

	// TODO : Penser au fait qu'on utilise un tableau de ctx, un par joueur
	do {
		res = PlayTurn(gameTurn, ctx[0], &state, &turn);
		if (res != 0)
		{
			int gameTurn = ValidAttack(&turn, &map, &state, 0);
		}

	} while (res != 0 && gameTurn == 0);


	if (gameTurn == 1)							// Si le tour du joueur a �chou�, on retablit les param�tres
	{
		RetablirEtat(&map, &state);
	}
	else										// Sinon on valide les param�tres
	{
		ValiderEtat(&map, &state);
	}

	EndGame(ctx[0], 1);

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
		c.infos.owner = rand() % 5 + 1;
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

	if (cellInfoTo.owner == playerID ||		// Si on s'attaque soi-m�me
		cellInfoFrom.owner != playerID ||	// Si on ne poss�de pas la cellule
		cellInfoFrom.nbDices <= 1)			// Si on ne poss�de pas assez de d�s
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

