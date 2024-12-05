#include "tile.h"

Tile::Tile() : position(Vector2({0.0f, 0.0f})), id(0), tilesetPath("") {}

Tile::Tile(int id, std::string tilesetPath) : id(id), position(Vector2{0.0f, 0.0f}), tilesetPath(tilesetPath) {}

Tile::Tile(int id, Vector2 position, std::string tilesetPath) : id(id), position(position), tilesetPath(tilesetPath) {}

void Tile::setPosition(const Vector2& newPos) {
    position = newPos;
}

int Tile::getId() const { 
    return id; 
}

std::string Tile::getTilesetPath() const
{
    return tilesetPath;
}

Vector2 Tile::getPosition() const {
    return position;
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
