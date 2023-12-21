#pragma once

#include <iostream>
#include <map>
#include <vector>

using std::vector;
using std::map;

/**
 * @class Analyser
 * @brief Classe représentant l'analyseur de plateau pour évaluer la situation du jeu.
 */
class Analyser
{
private:
	// Variables en majuscule sont les poids qu'on donne aus différents score lors de la partie
    int EARLY_CASE = 12; ///< Score attribué pour les cases en EARLYGAME.
    int EARLY_FLEXI = 7; ///< Score attribué pour la flexibilité en EARLYGAME.
    int EARLY_NBR = 1; ///< Score attribué pour le nombre de pions en EARLYGAME.

    int MIDDLE_CASE = 10; ///< Score attribué pour les cases en MIDDLEGAME.
    int MIDDLE_FLEXI = 9; ///< Score attribué pour la flexibilité en MIDDLEGAME.
    int MIDDLE_NBR = 1; ///< Score attribué pour le nombre de pions en MIDDLEGAME.

    int END_CASE = 6; ///< Score attribué pour les cases en ENDGAME.
    int END_FLEXI = 4; ///< Score attribué pour la flexibilité en ENDGAME.
    int END_NBR = 10; ///< Score attribué pour le nombre de pions en ENDGAME.

    int sco_case; ///< poid pour l'évaluation des cases.
    int sco_flexi; ///< poid pour la flexibilité.
    int sco_nbr; ///< poid pour le nombre de pions

    map<int,int> tableau_scoreCase; ///< Tableau de score pour chaque case du plateau.

public:
	/**
     * @brief Constructeur de la classe Analyser.
     */
    Analyser(void);
    
    /**
     * @brief Fonction d'analyse évaluant la situation du jeu sur le plateau.
     * @param plateau Dictionnaire représentant l'état actuel du plateau de jeu.
     * @param joueur Caractère représentant le joueur actuel ('X' ou 'O').
     * @return Score évalué pour la situation du joueur actuel.
     */
    float Analyse(const map<int,char>& plateau, const char& joueur);
	
	/**
     * @brief Définit les scores pour la phase EARLYGAME.
     */
    void set_sco_EARLYGAME();
    
    /**
     * @brief Définit les scores pour la phase MIDDLEGAME.
     */
    void set_sco_MIDDLEGAME();
    
    /**
     * @brief Définit les scores pour la phase ENDGAME.
     */
    void set_sco_ENDGAME();
};
