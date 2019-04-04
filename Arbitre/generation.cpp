#include "../Commun/interface.h"

#include <iostream>
#include <vector>

const int NB_CELL = 30;


SCell* generateCell(int nbCellToGenerate)
{
	SCellInfo infos;
	infos.id = 1;
	infos.owner = 1;
	infos.nbDices = 2;

	SCell *maCell = {};
	maCell->infos = infos;

	return nullptr;
}
/* Création d'une map */
int CreateMap() {
	SMap m = SMap();
	/* Génération d'une matrice pour la création des cellules 
	Simulation hexagonale */
	int matrix[10][20] = {{-1}};

	// Générer aléatoirement 2 chiffres (i et j)

	return 0;
}

//(int,int) 

/* Méthode pour l'adjacence d'une cellule */
bool IsAdjacentPoint(int tab[10][20],int id_territory,int i_test,int j_test) {
	bool res = false;
	/* Prendre tous les points de chaque territoire et vérifier si adjacent*/

	
	return res;
}

bool isFreePoint(int tab[10][20],int i, int j){
	return (tab[i][j] == -1);
}

bool isConnected() {
	return false;
}
