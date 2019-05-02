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



bool AttaqueCouteQueCoute(int nbCells, int id, const SGameState *state, const SMap* map, STurn* turn) {
	int cellFrom = -1;
	int cellTo = -1;
	int diffDes = -1;
	int diffDesMax = -1;
	SCell *territories = map->cells;

	SCellInfo myCell;
	SCellInfo neighborCell;
	for (int i = 0; i < nbCells; i++) {
		if (state->cells[i].owner == id) {
			// Si la cellule est celle du joueur
			myCell = state->cells[i];
			if (myCell.nbDices > 1)
			{
				for (int j = 0; j < territories[myCell.id].nbNeighbors; j++) {
					// Pour chaque voisin de la celulle en cours

					int neigId = territories[myCell.id].neighbors[j]->infos.id;
					if (state->cells[neigId].owner != (id)) {
						// La cellule n'est pas au joueur
						neighborCell = state->cells[neigId];
						diffDes = myCell.nbDices - neighborCell.nbDices;

						if (diffDes >= 0 && diffDes >= diffDesMax) {
							cellFrom = myCell.id;
							cellTo = neighborCell.id;
							diffDesMax = diffDes;
						}
					}
				}
			}

		}
	}

	if (cellFrom != -1 && cellTo != -1) {
		turn->cellFrom = cellFrom;
		turn->cellTo = cellTo;
		return true;
	}
	else {
		return false;
	}
}