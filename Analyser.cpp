#include "Analyser.h"

using std::cout;
using std::endl;

Analyser::Analyser(void) 
{
    sco_case = EARLY_CASE;
    sco_flexi = EARLY_FLEXI;
    sco_nbr = EARLY_NBR;

    tableau_scoreCase = {
        {11, 10}, {12, -3}, {13, 4}, {14, 3}, {15, 3}, {16, 4}, {17, -3}, {18, 10},
        {21, -3}, {22, -5}, {23, -2}, {24, -2}, {25, -2}, {26, -2}, {27, -5}, {28, -3},
        {31, 4}, {32, -2}, {33, 2}, {34, 0}, {35, 0}, {36, 2}, {37, -2}, {38, 4},
        {41, 3}, {42, -2}, {43, 0}, {44, 2}, {45, 2}, {46, 0}, {47, -2}, {48, 3},
        {51, 3}, {52, -2}, {53, 0}, {54, 2}, {55, 2}, {56, 0}, {57, -2}, {58, 3},
        {61, 4}, {62, -2}, {63, 2}, {64, 0}, {65, 0}, {66, 2}, {67, -2}, {68, 4},
        {71, -3}, {72, -5}, {73, -2}, {74, -2}, {75, -2}, {76, -2}, {77, -5}, {78, -3},
        {81, 10}, {82, -3}, {83, 4}, {84, 3}, {85, 3}, {86, 4}, {87, -3}, {88, 10}
    };
}


float Analyser::Analyse(const map<int,char>& plateau, const char& joueur)
{   
    float pion = 0.0f;
    float pion_adv = 0.0f;
    float carre = 0.0f;
    float carre_adv = 0.0f;
    float legal_moves = 0.0f;
    float legal_moves_adv = 0.0f;

    char adv = (joueur == 'X') ? 'O' : 'X';

    const int lower_limit = 10;
    const int upper_limit = 89;
	bool found = false;
    for (const auto& pair : plateau) {
        int square = pair.first;
        char current_piece = pair.second;

        // Traitement des pions du joueur
        if (current_piece == joueur) {
            pion += 1.0;
            carre += tableau_scoreCase[square];
        }
        // Traitement des pions de l'adversaire
        else if (current_piece == adv) {
            pion_adv += 1.0;
            carre_adv += tableau_scoreCase[square];
		
		} else {
            // Vérification des mouvements légaux
            for (const int& direction : {-1, -11, -10, -9, 1, 11, 10, 9}) {
                int scout = square + direction;
                while (scout > lower_limit && scout < upper_limit && plateau.find(scout)->second == joueur) {
                    scout += direction;
                    if (scout > lower_limit && scout < upper_limit && plateau.find(scout)->second == adv) {
                        legal_moves_adv += 1;
                        found = true;
                        break;
                    }
                }
                
                while (scout > lower_limit && scout < upper_limit && plateau.find(scout)->second == adv) {
                    scout += direction;
                    if (scout > lower_limit && scout < upper_limit && plateau.find(scout)->second == joueur) {
                        legal_moves += 1;
                        found = true;
                        break;
                    }
                }
                
                if (found) {
					found = false;
					break;
				}
            }            
        }
    }

    // Calcul du score
    float score = (pion / (pion + pion_adv))  * sco_nbr;
    if ((legal_moves + legal_moves_adv) != 0) {
        score += (legal_moves / (legal_moves + legal_moves_adv)) * sco_flexi;
    }
    if ((carre + carre_adv) != 0) {
		score += (carre / (carre + carre_adv)) * sco_case;
	}
	
	if (score == 0) {score = 100.0;} 
		
    return score;
}

void Analyser::set_sco_EARLYGAME()
{
    sco_case = EARLY_CASE;
    sco_flexi = EARLY_FLEXI;
    sco_nbr = EARLY_NBR;
}

void Analyser::set_sco_MIDDLEGAME()
{
    sco_case = MIDDLE_CASE;
    sco_flexi = MIDDLE_FLEXI;
    sco_nbr = MIDDLE_NBR;
}

void Analyser::set_sco_ENDGAME() 
{
    sco_case = END_CASE;
    sco_flexi = END_FLEXI;
    sco_nbr = END_NBR;
}
