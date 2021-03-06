#include "generation.h"

std::set<Coordinates> getVoisins(const Coordinates &coord, int L, int C, const Matrix & matrix) {
	std::set<Coordinates> list = {};
	int idxLigne = coord.first;
	int idxColonne = coord.second;

	if (isEven(idxLigne))
	{
		if (idxColonne - 1 >= 0 && idxLigne - 1 >= 0) {
			list.insert(std::make_pair(idxLigne - 1, idxColonne - 1));
		}

		if (idxColonne - 1 >= 0 && idxLigne + 1 < L) {
			list.insert(std::make_pair(idxLigne + 1, idxColonne - 1));
		}
	}
	else {
		if (idxColonne + 1 < C && idxLigne + 1 < L) {
			list.insert(std::make_pair(idxLigne + 1, idxColonne + 1));
		}

		if (idxColonne + 1 < C && idxLigne - 1 >= 0) {
			list.insert(std::make_pair(idxLigne - 1, idxColonne + 1));
		}
	}

	if (idxLigne - 1 >= 0) {
		list.insert(std::make_pair(idxLigne - 1, idxColonne));
	}

	if (idxColonne + 1 < C) {
		list.insert(std::make_pair(idxLigne, idxColonne + 1));
	}

	if (idxLigne + 1 < L) {
		list.insert(std::make_pair(idxLigne + 1, idxColonne));
	}

	if (idxColonne - 1 >= 0) {
		list.insert(std::make_pair(idxLigne, idxColonne - 1));
	}

	return list;
}

