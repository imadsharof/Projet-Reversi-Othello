#include "Game.h"

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib> // Pour la fonction system()
#include <cstdio> // pour la fonction remove ( permet de supprimer blanc.txt et noir.txt au début de chaque lancement de projet)

using std::cout;
using std::endl;
using std::cin;
using std::ofstream; // Pour écrire dans les fichiers
using std::ifstream; // Pour lire depuis les fichiers
using std::string;

using namespace std;


void Game::initializePlayers(){
	string file;
	int depthX = 5; // +1 pour profondeur réelle
	int depthO = 5; // +1 pour profondeur réelle


    cout << "Initialisation de la partie..." << endl;
    cout <<"Choisissez le type de joueur pour X ( H / A / F) : ";
    char typeX;
    string nomX;
    cin >> typeX;
    cout << "Joueur X de type : " << typeX << endl;
    
    if (typeX == 'H') {
		cout << "Entrez le nom du joueur X : ";
		cin >> nomX;
	} else if (typeX == 'F') {
		cout << "Veuillez entrer le chemin du dossier" << endl;
		cin >> file;
		cout << "Chemin dossier : " << file << endl;
	}
	

	
    cout <<"Choisissez le type de joueur pour O ( H / A / F) : ";
    char typeO;
    string nomO;
    cin >> typeO;
    cout << "Joueur O de type : " << typeX << endl;
    
    cout << "Types : " << typeO << " | " << typeX << endl;
    if (typeO == 'H') {
		cout << "Entrez le nom du joueur O : ";
		cin >> nomO;
	} else if (typeO == 'F') {
		cout << "Veuillez entrer le chemin du dossier" << endl;
		cin >> file;
		cout << "Chemin dossier : " << file << endl;
	}
	
	
    // Initialisation des joueurs
    switch(typeX){
        case 'H':
            joueurX= new Humain(nomX, 'X');
            break;
        case 'A':
			joueurX = new IA(nomX,'X', depthX);
            break;
        case 'F':
			joueurX= new Fechier(nomO, 'X', file);
            break;
        default:
            cout << "Type de joueur non reconnu. Utilisation par défaut d'un joueur humain." << endl;
            joueurX = new Player(nomX, 'X');
    }

    switch(typeO){
        case 'H':
            joueurO = new Humain(nomO, 'O');
            break;
        case 'A':
			joueurO = new IA(nomX, 'O', depthO);
            break;
        case 'F':
			joueurO = new Fechier(nomX, 'O', file);
            break;
        default:
            cout << "Type de joueur non reconnu. Utilisation par défaut d'un joueur humain." << endl;
            joueurO = new Player(nomO, 'O');
    }

}

void Game::startGame(){
    initializePlayers();
    int moveX = 999;
    int moveO = 999;
    
	if (plateau.get_nbPawn() == 4 ) {
		cout << "Début de la partie entre " << (joueurX)->name << " (noir) et " << (joueurO)->name << " (blanc)" <<endl;
		// Demande à l'utilisateur d'appuyer sur "Entrée"
		cout << "Appuyez sur 'Entree' pour continuer...";
		cin.ignore(10000, '\n'); // Ignore les 1000 prochains caractères ou jusqu'à '\n'
		cin.get();
		}
     while ( !plateau.isGameOver()  && !(joueurX->get_pass() && joueurO->get_pass())  )
    {
        plateau.display_board();
		 // Traitement pour joueurX
        switch (joueurX->type) {
            case TypeJoueur::Humain:
                moveX = dynamic_cast<Humain*>(joueurX)->play(plateau.get_legal_moves('X'));
                break;
            case TypeJoueur::IA:
                moveX = dynamic_cast<IA*>(joueurX)->play(plateau.get_board(), plateau.get_nbPawn());
                break;
            case TypeJoueur::Fichier:
                moveX = dynamic_cast<Fechier*>(joueurX)->play(plateau.get_legal_moves('X'), moveO);
                break;
        }

        plateau.add_pawn('X', moveX);
        plateau.display_board();
        
        if ( plateau.isGameOver() || (joueurX->get_pass() && joueurO->get_pass())) {
            break;
        }

        switch (joueurO->type) {
            case TypeJoueur::Humain:
                moveO = dynamic_cast<Humain*>(joueurO)->play(plateau.get_legal_moves('O'));
                break;
            case TypeJoueur::IA:
                moveO = dynamic_cast<IA*>(joueurO)->play(plateau.get_board(), plateau.get_nbPawn());
                break;
            case TypeJoueur::Fichier:
                moveO = dynamic_cast<Fechier*>(joueurO)->play(plateau.get_legal_moves('O'), moveX);
                break;
        }

        plateau.add_pawn('O', moveO);        
        if ( plateau.isGameOver() || (joueurX->get_pass() && joueurO->get_pass())) {
            plateau.display_board();
            break;
        }
    }

    endGame();
}

void Game::endGame(){
	
    cout << endl << "Fin de la partie" << endl;
    int scoreX = plateau.get_score_X();
    int scoreO = plateau.get_score_O();
    cout << "Noir" << ": " << scoreX << endl;
    cout << "Blanc" << ": " << scoreO << endl;

    delete joueurX;
    delete joueurO;
}
