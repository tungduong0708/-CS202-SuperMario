#ifndef TILE_H
#define TILE_H

#include <vector>
#include <raylib.h>
#include <string>
#include "my_bounding_box.h"
#include "renderer.h"
#include "static_object.h"

const int TILE_SIZE = 16;

class Tile : public StaticObject {
private:
    int id;
    std::string tilesetPath;
    std::string type;
    Vector2 position;

public:
    Tile();
    Tile(int id, std::string type, std::string tilesetPath);
    Tile(int id, Vector2 position, std::string type, std::string tilesetPath);
    ~Tile() = default;

    virtual Tile* clone() = 0;
    virtual void setId(int id);
    virtual void setPosition(const Vector2& newPos);
    virtual void setTilesetPath(const std::string& path);

    virtual std::string getType();
    virtual int getId() const;
    virtual std::string getTilesetPath() const;
    Vector2 getPosition();

    virtual void Update(Vector2 playerVelocity, float deltaTime);
    virtual void Draw();
    void print() const;
    virtual void OnBeginContact(SceneNode* other, b2Vec2 normal);
    virtual void OnEndContact(SceneNode* other);
};

#endif // TILE_H