#include "generation.h"

typedef std::pair<int, int> Coordinates;
typedef std::vector<std::vector<int>> Matrix;
typedef std::map<int, std::set<Coordinates>> Map;

unsigned int setNumberTerritories(unsigned int nb_players) {
	int a = 30 + rand() % (10 - 0) + 0;
	std::cout << "v1 = " << a << std::endl;

	int mod = a % nb_players;
	cout << "mod = " << a << "%" << nb_players << "=" << mod << endl;

	int final_nb = a;

	if (mod != 0) {
		final_nb = a + (nb_players - mod);
		cout << "final_db = " << a << "+" << (nb_players - mod) << "=" << final_nb << endl;
	}

	return final_nb;
}




std::set<Coordinates> odd_coordinates(const Coordinates &coord, int L, int C, const Matrix & matrix) {
	std::set<Coordinates> list;
	int x = coord.first;
	int y = coord.second;

	if (y - 1 >= 0 && matrix[x][y - 1] == -1) {
		list.insert(std::make_pair(x, y - 1));
	}

	if (x + 1 < C && y - 1 >= 0 && matrix[x + 1][y - 1] == -1) {
		list.insert(std::make_pair(x + 1, y - 1));
	}

	if (x + 1 < C && matrix[x + 1][y] == -1) {
		list.insert(std::make_pair(x + 1, y));
	}

	if (x + 1 < C && y + 1 < L && matrix[x + 1][y + 1] == -1) {
		list.insert(std::make_pair(x + 1, y + 1));
	}

	if (y + 1 < L && matrix[x][y + 1] == -1) {
		list.insert(std::make_pair(x, y + 1));
	}

	if (x - 1 >= 0 && matrix[x - 1][y] == -1) {
		list.insert(std::make_pair(x - 1, y));
	}

	std::cout << "odd_coordinates : " << std::endl;
	for (auto it = list.begin(); it != list.end(); it++) {
		std::cout << "(" << it->first << " " << it->second << ")";
	}
	std::cout << std::endl;

	return list;

}

std::set<Coordinates> even_coordinates(const Coordinates &coord, int L, int C, const Matrix & matrix) {
	std::set<Coordinates> list;
	int x = coord.first;
	int y = coord.second;

	if (x - 1 >= 0 && y - 1 >= 0 && matrix[x - 1][y - 1] == -1) {
		list.insert(std::make_pair(x - 1, y - 1));
	}

	if (y - 1 >= 0 && matrix[x][y - 1] == -1) {
		list.insert(std::make_pair(x, y - 1));
	}

	if (x + 1 < C && matrix[x + 1][y] == -1) {
		list.insert(std::make_pair(x + 1, y));
	}

	if (y + 1 < L && matrix[x][y + 1] == -1) {
		list.insert(std::make_pair(x, y + 1));
	}

	if (x - 1 >= 0 && y + 1 < L && matrix[x - 1][y + 1] == -1) {
		list.insert(std::make_pair(x - 1, y + 1));
	}

	if (x - 1 >= 0 && matrix[x - 1][y] == -1) {
		list.insert(std::make_pair(x - 1, y));
	}

	std::cout << "(" << x << "," << y << ")" << "even_coordinates : ";
	for (auto it = list.begin(); it != list.end(); it++) {
		std::cout << "(" << it->first << " " << it->second << "), ";
	}
	std::cout << std::endl;

	return list;
}

