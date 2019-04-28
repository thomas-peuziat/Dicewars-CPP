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
	//unsigned int gameTurn;//vérifier si toujours égal au coup précédent
};

API_EXPORT void* InitGame(unsigned int id, unsigned int nbPlayer, const SMap *map, SPlayerInfo *info)
{
	std::cout << "InitGame" << std::endl;
	SContext *ctx = new SContext();

	strcpy(info->name, "Stratégie Dijkstra");
	strcpy(info->members[0], "COUTY Killian");
	strcpy(info->members[1], "DANIEL Florian");
	strcpy(info->members[2], "GAUDUCHEAU Clément");
	strcpy(info->members[3], "PEUZIAT Thomas");

	ctx->id = id;
	ctx->nbPlayers = nbPlayer;
	ctx->infos = info;
	ctx->map = map;

	return(ctx);
}

// 0 tour terminé 
// 1 coup terminé
API_EXPORT int PlayTurn(unsigned int gameTurn, void *ctx, const SGameState *state, STurn *turn)
{
	std::cout << "PlayTurn DIJKSTRA" << std::endl;
	SContext* contexte = static_cast<SContext*>(ctx);
	SCell *territories = static_cast<SContext*>(ctx)->map->cells;
	const SMap *map = static_cast<SContext*>(ctx)->map;
	const int nbCells = map->nbCells;

	//si le coup précédent est valide
	if (gameTurn) {
		int cellFrom = 1, cellTo = 1;
		
		//state->nbCells donne le nombre de cell dans la map
		std::cout << "Appel InitDijkstra" << std::endl;
		initDijkstra(contexte->map, state->nbCells, 0);

		std::cout << "Fin initDijkstra" << std::endl;

		//getMaxConnexite
		//trouver les territoires les plus proches en partant de chaque endroit de la plus grande composante connexe => map??
		
		std::vector<int> vConnexite = calculateConnexite(0, map, state, nbCells);
		
		for (const int& it : vConnexite) {
			std::cout << it << std::endl;
		}
		std::cout << "FIN vConnexite" << std::endl;

		std::map<unsigned int, unsigned int> nbColor = getMapWithColors(vConnexite);
		for (auto &it : nbColor) {
			std::cout << "couleur : " << it.first << " occurence : " << it.second << std::endl;
		}
		
		int maxiConnexite = getMaxConnexite(nbColor);
		std::cout << "MAX Connexite : " << maxiConnexite << std::endl;

		/*
			Pour tous nos territoires (ou que la moitié puisqu'on va essayer de tous les rejoindre ? a vérifier)
				Récupérer un territoire de départ
				Calculer le plus court chemin avec tous les autres disponibles (même s'ils appartiennent à la même composante connexe)

			Dans le cas ou les plus courts chemins sont égaux
				Sélectionner le territoire attaquant avec la plus grosse différence de dés par rapport au 1er défenseur
				attaquer avec le territoire ayant le plus petit nombre de dés
					CAR - 2 dés contre 1 : environ 74% de chance de gagner
						- 3 dés contre 2 : environ 69% de chance de gagner
		*/
	}
	return 0;
}

API_EXPORT void EndGame(void *ctx, unsigned int idWinner)
{
	std::cout << "EndGame" << std::endl;
}
