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
typedef std::map<int, std::set<Coordinates>> Map;

unsigned int setNumberTerritories(unsigned int nb_players);
std::set<Coordinates> odd_coordinates(const Coordinates &coord, int L, int C);
std::set<Coordinates> even_coordinates(const Coordinates &coord, int L, int C);
Coordinates pattern_treatment(const Coordinates &coord, int L, int C, const std::vector<std::vector<int>> &matrix);
void displayMatrix(const int L, const int C, const Matrix &matrix);
void rd_expand_cell(const std::set<Coordinates> & list);
bool isEven(int a);
bool isIsolated(const Matrix &mat, Coordinates coord, int L, int C);
void afficherMap(const Map &m);
void initialisationMap();
