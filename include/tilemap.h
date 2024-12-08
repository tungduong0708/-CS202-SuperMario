#ifndef TILEMAP_H
#define TILEMAP_H

#include "tileset_handler.h"
#include "tileset.h"
#include "renderer.h"
#include <vector>
#include <string>
#include <raylib.h> 
#include "nlohmann/json.hpp" 

class Tilemap {
private:
    int width, height, tileSize;
    std::vector<SceneNode*> nodes;
    std::vector<std::pair<std::string, int>> tilesets;
    int tilesetColumns; 

public:
    Tilemap() = default;
    Tilemap(const std::string& filePath);
    ~Tilemap();

    std::pair<std::string, int> GetTilesetInfo(int tileIdx) const;
    void LoadMapFromJson(const std::string& filePath);
    void Update(Vector2 playerVelocity, float deltaTime);
    void Draw() const;

    int GetWidth() const;
    int GetHeight() const;
    int GetTileSize() const;
};

#endif
