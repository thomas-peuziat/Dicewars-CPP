#pragma once
#include "../../Commun/interface.h"
#include "../../Commun/library.h"
#include <vector>
const int NB_CELL = 10;



void InitMap(SMap *map);
void RetablirEtat(const SMap *map, SGameState *state);
void ValiderEtat(SMap *map, const SGameState*state);
bool ValidAttack(const STurn *turn, const SMap *map, const SGameState *state, int playerID);
void InitGameState(const SMap *map, SGameState *state);
void Confrontation(const STurn *turn, SGameState *state, /*--SGameTurn *sGameTurn, --*/int idPlayer);
int getNbTerritories(int IDPlayer, SGameState *state);
bool isWin(int idPlayer, SGameState *state);
//void PlayGame(const int nbPlayers, void* ctx[], const SGameState *state, const SMap *map, pInitGame* tab_InitGame, pPlayTurn* tab_PlayTurn , pEndGame* tab_InitGame);