std::set<Coordinates> getVoisinsDisponibles(const Coordinates &coord, int L, int C, const Matrix & matrix)
{
	std::set<Coordinates> listVoisins = getVoisins(coord, L, C, matrix);
	std::set<Coordinates> listVoisinsDisponibles = {};

	for (Coordinates coord : listVoisins) {
		if(matrix[coord.second][coord.first] == -1)
			listVoisinsDisponibles.insert(coord);
	}

	return listVoisinsDisponibles;
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

bool isEven(int a) {
	return a % 2 == 0;
}

void afficherMap(const MapTerritoire &m) {
	for (MapTerritoire::const_iterator it = m.begin(); it != m.end(); ++it) {
		std::cout << "key --> " << it->first << std::endl;
		for (Coordinates coord : it->second) {
			std::cout << " |   c= " << coord.second << ", l= " << coord.first << std::endl;
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
}

bool already_expanded(MapTerritoire & map, Matrix & matrix, int id_territory, int L, int C) {
	bool res = true;
	for (Coordinates coord : map.find(id_territory)->second) {
		std::set<Coordinates> list = getVoisinsDisponibles(coord, L, C, matrix);
		
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
			list = getVoisinsDisponibles(coord, L, C, matrix);
			list_base.insert(list.begin(), list.end());
		}
		if (!list_base.empty()) {
			res = false;
		}
	}
	return res;
}

void addNewNeighborsSCell(SMap *smap, int idCell, std::set<Coordinates> listVoisins, const Matrix &matrix)
{
	SCell & scell = smap->cells[idCell];
	std::vector<int> newNeighIDList = {};


	for (Coordinates coord : listVoisins)
	{
		int idHexagone = matrix[coord.second][coord.first];
		if (idHexagone != -1 && idHexagone != idCell) {
			bool alreadyNeighbouring = false;
			int idVoisin;
			
			for (int i = 0; i < scell.nbNeighbors; i++)
			{
				idVoisin = scell.neighbors[i]->infos.id;
				if (idHexagone == idVoisin) {
					alreadyNeighbouring = true;
				}
			}

			for (int id : newNeighIDList) {
				if (id == idHexagone)
					alreadyNeighbouring = true;
			}

			if (!alreadyNeighbouring) {
				
				newNeighIDList.push_back(idHexagone);
			}
		}
	}

	if (newNeighIDList.size() > 0) {
		if (scell.nbNeighbors <= 0) {
			// Modif nbNeighbors
			scell.nbNeighbors = newNeighIDList.size();

			// Premi�re allocation m�moire de scell.neighbors
			scell.neighbors = new SCell*[scell.nbNeighbors];

			// Ajout nouveaux voisins
			int j = 0;
			for (int id : newNeighIDList) {
				scell.neighbors[j] = &(smap->cells[id]);
				j++;

				// Ajout voisin dans la cellule nouvellement voisine
				if (smap->cells[id].nbNeighbors <= 0) {
					// Modif nbNeighbors
					smap->cells[id].nbNeighbors = 1;

					// Premi�re allocation m�moire de scell.neighbors
					smap->cells[id].neighbors = new SCell*[smap->cells[id].nbNeighbors];

					// Ajout nouveaux voisins
					smap->cells[id].neighbors[0] = &(scell);
				}
				else {
					// Equivalent realloc car scell.neighbors existe d�j� en m�moire
					SCell** listPtrVoisins = new SCell*[1 + smap->cells[id].nbNeighbors];
					for (int i = 0; i < smap->cells[id].nbNeighbors; i++) {
						listPtrVoisins[i] = smap->cells[id].neighbors[i];
					}
					delete[] smap->cells[id].neighbors;
					smap->cells[id].neighbors = new SCell*[1 + smap->cells[id].nbNeighbors];
					for (int i = 0; i < smap->cells[id].nbNeighbors; i++) {
						smap->cells[id].neighbors[i] = listPtrVoisins[i];
					}
					delete[] listPtrVoisins;

					// Ajout nouveaux voisins
					smap->cells[id].neighbors[smap->cells[id].nbNeighbors] = &(scell);

					// Modif nbNeighbors
					smap->cells[id].nbNeighbors += 1;
				}

			}
		}
		else {

			// Equivalent realloc car scell.neighbors existe d�j� en m�moire
			SCell** listPtrVoisins = new SCell*[newNeighIDList.size() + scell.nbNeighbors];
			for (int i = 0; i < scell.nbNeighbors; i++) {
				listPtrVoisins[i] = scell.neighbors[i];
			}
			delete[] scell.neighbors;
			scell.neighbors = new SCell*[newNeighIDList.size() + scell.nbNeighbors];
			for (int i = 0; i < scell.nbNeighbors; i++) {
				scell.neighbors[i] = listPtrVoisins[i];
			}
			delete[] listPtrVoisins;

			// Ajout nouveaux voisins
			int j = 0;
			for (int id : newNeighIDList) {
				scell.neighbors[j + scell.nbNeighbors] = &(smap->cells[id]);
				j++;

				// Ajout voisin dans la cellule nouvellement voisine
				if (smap->cells[id].nbNeighbors <= 0) {
					// Modif nbNeighbors
					smap->cells[id].nbNeighbors = 1;

					// Premi�re allocation m�moire de scell.neighbors
					smap->cells[id].neighbors = new SCell*[smap->cells[id].nbNeighbors];

					// Ajout nouveaux voisins
					smap->cells[id].neighbors[0] = &(scell);
				}
				else {
					// Equivalent realloc car scell.neighbors existe d�j� en m�moire
					SCell** listPtrVoisins = new SCell*[1 + smap->cells[id].nbNeighbors];
					for (int i = 0; i < smap->cells[id].nbNeighbors; i++) {
						listPtrVoisins[i] = smap->cells[id].neighbors[i];
					}
					delete[] smap->cells[id].neighbors;
					smap->cells[id].neighbors = new SCell*[1 + smap->cells[id].nbNeighbors];
					for (int i = 0; i < smap->cells[id].nbNeighbors; i++) {
						smap->cells[id].neighbors[i] = listPtrVoisins[i];
					}
					delete[] listPtrVoisins;

					// Ajout nouveaux voisins
					smap->cells[id].neighbors[smap->cells[id].nbNeighbors] = &(scell);

					// Modif nbNeighbors
					smap->cells[id].nbNeighbors += 1;
				}
			}

			// Modif nbNeighbors
			scell.nbNeighbors += newNeighIDList.size();
		}
	}
}