#include "tilemap.h"
#include "tileset.h"
#include "tile.h"
#include "static_tile.h"
#include "kinematic_tile.h"
#include "scene_node.h"
#include "background.h"
#include "static_object.h"
#include "physics.h"
#include "effect_manager.h"
#include "camera.h"
#include <fstream>
#include <iostream>
#include <box2d/box2d.h>
#include "nlohmann/json.hpp" 
using json = nlohmann::json;

Tilemap* Tilemap::instance;

Tilemap::Tilemap(const std::string& filePath) {
    LoadMapFromJson(filePath);
}

Tilemap::~Tilemap() {
    clearMap();
}

Tilemap* Tilemap::getInstance()
 {
    if (instance == nullptr) {
        instance = new Tilemap();
    }
    return instance;
}

void Tilemap::clearMap()
{
    for (auto& layer : nodes) {
        for (auto& node : layer) {
            delete node;
        }
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

void Tilemap::addNode(SceneNode *node)
{
    nodes.back().push_back(node);
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

    camera = MyCamera(38.0f, Vector2{ (float)width, (float)height }, screenWidth, screenHeight);

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
        std::vector<SceneNode*> nodeLayer;
        if (layer["type"] == "imagelayer" && layer["name"] != "Effect") {
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
            nodeLayer.push_back(background);
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
                    nodeLayer.push_back(tile->clone());
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
                        nodeLayer.push_back(obj);
                    }
                    else {
                        if (object.contains("type") && object["type"] == "player") {
                            std::cout << "marioaaa" << " " << x << " " << y << std::endl;
                            std::string name = object["name"].get<std::string>();
                            player = new Player(name);
                            player->Init(b2Vec2{x, y});
                            player->setName(name);
                        }
                        else {
                            EffectManager::effectMap[{(int)x, (int)y}] = object["name"].get<std::string>();
                            if (object.contains("type") && object["type"] != "") {
                                EffectManager::effectCount[{(int)x, (int)y}] = std::stoi(object["type"].get<std::string>());
                            }
                            else EffectManager::effectCount[{(int)x, (int)y}] = 1;
                        }
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
                    nodeLayer.push_back(obj);
                }
            }
        }
        nodes.push_back(nodeLayer);
    }
    file.close();
}

void Tilemap::Update(float deltaTime) {
    b2Vec2 playerVelocity = player->getVelocity();
    player->HandleInput();
    player->Update(Vector2{playerVelocity.x, playerVelocity.y}, deltaTime);
    for (auto& layer : nodes) {
        if (layer.empty()) {
            EffectManager::Update(deltaTime);
            continue;
        }
        for (auto& node : layer) {
            node->Update(Vector2{playerVelocity.x, playerVelocity.y}, deltaTime);
        }
    }
    camera.Update(player->getPosition());  

}

void Tilemap::Draw() const {
    BeginMode2D(camera.GetCamera());
    for (const auto& layer : nodes) {
        if (layer.empty()) {
            EffectManager::DrawLower();
            continue;
        }
        for (auto& node : layer) {
            node->Draw();
        }
    }
    EffectManager::DrawUpper();
    player->Draw();
    Vector2 cameraTarget = camera.GetCameraTarget();
    player->Draw(Vector2{cameraTarget.x - 9.0f, cameraTarget.y - 7.0f}, 0.0f);
    EndMode2D();

}

Vector2 Tilemap::GetPlayerPosition() const
{
    return player->getPosition();
}

int Tilemap::GetWidth() const
{
    return width;
}

int Tilemap::GetHeight() const {
    return height;
}

int Tilemap::GetTileSize() const {
    return tileSize;
}
