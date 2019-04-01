#include <iostream>
#include "../../Commun/interface.h"
#include "../../Commun/library.h"

#define GETFUNCTION(handler,name) \
	if ((name = (p##name)GETPROC(hLib, #name)) == nullptr)\
	{\
		std::cerr << "Impossible de trouver la fonction '"#name"' dans la dll" << std::endl;\
		return(-1);\
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
	void *ctx;

	for (unsigned int i = 0; i < NbMembers; ++i)
		player.members[i][0] = '\0';
	ctx = InitGame(1, 3, &map, &player);
	std::cout << "Nom de la stratégie : '" << player.name << "'" << std::endl;


	for (unsigned int i = 0; i < NbMembers; ++i)
		std::cout << "Nom du membre #" << (i + 1) << " : '" << player.members[i] << "'" << std::endl;

	PlayTurn(0, ctx, &state, &turn);

	EndGame(ctx, 1);

	CLOSELIB(hLib);

	return(0);
}
