#include "Utility.h"

int getNbTerritories(int IDPlayer, const SGameState *state) {
	int nbTerr = 0;
	for (unsigned int i = 0; i < state->nbCells; i++)
	{
		if (state->cells[i].owner == IDPlayer)
			nbTerr++;
	}
	return nbTerr;
}

int getNbDices(int IDPlayer, const SGameState * state)
{
	int nbDices = 0;
	for (unsigned int i = 0; i < state->nbCells; i++)
	{
		if (state->cells[i].owner == IDPlayer)
			nbDices += state->cells[i].nbDices;
	}
	return nbDices;
}
