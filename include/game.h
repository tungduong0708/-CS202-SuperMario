#ifndef GAME_H
#define GAME_H

#include "include.h"
#include "moving_object.h"
#include "physics.h"
#include "contactlistener.h"
#include "imagehandler.h"

class Game {
public:
    static Game* getInstance() {
        static Game instance;
        return &instance;
    }
    void Run();

private:
    Game();
    void Init();
    void Update(float deltaTime);
    void Draw();
    void Cleanup();

    vector<MovingObject*> movingObjects;      
};

#endif // GAME_H
