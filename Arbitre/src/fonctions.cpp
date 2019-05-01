#include "fonctions.h"
#include "generation.h"

void RetablirEtat(const SMap *map, SGameState *state)
{
	for (int i = 0; i < map->nbCells; i++)
		state->cells[i] = map->cells[i].infos;
}

void ValiderEtat(SMap *map, const SGameState*state)
{
	for (int i = 0; i < map->nbCells; i++) {
		map->cells[i].infos = state->cells[i];
	}
		
}

MapTerritoire InitMap(SMap *smap, int nbTerritoires, int nbLignes, int nbColonnes, int nbPlayers)
{

	Matrix matrix(nbColonnes, std::vector<int>(nbLignes, -1));
	MapTerritoire map;


	// Affichage Matrix	
	//displayMatrix(nbLignes, nbColonnes, matrix);

	// Calcul des bornes pour le random_
	int c_borne = nbColonnes - 1;
	int l_borne = nbLignes - 1;

	// Cr�ation SCell sans voisins
	smap->cells = new SCell[nbTerritoires]();
	smap->nbCells = nbTerritoires;
	for (auto i = 0; i < smap->nbCells; i++)
	{
		smap->cells[i].infos.id = i;
		smap->cells[i].infos.owner = -1;
		smap->cells[i].infos.nbDices = 2;
	}

	// Premier tour -> Cr�ation al�atoire des cellules de base
	for (int i = 0; i < nbTerritoires; i++) {


		bool check1 = false;
		while (!check1) {
			int c = rand() % (c_borne);
			int l = rand() % (l_borne);

			if (matrix[c][l] == -1) {
				check1 = true;
				matrix[c][l] = i;
				map[i].insert(std::make_pair(l, c));

				//displayMatrix(nbLignes, nbColonnes, matrix);
				std::set<Coordinates> listVoisins = getVoisins(std::make_pair(l, c), nbLignes, nbColonnes, matrix);
				addNewNeighborsSCell(smap, i, listVoisins, matrix);
			}
		}
	}

	

	afficherMap(map);

	bool end;
	bool isFullConnexe = false;
	int nbCellAdded = nbTerritoires;
	do {

		end = false;
		for (int k = 0; k < nbTerritoires; k++) {
			std::set<Coordinates> territory_cells = map.find(k)->second;

			if (!already_expanded(map, matrix, k, nbLignes, nbColonnes)) {
				std::set<Coordinates> list_base;
				std::set<Coordinates> list;
				for (Coordinates coord : territory_cells) {
					list = getVoisinsDisponibles(coord, nbLignes, nbColonnes, matrix);
					list_base.insert(list.begin(), list.end());
				}

				if (!list_base.empty()) {
					int size_list_base = list_base.size();
					int id_cell_rd = rand() % (size_list_base);
					
					std::set<Coordinates>::iterator it = list_base.begin();
					std::advance(it, id_cell_rd);
					Coordinates coord_a = *it;

					matrix[coord_a.second][coord_a.first] = k;
					map[k].insert(std::make_pair(coord_a.first, coord_a.second));

					nbCellAdded++;

					//displayMatrix(nbLignes, nbColonnes, matrix);
					std::set<Coordinates> listVoisins = getVoisins(coord_a, nbLignes, nbColonnes, matrix);
					addNewNeighborsSCell(smap, k, listVoisins, matrix);

					
					int connexite = getMaxConnexite(-1, smap);
					if(connexite == nbTerritoires)
						isFullConnexe = true;


				}

			}
			else {
				end = false;
			}

			bool check = false;

		
			//afficherMap(map);
			//displayMatrix(nbLignes, nbColonnes, matrix);
		}
		end = CheckEndInit(matrix, map, nbLignes, nbColonnes);
		//afficherMap(map);
		//displayMatrix(nbLignes, nbColonnes, matrix);

	} while ((!end) && !(isFullConnexe && (nbCellAdded >= (nbColonnes*nbLignes)*3/4) ));


	/* map->cells = (SCell*)malloc(sizeof(SCell)*NB_CELL);
	SCell cell[NB_CELL];
	std::vector<SCell*> ptcell;
	for (auto i = 0; i < NB_CELL; i++)
	{
		SCell c;
		c.infos.id = i;
		c.infos.owner = rand() % 3;
		std::cout << c.infos.owner << std::endl;
		c.infos.nbDices = rand() % 3 + 1;
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
	}*/
	
	displayMatrix(nbLignes, nbColonnes, matrix);
	return map;
}

