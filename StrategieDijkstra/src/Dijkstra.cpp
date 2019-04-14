#include "Dijkstra.h"



//la distance est symbolisée par le nombre de dés.


//idCellDepart => id de la cellue pour laquelle on cherche le plus court chemin
void initDijkstra(const SMap *map, const int nbCells, int idCellDepart) {

	std::vector<int> distance = {}; //tableau des distances entre les cellules
	std::vector<int> predecesseurs = {}; //tableau des predecesseurs
	std::vector<bool> sommetDijkstra = {}; //tableau qui indique quels sommets ont été inclus dans l'arborescence

	
	for (int i = 0; i < nbCells; i++) {
		distance[i] = INT_MAX; //initialise avec une distance à l'infini
		predecesseurs[i] = -1; //pas de prédécesseur pour le moment
		sommetDijkstra[i] = false; //aucun sommet analysé pour le moment
	}

	//on change les données pour notre cellule de départ
	distance[idCellDepart] = 0;
	sommetDijkstra[idCellDepart] = true;

	//itérations
	iterationDijkstra(map, nbCells, distance, predecesseurs, idCellDepart, sommetDijkstra);
	Affiche_Parcours_Min(distance, predecesseurs, 1, 2, map);
}

void iterationDijkstra(const SMap* map, int nbCells, std::vector<int> &distance, std::vector<int> &predecesseurs, int idCellDepart, std::vector<bool> &sommetDijkstra)
{
	//déclaration des variables
	int      newSommet = -1; //nouveau sommet à étudier
	SCell** successeurs = nullptr;  //maillon qui parcoure notre liste
	int    minSommet = INT_MAX;  //distance du sommet successeur le plus proche
	int    distanceTMP = 0; //valeur temporaire pour savoir si on a changé de distances
	int i = 0; //boucle
	int nbTraites = 0; //nombre de sommets traités

	while (nbTraites != nbCells) {
		//alloue la mémoire
		successeurs = (SCell**)malloc(sizeof(SCell));
		successeurs = map->cells->neighbors;

		//pour parcourir la liste des successeurs
		int i = 0;
		while (successeurs[i] != NULL)
		{
			//récupère la distance la plus courte (actuelle du sommet en cours)
			distanceTMP = distance[successeurs[i]->infos.id];
			//calcule la plus courte distance
			distance[successeurs[i]->infos.id] = mini(distanceTMP, distance[idCellDepart] + successeurs[i]->infos.nbDices); 
			 
			//si on a modification de la distance minimale alors on a changement de prédécesseur
			if (distanceTMP != distance[successeurs[i]->infos.id])
				predecesseurs[successeurs[i]->infos.id] = idCellDepart;
			i++;
		}

		//recherche le sommet suivant qui est le sommet de distance minimal non traité par dijkstra
		for (int j = 0; j < nbCells; j++)
		{
			if (sommetDijkstra[j] == 0)
			{
				minSommet = mini(minSommet, distance[j]);  //choisis la distance du successeur le plus proche
				if (minSommet == distance[j])               //choisis le successeur le plus proche
					newSommet = j;
			}
		}

		//ce nouveau sommet est considéré comme traité
		sommetDijkstra[newSommet] = 1;
		nbTraites++;
	}

}


int mini(int a, int b)
{
	//déclaration des variables
	int min = 0;   //minimum des deux

		//si a est le plus petit alors a est le minimum
	if (a < b)
		min = a;

	//sinon c'est b le minimum
	else
		min = b;

	return min;
}

// TODO a tester
void Affiche_Parcours_Min(std::vector<int> &distance, std::vector<int> &predecesseurs, int cellDep, int cellArr, const SMap* map)
{
	int currentCell = cellArr;
	int tmp = 0;

	//affiche le parcours minimal
	std::cout << "Trajet : \n" << std::endl;

	//si la distance est infinie
	if (distance[cellArr] == INT_MAX)
		std::cout << "Il n'y a pas de trajet\n" << std::endl;

	//s'il y'a un trajet possible
	else
	{
		//tant qu'on n'est pas revenu au point de départ
		while (currentCell != cellDep)
		{
			tmp = distance[currentCell];      //sauvegarde de la distance à la racine
			//affiche le nom du sommet successeur et la distance par rapport au départ
			std::cout << "distance par rapport au départ -> " << distance[currentCell] << std::endl;
			currentCell = predecesseurs[currentCell]; //le successeur devient le prédecesseur
			std::cout << " distance entre 2 sommets--> "<< tmp - distance[currentCell] << std::endl; //distance entre 2 sommets
		}
		std::cout << "point de départ" << map[cellDep].cells->infos.id << std::endl; //affiche le point de départ
	}
}
