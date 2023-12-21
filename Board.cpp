#include "Board.h"

using std::cout;
using std::endl;

// Initialise le plateau avec les pions de départ pour chaque joueur
Board::Board() 
{
    //La dizaine représente le numéro de la ligne 
    //et l'unité  représente le numéro de la colonne
    vector<int> squares = {
        11, 12, 13, 14, 15, 16, 17, 18,
        21, 22, 23, 24, 25, 26, 27, 28,
        31, 32, 33, 34, 35, 36, 37, 38,
        41, 42, 43, 44, 45, 46, 47, 48,
        51, 52, 53, 54, 55, 56, 57, 58,
        61, 62, 63, 64, 65, 66, 67, 68,
        71, 72, 73, 74, 75, 76, 77, 78,
        81, 82, 83, 84, 85, 86, 87, 88
    };

    //Pour les valeurs, '.' signifie une absence de pion
    //                  'X' vaut un pion du joueur 1
    //                  'O' vaut un pion du joueur 2
    for (int key : squares) {
        if (key == 45 || key == 54) {
            dico_board[key] = 'X'; //Pions du joueur 1 de départ
        }
        else if (key == 44 || key == 55) {
            dico_board[key] = 'O'; //Pions du joueur 2 de départ
        }
        else {dico_board[key] = '.';} //Cases vides
    }

    nbPawn = 4;
}

Board::Board(const map<int,char>& plateau)
{
    dico_board = plateau;
    evaluate_score();
}

// Retourne le score (nombre de pions) du joueur 'X'
int Board::get_score_X() const
{
    return num_playerX;
}

// Retourne le score (nombre de pions) du joueur 'O'
int Board::get_score_O() const
{
    return num_playerO;
}


map<int,char> Board::get_board() const 
{
    return dico_board;
}

int Board::get_nbPawn()
{
    return nbPawn;
}

// Evalue et met à jour les scores des deux joueurs en fonction du plateau actuel
void Board::evaluate_score() 
{   
    num_playerO = 0;
    num_playerX = 0;
    for (const auto & pair : dico_board) {
        char value = pair.second;
        if (value == 'X') {num_playerX++;}
        else if (value == 'O') {num_playerO++;}
    }

    nbPawn = num_playerO + num_playerX;
}

// Retourne un ensemble de mouvements légaux possibles
vector<int> Board::get_legal_moves(const char& player)
{   
    vector<int> legal_moves;
    char adv = (player == 'X') ? 'O' : 'X';

	bool found = false;
    int scout;
    for (const auto& pair : dico_board) {
        if (pair.second == '.') {
            for (const int& direction : {-1, -11, -10, -9, 1, 11, 10, 9}) {
                scout = pair.first + direction;

                while ((10 < scout) && (scout < 89) && dico_board.find(scout)->second == adv) {
                    scout = scout + direction;

                    if ((10 < scout) && (scout < 89) && dico_board.find(scout)->second == player) {
                        legal_moves.emplace_back(pair.first);
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
    return legal_moves;
}

// Tente d'ajouter un pion du joueur à une coordonnée donnée
//Met à jour le plateau et retourne si l'ajout est réussi
bool Board::add_pawn(const char& player, const int& coord) 
{   
    if ((player == 'X' || player == 'O') && (10 < coord) && (coord < 89) && dico_board[coord] == '.') {
        
        char adv;
        if (player == 'X') {adv = 'O';} 
        else {adv = 'X';}

        int scout;
        vector<int> will_take;
        for (const int& direction : {-1, -11, -10, -9, 1, 11, 10, 9}) { // On regarde dans les 8 directions
            scout = coord + direction;
            // Si le poin est un poin advairse on continue à regarder dans la direction
            while ((10 < scout) && (scout < 89) && dico_board[scout] == adv) { 
                will_take.emplace_back(scout);
                scout = scout + direction;
                // Si au bout de la direction il s'agit d'un de nos pions, le joueur prend tout les pions sur lequel il est passé
                if ((10 < scout) && (scout < 89) && dico_board[scout] == player) { 
                        will_take.emplace_back(coord);
                        for (const int& take : will_take) {
                            dico_board[take] = player;
                        }
                    }
            }
            will_take.clear();
        }
        nbPawn++;
        evaluate_score(); // Met à jour les scores
        return true;
    }
    // Gestion des erreurs; si le pion n'a pas été ajouté avec succès
    return false; 
}

// Fonction permettant de print le jeu sous forme d'un quadrillage
void Board::display_board() 
{
    cout << endl << "   | a | b | c | d | e | f | g | h |" << endl;
    cout << "---+---+---+---+---+---+---+---+---+--" << endl;
    for (int row = 1; row < 9; row++) {
        cout << " " << row << " | ";

        for (int col = 1; col < 9; col++) {
            int key = row * 10 + col; //Calcule la clé en fonction de la position
            char value = dico_board[key];
            
            cout << value << " | ";
        }
        cout << row << endl;
        cout << "---+---+---+---+---+---+---+---+---+--" << endl;
    }
    cout << "   | a | b | c | d | e | f | g | h |" << endl << endl;
}


void Board::set_dicoBoard(const map<int,char>& plateau)
{
    dico_board = plateau;
}

bool Board::isGameOver() {
    return get_nbPawn() == 64 || (get_legal_moves('X').empty() && get_legal_moves('O').empty());
}

