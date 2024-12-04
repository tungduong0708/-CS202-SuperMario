#ifndef STATICTILE_H
#define STATICTILE_H

#include "tile.h"
#include "my_bounding_box.h"
#include "renderer.h"
#include <box2d/box2d.h>
#include <vector>
#include <string>

class StaticTile : public Tile {
private:
    MyBoundingBox boundingBox;
public:
    StaticTile() = default;
    StaticTile(int id, std::string tilesetName);
    StaticTile(int id, Vector2 pos, std::string tilesetName);
    StaticTile(const StaticTile& other);
    Tile* clone() const;
    ~StaticTile() = default;

    void setPosition(const Vector2& position);
    void Update(Vector2 playerVelocity ,float deltaTime);
    void Draw();
};

#endif // STATICTILE_H