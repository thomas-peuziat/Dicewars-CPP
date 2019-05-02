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

	// Calcul des bornes pour le random_
	int c_borne = nbColonnes - 1;
	int l_borne = nbLignes - 1;

	// Création SCell sans voisins
	smap->cells = new SCell[nbTerritoires]();
	smap->nbCells = nbTerritoires;
	for (auto i = 0; i < smap->nbCells; i++)
	{
		smap->cells[i].infos.id = i;
		smap->cells[i].infos.owner = -1;
		smap->cells[i].infos.nbDices = 4;
	}

	// Premier tour -> Création aléatoire des cellules de base
	for (int i = 0; i < nbTerritoires; i++) {


		bool check1 = false;
		while (!check1) {
			int c = rand() % (c_borne);
			int l = rand() % (l_borne);

			if (matrix[c][l] == -1) {
				check1 = true;
				matrix[c][l] = i;
				map[i].insert(std::make_pair(l, c));

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

		}
		end = CheckEndInit(matrix, map, nbLignes, nbColonnes);

	} while ((!end) && !(isFullConnexe && (nbCellAdded >= (nbColonnes*nbLignes)*3/4) ));

	for (int i = 0; i < smap->nbCells; i++) {
		smap->cells[i].infos.owner = i % nbPlayers;
	}
	
	displayMatrix(nbLignes, nbColonnes, matrix);



	return map;
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
	for (unsigned int i = 0; i < 8; ++i)
		for (unsigned int j = 0; j < 2; ++j)
			sGameTurn->dices[j][i] = 0;

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
				int neigId = map->cells[i].neighbors[j]->infos.id;
				if (map->cells[neigId].infos.owner == IdPlayer)									// Le celulle doit être la sienne
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
		if (state->cells[i].owner == IdPlayer)									// Le celulle doit être la sienne
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
				if (state->cells[neigId].owner == IdPlayer)									// Le celulle doit être la sienne
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
			state->cells[TCellPerPlayer[cellPosition]].nbDices++;	// Ajoute un dés sur cette cellule dans le state
			map->cells[TCellPerPlayer[cellPosition]].infos.nbDices++;	// Ajoute un dés sur cette cellule dans la map

			if (state->cells[TCellPerPlayer[cellPosition]].nbDices >= 8)
				TCellPerPlayer.erase(TCellPerPlayer.begin() + cellPosition);
		}
		else
			state->diceStock[idPlayer]++;
	}

	while (!TCellPerPlayer.empty() && state->diceStock[idPlayer] > 0) 
	{
		cellPosition = rand() % TCellPerPlayer.size(); 
		state->cells[TCellPerPlayer[cellPosition]].nbDices++;	// Ajoute un dés sur cette cellule dans le state
		map->cells[TCellPerPlayer[cellPosition]].infos.nbDices++;	// Ajoute un dés sur cette cellule dans la map

		if (state->cells[TCellPerPlayer[cellPosition]].nbDices >= 8)
			TCellPerPlayer.erase(TCellPerPlayer.begin() + cellPosition);

		state->diceStock[idPlayer]--;
	}
}

void updatePoints(unsigned int nbPlayers, SGameState *state, const SMap *map) {
	for (unsigned int i =0; i < nbPlayers; i++)
		state->points[i] = getMaxConnexite(i, map, state);
}
