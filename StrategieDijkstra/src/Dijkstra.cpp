#include "Dijkstra.h"



//la distance est symbolis�e par le nombre de d�s.


//idCellDepart => id de la cellue pour laquelle on cherche le plus court chemin
void initDijkstra(const SMap *map, const int nbCells, int idCellDepart) {

	// les index correspondent aux ids des cellules
	std::vector<int> distance = {}; //tableau des distances entre la cellule de d�part et une cellule 'i'
	std::vector<int> predecesseurs = {}; //tableau des predecesseurs la valeur correspond � l'id de la cellule pr�c�dente
	std::vector<bool> sommetDijkstra = {}; //tableau qui indique quels sommets ont �t� inclus dans l'arborescence

	
	for (int i = 0; i < nbCells; i++) {
		distance.push_back(INT_MAX); //initialise avec une distance � l'infini
		predecesseurs.push_back(-1); //pas de pr�d�cesseur pour le moment
		sommetDijkstra.push_back(false); //aucun sommet analys� pour le moment
	}

	//on change les donn�es pour notre cellule de d�part
	distance[idCellDepart] = 0;
	sommetDijkstra[idCellDepart] = true;

	//it�rations
	iterationDijkstra(map, nbCells, distance, predecesseurs, idCellDepart, sommetDijkstra);
	
	//a enlever - simplement pour v�rifier l'initialisation - toutes les donn�es doivent �tre initialis�es
	for (std::vector<int>::iterator it = distance.begin(); it < distance.end(); it++) {
		std::cout << "tableau distance : " << *it << std::endl;
	}
	

	//pour tester si cela fonctionne
	Affiche_Parcours_Min(distance, predecesseurs, idCellDepart, 3, map);
}

void iterationDijkstra(const SMap* map, int nbCells, std::vector<int> &distance, std::vector<int> &predecesseurs, int idCellDepart, std::vector<bool> &sommetDijkstra)
{
	//d�claration des variables
	int      newSommet = -1; //nouveau sommet � �tudier
	SCell** successeurs = nullptr;  //maillon qui parcoure notre liste
	int    minSommet = INT_MAX;  //distance du sommet successeur le plus proche
	int    distanceTMP = 0; //valeur temporaire pour savoir si on a chang� de distances
	int nbTraites = 0; //nombre de sommets trait�s
	int currentCell = -1;
	int previousCell = idCellDepart;

	while (nbTraites != nbCells) {
		//alloue la m�moire
		
		successeurs = (SCell**)malloc(sizeof(SCell)); //faire des free??

		//successeurs => tableau des voisins 
		successeurs = map->cells->neighbors;
		
		// /!\ Probleme ! on ne parcourt que les voisins directs, on ne fait pas r�ellement les successeurs

		//pour parcourir la liste des successeurs/voisins
		int i = 0;
		while (i < map->cells->nbNeighbors)
		{
			currentCell = successeurs[i]->infos.id;
			//r�cup�re la distance la plus courte (actuelle du sommet en cours)
			distanceTMP = distance[currentCell];
			//calcule la plus courte distance
			distance[currentCell] = mini(distanceTMP, distance[idCellDepart] + successeurs[i]->infos.nbDices);
			 
			//si on a modification de la distance minimale alors on a changement de pr�d�cesseur
			if (distanceTMP != distance[currentCell])
				predecesseurs[currentCell] = previousCell;
			previousCell = currentCell;
			i++;
		}

		
		//recherche le sommet suivant qui est le sommet de distance minimal non trait� par dijkstra
		for (int j = 0; j < nbCells; j++)
		{
			if (!sommetDijkstra[j])
			{
				minSommet = mini(minSommet, distance[j]);  //choisis la distance du successeur le plus proche
				if (minSommet == distance[j])               //choisis le successeur le plus proche
					newSommet = j;
			}
		}

		//ce nouveau sommet est consid�r� comme trait�
		sommetDijkstra[newSommet] = true;
		
		nbTraites++;
	}

}


int mini(int a, int b)
{
	//d�claration des variables
	int min = 0;   //minimum des deux

	if (a < b)
		min = a;

	else
		min = b;

	return min;
}


//fonction pour tester
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
		//tant qu'on n'est pas revenu au point de d�part
		while (currentCell != cellDep)
		{
			tmp = distance[currentCell]; //correspond � la distance entre la cell courante et la cellule de d�part
			std::cout << "distance par rapport au d�part -> " << tmp << std::endl;
			currentCell = predecesseurs[currentCell]; //la cellule courante prend la valeur de son pr�c�dent
			std::cout << " distance entre 2 sommets--> "<< tmp - distance[currentCell] << std::endl; //distance entre 2 sommets
		}
	}
}
