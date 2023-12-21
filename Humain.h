#pragma once

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include "Player.h"

using std::string;
using std::vector;

class Humain : public Player
{
private:
    /**
    * @brief Fonction privée pour vérifier la validité d'une réponse
    * @param answer La réponse à vérifier
    * @param legal_moves Les mouvements légaux possibles
    * @return true si la réponse est valide, sinon false
    */
    bool verify_answer(const string& answer, const vector<string>& legal_moves);

public:
    /**
    * @brief Fonction principale pour le tour de jeu du joueur humain
    * @param moves Les mouvements légaux possibles
    * @return L'indice du mouvement joué ou 0 en cas de "pass"
    */
    int play(const vector<int>& legal_moves);
    /**
     * @brief Constructeur de la classe Humain
     * @param nom Le nom du joueur
     * @param pion Le pion du joueur ('X' ou 'O')
     */
    Humain(string nom, char pion);
};

