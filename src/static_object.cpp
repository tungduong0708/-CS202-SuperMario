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

Gate::Gate(b2Body* body, std::string addressNext) : StaticObject(body), addressNext(addressNext) {
}

std::string Gate::getAddressNext() const
{
    return addressNext;
}

void Gate::Update(Vector2 playerVelocity, float deltaTime)
{
}

void Gate::Draw()
{
}

void Gate::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
    Player* player = dynamic_cast<Player*>(other);
    if (player != nullptr) {
        Game::getInstance()->changeState(Game::getInstance()->changeStageState.get());
        Tilemap::getInstance()->SetNewMapPath(addressNext);
    }
}

void Gate::OnEndContact(SceneNode *other)
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
