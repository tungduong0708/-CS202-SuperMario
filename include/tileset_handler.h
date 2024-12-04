#ifndef TILESET_HANDLER_H
#define TILESET_HANDLER_H

#include "tileset.h" 
#include "tile.h"
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <box2d/box2d.h>

class Tileset;

class TilesetHandler {
private:
    static std::map<std::string, Tileset*> tilesets;
    static std::map<std::string, Texture2D> tilesetTextures;
public:
    static void Init();
    static void Clear();
    static Tile* getTile(const std::string& tilesetPath, int id);
    static int getSpacing(const std::string& tilesetPath);
    static int getColumns(const std::string& tilesetPath);
    static Texture2D getTexture(const std::string& tilesetPath);
    static std::vector<b2Vec2>& getBoxVertices(const std::string& tilesetPath, int id);

    static void addTileSet(const std::string& tilesetPath, const std::string& tilesetImagePath);
    static void printTilesetsStatus();
};

#endif // TILESET_HANDLER_H