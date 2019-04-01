#pragma once
// v 1.0

#include "../../Commun/interface.h"
#include "../../Commun/library.h"

extern "C"
{
	// Fonction � appeler au d�but de chaque partie
	// La strat�gie doit compl�ter la structure SPlayerInfo
	//	* Ne pas oublier pour l'arbitre de dupliquer toute la structure map pour chaque appel !
	// valeur de retour : contexte de la strat�gie (� envoyer dans chaque appel de PlayTurn)
	API_EXPORT void* InitGame(unsigned int id, unsigned int nbPlayer, const SMap *map, SPlayerInfo *info);

	// Fonction � appeler � chaque tour sur la strat�gie et tant que le retour de fonction est vrai et qu'il n'y a pas d'erreur.
	//	* En cas d'erreur, r�tablir la carte dans l'�tat initial avant le premier tour du joueur.
	// valeur de retour : bool�en : 0 coups termin�s, 1 structure turn compl�t�e avec un nouveau coup � jouer.
	// gameTurn : tour de jeu (permet � la strat�gie de savoir si elle a �chou�e au tour pr�c�dant)
	// ctx : contexte de la strat�gie
	API_EXPORT int PlayTurn(unsigned int gameTurn, void *ctx, const SGameState *state, STurn *turn);

	// Fonction � appeler � la fin de chaque partie
	// ctx : contexte de la strat�gie
	API_EXPORT void EndGame(void *ctx, unsigned int idWinner);
}
