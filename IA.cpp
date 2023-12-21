#include "IA.h"
#include <limits>
#include <random>

using std::numeric_limits;
using std::endl;
using std::cout;
using std::cin;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> distribution(1, 100);

IA::IA(string nom, char pion, int niveau) : Player(nom, pion)
{
    name = nom;
    pawn_color = pion;
    level = niveau;
    pass = false;
    playerAdv = (pion == 'X') ? 'O' : 'X';
    type = TypeJoueur::IA;
}


float IA::arbre(const map<int,char>& dico_board, const int& niveau, const bool& ai, const float& alpha, const float& beta) 
{   
    const char& joueur = (ai) ? pawn_color : playerAdv;
    float score;
    if (niveau == 0) { //Calcul du score pour les derniers niveaux uniquement (Les Feuilles) (si level == 0 : Racines = Feuilles)
        score = Anal.Analyse(plateau.get_board(), joueur);

    } else { //Niveau intermédiaire (=! Racines =! Feuilles)
        const vector<int>& legal_moves = plateau.get_legal_moves(joueur);

        if (legal_moves.empty()) { //Le joueur ne sait pas jouer => On renvoit le plateau à l'advairsaire
            return arbre(dico_board, niveau-1, !ai, alpha, beta);
        } else {
            if (ai) { //Si le joueur est l'IA, on garde le score le plus haut parmis les scores des branches suivantes
                //score = numeric_limits<float>::lowest(); //Initialisation du score du noeud à la valeur numérique la plus basse
                score = -99;
                for (const int& move : legal_moves) {
                    plateau.add_pawn(joueur, move);
					score = std::max(score, arbre(plateau.get_board(), niveau-1, false, score, beta)); //On prend le maximum
					
					if (beta <= score) {break;}
					
                }
            } else { //Si le joueur est l'advaisraire, on garde le score le plus bas parmis les scores des branches suivantes
                score = numeric_limits<float>::max(); //Initialisation du score du noeud à la valeur numérique la plus haute
                score = 99;
                for (const int& move : legal_moves) {
                    plateau.add_pawn(joueur, move);
					score = std::min(score, arbre(plateau.get_board(), niveau-1, true, alpha, score)); //On prend le minimum
					
					if (score <= alpha) {break;}
                }
            }
        }
    }
    plateau.set_dicoBoard(dico_board);
    return score; //Retour du score minimum ou maximum pour les branches intermédiaires
}


int IA::play(const map<int,char>& dico_board, const int& pawn)
{   
	if (pawn > 40) {
		Anal.set_sco_ENDGAME();
	} else if (pawn > 20) {
		Anal.set_sco_MIDDLEGAME();
	}
	
    plateau.set_dicoBoard(dico_board);
    const vector<int>& legal_moves = plateau.get_legal_moves(pawn_color);
    int nbRoot = legal_moves.size();
	int RootDone = 0;
	
    int best_move = 0;
    float max = numeric_limits<float>::lowest();
    for (const int& move : legal_moves) { // On parcour les premières branches (Les Racines)
        plateau.add_pawn(pawn_color, move);
        float score = arbre(dico_board, level, false, max, numeric_limits<float>::max());
        if (max < score) {
			max = score;
            best_move = move;
        }
        RootDone++;
        cout << "Calcul IA ... " << best_move << " : " << max << " (" << RootDone << "/" << nbRoot << ")" << endl;
        
    }
    
	string coord = traduction(best_move);
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
	
    if (best_move == 0) {pass = true;}
    else {pass = false;}

    return best_move;
}

int IA::get_level() const 
{
    return level;
}
