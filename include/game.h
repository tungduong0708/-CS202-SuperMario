// Game.h
#ifndef GAME_H
#define GAME_H

#include "include.h"
#include "moving_object.h"
#include "contactlistener.h"
#include "image.h"

class Game {
public:
    // Singleton pattern
    static Game* getInstance() {
        static Game instance;
        return &instance;
    }
    void Run();

private:
    Game();
    void Init();
    void LoadData();
    void UpdatePhysics();
    void Draw();
    void Cleanup();

    json j;                  // JSON data
    b2World* world;          // Box2D world
    b2Body* groundBody;      // Ground body
    vector<MovingObject*> movingObjects; // Vector of moving objects    
    std::string filePath;    // Path to JSON file
    
};

#endif // GAME_H
