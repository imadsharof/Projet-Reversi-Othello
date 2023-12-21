#pragma once

#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>

using std::map;
using std::vector;

/**
 * @brief La classe Board représente le plateau du jeu Othello.
 */
class Board
{
private:
    map<int,char> dico_board; ///< Dictionnaire représentant le plateau de jeu.
    int num_playerX; ///< Nombre de pions pour le joueur X.
    int num_playerO; ///< Nombre de pions pour le joueur O.
    int nbPawn; ///< Nombre total de pions sur le plateau.

public:

    /**
     * @brief Constructeur par défaut de la classe Board.
     * Initialise le plateau avec la configuration par défaut.
     * (Plateau vide avec les 4 pions centraux de départ)
     */
    Board();

    /**
     * @brief Constructeur de la classe Board avec une configuration de plateau initiale donnée.
     * @param plateau La nouvelle configuration initiale du plateau.
     */
    Board(const map<int,char>& plateau);

    /**
     * @brief Évalue le score du plateau de jeu, mettant à jour le nombre de pions pour chaque joueur.
     */
    void evaluate_score();

    /**
     * @brief Obtient le score du joueur X.
     * @return Le score du joueur X.
     */
    int get_score_X() const;

    /**
     * @brief Obtient le score du joueur O.
     * @return Le score du joueur O.
     */
    int get_score_O() const;

    /**
     * @brief Obtient le nombre total de pions sur le plateau.
     * @return Le nombre total de pions sur le plateau.
     */
    int get_nbPawn() ;

    /**
     * @brief Obtient la configuration actuelle du plateau.
     * @return La configuration actuelle du plateau.
     */
    map<int,char> get_board() const;

    /**
     * @brief Obtient les mouvements légaux pour un joueur donné pour le plateau actuel.
     * @param player Le joueur pour lequel les mouvements légaux doivent être déterminés.
     * @return Un vecteur contenant les mouvements légaux pour le joueur spécifié.
     */
    vector<int> get_legal_moves(const char& player);

    /**
     * @brief Ajoute un pion au plateau pour un joueur spécifié à une coordonnée donnée.
     * @param player Le joueur ajoutant le pion.
     * @param coord La coordonnée où le pion est ajouté.
     * @return Vrai si le pion est ajouté avec succès, faux sinon.
     */
    bool add_pawn(const char& player, const int& coord);

    /**
     * @brief Affiche l'état actuel du plateau de jeu dans le stdout.
     */
    void display_board();

    /**
     * @brief Définit la configuration du plateau.
     * @param plateau La configuration du plateau.
     */
    void set_dicoBoard(const map<int,char>& plateau);

    /**
     * @brief Vérifie si le jeu est terminé.
     * @return Vrai si le jeu est terminé, faux sinon.
     */
    bool isGameOver();
};
