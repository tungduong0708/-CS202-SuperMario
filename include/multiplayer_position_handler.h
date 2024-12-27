#pragma once

#include "include.h"

class Player;
class MyCamera;


class MultiplayerHandlerVisitor{
protected:
    MyCamera* camera;
public:
    MultiplayerHandlerVisitor(MyCamera* camera);
    virtual void VisitPlayer(Player* player) = 0;
};

class MultiplayerUpdatePosition : public MultiplayerHandlerVisitor{
private:
    b2Vec2 otherPlayerVelocity;
public:
    MultiplayerUpdatePosition(MyCamera* camera, b2Vec2 velocity);
    void VisitPlayer(Player* player) override;
};

class MultiplayerUpdateSpawnPosition : public MultiplayerHandlerVisitor{
private:
    Player* otherPlayer;
public:
    MultiplayerUpdateSpawnPosition(Player* otherPlayer, MyCamera* camera);
    void VisitPlayer(Player* player) override;
};