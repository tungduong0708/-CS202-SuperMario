#include "static_object.h"
#include "character.h"
#include "string.h"
#include "tilemap.h"
#include "game.h"

StaticObject::StaticObject() : body(nullptr) {
}

StaticObject::StaticObject(b2Body *body) : body(body) {
}

StaticObject::StaticObject(std::string type) : body(nullptr), type(type) {
}

StaticObject::StaticObject(const StaticObject &other) : body(nullptr), type(other.type)
{
}

StaticObject::~StaticObject()
{
    if (body)
    {
        Physics::world.DestroyBody(body);
        body = nullptr;
    }
}

StaticObject *StaticObject::clone()
{
    return new StaticObject(*this);
}

std::string StaticObject::getType()
{
    return type;
}

b2Body *StaticObject::GetBody()
{
    return body;
}

Vector2 StaticObject::getPosition()
{
    b2Vec2 pos = body->GetPosition();
    return Vector2{pos.x, pos.y};
}

void StaticObject::SetBody(b2Body *body)
{
    this->body = body;
}

void StaticObject::SetType(std::string type)
{
    this->type = type;
}

void StaticObject::Update(Vector2 playerVelocity, float deltaTime)
{
}

void StaticObject::Draw()
{
}

void StaticObject::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
}

void StaticObject::OnEndContact(SceneNode *other)
{
}

void StaticObject::accept(FileVisitor *visitor)
{
}

Gate::Gate(b2Body* body, std::string addressNext) : StaticObject(body), addressNext(addressNext) {
    elapsedTime = 0.0f;
    delay = 6.5f;
    start = false;
}

std::string Gate::getAddressNext() const
{
    return addressNext;
}

void Gate::Update(Vector2 playerVelocity, float deltaTime)
{
    if (!start) return;
    elapsedTime += deltaTime;
    Player* player = Tilemap::getInstance()->GetPlayer();
    if (elapsedTime > delay) {
        elapsedTime = 0.0f;
        Game::getInstance()->changeState(Game::getInstance()->changeStageState.get());
        Tilemap::getInstance()->SetNewMapPath(addressNext);

        player->setAllowInput(true);
        Game* game = Game::getInstance();
        game->getSettings().volume = prevVolume;
        player->setSpeed(prevSpeed);
    }
    else {
        float ratio = player->getTime() / (delay) * deltaTime;
        if (player->getTime() > 0 and start) {
            player->setTime(player->getTime() - ratio*5);
            player->setScore(player->getScore() + ratio*100);
        }
    }
}

void Gate::Draw()
{
}

void Gate::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
    Player* player = dynamic_cast<Player*>(other);
    if (player != nullptr) {
        Tilemap* tilemap = Tilemap::getInstance();
        if (addressNext[7] == '-' && tilemap->GetCurrentMapPath()[7] == '-') {
            tilemap->SetNewMapPath(addressNext);
        } else {
            start = true;
            elapsedTime = 0.0f;

            playSoundEffect(SoundEffect::STAGE_CLEAR);
            player->setAllowInput(false);
            prevSpeed = player->getSpeed();
            player->setSpeed(0.0f);

            Game* game = Game::getInstance();
            prevVolume = game->getSettings().volume;
            game->getSettings().volume = 0.0f;
        }
    }
}

void Gate::OnEndContact(SceneNode *other)
{
}

void Gate::accept(FileVisitor *visitor)
{
}

DeadLine::DeadLine(b2Body *body) : StaticObject(body)
{
}

void DeadLine::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
    Player *player = dynamic_cast<Player *>(other);
    if (player != nullptr)
    {
        player->setHealth(player->getHealth() - 1000);
    }
}

void DeadLine::OnEndContact(SceneNode *other)
{
}

void DeadLine::accept(FileVisitor *visitor)
{
}
