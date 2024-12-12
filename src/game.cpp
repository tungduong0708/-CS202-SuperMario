#include "game.h"
#include "include.h"
#include "object.h"

Game* Game::instance = nullptr;

Game::Game(){
    Init();
}

void Game::Init() {
    SetTraceLogCallback([](int logLevel, const char* text, va_list args) {
        return;
    });
    InitWindow(screenWidth, screenHeight, "Super Mario Bros");
    SetTargetFPS(60);

    Physics::Init(); 
    AnimationEffectCreator::InitEffects();
    TextHelper::loadFont("highway_gothic", "");
    TextHelper::loadTexture("coin");

    TilesetHandler::Init();
    Tilemap* tilemap = Tilemap::getInstance();
    tilemap->LoadMapFromJson("resources/tilemaps/map-1-1.json");
}

void Game::Update(float deltaTime) {
    Physics::Update(deltaTime); 
 
    Tilemap* tilemap = Tilemap::getInstance();
    tilemap->Update(deltaTime);
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    Tilemap* tilemap = Tilemap::getInstance();
    tilemap->Draw();
   
    EndDrawing();
}

void Game::Cleanup() {
    TilesetHandler::Clear();
    CloseWindow();
}

void Game::Run() {
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        Update(deltaTime); 
        Draw();         
    }
    Cleanup(); 
}
