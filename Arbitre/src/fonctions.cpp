#include "fonctions.h"


void RetablirEtat(const SMap *map, SGameState *state)
{
	for (int i = 0; i < map->nbCells; i++)
		state->cells[i] = map->cells[i].infos;
}

void ValiderEtat(SMap *map, const SGameState*state)
{
	for (int i = 0; i < map->nbCells; i++)
		map->cells[i].infos = state->cells[i];
}

void InitMap(SMap *map)
{
	map->cells = (SCell*)malloc(sizeof(SCell)*NB_CELL);
	SCell cell[NB_CELL];
	std::vector<SCell*> ptcell;
	for (auto i = 0; i < NB_CELL; i++)
	{
		SCell c;
		c.infos.id = i;
		c.infos.owner = rand() % 5;
		c.infos.nbDices = rand() % 6 + 1;
		cell[i] = c;


	}


	for (auto i = 0; i < NB_CELL; i++)
	{
		map->cells[i] = cell[i];
	}
	map->nbCells = NB_CELL;


	SCell* ptcell0 = &map->cells[0];
	SCell* ptcell1 = &map->cells[1];
	SCell* ptcell2 = &map->cells[2];
	SCell* ptcell3 = &map->cells[3];
	SCell* ptcell4 = &map->cells[4];
	SCell* ptcell5 = &map->cells[5];
	SCell* ptcell6 = &map->cells[6];
	SCell* ptcell7 = &map->cells[7];
	SCell* ptcell8 = &map->cells[8];
	SCell* ptcell9 = &map->cells[9];


	std::vector<SCell*> v1 = { ptcell1, ptcell2 };
	map->cells[0].nbNeighbors = 2;
	map->cells[0].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[0].nbNeighbors);
	for (auto i = 0; i < map->cells[0].nbNeighbors; i++)
	{
		map->cells[0].neighbors[i] = v1[i];
	}
	std::vector<SCell*> v2 = { ptcell0, ptcell7 };

	map->cells[1].nbNeighbors = 2;
	map->cells[1].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[1].nbNeighbors);
	for (auto i = 0; i < map->cells[1].nbNeighbors; i++)
	{
		map->cells[1].neighbors[i] = v2[i];
	}

	std::vector<SCell*> v3 = { ptcell0,ptcell7, ptcell3 };

	map->cells[2].nbNeighbors = 3;
	map->cells[2].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[2].nbNeighbors);
	for (auto i = 0; i < map->cells[2].nbNeighbors; i++)
	{
		map->cells[2].neighbors[i] = v3[i];
	}

	std::vector<SCell*> v4 = { ptcell2,ptcell4, ptcell8 };

	map->cells[3].nbNeighbors = 3;
	map->cells[3].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[3].nbNeighbors);
	for (auto i = 0; i < map->cells[3].nbNeighbors; i++)
	{
		map->cells[3].neighbors[i] = v4[i];
	}

	std::vector<SCell*> v5 = { ptcell6, ptcell5, ptcell3 };

	map->cells[4].nbNeighbors = 3;
	map->cells[4].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[4].nbNeighbors);
	for (auto i = 0; i < map->cells[4].nbNeighbors; i++)
	{
		map->cells[4].neighbors[i] = v5[i];
	}

	std::vector<SCell*> v6 = { ptcell4, ptcell8 };

	map->cells[5].nbNeighbors = 2;
	map->cells[5].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[5].nbNeighbors);
	for (auto i = 0; i < map->cells[5].nbNeighbors; i++)
	{
		map->cells[5].neighbors[i] = v6[i];
	}

	std::vector<SCell*> v7 = { ptcell4 };

	map->cells[6].nbNeighbors = 1;
	map->cells[6].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[6].nbNeighbors);
	for (auto i = 0; i < map->cells[6].nbNeighbors; i++)
	{
		map->cells[6].neighbors[i] = v7[i];
	}

	std::vector<SCell*> v8 = { ptcell2, ptcell1 };

	map->cells[7].nbNeighbors = 2;
	map->cells[7].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[7].nbNeighbors);
	for (auto i = 0; i < map->cells[7].nbNeighbors; i++)
	{
		map->cells[7].neighbors[i] = v8[i];
	}

	std::vector<SCell*> v9 = { ptcell3, ptcell5, ptcell9 };


	map->cells[8].nbNeighbors = 3;
	map->cells[8].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[8].nbNeighbors);
	for (auto i = 0; i < map->cells[8].nbNeighbors; i++)
	{
		map->cells[8].neighbors[i] = v9[i];
	}

	std::vector<SCell*> v10 = { ptcell8 };

	map->cells[9].nbNeighbors = 1;
	map->cells[9].neighbors = (SCell**)malloc(sizeof(SCell*)*	map->cells[9].nbNeighbors);
	for (auto i = 0; i < map->cells[9].nbNeighbors; i++)
	{
		map->cells[9].neighbors[i] = v10[i];
	}
}

