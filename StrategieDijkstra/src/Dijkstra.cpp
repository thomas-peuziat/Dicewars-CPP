#include "Dijkstra.h"


//idCellDepart => id de la cellule pour laquelle on cherche le plus court chemin
//tableau des predecesseurs la valeur correspond à l'id de la cellule précédente
int initDijkstra(const SMap *map, const int nbCells, int idCellDepart, int idCellArrivee, std::vector<int>& predecesseurs) {

	// les index correspondent aux ids des cellules
	std::vector<int> distance = {}; //tableau des distances entre la cellule de départ et une cellule 'i'
	std::vector<bool> sommetDijkstra = {}; //tableau qui indique quels sommets ont été inclus dans l'arborescence

	
	for (int i = 0; i < nbCells; i++) {
		distance.push_back(INT_MAX); //initialise avec une distance à l'infini
		predecesseurs.push_back(-1); //pas de prédécesseur pour le moment
		sommetDijkstra.push_back(false); //aucun sommet analysé pour le moment
	}

	//on change les données pour notre cellule de départ
	distance[idCellDepart] = 0;
	sommetDijkstra[idCellDepart] = true;

	//itérations
	iterationDijkstra(map, nbCells, distance, predecesseurs, idCellDepart, sommetDijkstra);
		
	return distance[idCellArrivee];
}

void iterationDijkstra(const SMap* map, int nbCells, std::vector<int> &distance, std::vector<int> &predecesseurs, int idCellDepart, std::vector<bool> &sommetDijkstra)
{
	//déclaration des variables
	int      newSommet = -1; //nouveau sommet à étudier
	std::vector<SCell**> tabVoisinsCell;  //maillon qui parcoure notre liste
	int    minSommet = INT_MAX;  //distance du sommet successeur le plus proche
	int    distanceTMP = 0; //valeur temporaire pour savoir si on a changé de distances
	int nbTraites = 0; //nombre de sommets traités
	int currentVoisin = -1;
	int currentCell = idCellDepart;

	while (nbTraites != nbCells) {
		

		tabVoisinsCell.push_back(map->cells[currentCell].neighbors);	// tableau des voisins d'une cellule

		
		
		//pour parcourir la liste des voisins d'une cellule
		int i = 0;
		while (i < map->cells[currentCell].nbNeighbors)
		{
			currentVoisin = tabVoisinsCell[nbTraites][i]->infos.id;
			//récupère la distance la plus courte (actuelle du sommet en cours)
			distanceTMP = distance[currentVoisin];
			//calcule la plus courte distance
			distance[currentVoisin] = mini(distanceTMP, distance[currentCell] + tabVoisinsCell[nbTraites][i]->infos.nbDices);
			
			//si on a modification de la distance minimale alors on a changement de prédécesseur
			if (distanceTMP != distance[currentVoisin])
				predecesseurs[currentVoisin] = currentCell;
			
			i++;
		}
		
		
		//recherche le sommet suivant qui est le sommet de distance minimal non traité par dijkstra
		for (int j = 0; j < nbCells; j++)
		{
			if (!sommetDijkstra[j])//s'il n'est pas encore traité
			{
				minSommet = mini(minSommet, distance[j]);  //choisis la distance du successeur le plus proche
				if (minSommet == distance[j])               //choisis le successeur le plus proche
					newSommet = j;
			}
		}

		//ce nouveau sommet est considéré comme traité
		sommetDijkstra[newSommet] = true;
		minSommet = INT_MAX;
		currentCell = newSommet;

		nbTraites++;
	}
	
}


int mini(int a, int b)
{
	int min = 0;   //minimum des deux

	if (a < b)
		min = a;
	else
		min = b;

	return min;
}


//fonction pour tester et visualiser le trajet effectué (en partant de l'arrivée)
void Affiche_Parcours_Min(std::vector<int> &distance, std::vector<int> &predecesseurs, int cellDep, int cellArr, const SMap* map)
{
	int currentCell = cellArr;
	int tmp = 0;

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
			tmp = distance[currentCell]; //correspond à la distance entre la cell courante et la cellule de départ
			std::cout << "distance totale par rapport au depart -> " << tmp << std::endl;
			currentCell = predecesseurs[currentCell]; //la cellule courante prend la valeur de son précédent
			std::cout << " distance entre sommet actuel et sommet suivant "<< tmp - distance[currentCell] << std::endl; //distance entre 2 sommets
			
		}
	}
}
