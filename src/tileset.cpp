#include "tileset.h"
#include "kinematic_tile.h"
#include "static_tile.h"
#include "my_bounding_box.h"
#include "physics.h"
#include <box2d/box2d.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <ranges>

using json = nlohmann::json;

Tileset::Tileset(const std::string& tilesetPath, const std::string& tilesetImagePath) {
    load(tilesetPath, tilesetImagePath);
}

void Tileset::load(const std::string& tilesetPath, const std::string& tilesetImagePath) {
    std::cout << "loading tileset " << tilesetPath << std::endl;
    tilesetTexture = LoadTexture(tilesetImagePath.c_str());

    std::ifstream file(tilesetPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << tilesetPath << std::endl;
        return;
    }

    json data;
    file >> data;

    name = data["name"].get<std::string>();
    spacing = data["spacing"].get<int>();
    tileWidth = data["tilewidth"].get<int>(); 
    tileCount = data["tilecount"].get<int>();
    tileColumns = data["columns"].get<int>();
    tiles.resize(tileCount);
    boxVertices.resize(tileCount);

    if (data.contains("tiles")) {
        for (const auto& tileData : data["tiles"]) {
            int tileId = tileData["id"].get<int>();

            if (tileData.contains("animation")) {
                std::vector<std::pair<int, int>> frames;
                for (const auto& frame : tileData["animation"]) {
                    int frameId = frame["tileid"].get<int>();
                    int duration = frame["duration"].get<int>();
                    frames.push_back({frameId, duration});
                }
                tiles[tileId] = new KinematicTile(tileId, tilesetPath, frames);
            }

            if (tileData.contains("objectgroup")) {
                std::vector<b2Vec2> vertices;
                for (const auto& objData : tileData["objectgroup"]["objects"]) {
                    float originX = objData["x"].get<float>();
                    float originY = objData["y"].get<float>();
                    if (objData.contains("polygon")) {
                        for (const auto& vertex : objData["polygon"]) {
                            float x = vertex["x"].get<float>() + originX;  
                            float y = vertex["y"].get<float>() + originY;
                            vertices.emplace_back(x / tileWidth, y / tileWidth);
                        } 
                    }
                    else {
                        float width = objData["width"].get<float>();
                        float height = objData["height"].get<float>();

                        vertices.emplace_back(originX / tileWidth, originY / tileWidth);
                        vertices.emplace_back((originX + width) / tileWidth, originY / tileWidth);
                        vertices.emplace_back((originX + width) / tileWidth, (originY + height) / tileWidth);
                        vertices.emplace_back(originX / tileWidth, (originY + height) / tileWidth);
                    }
                    boxVertices[tileId] = vertices;
                }
            }
        }
    }

    for (int i = 0; i < tileCount; i++) {
        if (tiles[i] == nullptr) {
            tiles[i] = new StaticTile(i, tilesetPath);
        }
    }
}

void Tileset::print() const {
    std::cout << "Tileset: " << name 
              << ", Tile Width: " << tileWidth 
              << ", Total Tiles: " << tileCount << "\n";
}
std::vector<b2Vec2> &Tileset::getBoxVertices(int id) {
    if (id < 0 || id >= boxVertices.size()) {
        throw std::out_of_range("Invalid id for box vertices");
    }
    return boxVertices[id];
}

Tile *Tileset::getTile(int id)
{
    if (id < 0 || id >= tiles.size()) {
        std::cerr << "Tileset::Invalid tile ID: " << id << " in " << name << std::endl;
        return nullptr;
    }
    return tiles[id];
}

Texture2D Tileset::getTexture() const {
    return tilesetTexture;
}

int Tileset::getSpacing() const
{
    return spacing;
}

int Tileset::getColumns() const
{
    return tileColumns;
}

Tile *Tileset::operator[](int id) const
{
    if (id < 0 || id >= tiles.size()) {
        // throw std::out_of_range("Invalid tile ID");
        std::cerr << "Invalid tile ID: " << id << std::endl;
        return nullptr;
    }
    return tiles[id];
}
