#include <iostream>
#include "../../Commun/interface.h"
#include "../../Commun/library.h"

#define GETFUNCTION(handler,name) \
	if ((name = (p##name)GETPROC(hLib, #name)) == nullptr)\
	{\
		std::cerr << "Impossible de trouver la fonction '"#name"' dans la dll" << std::endl;\
		return(-1);\
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
	Utilisation de la macro GETFUNCTION pour éviter de réécrire le même genre de code (attention à adapter dans votre projet en fonction de votre contexte !)
	Génère le code suivant pour 'GETFUNCTION(hLib, InitGame)' :

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
	void *ctx[NbMembers];

	for (unsigned int i = 0; i < NbMembers; ++i) {
		player.members[i][0] = '\0';
		ctx[i] = InitGame(i, 3, &map, &player);
		std::cout << "Nom de la stratégie : '" << player.name << "'" << std::endl;
	}

	for (unsigned int i = 0; i < NbMembers; ++i)
		std::cout << "Nom du membre #" << (i + 1) << " : '" << player.members[i] << "'" << std::endl;
	int res = 0;
	int gameTurn = 0;

	// TODO : Penser au fait qu'on utilise un tableau de ctx, un par joueur
	do {
		res = PlayTurn(gameTurn, ctx, &state, &turn);
		if (res != 0) 
		{
			int gameTurn = ValidAttack(&turn, &map, &state, /*playerID*/);
		}
		
	} while (res != 0 && gameTurn == 0);


	if (gameTurn == 1)							// Si le tour du joueur a échoué, on retablit les paramètres
	{
		RetablirEtat(&map, &state);
	}
	else										// Sinon on valide les paramètres
	{
		ValiderEtat(&map, &state);
	}

	EndGame(ctx, 1);

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

