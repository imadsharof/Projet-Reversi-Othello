#include "Fechier.h"

using std::cout;
using std::endl;
using std::cin;
using std::cerr;


Fechier::Fechier(const string& nom, char pion, string file) : Player(nom, pion)
{
	nom_adv = nom;
	pawn_color = pion;
    type = TypeJoueur::Fichier;
	dossier = file;
 
    nom_fichier_noir = dossier + "noir.txt";
	nom_fichier_blanc = dossier + "blanc.txt";
	cout << nom_fichier_noir << " | " << nom_fichier_blanc << endl;

    if (!initNom()) {cerr << "Erreur initialisation" << endl;}
}

Fechier::~Fechier() 
{
    fichier_Write.close();
    fichier_Read.close();
}


bool Fechier::initNom()
{	
	if (pawn_color == 'O') {
		fichier_Write.open(nom_fichier_noir);
		fichier_Read.open(nom_fichier_blanc);
	} else if (pawn_color == 'X') {
		fichier_Write.open(nom_fichier_blanc);
		fichier_Read.open(nom_fichier_noir);
	} else {return false;}

    while (!(fichier_Read.is_open()))
    {
        cout << "Attente du joueur Fichier (fichier texte indisponible)" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        if (pawn_color == 'O') {
			fichier_Read.open(nom_fichier_blanc);
		} else {
			fichier_Read.open(nom_fichier_noir);
		}
    }

    fichier_Write << nom_adv << endl;

    while (!(getline(fichier_Read, name)))
    {
        fichier_Read.clear();
        fichier_Read.sync();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    
    cout << "Nom : " << name << endl;

    return true;
}

bool Fechier::advMove(int coord) 
{
    if (coord != 999) {
        string move = traduction(coord);
        fichier_Write << move << endl;
        return true; 
    } else {
		if (pawn_color == 'X') {
			cout << "Début de la partie entre " << name << " (noir) et " << nom_adv << " (blanc)" << endl;
		} else {
			cout << "Début de la partie entre " << nom_adv << " (noir) et " << name << " (blanc)" << endl;
		}
	}
    return false;
    
}

bool Fechier::verify_answer(const string& answer, const vector<string>& legal_moves)
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
        cout << "Ce mouvement n'est pas valide. Veuillez recommencez fichier !" << endl;
        return false;

        } else {return true;}
    } else {
		if (legal_moves[0] != "00") {return false;}
	}

    return true;
}   

int Fechier::play(const vector<int>& moves, const int& moveAdv)
{
	advMove(moveAdv);
	
	vector<string> legals = legal_moves(moves);
	string coord = "";
	while (true) {
		while (!(getline(fichier_Read, coord)))
		{
			cout << "Attente du joueur fichier" << endl;
			fichier_Read.clear();
			fichier_Read.sync();
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
		
		if (verify_answer(coord, legals)) { break;}
	}

	string couleur = (pawn_color == 'X') ? "noir" : "blanc";
    cout << "Le joueur " << couleur << " a placé son pion en " << coord << endl;
    cout << "Appuyez sur 'Entree' pour continuer fichier...";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (coord == "00" || coord == "0") {
        cout << "Le joueur " << couleur << " a passé son tour" << endl;
        pass = true;
        return 0;
    } else {
        pass = false;
        return this->traduction(coord);
    }

}

string Fechier::get_name_adv() const
{
	return name;
}
