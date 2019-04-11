#pragma once
// v 1.0

#include "interface.h"

typedef struct
{
	unsigned int idPlayer;
	STurn turn;
	unsigned int dices[2][8];
} SGameTurn;

struct SRegionCell
{
	unsigned int x;
	unsigned int y;
};

struct SRegion
{
	size_t nbCells;
	SRegionCell *cells;
};

struct SRegions
{
	size_t nbRegions;
	SRegion *region;
};

#ifndef GUI_DLL
#include "library.h"

extern "C"
{
	API_EXPORT void* InitGUI(unsigned int nbPlayer, const SRegions *map);
	API_EXPORT void SetPlayerInfo(void *ctx, unsigned int id, const SPlayerInfo *info);
	API_EXPORT void SetGameState(void *ctx, unsigned int idTurn, const SGameState *state);
	API_EXPORT void UpdateGameState(void *ctx, unsigned int idTurn, const SGameTurn *turn, const SGameState *finalState);
	API_EXPORT void UninitGUI(void *ctx);
}
#endif
