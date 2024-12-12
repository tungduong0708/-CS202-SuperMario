#ifndef TILEMAP_H
#define TILEMAP_H

#include "tileset_handler.h"
#include "tileset.h"
#include "renderer.h"
#include "character.h"
#include "camera.h"
#include <vector>
#include <string>
#include <raylib.h> 
#include "nlohmann/json.hpp" 

const int screenWidth = 800;
const int screenHeight = 600;

class Tilemap {
private:
    MyCamera camera;
    int width, height, tileSize;
    Player* player;
    std::vector<std::vector<SceneNode*>> nodes;
    std::vector<std::pair<std::string, int>> tilesets;
    int tilesetColumns;

    Tilemap() = default;
    Tilemap(const std::string& filePath);
    
    Tilemap(const Tilemap&) = delete;
    Tilemap& operator=(const Tilemap&) = delete;

    static Tilemap* instance;

public:
    ~Tilemap();
    static Tilemap* getInstance();
    void clearMap();

    std::pair<std::string, int> GetTilesetInfo(int tileIdx) const;
    void addNode(SceneNode* node);
    void LoadMapFromJson(const std::string& filePath);
    void Update(float deltaTime);
    void Draw() const;

    Vector2 GetPlayerPosition() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetTileSize() const;
};

#endif
