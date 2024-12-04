#ifndef TILESET_H
#define TILESET_H

#include "tileset_handler.h"
#include "tile.h"
#include <string>
#include <vector>
#include <map>
#include <raylib.h>
#include <memory> 

class Tileset {
private:
    std::string name;
    int spacing;
    int tileWidth;
    int tileCount;
    int tileColumns;
    std::vector<Tile*> tiles;
    std::vector<std::vector<b2Vec2>> boxVertices;
    Texture2D tilesetTexture;
public:
    Tileset() = default;
    Tileset(const std::string& tilesetPath, const std::string& tilesetImagePath); 

    void load(const std::string& tilesetPath, const std::string& tilesetImagePath);

    void print() const;
    std::vector<b2Vec2>& getBoxVertices(int id);
    Tile* getTile(int id);
    Texture2D getTexture() const;
    int getSpacing() const;
    int getColumns() const;

    Tile* operator[](int id) const;
};

#endif  // TILESET_H
