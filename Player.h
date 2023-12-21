#pragma once

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include "Board.h"

using std::string;
using std::set;
using std::vector;

/**
 * @brief Enumération des types de joueurs
 */
enum class TypeJoueur {
    Humain,
    IA,
    Fichier
};

/**
 * @brief Classe de base pour un joueur
 */
class Player
{
public:
	TypeJoueur type;
    char pion;    
    /**
     * @brief Récupère l'état du "pass" du joueur
     * @return true si le joueur a passé son tour, sinon false
     */
    bool get_pass() const;
    /**
     * @brief Récupère le nom du joueur
     * @return Le nom du joueur
     */
    string get_name() const;
    /**
     * @brief Constructeur de la classe Player
     * @param nom Le nom du joueur
     * @param pion Le pion du joueur ('X' ou 'O')
     */
	Player(string nom, char pion);
    /**
     * @brief Destructeur virtuel de la classe Player
     */
	virtual ~Player();
    /**
     * @brief Traduit un indice en coordonnées de plateau
     * @param coord L'indice à traduire
     * @return La coordonnée correspondante
     */ 
	string traduction(int coord);
    /**
     * @brief Traduit des coordonnées de plateau en indice
     * @param coord Les coordonnées à traduire
     * @return L'indice correspondant
     */
	int traduction(string coord);
	string name;
protected:
    char pawn_color;
    bool pass;
    /**
     * @brief Génère une liste de mouvements légaux traduits en coordonnées
     * @param legal Les mouvements légaux en indices
     * @return Une liste de mouvements légaux traduits en coordonnées
     */
    vector<string> legal_moves(const vector<int>& legal);
};
