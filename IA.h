#pragma once

#include "Analyser.h"
#include "Board.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include "Player.h"

using std::vector;
using std::map;
using std::set;
using std::string;

/**
 * @class IA
 * @brief Classe représentant le joueur IA dans le jeu Othello.
 */
class IA : public Player
{
private:
    int level; ///< Profondeur de l'arbre de l'IA (-1 car on ne compte pas la racine)
    char playerAdv; ///< Couleur de pion de l'IA ('X' ou 'O')

    Analyser Anal; ///< Instance de la classe Analyse pour savoir qui a l'avantage à partir d'un plateau de jeu Othello.
    Board plateau; ///< Instance de la classe Plateau pour représenter un plateau de jeu.
	
	/**
     * @brief Fonction qui évalue l'arbre de jeu avec l'algorithme minimax et alpha-beta pruning.
     * @param dico_board Dictionnaire représentant l'état actuel du plateau de jeu.
     * @param niveau Niveau de l'arbre à évaluer.
     * @param ai Booléen indiquant si c'est le tour de l'IA.
     * @param alpha Valeur alpha pour le pruning.
     * @param beta Valeur beta pour le pruning.
     * @return Score évalué pour le joueur courant.
     */
    float arbre(const map<int,char>& dico_board, const int& niveau, const bool& ai, const float& alpha, const float& beta);
    
public:

	/**
     * @brief Constructeur de la classe IA.
     * @param nom Nom du joueur IA.
     * @param pion Caractère représentant le pion de l'IA ('X' ou 'O').
     * @param niveau Niveau de difficulté de l'IA.
     */
    IA(string nom, char pion, int niveau);
	
	/**
     * @brief Fonction qui effectue le coup optimal pour l'IA en utilisant l'algorithme minimax.
     * @param dico_board Dictionnaire représentant l'état actuel du plateau de jeu.
     * @param pawn Nombre de pions actuellement placés sur le plateau.
     * @return Meilleur coup à jouer.
     */
    int play(const map<int,char>& dico_board, const int& pawn);
	
	/**
     * @brief Obtient le niveau de difficulté de l'IA.
     * @return Niveau de difficulté.
     */
    int get_level() const;
};
