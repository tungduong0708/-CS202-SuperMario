#include "game.h"

int main() {
    Game* game = Game::getInstance();
    game->Run();
    delete game;
    return 0;
}