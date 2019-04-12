#include "interface_lib.h"
#include <cstring>
#include <iostream>
#include <time.h>

#ifdef _MSC_VER
#pragma warning(disable:4996)	// disable _CRT_SECURE_NO_WARNINGS
#endif


struct SContext
{
	int id;
	int nbPlayers;
	SPlayerInfo* infos;
	const SMap* map;
	unsigned int gameTurn;//v�rifier si toujours �gal au coup pr�c�dent
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
	std::cout << "PlayTurn" << std::endl;

	SCell *territories = static_cast<SContext*>(ctx)->map->cells;


	//si le coup pr�c�dent est valide
	if (gameTurn == 0) {
		int cellFrom = -1, cellTo = -1;
		
		//state->nbCells donne le nombre de cell dans la map

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
	}
	return 0;
}

API_EXPORT void EndGame(void *ctx, unsigned int idWinner)
{
	std::cout << "EndGame" << std::endl;
}
