#include "tile.h"
#include "static_object.h"

Tile::Tile() : position(Vector2({0.0f, 0.0f})), id(0), tilesetPath("") {}

Tile::Tile(int id, std::string type, std::string tilesetPath) : StaticObject(type), id(id), position(Vector2{0.0f, 0.0f}), tilesetPath(tilesetPath) {}

Tile::Tile(int id, Vector2 position, std::string type, std::string tilesetPath) : StaticObject(type), id(id), position(position), tilesetPath(tilesetPath) {}

Tile::Tile(const Tile &other) : StaticObject(other)
{
    id = other.id;
    position = other.position;
    tilesetPath = other.tilesetPath;
}

StaticObject *Tile::clone()
{
    return new Tile(*this);
}

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

void Tile::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
}

void Tile::OnEndContact(SceneNode *other)
{
}

void Tile::accept(FileVisitor *v)
{
    v->VisitFile(this);
}
