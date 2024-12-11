#include "static_object.h"

StaticObject::StaticObject() : body(nullptr) {
}

StaticObject::StaticObject(b2Body *body) : body(body) {
}

StaticObject::~StaticObject()
{
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
