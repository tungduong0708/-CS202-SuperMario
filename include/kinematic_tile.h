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
    std::vector<std::pair<int, int>> frames;
    b2Body* invisibleBody = nullptr;
    b2PrismaticJoint* joint = nullptr;
    bool animation = true;
public:
    KinematicTile() = default;
    KinematicTile(int id, std::string type, std::string tilesetName, const std::vector<std::pair<int, int>>& frames);
    KinematicTile(int id, Vector2 pos, std::string type, std::string tilesetName, const std::vector<std::pair<int, int>>& frames);
    KinematicTile(KinematicTile& other);
    StaticObject* clone();
    virtual ~KinematicTile();

    std::vector<std::pair<int, int>> getFrames();
    void setPosition(const Vector2& position);
    void Update(Vector2 playerVelocity, float deltaTime);
    void Draw();
    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);
    void accept(FileVisitor* v);
};

#endif // KINEMATIC_TILE_H