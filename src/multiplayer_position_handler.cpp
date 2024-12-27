#include "multiplayer_position_handler.h"
#include "character.h"
#include "camera.h"

bool MultiplayerHandlerVisitor::isSpawning = false;

bool MultiplayerHandlerVisitor::GetIsSpawning(){
    return isSpawning;
}

void MultiplayerHandlerVisitor::SetIsSpawning(bool spawn){
    isSpawning = spawn;
}

MultiplayerHandlerVisitor::MultiplayerHandlerVisitor(MyCamera* camera) : camera(camera){
}

MultiplayerUpdatePosition::MultiplayerUpdatePosition(MyCamera* camera, b2Vec2 velocity): MultiplayerHandlerVisitor(camera), otherPlayerVelocity(velocity){
}

void MultiplayerUpdatePosition::VisitPlayer(Player* player){
    if (!player || !player->isAlive())
        return;
    
    if (player->getPosition().x < camera->GetLeftEdge() && player->getVelocity().x < 0){
        player->getBody()->SetLinearVelocity({otherPlayerVelocity.x, 0.0f});
    }

    if (!MultiplayerHandlerVisitor::GetIsSpawning() && player->getPosition().x + player->getSize().x < camera->GetLeftEdge()){
        player->setHealth(player->getHealth() - 1000);
        player->Dead();
        cout << "Dead cause too far" << endl;
    }
    else if (player->getPosition().x < camera->GetLeftEdge() && otherPlayerVelocity.x > 0.0f){
        player->getBody()->SetLinearVelocity({otherPlayerVelocity.x, 0.0f});
    }
}

MultiplayerUpdateSpawnPosition::MultiplayerUpdateSpawnPosition(Player* otherPlayer, MyCamera* camera): otherPlayer(otherPlayer), MultiplayerHandlerVisitor(camera){
}

void MultiplayerUpdateSpawnPosition::VisitPlayer(Player* player){
    if (!player)
        return;
    
    // 
    if (otherPlayer->isAlive()) {
        player->SetSpawnPosition({otherPlayer->getPosition().x, otherPlayer->getPosition().y - 2.0f});
    }
    else {
        cout << "Both player dead" << endl;
        MultiplayerHandlerVisitor::SetIsSpawning(true);
        player->SetSpawnPosition({player->getInitialPosition().y, player->getInitialPosition().y});
    }
}   