bool ValidAttack(const STurn *turn, const SMap *map, const SGameState *state, int playerID) {
	const SCell& cellFrom = map->cells[turn->cellFrom];
	const SCellInfo& cellInfoFrom = state->cells[cellFrom.infos.id];

	const SCell& cellTo = map->cells[turn->cellTo];
	const SCellInfo& cellInfoTo = state->cells[cellTo.infos.id];

	if (cellInfoTo.owner == playerID ||		// Si on s'attaque soi-m�me
		cellInfoFrom.owner != playerID ||	// Si on ne poss�de pas la cellule
		cellInfoFrom.nbDices <= 1)			// Si on ne poss�de pas assez de d�s
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

void InitGameState(const SMap *map, SGameState *state, unsigned int nbPlayer)
{
	state->cells = (SCellInfo*)malloc(sizeof(SCellInfo)*map->nbCells);
	for (int i = 0; i < map->nbCells; i++)
		state->cells[i] = map->cells[i].infos;

	state->nbCells = map->nbCells;

	for (unsigned int i = 0; i < nbPlayer; i++)
	{
		state->points[i] = 0;
		state->diceStock[i] = 0;
	}
}

void Confrontation(const STurn *turn, SGameState *state, SGameTurn* sGameTurn, int idPlayer)
{
	int NbDicesFrom = state->cells[turn->cellFrom].nbDices;
	int NbDicesTo = state->cells[turn->cellTo].nbDices;

	int TotalFrom = 0;
	int TotalTo = 0;

	int scoreDes;

	for (int i = 0; i < NbDicesFrom; i++) {
		scoreDes = (rand() % 6) + 1;
		TotalFrom += scoreDes;
		sGameTurn->dices[0][i] = scoreDes;
	}

	for (int i = 0; i < NbDicesTo; i++) {
		scoreDes = (rand() % 6) + 1;
		TotalTo += scoreDes;
		sGameTurn->dices[1][i] = scoreDes;
	}

	if (TotalFrom > TotalTo)				// si l'attaquant a gagn�
	{
		state->cells[turn->cellTo].owner = state->cells[turn->cellFrom].owner;
		state->cells[turn->cellTo].nbDices = state->cells[turn->cellFrom].nbDices - 1;
		state->cells[turn->cellFrom].nbDices = 1;

	}
	else									// si l'attaquant a perdu 
	{
		state->cells[turn->cellFrom].nbDices = 1;
	}
	sGameTurn->idPlayer = idPlayer;
	sGameTurn->turn = *(turn);
}

int getNbTerritories(int IDPlayer, SGameState *state) {
	int nbTerr = 0;
	for (unsigned int i = 0; i < state->nbCells; i++)
	{
		if (state->cells[i].owner == IDPlayer)
			nbTerr++;
	}
	return nbTerr;
}

bool isWin(int idPlayer, SGameState *state)
{
	if (getNbTerritories(idPlayer, state) == state->nbCells) {
		std::cout << "Player " << idPlayer << " win" << std::endl;
		return true;
	}

	return false;
	
}

int getMaxConnexite(int IdPlayer, const SMap * map)
{
	int color = 0;
	std::vector<int> colorVector(map->nbCells, color);									// Initialisation du vector
	for (int i = 0; i < map->nbCells; i++)
	{
		if (map->cells[i].infos.owner == IdPlayer)									// Le celulle doit �tre la sienne
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
				int neigId = map->cells[i].neighbors[j]->infos.id;
				if (map->cells[neigId].infos.owner == IdPlayer)									// Le celulle doit �tre la sienne
				{
					int idCell = map->cells[i].neighbors[j]->infos.id;
					if (colorVector.at(idCell) != 0)
					{
						if (colorVector.at(idCell) != colorVector.at(i))
							modifierValuesVector(colorVector.at(idCell), colorVector.at(i), colorVector);
					}
					else {
						colorVector.at(idCell) = colorVector.at(i);
					}
				}
			}

		}
	}

	std::map<unsigned int, unsigned int> nbColor;

	for (const int& it : colorVector) {
		if (it != 0) {
			auto search = nbColor.find(it);
			if (search == nbColor.end()) {
				unsigned int value = it;
				nbColor.insert({ it, 1 });
			}
			else {
				search->second++;
			}
		}
	}

	unsigned int max = 0;
	for (auto it : nbColor) {
		if (it.second > max) {
			max = it.second;
		}
	}

	return max;
}

