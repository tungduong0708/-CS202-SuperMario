#include "tilemap.h"
#include "tileset.h"
#include "tile.h"
#include "static_tile.h"
#include "kinematic_tile.h"
#include "scene_node.h"
#include "background.h"
#include "static_object.h"
#include "physics.h"
#include <fstream>
#include <iostream>
#include <box2d/box2d.h>
#include "nlohmann/json.hpp" 
using json = nlohmann::json;

Tilemap::Tilemap(const std::string& filePath) {
    LoadMapFromJson(filePath);
}

Tilemap::~Tilemap() {
    for (auto& node : nodes) {
        delete node;
    }
}

std::pair<std::string, int> Tilemap::GetTilesetInfo(int tileIdx) const
{
    int l = 0;
    int r = tilesets.size() - 1;
    while (l <= r) {
        int mid = (l + r)/2;
        if (tilesets[mid].second <= tileIdx) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    return tilesets[r];
}

void Tilemap::LoadMapFromJson(const std::string &filePath)
{
    std::cout << "Loading map " << filePath << std::endl;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open map file!" << std::endl;
        return;
    }

    json j;
    file >> j;
    
    width = j["width"];
    height = j["height"];
    tileSize = j["tilewidth"];

    for (const auto& tileset : j["tilesets"]) {
        std::string tilesetPath = tileset["source"].get<std::string>();
        size_t pos = tilesetPath.find_last_of('/');
        if (pos != std::string::npos) {
            tilesetPath = tilesetPath.substr(pos + 1);
        }
        tilesetPath = "resources/tilesets/" + tilesetPath;
        tilesets.push_back({tilesetPath, tileset["firstgid"].get<int>()});
    }

    for (const auto& layer : j["layers"]) {
        if (layer["type"] == "imagelayer") {
            std::cout << "Image layer found!" << std::endl;
            std::string imagePath = layer["image"].get<std::string>();
            size_t pos = imagePath.find_last_of('/');
            if (pos != std::string::npos) {
                imagePath = imagePath.substr(pos + 1);
            }
            Texture2D source = LoadTexture(("resources/background/" + imagePath).c_str());
            source.width /= 16;
            source.height /= 16;
            int numRepeated = width / source.width + 1;
            const auto& x = layer["x"].get<float>();
            const auto& y = layer["y"].get<float>();
            Background* background = new Background(source, Vector2{x, y}, numRepeated); 
            nodes.push_back(background);
        }
        else if (layer["type"] == "tilelayer" && layer.contains("data")) {
            std::cout << "Tile layer found!" << std::endl;
            const auto& data = layer["data"];

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    int id = data[y * width + x].get<int>();  
                    if (id == 0) {
                        continue;  
                    }
                    const auto& [tilesetPath, firstGid] = GetTilesetInfo(id);
                    id -= firstGid;
                    Vector2 pos = {static_cast<float>(x), static_cast<float>(y)};

                    Tile* tile = TilesetHandler::getTile(tilesetPath, id);
                    if (tile == nullptr) {
                        std::cerr << "Tilemap::Tile with ID: " << id << "not exist" << std::endl;
                        std::cout << "Map id: " << data[y * width + x] << std::endl;
                        continue;  
                    }
                    tile->setPosition(pos);
                    nodes.push_back(tile->clone());    
                }
            }
        }
        else if (layer["type"] == "objectgroup" && layer.contains("objects")) {
            std::cout << "Object layer found!" << std::endl;
            for (const auto& object : layer["objects"]) {
                float width = object["width"].get<float>() / tileSize;
                float height = object["height"].get<float>() / tileSize;
                float x = object["x"].get<float>() / tileSize;
                float y = object["y"].get<float>() / tileSize;
                if (width == 0 && height == 0) {
                    if (object.contains("polygon")) {
                        std::cout << "Polygon object found!" << std::endl;
                        std::vector<b2Vec2> vertices;
                        for (const auto& vertex : object["polygon"]) {
                            vertices.push_back(b2Vec2{vertex["x"].get<float>() / tileSize, vertex["y"].get<float>() / tileSize});
                        }
                        b2Body* body;
                        MyBoundingBox::createBody(body, b2_staticBody, vertices, Vector2{x, y});
                        StaticObject* obj = new StaticObject(body);
                        nodes.push_back(obj);
                    }
                    else {
                    // create object with a single point
                    }
                }
                else {
                    std::vector<b2Vec2> vertices = {
                        b2Vec2{0.0f, 0.0f},
                        b2Vec2{width, 0.0f},
                        b2Vec2{width, height},
                        b2Vec2{0.0f, height}
                    };
                    b2Body* body;
                    MyBoundingBox::createBody(body, b2_staticBody, vertices, Vector2{x, y});
                    StaticObject* obj = new StaticObject(body);
                    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(obj);
                    nodes.push_back(obj);
                }
            }
        }
    }
    file.close();
}

void Tilemap::Update(Vector2 playerVelocity, float deltaTime) {
    for (auto& node : nodes) {
        node->Update(playerVelocity, deltaTime);
    }
}

void Tilemap::Draw() const {
    for (const auto& node : nodes) {
        node->Draw();
    }
}

int Tilemap::GetWidth() const {
    return width;
}

int Tilemap::GetHeight() const {
    return height;
}

int Tilemap::GetTileSize() const {
    return tileSize;
}
