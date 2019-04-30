#pragma once
#include "../../Commun/interface.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <set>
#include <map>


typedef std::pair<int, int> Coordinates;
typedef std::vector<std::vector<int>> Matrix;
typedef std::map<int, std::set<Coordinates>> MapTerritoire;

unsigned int setNumberTerritories(unsigned int nb_players);
std::set<Coordinates> getVoisins(const Coordinates &coord, int L, int C, const Matrix & matrix);
std::set<Coordinates> getVoisinsDisponibles(const Coordinates &coord, int L, int C, const Matrix & matrix);

bool CheckEndInit(Matrix & matrix, MapTerritoire & map, int L, int C);
bool already_expanded(MapTerritoire & map, Matrix & matrix, int id_territory, int L, int C);
void displayMatrix(const int L, const int C, const Matrix &matrix);
bool isEven(int a);
void afficherMap(const MapTerritoire &m);