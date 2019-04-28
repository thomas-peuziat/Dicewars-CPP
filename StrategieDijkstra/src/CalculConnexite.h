#pragma once
#include "../../Commun/interface.h"
#include <vector>
#include <map>

std::vector<int> calculateConnexite(int IdPlayer, const SMap * map, const SGameState * state, const int nbCells);

std::map<unsigned int, unsigned int> getMapWithColors(std::vector<int> &colorVector);

int getMaxConnexite(std::map<unsigned int, unsigned int>& nbColor);

void modifierValuesVector(int oldColorNumber, int newColorNumber, std::vector<int> &colorVector, const int nbCells);