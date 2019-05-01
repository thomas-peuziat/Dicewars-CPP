#pragma once
#include "../../Commun/interface.h"
#include "../../Commun/library.h"
#include "../../Commun/interface_gui.h"
#include <vector>
#include <map>
#include <iostream>

void InitMap(SMap *map);
void RetablirEtat(const SMap *map, SGameState *state);
void ValiderEtat(SMap *map, const SGameState*state);
bool ValidAttack(const STurn *turn, const SMap *map, const SGameState *state, int playerID);
void InitGameState(const SMap *map, SGameState *state, unsigned int nbPlayer);
void Confrontation(const STurn *turn, SGameState *state, SGameTurn *sGameTurn, int idPlayer);
int getNbTerritories(int IDPlayer, SGameState *state);
bool isWin(int idPlayer, SGameState *state);
int getMaxConnexite(int IdPlayer, const SMap *map, const SGameState *state);
void modifierValuesVector(int oldColorNumber, int newColorNumber, std::vector<int> &colorVector);
void distributionDes(int idPlayer, int nbDes, SGameState *state, SMap *map);
void updatePoints(unsigned int nbPlayers, SGameState *state, const SMap *map);
//void PlayGame(const int nbPlayers, void* ctx[], const SGameState *state, const SMap *map, pInitGame* tab_InitGame, pPlayTurn* tab_PlayTurn , pEndGame* tab_InitGame);