bool ValidAttack(const STurn *turn, const SMap *map, const SGameState *state, int playerID) {
	const SCell& cellFrom = map->cells[turn->cellFrom];
	const SCellInfo& cellInfoFrom = state->cells[cellFrom.infos.id];
	const SCell& cellTo = map->cells[turn->cellTo];
	const SCellInfo& cellInfoTo = state->cells[cellTo.infos.id];

	if (cellInfoTo.owner == playerID ||		// Si on s'attaque soi-même
		cellInfoFrom.owner != playerID ||	// Si on ne possède pas la cellule
		cellInfoFrom.nbDices <= 1)			// Si on ne possède pas assez de dés
		return false;

	bool isNeighbor = false;
	for (int i = 0; i < cellFrom.nbNeighbors; i++) {
		if (cellFrom.neighbors[i]->infos.id == cellTo.infos.id) {
			isNeighbor = true;
			break;
		}
	}

	if (!isNeighbor)
		return false;		// La cellule n'est pas voisine

	return true;	// Le coup est valide
}

void InitGameState(const SMap *map, SGameState *state)
{
	state->cells = (SCellInfo*)malloc(sizeof(SCellInfo)*NB_CELL);
	for (int i = 0; i < map->nbCells; i++)
		state->cells[i] = map->cells[i].infos;

	state->nbCells = NB_CELL;
}

void Confrontation(const STurn *turn, SGameState *state, /*--SGameTurn* sGameTurn, --*/int idPlayer)
{
	int NbDicesFrom = state->cells[turn->cellFrom].nbDices;
	int NbDicesTo = state->cells[turn->cellTo].nbDices;

	int TotalFrom = 0;
	int TotalTo = 0;

	int scoreDes;

	for (int i = 0; i < NbDicesFrom; i++) {
		scoreDes = (rand() % 6) + 1;
		TotalFrom += scoreDes;
		//--sGameTurn->dices[0][i] = scoreDes;
	}

	for (int i = 0; i < NbDicesTo; i++) {
		scoreDes = (rand() % 6) + 1;
		TotalTo += scoreDes;
		//--sGameTurn->dices[1][i] = scoreDes;
	}

	if (TotalFrom > TotalTo)				// si l'attaquant a gagné
	{
		state->cells[turn->cellTo].owner = state->cells[turn->cellFrom].owner;
		state->cells[turn->cellTo].nbDices = state->cells[turn->cellFrom].nbDices - 1;
		state->cells[turn->cellFrom].nbDices = 1;

	}
	else									// si l'attaquant a perdu 
	{
		state->cells[turn->cellFrom].nbDices = 1;
	}
	//--sGameTurn->idPlayer = idPlayer;
	//--sGameTurn->turn = *(turn);
}

int getNbTerritories(int IDPlayer, SGameState *state) {
	int nbTerr = 0;
	for (int i = 0; i < state->nbCells; i++)
	{
		if (state->cells[i].owner == IDPlayer)
			nbTerr++;
	}
	return nbTerr;
}

bool isWin(int idPlayer, SGameState *state)
{
	return (getNbTerritories(idPlayer, state) == NB_CELL);
}


int getMaxConnexite(int IdPlayer, const SMap *map)
{

	int color = 0;
	std::vector<int> colorVector(NB_CELL, color);									// Initialisation du vector
	for (int i = 0; i < NB_CELL; i++) 
	{
		if (map->cells[i].infos.owner == IdPlayer)									// Le celulle doit être la sienne
		{
			if (colorVector.at(i) == 0)
			{
				color++;
				colorVector.at(i) = color;
			}
			else {
			}

			for (int j = 0; j < map->cells[i].nbNeighbors; j++)
			{
				if (map->cells[i].neighbors[j]->infos.owner == IdPlayer)									// Le celulle doit être la sienne
				{
					int idCell = map->cells[i].neighbors[j]->infos.id;
					if (colorVector.at(idCell) != 0)
					{
						modifierValuesVector(colorVector.at(idCell), color, colorVector);
					}
					else {
						colorVector.at(j) = color;
					}
				}
			}
			
		}
	}

	return 0;
}

void modifierValuesVector(int oldColorNumber, int newColorNumber, std::vector<int> &colorVector) {

	for (int i = 0; i < NB_CELL; i++) {
		if (colorVector.at(i) == oldColorNumber) {
			colorVector.at(i) = newColorNumber;
		}
	}
}