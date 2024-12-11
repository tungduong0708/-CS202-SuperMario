#include "tile.h"

Tile::Tile() : position(Vector2({0.0f, 0.0f})), id(0), type(""), tilesetPath("") {}

Tile::Tile(int id, std::string type, std::string tilesetPath) : id(id), position(Vector2{0.0f, 0.0f}), type(type), tilesetPath(tilesetPath) {}

Tile::Tile(int id, Vector2 position, std::string type, std::string tilesetPath) : id(id), position(position), type(type), tilesetPath(tilesetPath) {}

void Tile::setId(int id)
{
    this->id = id;
}

void Tile::setPosition(const Vector2 &newPos)
{
    position = newPos;
}

void Tile::setTilesetPath(const std::string &path)
{
    tilesetPath = path;
}

std::string Tile::getType()
{
    return type;
}

int Tile::getId() const
{
    return id;
}

std::string Tile::getTilesetPath() const
{
    return tilesetPath;
}

Vector2 Tile::getPosition() {
    b2Body* body = GetBody();
    if (body == nullptr) return position;
    b2Vec2 pos = GetBody()->GetPosition();
    return Vector2{pos.x, pos.y};
}

void Tile::Update(Vector2 playerVelocity, float deltaTime) 
{
}

void Tile::Draw() 
{
}

void Tile::print() const {
    std::cout << "Tile ID: " << id << " at Pos " << position.x << ", " << position.y << " in " << tilesetPath << "\n";
}

void Tile::OnBeginContact(SceneNode *other)
{
}

void Tile::OnEndContact(SceneNode *other)
{
}
