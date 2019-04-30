#include "generation.h"

typedef std::pair<int, int> Coordinates;
typedef std::vector<std::vector<int>> Matrix;
typedef std::map<int, std::set<Coordinates>> MapTerritoire;

unsigned int setNumberTerritories(unsigned int nb_players) {
	int a = 30 + rand() % (10 - 0) + 0;
	//std::cout << "v1 = " << a << std::endl;

	int mod = a % nb_players;
	//std::cout << "mod = " << a << "%" << nb_players << "=" << mod << std::endl;

	int final_nb = a;

	if (mod != 0) {
		final_nb = a + (nb_players - mod);
		//std::cout << "final_db = " << a << "+" << (nb_players - mod) << "=" << final_nb << std::endl;
	}

	return final_nb;
}




std::set<Coordinates> odd_coordinates(const Coordinates &coord, int L, int C, const Matrix & matrix) {
	std::set<Coordinates> list;
	int y = coord.first;
	int x = coord.second;

	if (y - 1 >= 0 && matrix[x][y - 1] == -1) {
		list.insert(std::make_pair(y - 1, x));
	}

	if (x + 1 < C && y - 1 >= 0 && matrix[x + 1][y - 1] == -1) {
		list.insert(std::make_pair(y - 1, x + 1));
	}

	if (x + 1 < C && matrix[x + 1][y] == -1) {
		list.insert(std::make_pair(y, x + 1));
	}

	if (x + 1 < C && y + 1 < L && matrix[x + 1][y + 1] == -1) {
		list.insert(std::make_pair(y + 1, x + 1));
	}

	if (y + 1 < L && matrix[x][y + 1] == -1) {
		list.insert(std::make_pair(y + 1, x));
	}

	if (x - 1 >= 0 && matrix[x - 1][y] == -1) {
		list.insert(std::make_pair(y, x - 1));
	}

	//std::cout << "odd_coordinates : " << std::endl;
	for (auto it = list.begin(); it != list.end(); it++) {
		//std::cout << "(" << it->first << " " << it->second << ")";
	}
	//std::cout << std::endl;

	return list;

}

std::set<Coordinates> even_coordinates(const Coordinates &coord, int L, int C, const Matrix & matrix) {
	std::set<Coordinates> list;
	int y = coord.first;
	int x = coord.second;

	if (x - 1 >= 0 && y - 1 >= 0 && matrix[x - 1][y - 1] == -1) {
		list.insert(std::make_pair(y - 1, x - 1));
	}

	if (y - 1 >= 0 && matrix[x][y - 1] == -1) {
		list.insert(std::make_pair(y - 1, x));
	}

	if (x + 1 < C && matrix[x + 1][y] == -1) {
		list.insert(std::make_pair(y, x + 1));
	}

	if (y + 1 < L && matrix[x][y + 1] == -1) {
		list.insert(std::make_pair(y + 1, x));
	}

	if (x - 1 >= 0 && y + 1 < L && matrix[x - 1][y + 1] == -1) {
		list.insert(std::make_pair(y + 1, x - 1));
	}

	if (x - 1 >= 0 && matrix[x - 1][y] == -1) {
		list.insert(std::make_pair(y, x - 1));
	}

	//std::cout << "(" << x << "," << y << ")" << "even_coordinates : ";
	if (list.empty()) {
		//std::cout << "C'est VIDE !!" << std::endl;
	}
	for (auto it = list.begin(); it != list.end(); it++) {
		//std::cout << "(" << it->first << " " << it->second << "), ";
	}
	//std::cout << std::endl;

	return list;
}


void displayMatrix(const int L, const int C, const Matrix &matrix) {
	for (int l = 0; l < L; l++) {
		if (isEven(l)) {
			std::cout << "[ ";
		}
		else {
			std::cout << "[  ";
		}

		for (int c = 0; c < C; c++) {
			std::cout << " " << matrix[c][l];
		}
		std::cout << " ]" << std::endl;
	}
	std::cout << "----------" << std::endl;

}

void rd_expand_cell(const std::set<Coordinates> & list) {
	//std::cout << "list.size= " << list.size() << std::endl;
}

bool isEven(int a) {
	return a % 2 == 0;
}

void afficherMap(const MapTerritoire &m) {
	for (MapTerritoire::const_iterator it = m.begin(); it != m.end(); ++it) {
		//std::cout << "key --> " << it->first << std::endl;
		for (Coordinates coord : it->second) {
			//std::cout << " |   c= " << coord.second << ", l= " << coord.first << std::endl;
		}
		//std::cout << std::endl;
		//std::cout << std::endl;
	}
}


