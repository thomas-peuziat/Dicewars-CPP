#pragma once
// v 1.3

#include <cstddef>

// Structure d�finissant l'identification de la strat�gie
const unsigned int NbMembers = 5;
typedef struct
{
	char name[30];	// Nom de la strat�gie
	char members[NbMembers][50];	// Noms des personnes participant au projet.
} SPlayerInfo;

// Structure d�finissant les caract�ristiques d'une cellule
typedef struct
{
	int id;	// Id de la cellule
	int owner;	// Id du joueur qui la poss�de
	int nbDices;	// Nombre de d�s
} SCellInfo;

typedef struct
{
	SCellInfo *cells;	// Informations sur le contenu des cellules
	std::size_t nbCells;
	unsigned int points[8];	// Points de chaque joueur
	unsigned int diceStock[8];	// R�serve de d�s de chaque joueur
} SGameState;

// Structure d�finissant les caract�ristiques d'une cellule
typedef struct SCell
{
	SCellInfo infos;	// Informations sur la cellule
	struct SCell **neighbors;	// Tableau de poiteur vers des cellules voisines
	int nbNeighbors;	// Nombre de cellules voisines
} SCell;

// Structure d�finissant la carte globale du jeu
typedef struct
{
  SCell *cells;	// Tableau des cellules
  int nbCells;	// Nombre de cellules
} SMap;


// Structure d�finissant les param�tres d'un coup jou�
typedef struct
{
	unsigned int cellFrom;
	unsigned int cellTo;
} STurn;

// Types pr�d�finis pour simplifier la r�cup�ration des fonctions dans la librairie
typedef void* (*pInitGame)(unsigned int id, unsigned int nbPlayer,  const SMap *map, SPlayerInfo *info);
typedef int (*pPlayTurn)(unsigned int gameTurn, void *ctx, const SGameState *state , STurn *turn);
typedef void (*pEndGame)(void *ctx, unsigned int idWinner);
