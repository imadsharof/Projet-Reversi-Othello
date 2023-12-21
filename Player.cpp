#include "Player.h"
#include <map>

using std::cout;
using std::endl;
using std::cin;
using std::map;

Player::Player(string nom, char pion) 
{
	name = nom;
    pawn_color = pion;
    pass = false;
} 


Player::~Player() {

}

string Player::traduction(int coord) 
{
    if (coord == 0) {return "00";}

    string translated;
    map<int,char> translator = {{1, 'a'}, {2, 'b'}, {3, 'c'}, 
    {4, 'd'}, {5, 'e'}, {6, 'f'}, {7, 'g'}, {8, 'h'}};

    translated += translator[coord%10];
    translated += '0' + (coord / 10);
	
    return translated;
}


int Player::traduction(string coord) 
{   
    if (coord == "00" || coord == "0" || coord == "") {return 0;}

    map<int,char> translator = {{'a', 1}, {'b', 2}, {'c', 3}, 
    {'d', 4}, {'e', 5}, {'f', 6}, {'g', 7}, {'h', 8}};

    int translated = translator[coord[0]] + (coord[1] - '0') * 10;
    return translated;
}

vector<string> Player::legal_moves(const vector<int>& legal)
{   
    vector<string> legal_translated;
    cout << "Action possible : ";

    string translated;

    if (legal.empty()) {
        legal_translated.push_back("00");
        cout << "00 (Pass)" << endl;
        return legal_translated;
    }

    for (int element : legal) {
        translated = traduction(element);
        cout << translated << " ";
        legal_translated.push_back(translated);
    }
    cout << endl;

    return legal_translated;
}

bool Player::get_pass() const
{
    return pass;
}

string Player::get_name() const
{
    return name;
}

