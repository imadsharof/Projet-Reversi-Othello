#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "Player.h"

using std::string;
using std::ofstream;
using std::ifstream;

/**
 * @class Fechier
 * @brief Classe représentant un joueur de type Fichier dans le jeu.
 */
class Fechier : public Player
{
private:
    string dossier; ///< Dossier contenant les fichiers de communication avec l'adversaire.

	string nom_fichier_noir; ///< Nom du fichier de communication pour le joueur noir.
	string nom_fichier_blanc; ///< Nom du fichier de communication pour le joueur blanc.

	ofstream fichier_Write; ///< Flux de sortie pour écrire dans le fichier de communication.
	ifstream fichier_Read; ///< Flux d'entrée pour lire depuis le fichier de communication.

	string nom_adv; ///< Nom de l'adversaire lu depuis le fichier.
  
	/**
     * @brief Initialise les noms des fichiers de communication et ouvre les flux de fichiers.
     * @return true si l'initialisation est réussie, false sinon.
     */
    bool initNom();
    
    /**
     * @brief Envoie le coup de l'adversaire dans le fichier de communication.
     * @param coord Coordonnée du coup de l'adversaire.
     * @return true si l'envoi est réussi, false sinon.
     */
    bool advMove(int coord);
    
    /**
     * @brief Vérifie la validité de la réponse du joueur dans le fichier.
     * @param answer Réponse du joueur.
     * @param legal_moves Mouvements légaux possibles.
     * @return true si la réponse est valide, false sinon.
     */
	bool verify_answer(const string& answer, const vector<string>& legal_moves);
	
public:
	
	/**
     * @brief Constructeur de la classe Fechier.
     * @param nom Nom du joueur.
     * @param pion Caractère représentant le pion du joueur ('X' ou 'O').
     * @param file Dossier contenant les fichiers de communication.
     */
    Fechier(const string& nom, char pion, string file);
    
    /**
     * @brief Destructeur de la classe Fechier.
     */
    ~Fechier();
    
    /**
     * @brief Fonction qui gère le tour de jeu du joueur Fichier.
     * @param moves Liste des mouvements précédents.
     * @param moveAdv Dernier coup de l'adversaire.
     * @return Coup à jouer par le joueur Fichier.
     */
    int play(const vector<int>& moves, const int& moveAdv);
	
	/**
     * @brief Obtient le nom de l'adversaire lu depuis le fichier.
     * @return Nom de l'adversaire.
     */
	string get_name_adv() const;
};
