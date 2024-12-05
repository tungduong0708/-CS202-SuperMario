#ifndef KINEMATIC_TILE_H
#define KINEMATIC_TILE_H

#include "tile.h"
#include "my_bounding_box.h"
#include "renderer.h"
#include "scene_node.h"
#include <box2d/box2d.h>
#include <vector>
#include <string>
#include <utility>

class KinematicTile : public Tile {
private:
    int currentFrameId = 0;
    float elapsedTime = 0.0f;
    MyBoundingBox boundingBox;
    std::vector<std::pair<int, int>> frames;
public:
    KinematicTile() = default;
    KinematicTile(int id, std::string tilesetName, const std::vector<std::pair<int, int>>& frames);
    KinematicTile(int id, Vector2 pos, std::string tilesetName, const std::vector<std::pair<int, int>>& frames);
    KinematicTile(const KinematicTile& other);
    Tile* clone() const;
    ~KinematicTile() = default;

    void setPosition(const Vector2& position);
    void Update(Vector2 playerVelocity, float deltaTime);
    void Draw();
    void OnBeginContact(SceneNode* other);
    void OnEndContact(SceneNode* other);
};

#endif // KINEMATIC_TILE_H