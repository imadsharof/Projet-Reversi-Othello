#include <iostream>
#include "Player.h"
#include "Board.h"
#include "IA.h"
#include "Humain.h"
#include "Fechier.h"
#include <typeinfo>
#include <chrono>

using std::string;

class Game{
private:
    Board plateau;
    Player* joueurX;
    Player* joueurO;
public:
    /**
     * @brief Initialise les joueurs pour la partie.
     */
    void initializePlayers();
    /**
     * @brief Démarre la partie.
     */
    void startGame();
    /**
     * @brief Affiche les résultats de la partie et libère la mémoire des joueurs.
     */
    void endGame();

    /**
     * @brief Lit le contenu d'un fichier et le retourne sous forme de chaîne de caractères.
     * @param cheminFichier Le chemin du fichier à lire.
     * @return Le contenu du fichier sous forme de chaîne de caractères.
     */
    string lireDuFichier(const string& cheminFichier);

    /**
     * @brief Écrit une chaîne de caractères dans un fichier.
     * @param cheminFichier Le chemin du fichier où écrire.
     * @param texte Le texte à écrire dans le fichier.
     */
    void ecrireDansFichier(const string& cheminFichier, const string& texte);
    
    /**
     * @brief Écrit le nom du joueur dans un fichier, attend la réponse de l'autre joueur, et       effectue des actions en conséquence.
     * @param fichierEcriture Le fichier où écrire le nom du joueur.
     * @param nomJoueur Le nom du joueur à écrire dans le fichier.
     * @param fichierLecture Le fichier où lire la réponse de l'autre joueur.
     * @param couleurJoueur La couleur du joueur (noir ou blanc).
     */
    void ecrireEtAttendreNomJoueur(const string& fichierEcriture, const string& nomJoueur, const string& fichierLecture, const string& couleurJoueur);
};
