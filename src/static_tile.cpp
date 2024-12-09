#include "static_tile.h"
#include "tileset_handler.h"
#include "renderer.h"
#include "scene_node.h"
#include "moving_object.h"
#include "my_bounding_box.h"

StaticTile::StaticTile(int id, std::string tilesetName) : Tile(id, tilesetName)
{
}

StaticTile::StaticTile(int id, Vector2 pos, std::string tilesetName)
    : Tile(id, pos, tilesetName)
{
}

StaticTile::StaticTile(StaticTile& other) : Tile(other)
{
    std::vector<b2Vec2> vertices = TilesetHandler::getBoxVertices(Tile::getTilesetPath(), getId());
    if (!vertices.empty()) {
        b2Body* body = GetBody();
        MyBoundingBox::createBody(body, b2_staticBody, vertices, getPosition());
        body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
        SetBody(body);
    }
}

Tile *StaticTile::clone()
{
    return new StaticTile(*this);
}

void StaticTile::setPosition(const Vector2& position)
{
    Tile::setPosition(position);
}

void StaticTile::Update(Vector2 playerVelocity, float deltaTime)
{
}

void StaticTile::Draw()
{
    std::string tilesetPath = Tile::getTilesetPath();
    int columns = TilesetHandler::getColumns(tilesetPath);
    int spacing = TilesetHandler::getSpacing(tilesetPath);
    int src_x = (getId() % columns) * (TILE_SIZE + spacing);
    int src_y = (getId() / columns) * (TILE_SIZE + spacing);
    
    Rectangle srcRect = { static_cast<float>(src_x), static_cast<float>(src_y), 
                        static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) };
    Renderer::DrawPro(TilesetHandler::getTexture(tilesetPath), srcRect, getPosition(), Vector2{ 1, 1 }, true);
    Physics::DebugDraw();
}

void StaticTile::OnBeginContact(SceneNode* other)
{
}

void StaticTile::OnEndContact(SceneNode* other)
{
}
