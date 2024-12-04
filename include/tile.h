#ifndef TILE_H
#define TILE_H

#include <vector>
#include <raylib.h>
#include <string>
#include "my_bounding_box.h"
#include "renderer.h"
#include "scene_node.h"

const int TILE_SIZE = 16;

class Tile : public SceneNode {
private:
    int id;
    std::string tilesetPath;
    Vector2 position;

public:
    Tile();
    Tile(int id, std::string tilesetPath);
    Tile(int id, Vector2 position, std::string tilesetPath);
    ~Tile() = default;

    virtual Tile* clone() const = 0;
    virtual void setPosition(const Vector2& newPos);

    virtual int getId() const;
    virtual std::string getTilesetPath() const;
    virtual Vector2 getPosition() const;

    virtual void Update(Vector2 playerVelocity, float deltaTime);
    virtual void Draw();
    void print() const;
};

#endif // TILE_H