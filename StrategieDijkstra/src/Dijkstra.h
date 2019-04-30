#pragma once
#include "interface_lib.h"
#include <cstring>
#include <iostream>
#include <vector>
#include <time.h>
#include <limits.h>
#include "../../Commun/interface.h"

int initDijkstra(const SMap *map, const int nbCells, int idCellDepart, int idCellArrivee, std::vector<int> &predecesseurs);
void iterationDijkstra(const SMap* map, int nbCells, std::vector<int> &Dist, std::vector<int> &Pred, int idCellDepart, std::vector<bool> &sommetDijkstra);
int mini(int a, int b);
void Affiche_Parcours_Min(std::vector<int> &distance, std::vector<int> &predecesseurs, int cellDep, int cellArr, const SMap *map);
