#include "Dijkstra.h"
#include "../../Commun/interface.h"

#include <vector>
#include <limits.h>

void initDijkstra(SMap *map, const int nbCells, int idCellDepart) {

	std::vector<int> distance = {}; //tableau des distances entre les cellules
	std::vector<int> pred = {}; //tableau des predecesseurs
	std::vector<bool> dijkstra = {}; //tableau qui indique quels sommets ont été inclus dans l'arborescence

	
	for (int i = 0; i < nbCells; i++) {
		distance[i] = INT_MAX; //init distance à l'infini
		pred[i] = -1; //pas de prédécesseur pour le moment
		dijkstra[i] = false; //aucun sommet analysé pour le moment
	}

	//on change pour notre cellule de départ
	distance[idCellDepart] = 0;
	dijkstra[idCellDepart] = true;

	// TODO faire itérations
}