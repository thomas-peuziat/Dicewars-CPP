#include "fonctionsStratDijkstra.h"


std::vector<int> calculateConnexite(int IdPlayer, const SMap * map, const SGameState * state, const int nbCells)
{
	int color = 0;
	std::vector<int> colorVector(nbCells, color);									// Initialisation du vector
	for (int i = 0; i < nbCells; i++)
	{
		if (state->cells[i].owner == IdPlayer)									// Le celulle doit être la sienne
		{
			if (colorVector.at(i) == 0)
			{
				color++;
				colorVector.at(i) = color;
			}
			else {
			}

			for (int j = 0; j < map->cells[i].nbNeighbors; j++)
			{
				if (map->cells[i].neighbors[j]->infos.owner == IdPlayer)									// Le celulle doit être la sienne
				{
					int idCell = map->cells[i].neighbors[j]->infos.id;
					if (colorVector.at(idCell) != 0)
					{
						if (colorVector.at(idCell) != colorVector.at(i))
							modifierValuesVector(colorVector.at(idCell), colorVector.at(i), colorVector, nbCells);
					}
					else {
						colorVector.at(idCell) = colorVector.at(i);
					}
				}
			}

		}
	}

	return colorVector;
}


std::map<unsigned int, unsigned int> getMapWithColors(std::vector<int> &colorVector) {
	std::map<unsigned int, unsigned int> nbColor;

	for (auto it : colorVector) {
		if (it != 0) {
			auto search = nbColor.find(it);
			if (search == nbColor.end()) {
				//unsigned int value = it;  
				nbColor.insert({ it, 1 });
			}
			else {
				search->second++;
			}
		}
	}
	return nbColor;
}

int getMaxConnexite(std::map<unsigned int, unsigned int> &nbColor) {
	unsigned int max = 0;
	for (auto it : nbColor) {
		if (it.second > max) {
			max = it.second;
		}
	}

	return max;
}


void modifierValuesVector(int oldColorNumber, int newColorNumber, std::vector<int> &colorVector, const int nbCells) {

	for (int i = 0; i < nbCells; i++) {
		if (colorVector.at(i) == oldColorNumber) {
			colorVector.at(i) = newColorNumber;
		}
	}
}


void constructWayToFollow(std::vector<int> &wayToFollow, std::vector<int> &pred, int cellDep, int cellArr) {
	int currentCell = cellArr;
	while (currentCell != cellDep)
	{
		wayToFollow.push_back(currentCell);
		currentCell = pred[currentCell]; //la cellule courante prend la valeur de son précédent
	}
}

void attaquerEnFonctionNbDes(std::vector<int>& vIndexeCellFrom, const SMap* map, STurn* turn) {
	int diffMaximale = -1;
	int diffDes = 0;
	int desCellAtk = 0;
	int desCellDef = 0;
	for (const int& i : vIndexeCellFrom) {
		//std::cout << "une cell que je possede -> " << i << std::endl;
		for (int j = 0; j < map->cells[i].nbNeighbors; j++) {
			//std::cout << "	une cellule voisine a attaquer -> " << map->cells[i].neighbors[j]->infos.id << std::endl;
			desCellAtk = map->cells[i].infos.nbDices;
			desCellDef = map->cells[i].neighbors[j]->infos.nbDices;
			diffDes = desCellAtk - desCellDef;
			if ((diffDes > diffMaximale) && (diffDes > 0)) {
				diffMaximale = diffDes;
				turn->cellFrom = i;
				turn->cellTo = map->cells[i].neighbors[j]->infos.id;
			}
		}
	}
}