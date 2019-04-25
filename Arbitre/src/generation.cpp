#include "generation.h"

unsigned int setNumberTerritories(unsigned int nb_players) {
	int a = 30 + rand() % (10 - 0) + 0;
	std::cout << "v1 = " << a << std::endl;

	int mod = a % nb_players;
	std::cout << "mod = " << a << "%" << nb_players << "=" << mod << std::endl;

	int final_nb = a;

	if (mod != 0) {
		final_nb = a + (nb_players - mod);
		std::cout << "final_db = " << a << "+" << (nb_players - mod) << "=" << final_nb << std::endl;
	}

	return final_nb;
}

std::set<Coordinates> odd_coordinates(const Coordinates &coord, int L, int C) {
	std::set<Coordinates> list;
	if (coord.second - 1 >= 0) {
		list.insert(std::make_pair(coord.first, coord.second - 1));
	}

	if (coord.first + 1 < C && coord.second - 1 >= 0) {
		list.insert(std::make_pair(coord.first + 1, coord.second - 1));
	}

	if (coord.first + 1 < C) {
		list.insert(std::make_pair(coord.first + 1, coord.second));
	}

	if (coord.first + 1 < C && coord.second + 1 < L) {
		list.insert(std::make_pair(coord.first + 1, coord.second + 1));
	}

	if (coord.second + 1 < L) {
		list.insert(std::make_pair(coord.first, coord.second + 1));
	}

	if (coord.first - 1 >= 0) {
		list.insert(std::make_pair(coord.first - 1, coord.second));
	}

	for (auto it = list.begin(); it != list.end(); it++) {
		std::cout << "(" << it->first << " " << it->second << ")";
	}
	std::cout << std::endl;

	return list;

}


std::set<Coordinates> even_coordinates(const Coordinates &coord, int L, int C) {
	std::set<Coordinates> list;
	if (coord.first - 1 >= 0 && coord.second - 1 >= 0) {
		list.insert(std::make_pair(coord.first - 1, coord.second - 1));
	}

	if (coord.second - 1 >= 0) {
		list.insert(std::make_pair(coord.first, coord.second - 1));
	}

	if (coord.first + 1 < C) {
		list.insert(std::make_pair(coord.first + 1, coord.second));
	}

	if (coord.second + 1 < L) {
		list.insert(std::make_pair(coord.first, coord.second + 1));
	}

	if (coord.first - 1 >= 0 && coord.second + 1 < L) {
		list.insert(std::make_pair(coord.first - 1, coord.second + 1));
	}

	if (coord.first - 1 >= 0) {
		list.insert(std::make_pair(coord.first - 1, coord.second));
	}

	for (auto it = list.begin(); it != list.end(); it++) {
		std::cout << "(" << it->first << " " << it->second << ")";
	}
	std::cout << std::endl;

	return list;
}

Coordinates pattern_treatment(const Coordinates &coord, int L, int C, const std::vector<std::vector<int>> &matrix) {
	std::set<Coordinates> list;
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
	std::cout << "random values = " << rd_id << "/" << size << std::endl;

	return *first;

}


void displayMatrix(const int L, const int C, const Matrix &matrix) {
	for (int i = 0; i < L; i++) {
		std::cout << "[ ";
		for (int j = 0; j < C; j++) {
			std::cout << " " << matrix[j][i];
		}
		std::cout << " ]" << std::endl;
	}
	std::cout << "----------" << std::endl;

}

void rd_expand_cell(const std::set<Coordinates> & list) {
	std::cout << "list.size= " << list.size() << std::endl;
}

bool isEven(int a) {
	return a % 2 == 0;
}

bool isIsolated(const Matrix &mat, Coordinates coord, int L, int C) {
	std::set<Coordinates> neighbors;
	bool res = true;
	if (isEven(coord.first)) {
		neighbors = even_coordinates(coord, L, C);
	}
	else {
		neighbors = odd_coordinates(coord, L, C);
	}

	std::cout << "isIsolated --> ";
	for (Coordinates neigh : neighbors) {
		int x = neigh.first;
		std::cout << "-";
		int y = neigh.second;
		std::cout << "-";

		std::cout << "(x" << x << ",y" << y << ")";
		std::cout << "(" << mat[x][y] << ")";
		std::cout << "-";
		if (mat[y][x] == -1) {
			res = false;
		}
		std::cout << "(" << neigh.first << ", " << neigh.second << ")";
	}
	std::cout << std::endl;
	std::cout << std::endl;

	return res;
}

void afficherMap(const Map &m){
	for (Map::const_iterator it = m.begin(); it != m.end(); ++it) {
		std::cout << "key --> " << it->first << std::endl;
		for (Coordinates coord : it->second) {
			std::cout << " |   x= " << coord.first << ", y= " << coord.second << std::endl;
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
}


void initialisationMap() {
	srand((unsigned int)time(NULL));

	int n = 4;
	int C = 10;
	int L = 5;

	Matrix matrix(C, std::vector<int>(L, -1));
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
		std::cout << " Cell de base --> (x = " << x << ",y = " << y << ")" << std::endl;

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
		map[i].insert(std::make_pair(x, y));
	}

	afficherMap(map);


	for (int k = 0; k < n; k++) {
		int size_territory = map.find(k)->second.size();
		std::set<Coordinates> territory_cells = map.find(k)->second;

		std::cout << "size_territory = " << size_territory << std::endl;
		int id_cell_rd = rand() % (size_territory - 0) + 0;

		std::cout << "id_cell_rd = " << id_cell_rd << std::endl;


		std::set<Coordinates>::iterator it = territory_cells.begin();
		advance(it, id_cell_rd);
		Coordinates coord_a = *it;
		std::cout << "coord_a --> " << coord_a.first << ", " << coord_a.second << std::endl;

		bool check = false;
		while (!check) {
			Coordinates c = pattern_treatment(coord_a, L, C, matrix);
			std::cout << "coord_c --> " << c.first << ", " << c.second << std::endl;
			if (!isIsolated(matrix, c, L, C)) {
				check = true;
			}
		}

		std::cout << std::endl;


		//std::cout << "(" << coord_a.first << ":" << coord_a.second << ")" << std::endl;
		//
		//matrix[coord_a.second][coord_a.first] = k;
		//map[k].insert(std::make_pair(coord_a.first, coord_a.second));
	}
	afficherMap(map);

	displayMatrix(L, C, matrix);

}