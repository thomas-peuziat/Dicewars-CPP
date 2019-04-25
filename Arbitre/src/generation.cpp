#include "../../Commun/interface.h"
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


/*

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <set>
#include <map>

using namespace std;
typedef pair<int, int> Coordinates;
typedef vector<vector<int>> Matrix;
typedef map<int, set<Coordinates>> Map;

unsigned int setNumberTerritories(unsigned int nb_players) {
	int a = 30 + rand() % (10 - 0) + 0;
	cout << "v1 = " << a << endl;

	int mod = a % nb_players;
	cout << "mod = " << a << "%" << nb_players << "=" << mod << endl;

	int final_nb = a;

	if (mod != 0) {
		final_nb = a + (nb_players - mod);
		cout << "final_db = " << a << "+" << (nb_players - mod) << "=" << final_nb << endl;
	}

	return final_nb;
}

set<Coordinates> odd_coordinates(const Coordinates &coord, int L, int C) {
	set<Coordinates> list;
	if (coord.second - 1 >= 0) {
		list.insert(make_pair(coord.first, coord.second - 1));
	}

	if (coord.first + 1 <= C && coord.second - 1 >= 0) {
		list.insert(make_pair(coord.first + 1, coord.second - 1));
	}

	if (coord.first + 1 <= C) {
		list.insert(make_pair(coord.first + 1, coord.second));
	}

	if (coord.first + 1 <= C && coord.second + 1 <= L) {
		list.insert(make_pair(coord.first + 1, coord.second + 1));
	}

	if (coord.second + 1 <= L) {
		list.insert(make_pair(coord.first, coord.second + 1));
	}

	if (coord.first - 1 >= 0) {
		list.insert(make_pair(coord.first - 1, coord.second));
	}

	for (auto it = list.begin(); it != list.end(); it++) {
		cout << "(" << it->first << " " << it->second << ")";
	}
	cout << endl;

	return list;

}


set<Coordinates> even_coordinates(const Coordinates &coord, int L, int C) {
	set<Coordinates> list;
	if (coord.first - 1 >= 0 && coord.second - 1 >= 0) {
		list.insert(make_pair(coord.first - 1, coord.second - 1));
	}

	if (coord.second - 1 >= 0) {
		list.insert(make_pair(coord.first, coord.second - 1));
	}

	if (coord.first + 1 <= C) {
		list.insert(make_pair(coord.first + 1, coord.second));
	}

	if (coord.second + 1 <= L) {
		list.insert(make_pair(coord.first, coord.second + 1));
	}

	if (coord.first - 1 >= 0 && coord.second + 1 <= L) {
		list.insert(make_pair(coord.first - 1, coord.second + 1));
	}

	if (coord.first - 1 >= 0) {
		list.insert(make_pair(coord.first - 1, coord.second));
	}

	for (auto it = list.begin(); it != list.end(); it++) {
		cout << "(" << it->first << " " << it->second << ")";
	}
	cout << endl;

	return list;
}

Coordinates pattern_treatment(const Coordinates &coord, int L, int C, const vector<vector<int>> &matrix) {
	set<Coordinates> list;
	if (coord.first % 2 == 0) {
		list = even_coordinates(coord, L, C);
	}
	else {
		list = odd_coordinates(coord, L, C);
	}
	int size = list.size();
	int rd_id = rand() % ((size - 1) - 0) + 0;

	auto first = list.begin(); // get iterator to 1st element

	advance(first, rd_id + 1);
	cout << "random values = " << rd_id << "/" << size << endl;

	return *first;

}


void displayMatrix(const int L, const int C, const Matrix &matrix) {
	for (int i = 0; i < L; i++) {
		cout << "[ ";
		for (int j = 0; j < C; j++) {
			cout << " " << matrix[j][i];
		}
		cout << " ]" << endl;
	}
	cout << "----------" << endl;

}

void rd_expand_cell(const set<Coordinates> & list) {
	cout << "list.size= " << list.size() << endl;
}

bool isEven(int a) {
	return a % 2 == 0;
}

bool isIsolated(const Matrix &mat, Coordinates coord, int L, int C) {
	set<Coordinates> neighbors;
	bool res = true;
	if (isEven(coord.first)) {
		neighbors = even_coordinates(coord, L, C);
	}
	else {
		neighbors = odd_coordinates(coord, L, C);
	}

	cout << "isIsolated --> ";
	for (Coordinates neigh : neighbors) {
		int x = neigh.first;
		cout << "-";
		int y = neigh.second;
		cout << "-";

		cout << "(x" << x << ",y" << y << ")";
		cout << "(" << mat[x][y] << ")";
		cout << "-";
		if (mat[y][x] == -1) {
			res = false;
		}
		cout << "(" << neigh.first << ", " << neigh.second << ")";
	}
	cout << endl;
	cout << endl;

	return res;
}

void afficherMap(const Map &m) {
	for (Map::const_iterator it = m.begin(); it != m.end(); ++it) {
		std::cout << "key --> " << it->first << endl;
		for (Coordinates coord : it->second) {
			cout << " |   x= " << coord.first << ", y= " << coord.second << endl;
		}
		cout << endl;
		cout << endl;
	}
}


int main() {
	srand((unsigned int)time(NULL));

	int n = 4;
	int C = 10;
	int L = 5;

	Matrix matrix(C, vector<int>(L, -1));
	Map map;

	// Affichage Matrix	
	displayMatrix(L, C, matrix);

	// Calcul des bornes pour le random_
	int y_borne_rd = L - 1;
	int x_borne_rd = C - 1;

	// Premier tour -> Création aléatoire des cellules de base
	for (int i = 0; i < n; i++) {

		int x = rand() % (x_borne_rd - 0) + 0;
		int y = rand() % (y_borne_rd - 0) + 0;
		cout << " Cell de base --> (x = " << x << ",y = " << y << ")" << endl;

		bool check1 = false;
		matrix[x][y] = 36;
		// Changer valeur dans la Matrix
		//while (!check1) {
		//	if (matrix[y][x] == -1) {
		//		check1 = true;
		//		matrix[y][x] = i;
		//	}
		//}

		// Insérer les coordonnées dans la map à la clé "n° de territoire"
		map[i].insert(make_pair(x, y));
	}

	afficherMap(map);


	for (int k = 0; k < n; k++) {
		int size_territory = map.find(k)->second.size();
		set<Coordinates> territory_cells = map.find(k)->second;

		cout << "size_territory = " << size_territory << endl;
		int id_cell_rd = rand() % (size_territory - 0) + 0;

		cout << "id_cell_rd = " << id_cell_rd << endl;


		set<Coordinates>::iterator it = territory_cells.begin();
		advance(it, id_cell_rd);
		Coordinates coord_a = *it;
		cout << "coord_a --> " << coord_a.first << ", " << coord_a.second << endl;

		bool check = false;
		while (!check) {
			Coordinates c = pattern_treatment(coord_a, L, C, matrix);
			cout << "coord_c --> " << c.first << ", " << c.second << endl;
			if (!isIsolated(matrix, c, L, C)) {
				check = true;
			}
		}

		cout << endl;


		//cout << "(" << coord_a.first << ":" << coord_a.second << ")" << endl;
		//
		//matrix[coord_a.second][coord_a.first] = k;
		//map[k].insert(make_pair(coord_a.first, coord_a.second));
	}
	afficherMap(map);

	displayMatrix(L, C, matrix);

}

*/