#pragma once
#include "interface_lib.h"
#include <cstring>
#include <iostream>
#include <vector>
#include <time.h>

void initDijkstra(SMap *map, const int nbCells, int idCellDepart);
void iterationDijkstra(SMap* map, int nbCells, std::vector<int> &Dist, std::vector<int> &Pred, int idCellDepart, std::vector<int> &sommetDijkstra, int nbSommetsTraites);
int mini(int a, int b);
void Affiche_Parcours_Min(std::vector<int> &distance, std::vector<int> &predecesseurs, int cellDep, int cellArr, SMap *map);
