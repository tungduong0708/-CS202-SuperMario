#pragma once

#include "include.h"

enum PlayerInput {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    SHOOT
};

class PlayerInputSet {
private:
    static vector<int> player1Input;
    static vector<int> player2Input;

    PlayerInputSet() = default;
public:
    static vector<int> GetPlayer1Input();
    static vector<int> GetPlayer2Input();
};