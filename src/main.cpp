// main.cpp
#include "Game.h"

int main() {
    std::string file_path = "resources/data.json";
    
    // Create an instance of Game
    Game game(file_path);
    
    // Run the game
    game.Run();

    return 0;
}
