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
public:
    MultiplayerUpdatePosition(MyCamera* camera);
    void VisitPlayer(Player* player) override;
};

class MultiplayerUpdateSpawnPosition : public MultiplayerHandlerVisitor{
public:
    MultiplayerUpdateSpawnPosition(MyCamera* camera);
    void VisitPlayer(Player* player) override;
};