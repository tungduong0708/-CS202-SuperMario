// Game.h
#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <raylib.h>
#include <filesystem>
#include <box2d/box2d.h>
using json = nlohmann::json;

class Game {
public:
    Game(const std::string& jsonFilePath);
    void Run();

private:
    void Init();
    void LoadData();
    void UpdatePhysics();
    void Draw();
    void Cleanup();

    json j;                  // JSON data
    b2World* world;          // Box2D world
    b2Body* groundBody;      // Ground body
    b2Body* dynamicBody;     // Dynamic body
    std::string filePath;    // Path to JSON file
};

#endif // GAME_H
