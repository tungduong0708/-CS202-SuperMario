#include "kinematic_tile.h"
#include "tileset_handler.h"
#include "renderer.h"

KinematicTile::KinematicTile(int id, std::string tilesetName, const std::vector<std::pair<int, int>> &frames)
    : Tile(id, tilesetName), frames(frames)
{
}

KinematicTile::KinematicTile(int id, Vector2 pos, std::string tilesetName, const std::vector<std::pair<int, int>> &frames)
    : Tile(id, pos, tilesetName), frames(frames)
{
}

KinematicTile::KinematicTile(const KinematicTile& other) 
    : Tile(other), 
      currentFrameId(other.currentFrameId),
      elapsedTime(other.elapsedTime),
      frames(other.frames) 
{
    std::vector<b2Vec2> vertices = TilesetHandler::getBoxVertices(Tile::getTilesetPath(), getId());
    if (!vertices.empty())
        boundingBox = MyBoundingBox(vertices, Tile::getPosition());
}

Tile *KinematicTile::clone() const
{
    return new KinematicTile(*this);
}

void KinematicTile::setPosition(const Vector2& position)
{
    Tile::setPosition(position);
}

void KinematicTile::Update(Vector2 playerVelocity, float deltaTime)
{
    if (frames.empty()) return;

    elapsedTime += deltaTime;
    std::pair<int, int> currentFrame = frames[currentFrameId];

    if (elapsedTime >= currentFrame.second / 1000.0f) { // Duration in milliseconds
        elapsedTime = 0.0f;
        currentFrameId = (currentFrameId + 1) % frames.size(); // Loop back to start
    }
}

void KinematicTile::Draw()
{
    std::string tilesetPath = Tile::getTilesetPath();
    int columns = TilesetHandler::getColumns(tilesetPath);
    int spacing = TilesetHandler::getSpacing(tilesetPath);
    int id = frames[currentFrameId].first;

    boundingBox.updateFixture(TilesetHandler::getBoxVertices(tilesetPath, id));
    int src_x = (id % columns) * (TILE_SIZE + spacing);
    int src_y = (id / columns) * (TILE_SIZE + spacing);
    
    Rectangle srcRect = { static_cast<float>(src_x), static_cast<float>(src_y), 
                        static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) };

    Renderer::DrawPro(TilesetHandler::getTexture(tilesetPath), srcRect, getPosition(), Vector2{ 1, 1 }, true);
}