bool already_expanded(MapTerritoire & map, Matrix & matrix, int id_territory, int L, int C) {
	bool res = true;
	for (Coordinates coord : map.find(id_territory)->second) {
		std::set<Coordinates> list;
		if (isEven(coord.first)) {
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

bool CheckEndInit(Matrix & matrix, MapTerritoire & map, int L, int C) {
	bool res = true;
	int size = map.size();
	for (int i = 0; i < size; i++) {
		std::set<Coordinates> territory_cells = map.find(i)->second;

		std::set<Coordinates> list_base;
		std::set<Coordinates> list;
		for (Coordinates coord : territory_cells) {
			if (isEven(coord.first)) {
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

	int nbTerritoires = 4;
	int nbLignes = 10;
	int nbColonnes = 5;

	Matrix matrix(nbColonnes, std::vector<int>(nbLignes, -1));
	MapTerritoire map;


	// Affichage Matrix	
	displayMatrix(nbLignes, nbColonnes, matrix);

	// Calcul des bornes pour le random_
	int c_borne = nbColonnes - 1;
	int l_borne = nbLignes - 1;

	// Premier tour -> Création aléatoire des cellules de base
	for (int i = 0; i < nbTerritoires; i++) {


		bool check1 = false;
		while (!check1) {
			int c = rand() % (c_borne); 
			int l = rand() % (l_borne);
			//std::cout << " Cell de base --> (c = " << c << ",l = " << l << ")" << std::endl;

			if (matrix[c][l] == -1) {
				check1 = true;
				matrix[c][l] = i;
				map[i].insert(std::make_pair(l, c));
			}
		}
	}

	afficherMap(map);



	bool end;
	do {

		end = false;
		for (int k = 0; k < nbTerritoires; k++) {
			//Vérifier si le territoire n'est pas tout recouvert
			//std::cout << "Iterate on K !" << std::endl;
			std::set<Coordinates> territory_cells = map.find(k)->second;
			//std::cout << "K2" << std::endl;
			//std::cout << "bool already expand = " << already_expanded(map, matrix, k, nbLignes, nbColonnes) << std::endl;
			
			if (!already_expanded(map, matrix, k, nbLignes, nbColonnes)) {
				std::set<Coordinates> list_base;
				std::set<Coordinates> list;
				for (Coordinates coord : territory_cells) {
					if (isEven(coord.first)) {
						list = even_coordinates(coord, nbLignes, nbColonnes, matrix);
					}
					else {
						list = odd_coordinates(coord, nbLignes, nbColonnes, matrix);
					}
					list_base.insert(list.begin(), list.end());
				}

				//std::cout << "vide la liste_base ? " << list_base.empty() << std::endl;
				//std::cout << "Coordonnees list_base : ";
				for (Coordinates coordin : list_base) {
					//std::cout << "(" << coordin.first << ", " << coordin.second << ")";
				}
				//std::cout << std::endl;

				//int size_territory = map.find(k)->second.size();
				if (!list_base.empty()) {
					int size_list_base = list_base.size();
					int id_cell_rd = rand() % (size_list_base - 0) + 0;
					//std::cout << std::endl;
					//std::cout << "size_territory/id_cell_rd : " << size_list_base << " " << id_cell_rd << std::endl;
					//std::cout << std::endl;

					std::set<Coordinates>::iterator it = list_base.begin();
					//std::cout << "-" << std::endl;
					std::advance(it, id_cell_rd);
					//std::cout << "-" << std::endl;
					Coordinates coord_a = *it;
					//std::cout << "-" << std::endl;
					//std::cout << "coord_a --> " << coord_a.first << ", " << coord_a.second << std::endl;
					//std::cout << "-" << std::endl;

					matrix[coord_a.second][coord_a.first] = k;
					map[k].insert(std::make_pair(coord_a.first, coord_a.second));
				}

			}
			else {
				//std::cout << std::endl;
				//std::cout << "------------ C'EST LA FIN ------------" << std::endl;
				//std::cout << std::endl;
				end = false;
			}

			bool check = false;

			//std::cout << "Afficher MAP 1" << std::endl;
			afficherMap(map);
			//std::cout << "Afficher MAP 1 - APRES" << std::endl;
			displayMatrix(nbLignes, nbColonnes, matrix);
			//std::cout << "Afficher DISPLAY 1 - APRES" << std::endl;
		}
		//std::cout << "SALUT" << std::endl;

		end = CheckEndInit(matrix, map, nbLignes, nbColonnes);

		//std::cout << "CAVA" << std::endl;

		//std::cout << "Afficher MAP 2" << std::endl;

		afficherMap(map);

		//std::cout << "UI ET TOI" << std::endl;

		displayMatrix(nbLignes, nbColonnes, matrix);

		//std::cout << "NICKEL" << std::endl;

		//std::cout << end << std::endl;
	} while (!end);

	
}