Coordinates pattern_treatment(const Coordinates &coord, int L, int C, const Matrix &matrix) {
	std::set<Coordinates> list;
	if (coord.second % 2 == 0) {
		list = even_coordinates(coord, L, C, matrix);
	}
	else {
		list = odd_coordinates(coord, L, C, matrix);
	}
	int size = list.size();
	std::cout << size << std::endl;
	if (list.empty()) {
		std::cout << "La liste est vide !!" << std::endl;
	}

	int rd_id = rand() % list.size();
	std::cout << "rd_id = " << rd_id << std::endl;


	auto first = list.begin(); // get iterator to 1st element

	advance(first, rd_id);
	cout << "random values = " << rd_id << "/" << size << endl;

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

void rd_expand_cell(const std::set<Coordinates> & list) {
	std::cout << "list.size= " << list.size() << std::endl;
}

bool isEven(int a) {
	return a % 2 == 0;
}

bool isIsolated(const Matrix &mat, Coordinates coord, int L, int C) {
	std::set<Coordinates> neighbors;
	bool res = true;
	if (isEven(coord.second)) {
		neighbors = even_coordinates(coord, L, C, mat);
	}
	else {
		neighbors = odd_coordinates(coord, L, C, mat);
	}
	std::cout << "issEmpty" << neighbors.empty() << std::endl;
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
		std::cout << "key --> " << it->first << std::endl;
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
		std::set<Coordinates> list;
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
		std::set<Coordinates> list;
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


		bool check1 = false;
		while (!check1) {
			int x = rand() % (x_borne_rd - 0) + 0;
			int y = rand() % (y_borne_rd - 0) + 0;
			std::cout << " Cell de base --> (x = " << x << ",y = " << y << ")" << std::endl;

			if (matrix[x][y] == -1) {
				check1 = true;
				matrix[x][y] = i;
				map[i].insert(std::make_pair(x, y));
			}
		}

		// Changer valeur dans la Matrix
		//while (!check1) {
		//	if (matrix[y][x] == -1) {
		//		check1 = true;
		//		matrix[y][x] = i;
		//	}
		//}

		// Insérer les coordonnées dans la map à la clé "n° de territoire"

	}

	afficherMap(map);



	bool end;
	do {

		end = false;
		for (int k = 0; k < n; k++) {
			//Vérifier si le territoire n'est pas tout recouvert

			std::set<Coordinates> territory_cells = map.find(k)->second;
			std::cout << "bool already expand = " << already_expanded(map, matrix, k, L, C) << std::endl;
			if (!already_expanded(map, matrix, k, L, C)) {
				set<Coordinates> list_base;
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

				cout << "vide la liste_base ? " << list_base.empty() << endl;
				cout << "Coordonnees list_base : ";
				for (Coordinates coordin : list_base) {
					cout << "(" << coordin.first << ", " << coordin.second << ")";
				}
				cout << endl;

				//int size_territory = map.find(k)->second.size();
				if (!list_base.empty()) {
					int size_list_base = list_base.size();
					int id_cell_rd = rand() % (size_list_base - 0) + 0;
					cout << endl;
					cout << "size_territory/id_cell_rd : " << size_list_base << " " << id_cell_rd << endl;
					cout << endl;

					set<Coordinates>::iterator it = list_base.begin();
					cout << "-" << endl;
					advance(it, id_cell_rd);
					cout << "-" << endl;
					Coordinates coord_a = *it;
					cout << "-" << endl;
					cout << "coord_a --> " << coord_a.first << ", " << coord_a.second << endl;
					cout << "-" << endl;

					matrix[coord_a.first][coord_a.second] = k;
					map[k].insert(make_pair(coord_a.first, coord_a.second));
				}



				/*set<Coordinates>::iterator coord_a = next(list_base.begin(), 5);
				cout << "coord_a --> " << coord_a.first << ", " << coord_a.second << endl;*/



			}
			else {
				cout << endl;
				cout << "------------ C'EST LA FIN ------------" << endl;
				cout << endl;
				end = false;
			}
			// pour chaque id_territoire :
				// obtenir la liste valeur de la map -- set<Coordinates>
				// créer liste de Corrdinates avec possibilité de mvt
				// pour chaque <Coordinates> :
					// ajouter la Coordinates à la set<Coordinates>

			// si new Liste set<Coordinates> n'est pas vide
				// on choisit une corrdinate
				// et on expand

			//int size_territory = map.find(k)->second.size();


	/*		cout << " ID_TERRITORY = " << k << endl;
			cout << " ---------------- " << endl;
			int id_cell_rd = rand() % (size_territory - 0) + 0;*/

			//	cout << "id_cell_rd = " << id_cell_rd << endl;
			//	

		/*	set<Coordinates>::iterator it = territory_cells.begin();
			advance(it, id_cell_rd);
			Coordinates coord_a = *it;
			cout << "coord_a --> " << coord_a.first << ", " << coord_a.second << endl;*/


			bool check = false;
			//Coordinates coord_b = make_pair(9,4);
			//matrix()
			//cout << isIsolated(matrix, coord_a, L, C) << endl;


			cout << endl;
			cout << endl;


			// si on peut encore trouver des voisins possibles.
			//if (!already_expanded(map,matrix,k,L,C)) {
			//	while (!check) {
			//		cout << "Entrée dans la boucle" << endl;
			//		cout << "!isIsolated -->" << (!isIsolated(matrix, coord_a, L, C))  << endl;
			//		if (!isIsolated(matrix, coord_a, L, C)) {
			//			Coordinates c = pattern_treatment(coord_a, L, C, matrix);
			//			cout << "coord_c --> " << c.first << ", " << c.second << endl;
			//			check = true;
			//			coord_a = make_pair(c.first, c.second);
			//		}
			//	}

			//	matrix[coord_a.first][coord_a.second] = k;
			//	map[k].insert(make_pair(coord_a.first, coord_a.second));
			//}



			//for (Coordinates coord : map.find(k)->second) {
			//	set<Coordinates> list;
			//	if (coord.first % 2 == 0) {
			//		list = even_coordinates(coord, L, C);
			//	}
			//	else {
			//		list = odd_coordinates(coord, L, C);
			//	}

			//	if (list.empty()) {
			//		end = false;
			//	}

			//	//	cout << endl;


			//	//	//cout << "(" << coord_a.first << ":" << coord_a.second << ")" << endl;
			//	//	//
			//}



			afficherMap(map);

			displayMatrix(L, C, matrix);
		}
		end = CheckEndInit(matrix, map, L, C);

		afficherMap(map);

		displayMatrix(L, C, matrix);

		cout << end << endl;
	} while (!end);

}