#ifndef STATICTILE_H
#define STATICTILE_H

#include "tile.h"
#include "my_bounding_box.h"
#include "renderer.h"
#include "scene_node.h"
#include "file_visitor.h"
#include <box2d/box2d.h>
#include <vector>
#include <string>
#include <set>

class StaticTile : public Tile {
private:
    b2Body* invisibleBody = nullptr;
    b2PrismaticJoint* joint = nullptr;
    bool isDestroyed = false;
    bool isActivated = false;
public:
    StaticTile() = default;
    StaticTile(int id, std::string type, std::string tilesetName);
    StaticTile(int id, Vector2 pos, std::string type, std::string tilesetName);
    StaticTile(StaticTile& other);
    void createBody();
    StaticObject* clone();
    virtual ~StaticTile();

    bool getIsDestroyed();
    bool getIsActivated();
    void setIsDestroyed(bool isDestroyed);
    void setIsActivated(bool isActivated);
    void setPosition(const Vector2& position);
    void Update(Vector2 playerVelocity ,float deltaTime);
    void Draw();
    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);
    void accept(FileVisitor* v);
};

#endif // STATICTILE_H