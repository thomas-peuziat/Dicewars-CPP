#include "../../Commun/interface.h"
#include <iostream>
#include <vector>
#include <set>
#include <map>

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



typedef std::pair<int, int> Coordinates;
typedef std::vector<std::vector<int>> Matrix;
typedef std::map<int, std::set<Coordinates>> Map;


unsigned int setNumberTerritories(unsigned int nb_players) {
	int a = 30 + rand() % (10 - 0) + 0;
	std::cout << "v1 = " << a << std::endl;

	int mod = a % nb_players;
	std::cout << "mod = " << a << "%" << nb_players << "=" << mod << std::endl;

	int final_nb = a;

	if (mod != 0) {
		final_nb = a + (nb_players - mod);
		std::cout << "final_db = " << a << "+" << (nb_players - mod) << "=" << final_nb << endl;
	}

	return final_nb;
}




std::set<Coordinates> odd_coordinates(const Coordinates &coord,int L, int C, const Matrix & matrix) {
	std::set<Coordinates> list;
	int x = coord.first;
	int y = coord.second;

	if (y - 1 >= 0 && matrix[x][y-1] == -1) {
		list.insert(make_pair(x,y- 1));
	}

	if (x+1 < C && y - 1 >= 0 && matrix[x+1][y-1] == -1) {
		list.insert(make_pair(x + 1, y-1));
	}

	if (x + 1 < C && matrix[x+1][y] == -1) {
		list.insert(make_pair(x + 1, y));
	}

	if (x+1 < C && y+1 < L && matrix[x+1][y+1] == -1) {
		list.insert(make_pair(x + 1, y+1));
	}

	if (y+ 1 < L && matrix[x][y+1] == -1) {
		list.insert(make_pair(x, y+ 1));
	}

	if (x - 1 >= 0 && matrix[x-1][y] == -1) {
		list.insert(make_pair(x-1, y));
	}

	std::cout << "odd_coordinates : " << endl;
	for (auto it = list.begin(); it != list.end(); it++) {
		std::cout << "("<< it->first << " " << it->second << ")";
	}
	std::cout << std::endl;

	return list;

}

std::set<Coordinates> even_coordinates(const Coordinates &coord, int L, int C, const Matrix & matrix) {
	std::set<Coordinates> list;
	int x = coord.first;
	int y = coord.second;

	if (x - 1 >= 0 && y - 1 >= 0 && matrix[x-1][y-1] == -1) {
		list.insert(make_pair(x - 1, y - 1));
	}

	if (y - 1 >= 0 && matrix[x][y-1] == -1) {
		list.insert(make_pair(x, y- 1));
	}

	if (x + 1 < C && matrix[x+1][y] == -1) {
		list.insert(make_pair(x + 1, y));
	}

	if (y + 1 < L && matrix[x][y+1] == -1) {
		list.insert(make_pair(x, y + 1));
	}

	if (x - 1 >= 0 && y + 1 < L && matrix[x-1][y+1] == -1) {
		list.insert(make_pair(x - 1, y + 1));
	}

	if (x - 1 >= 0 && matrix[x-1][y] == -1) {
		list.insert(make_pair(x - 1, y));
	}

	std::cout << "(" << x << "," << y << ")" <<"even_coordinates : ";
	for (auto it = list.begin(); it != list.end(); it++) {
		std::cout << "(" << it->first << " " << it->second << "), ";
	}
	std::cout << endl;

	return list;
}

Coordinates pattern_treatment(const Coordinates &coord,int L,int C, const Matrix &matrix) {
	std::set<Coordinates> list;
	if (coord.second % 2 == 0) {
		list = even_coordinates(coord,L,C, matrix);
	}
	else {
		list = odd_coordinates(coord, L, C, matrix);
	}
	int size = list.size();
	std::cout << size << endl;
	if (list.empty()) {
		std::cout <<  "La liste est vide !!" << endl;
	}

	int rd_id = rand() % list.size();
	std::cout <<  "rd_id = " << rd_id << endl;


	auto first = list.begin(); // get iterator to 1st element

	advance(first, rd_id);
	std::cout << "random values = " << rd_id << "/"<< size << endl;

	return *first;

}


void displayMatrix(const int L, const int C, const Matrix &matrix) {
	for (int i = 0; i < L; i++) {
		if (i % 2 == 0) {
			std::cout << "[ ";
		}
		else {
			std::cout << "[  ";
		}

		for (int j = 0; j < C; j++) {
			std::cout << " " << matrix[j][i];
		}
		std::cout << " ]" << std::endl;
	}
	std::cout << "----------" << std::endl;

}

void rd_expand_cell(const set<Coordinates> & list) {
	std::cout << "list.size= " << list.size() << endl;
}

bool isEven(int a) {
	return a % 2 == 0;
}

bool isIsolated(const Matrix &mat,Coordinates coord,int L, int C) {
	std::set<Coordinates> neighbors;
	bool res = true;
	if (isEven(coord.second)) {
		neighbors = even_coordinates(coord, L, C, mat);
	}
	else {
		neighbors = odd_coordinates(coord, L, C, mat);
	}
	std::cout << "issEmpty" <<  neighbors.empty() << std::endl;
	for (Coordinates neigh : neighbors) {
		int x = neigh.first;
		int y = neigh.second;

		std::cout << "(" << x << "," << y << ":" << mat[x][y] << ") ";

		if (mat[x][y] == -1) {
			res = false;
		}
	}
	std::cout << std::endl;
	std::cout << std::endl;

	return res;
}

void afficherMap(const Map &m) {
	for (Map::const_iterator it = m.begin(); it != m.end(); ++it) {
		std::cout << "key --> " << it->first << endl;
		for (Coordinates coord : it->second) {
			std::cout << " |   x= " << coord.first << ", y= " << coord.second << std::endl;
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
}


bool already_expanded(Map & map, Matrix & matrix, int id_territory, int L, int C) {
	bool res = true;
	for (Coordinates coord : map.find(id_territory)->second) {
		set<Coordinates> list;
		if (coord.second % 2 == 0) {
			list = even_coordinates(coord, L, C, matrix);
		}
		else {
			list = odd_coordinates(coord, L, C, matrix);
		}

		if (!list.empty()) {
			res = false;
		}
	}
	return res;
}

bool CheckEndInit(Matrix & matrix, Map & map, int L, int C) {
	bool res = true;
	int size = map.size();
	for (int i = 0; i < size; i++) {
		std::set<Coordinates> territory_cells = map.find(i)->second;

		std::set<Coordinates> list_base;
		set<Coordinates> list;
		for (Coordinates coord : territory_cells) {
			if (coord.second % 2 == 0) {
				list = even_coordinates(coord, L, C, matrix);
			}
			else {
				list = odd_coordinates(coord, L, C, matrix);
			}
			list_base.insert(list.begin(), list.end());
		}
		if (!list_base.empty()) {
			res = false;
		}
	}
	return res;
}




