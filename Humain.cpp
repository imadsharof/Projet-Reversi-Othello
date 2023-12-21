#include "Humain.h"
#include <map>
#include <limits> // Pour std::numeric_limits
#include <iostream>

using std::cout;
using std::endl;
using std::cin;
using std::map;

using namespace std;

Humain::Humain(string nom, char pion):Player(nom,pion)
{
	name = nom;
    pawn_color = pion;
    pass = false;
    type = TypeJoueur::Humain;
} 

bool Humain::verify_answer(const string& answer, const vector<string>& legal_moves)
{    
    if (answer != "000" && answer != "00") {
        if (answer.size() > 2) { 
            cout << "La reponse est trop grande. 2 caracteres sont recquis" << endl;
            return false;
        }
        
        if (answer.size() != 2 && answer.size() != 0) { 
            cout << "La reponse est petite" << endl;
            return false;
        }
        
        if (answer.size() != 0 && (!std::isalpha(answer[0]) || !std::isdigit(answer[1]))) {
            cout << "La reponse doit contenir d'abord une lettre et puis un chiffre." << endl;
            return false;
        }

        for (const string& element : legal_moves) {
            if (element == answer) {
                cout << element << endl;
                return true;
            }
        }
        if (answer.size() != 0) {
        cout << "Ce mouvement n'est pas valide. Veuillez recommencez !" << endl;
        return false;

        } else {return true;}
    } else {
		if (legal_moves[0] != "00") {return false;}
	}

    return true;
}   


int Humain::play(const vector<int>& moves)
{   
    cout << "Au tour de " << name << " (" << pawn_color << ")" << endl;

    vector<string> legal = legal_moves(moves);
    string coord;
    bool moveValid = false;

    while (!moveValid) {
        // Demande le coup à jouer
        cout << "Quel coup voulez-vous jouer ? : ";
        cin >> coord;
        cout << coord << endl;
		
		// Nettoie le tampon d'entrée
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if( pawn_color == 'O'){
			if(coord != "0" && coord != "00" && coord != "000" && coord != "")
					cout << "Le joueur blanc a placé son pion en " << coord << endl;
			else
				cout << "Le joueur blanc a passé son tour" << endl;
			}
		else {
			if(coord != "0" && coord != "00" && coord != "000" && coord != "")
				cout << "Le joueur noir a placé son pion en " << coord << endl;
			else
				cout << "Le joueur noir a passé son tour" << endl;
		}
		
		// Demande à l'utilisateur d'appuyer sur "Entrée"
		cout << "Appuyez sur 'Entree' pour continuer...";
		cin.ignore(10000, '\n'); // Ignore les 1000 prochains caractères ou jusqu'à '\n'
		
        // Vérifie si la réponse est valide 
        if (!verify_answer(coord, legal)) {
            // Si la réponse n'est pas valide, continuez à demander une nouvelle entrée
            continue;
        } else {
            moveValid = true;
        }
    }

    // Gère le cas du "pass"
    if (coord == "00" || coord == "") {
		string couleur = (pawn_color == 'X') ? "noir" : "blanc";
        cout << "Le joueur " << couleur << " a passé son tour" << endl;
        pass = true;
        return 0;
    } else {
        pass = false;
        return this->traduction(coord);
    }
}