int getMaxConnexite(int IdPlayer, const SMap * map, const SGameState * state)
{
	int color = 0;
	std::vector<int> colorVector(map->nbCells, color);									// Initialisation du vector
	for (int i = 0; i < map->nbCells; i++)
	{
		if (state->cells[i].owner == IdPlayer)									// Le celulle doit �tre la sienne
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
				int neigId = map->cells[i].neighbors[j]->infos.id;
				if (state->cells[neigId].owner == IdPlayer)									// Le celulle doit �tre la sienne
				{
					int idCell = map->cells[i].neighbors[j]->infos.id;
					if (colorVector.at(idCell) != 0)
					{
						if (colorVector.at(idCell) != colorVector.at(i))
							modifierValuesVector(colorVector.at(idCell), colorVector.at(i), colorVector);
					}
					else {
						colorVector.at(idCell) = colorVector.at(i);
					}
				}
			}

		}
	}

	std::map<unsigned int, unsigned int> nbColor;

	for (const int& it : colorVector) {
		if (it != 0) {
			auto search = nbColor.find(it);
			if (search == nbColor.end()) {
				unsigned int value = it;
				nbColor.insert({ it, 1 });
			}
			else {
				search->second++;
			}
		}
	}

	unsigned int max = 0;
	for (auto it : nbColor) {
		if (it.second > max) {
			max = it.second;
		}
	}

	return max;
}


void modifierValuesVector(int oldColorNumber, int newColorNumber, std::vector<int> &colorVector) {

	for (int i = 0; i < colorVector.size(); i++) {
		if (colorVector.at(i) == oldColorNumber) {
			colorVector.at(i) = newColorNumber;
		}
	}
}

void distributionDes(int idPlayer, int nbDes, SGameState *state, SMap *map)
{
	std::vector<unsigned int> TCellPerPlayer;
	unsigned int cellPosition;

	// TODO : Optimiser parce qu'on passe plusieurs fois dans toute la map pour tout les joueurs
	// Faire une struct joueur avec tableau de cellule pour chaque joueur ?
	for (unsigned int i = 0; i < state->nbCells; i++) {
		if (state->cells[i].owner == idPlayer && state->cells[i].nbDices < 8) {
			TCellPerPlayer.push_back(state->cells[i].id);
		}
	}

	for (auto j = 0; j < nbDes; j++) {	
		if (!TCellPerPlayer.empty()) 
		{
			cellPosition = rand() % TCellPerPlayer.size();				// Choisi une cellule parmis les cellules d'un joueur
			state->cells[TCellPerPlayer[cellPosition]].nbDices++;	// Ajoute un d�s sur cette cellule dans le state
			map->cells[TCellPerPlayer[cellPosition]].infos.nbDices++;	// Ajoute un d�s sur cette cellule dans la map

			if (state->cells[TCellPerPlayer[cellPosition]].nbDices >= 8)
				TCellPerPlayer.erase(TCellPerPlayer.begin() + cellPosition);
		}
		else
			state->diceStock[idPlayer]++;
	}

	while (!TCellPerPlayer.empty() && state->diceStock[idPlayer] > 0) 
	{
		cellPosition = rand() % TCellPerPlayer.size(); 
		state->cells[TCellPerPlayer[cellPosition]].nbDices++;	// Ajoute un d�s sur cette cellule dans le state
		map->cells[TCellPerPlayer[cellPosition]].infos.nbDices++;	// Ajoute un d�s sur cette cellule dans la map

		if (state->cells[TCellPerPlayer[cellPosition]].nbDices >= 8)
			TCellPerPlayer.erase(TCellPerPlayer.begin() + cellPosition);

		state->diceStock[idPlayer]--;
	}
}

void updatePoints(unsigned int nbPlayers, SGameState *state, const SMap *map) {
	for (unsigned int i =0; i < nbPlayers; i++)
		state->points[i] = getMaxConnexite(i, map, state);
}
