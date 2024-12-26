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

enum TilemapType {
    TILEMAP_1P,
    TILEMAP_2P
};

class Tilemap {
protected:
    int difficulty;
    bool playerLoaded = false;
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
    std::vector<Tile*> changedTiles;

    static TilemapType mapType;

    static Tilemap* instance;

    Tilemap();
    Tilemap(const std::string& filePath, int difficulty);
    Tilemap(const Tilemap&) = delete;
    Tilemap& operator=(const Tilemap&) = delete;

public:
    static Tilemap* getInstance();

    virtual ~Tilemap();

    void clearMap();
    void changeMap(const std::string& filePath);
    std::pair<std::string, int> GetTilesetInfo(int tileIdx) const;
    void addNode(SceneNode* node);
    void addChangedTile(Tile* tile);
    void LoadMapFromJson(const std::string& filePath, int difficulty);
    void SetNewMapPath(const std::string& path);
    std::string GetCurrentMapPath() const;
    std::string GetNewMapPath() const;
    Camera2D getCamera() const;
    EffectManager* GetEffectManager();
    int GetWidth() const;
    int GetHeight() const;
    int GetTileSize() const;

    static void SetMapType(TilemapType type);

    virtual void SaveGame(std::string filePath) const;
    virtual void LoadSaveGame(const std::string& filePath);
    virtual void Update(float deltaTime);
    virtual void Draw() const;
    virtual void setPlayer(const std::string name);
    virtual Player* GetPlayer();
    virtual Vector2 GetPlayerPosition() const;

    virtual Player* GetLeadingPlayer() const = 0;
    virtual Player* GetFollowingPlayer() const = 0;
    virtual Vector2 GetLeadingPlayerPosition() const = 0;

    virtual void setPlayer2(const std::string name);
    virtual Player* GetPlayer2();
    virtual Vector2 GetPlayer2Position() const;
};

class Tilemap1P : public Tilemap {
private:
public:
    Tilemap1P();
    Tilemap1P(const std::string& filePath, int difficulty);
    ~Tilemap1P();

    void SaveGame(std::string filePath) const override;
    void LoadSaveGame(const std::string& filePath) override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void setPlayer(const std::string name) override;
    Player* GetPlayer() override;
    Vector2 GetPlayerPosition() const override;

    Player* GetLeadingPlayer() const override;
    Player* GetFollowingPlayer() const override;
    Vector2 GetLeadingPlayerPosition() const override;
};

class Tilemap2P : public Tilemap {
private:
    Player* player2;
    Vector2 player2Position;
    bool player2Loaded = false;


public:
    Tilemap2P();
    Tilemap2P(const std::string& filePath, int difficulty);
    ~Tilemap2P();

    void SaveGame(std::string filePath) const override;
    void LoadSaveGame(const std::string& filePath) override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void setPlayer(const std::string name) override;
    Player* GetPlayer() override;
    Vector2 GetPlayerPosition() const override;
    void setPlayer2(const std::string name);
    Player* GetPlayer2();
    Vector2 GetPlayer2Position() const;

    Player* GetLeadingPlayer() const override;
    Player* GetFollowingPlayer() const override;
    Vector2 GetLeadingPlayerPosition() const override;

    void UpdateMultiplayerPosition();
};

#endif
