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
#include <set>
#include <raylib.h> 
#include "nlohmann/json.hpp" 

class Tilemap {
private:
    bool isChangingMap = false;
    std::string filePath;
    std::string newMapPath;
    float width, height, tileSize, tilesetColumns;
    MyCamera camera;
    Vector2 playerPosition;
    Player* player;
    EffectManager* effectManager;
    std::vector<std::vector<SceneNode*>> nodes;
    std::vector<std::pair<std::string, int>> tilesets;
    std::vector<SceneNode*> loadedNodes;
    std::set<std::pair<int, int>> activatedTiles;
    
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
    void LoadSaveGame(const std::string& filePath);
    void SaveGame();
    void Update(float deltaTime);
    void Draw() const;
    
    void setPlayer(const std::string name);
    void SetNewMapPath(const std::string& path);
    std::string GetCurrentMapPath() const;
    EffectManager* GetEffectManager();
    Player* GetPlayer();
    Vector2 GetPlayerPosition() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetTileSize() const;
};

#endif
