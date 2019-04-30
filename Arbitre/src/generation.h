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
std::set<Coordinates> odd_coordinates(const Coordinates &coord, int L, int C, Matrix & matrix);
std::set<Coordinates> even_coordinates(const Coordinates &coord, int L, int C, Matrix & matrix);
void displayMatrix(const int L, const int C, const Matrix &matrix);
void rd_expand_cell(const std::set<Coordinates> & list);
bool already_expanded(MapTerritoire & map, Matrix & matrix, int id_territory, int L, int C);
bool CheckEndInit(Matrix & matrix, MapTerritoire & map, int L, int C);
bool isEven(int a);
void afficherMap(const MapTerritoire &m);
void initialisationMap();
