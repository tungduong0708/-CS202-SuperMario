// main.cpp
#include "game.h"
#include "character.cpp"

int main() {
    Game* game = Game::getInstance();
    game->Run();
    return 0;
}