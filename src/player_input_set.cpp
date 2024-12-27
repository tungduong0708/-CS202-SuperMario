#include "player_input_set.h"

vector<int> PlayerInputSet::player1Input = {KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_RIGHT_SHIFT, KEY_SPACE};
vector<int> PlayerInputSet::player2Input = {KEY_A, KEY_D, KEY_W, KEY_S, KEY_TWO, KEY_LEFT_SHIFT};


vector<int> PlayerInputSet::GetPlayer1Input()
{
    return player1Input;
}

vector<int> PlayerInputSet::GetPlayer2Input()
{
    return player2Input;
}