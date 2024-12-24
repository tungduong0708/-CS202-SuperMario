#include "multiplayer_player_handler.h"
#include "character.h"
#include "camera.h"

MultiplayerHandlerVisitor::MultiplayerHandlerVisitor(MyCamera* camera) : camera(camera){
}

MultiplayerUpdatePosition::MultiplayerUpdatePosition(MyCamera* camera): MultiplayerHandlerVisitor(camera){
}

void MultiplayerUpdatePosition::VisitPlayer(Player* player){
    if (!player)
        return;
    if (player->getPosition().x < camera->GetLeftEdge()){
        player->setPositionBody(b2Vec2{camera->GetLeftEdge() + 0.2f, player->getPosition().y});
    }
}

MultiplayerUpdateSpawnPosition::MultiplayerUpdateSpawnPosition(MyCamera* camera): MultiplayerHandlerVisitor(camera){
}

void MultiplayerUpdateSpawnPosition::VisitPlayer(Player* player){
    if (!player)
        return;
    player->SetSpawnPosition({camera->GetLeftEdge() + 3.0f, player->getInitialPosition().y});
}