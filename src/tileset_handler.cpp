#include "tileset_handler.h"
#include "kinematic_tile.h"
#include "static_tile.h"
#include <iostream>
#include <map>
#include <vector>

std::map<std::string, Tileset*> TilesetHandler::tilesets;
std::map<std::string, Texture2D> TilesetHandler::tilesetTextures;

void TilesetHandler::Init()
{
    std::vector<std::string> tilesetPaths = { "resources/tilesets/SMW_tileset",
                                              "resources/tilesets/Sprite",
                                              "resources/tilesets/Castle",
                                              "resources/tilesets/castle1",
                                              "resources/tilesets/castle2",
                                              "resources/tilesets/OverWorld",
                                              "resources/tilesets/tiles-2",
                                              "resources/tilesets/bg-1-2",
                                              "resources/tilesets/bg-1-3",
                                              "resources/tilesets/bg-1-4",
                                              "resources/tilesets/bg-2-1",
                                              "resources/tilesets/bg-2-2",
                                              "resources/tilesets/bg-2-3",
                                              "resources/tilesets/bg-2-4",
                                              "resources/tilesets/items-objects"};
    for (const auto& tilesetPath : tilesetPaths) {
        addTileSet(tilesetPath + ".json", tilesetPath + ".png");
    }
}

void TilesetHandler::Clear()
{
    for (auto& [tilesetPath, tileset] : tilesets) delete tileset;
    tilesets.clear();
}

Tile *TilesetHandler::getTile(const std::string &tilesetPath, int id)
{
    return tilesets[tilesetPath]->getTile(id);
}

int TilesetHandler::getSpacing(const std::string &tilesetPath)
{
    return tilesets[tilesetPath]->getSpacing();
}

int TilesetHandler::getColumns(const std::string &tilesetPath)
{
    return tilesets[tilesetPath]->getColumns();
}

Texture2D TilesetHandler::getTexture(const std::string &tilesetPath)
{
    return tilesets[tilesetPath]->getTexture();
}

std::vector<b2Vec2>& TilesetHandler::getBoxVertices(const std::string &tilesetPath, int id)
{
    return tilesets[tilesetPath]->getBoxVertices(id);
}

void TilesetHandler::addTileSet(const std::string &tilesetPath, const std::string &tilesetImagePath)
{
    if (tilesets.find(tilesetPath) != tilesets.end()) {
        std::cerr << "addTileSet: Tileset " << tilesetPath << " already exists!" << std::endl;
        return;
    }
    std::cout << "Adding tileset: " << tilesetPath << " " << tilesetImagePath << std::endl;
    Tileset* tileset = new Tileset(tilesetPath, tilesetImagePath);
    tileset->print();
    tilesets[tilesetPath] = tileset;
    tilesetTextures[tilesetPath] = tileset->getTexture();
}

void TilesetHandler::printTilesetsStatus() {
    std::cout << "TilesetHandler status:" << std::endl;
    std::cout << "Total size: " << tilesets.size() << std::endl;
    for (const auto& [tilesetPath, tileset] : tilesets) {
        std::cout << "Tileset: " << tilesetPath << std::endl;
        tileset->print();
    }
}
