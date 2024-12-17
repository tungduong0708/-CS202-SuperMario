#ifndef TILEMAP_H
#define TILEMAP_H

#include "tileset_handler.h"
#include "tileset.h"
#include "renderer.h"
#include "character.h"
#include "camera.h"
#include "effect_manager.h"
#include <vector>
#include <string>
#include <raylib.h> 
#include "nlohmann/json.hpp" 

const int screenWidth = 800;
const int screenHeight = 600;

class Tilemap {
private:
    bool isChangingMap = false;
    std::string newMapPath;
    float width, height, tileSize, tilesetColumns;
    MyCamera camera;
    Player* player;
    EffectManager* effectManager;
    std::vector<std::vector<SceneNode*>> nodes;
    std::vector<std::pair<std::string, int>> tilesets;
    
    Tilemap();
    Tilemap(const std::string& filePath);
    
    Tilemap(const Tilemap&) = delete;
    Tilemap& operator=(const Tilemap&) = delete;

    static Tilemap* instance;

public:
    ~Tilemap();
    static Tilemap* getInstance();
    void clearMap();
    void changeMap(const std::string& filePath);

    std::pair<std::string, int> GetTilesetInfo(int tileIdx) const;
    void addNode(SceneNode* node);
    void LoadMapFromJson(const std::string& filePath);
    void Update(float deltaTime);
    void Draw() const;

    void SetNewMapPath(const std::string& path);
    EffectManager* GetEffectManager();
    Player* GetPlayer();
    Vector2 GetPlayerPosition() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetTileSize() const;
};

#endif
