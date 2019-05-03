#pragma once
#include "../../Commun/interface.h"
#include "generation.h"
#include "../../Commun/library.h"
#include "../../Commun/interface_gui.h"
#include <vector>
#include <map>
#include <iostream>
#include "MapLoader.h"


MapTerritoire InitMap(SMap *smap, int nbTerritoires, int nbLignes, int nbColonnes, int nbPlayers);
void RetablirEtat(const SMap *map, SGameState *state);
void ValiderEtat(SMap *map, const SGameState*state);
bool ValidAttack(const STurn *turn, const SMap *map, const SGameState *state, int playerID);
void InitGameState(const SMap *map, SGameState *state, unsigned int nbPlayer);
void Confrontation(const STurn *turn, SGameState *state, SGameTurn *sGameTurn, int idPlayer);
int getNbTerritories(int IDPlayer, SGameState *state);
bool isWin(int idPlayer, SGameState *state);
int getMaxConnexite(int IdPlayer, const SMap *map, const SGameState *state);
int getMaxConnexite(int IdPlayer, const SMap * map);
void modifierValuesVector(int oldColorNumber, int newColorNumber, std::vector<int> &colorVector);
void distributionDes(int idPlayer, int nbDes, SGameState *state, SMap *map);
void updatePoints(unsigned int nbPlayers, SGameState *state, const SMap *map);
void LoadMapPerso(Regions &regions, MapTerritoire map);