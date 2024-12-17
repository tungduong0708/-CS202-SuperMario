#include "game.h"

int main() {
    Game* game = Game::getInstance();
    game->run();
    
    return 0;
}