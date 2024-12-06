#include "game.h"
#include "include.h"
#include "physics.h"
#include "contactlistener.h"
#include "moving_object.h"
#include "imagehandler.h"
#include "tileset_handler.h"
#include "tilemap.h"
#include "camera.h"
#include <raylib.h>
#include <box2d/box2d.h>

const int screenWidth = 800;
const int screenHeight = 600;

Player player("Player", 0, 0, true, false, 0, 0, 0, 0, Vector2{0, 0}, 0, 0, {}, "smallmario");
MyCamera camera;
Tilemap tilemap;

Game::Game(){
    Init();
}

void Game::Init() {
    InitWindow(screenWidth, screenHeight, "Super Mario Bros");
    SetTargetFPS(60);

    Physics::Init(); 

    ImageSet idleImageSet = IDLE;
    movingObjects.push_back(player.copy());

    movingObjects[0]->InitCharacter(b2Vec2(10.0f, 12.0f), idleImageSet);

    TilesetHandler::Init();
    tilemap.LoadMapFromJson("resources/tilemaps/map-1-1.json");
    camera = MyCamera(40.0f, Vector2{ (float)tilemap.GetWidth(), (float)tilemap.GetHeight() }, screenWidth, screenHeight);
}

void Game::Update(float deltaTime) {
    if (GetMouseWheelMove() > 0) camera.SetZoom(camera.GetZoom() * 1.1f);
    if (GetMouseWheelMove() < 0) camera.SetZoom(camera.GetZoom() / 1.1f);

    b2Vec2 pos = movingObjects[0]->getPosition();
    b2Vec2 velocity = movingObjects[0]->getVelocity();
    camera.Update(Vector2{ pos.x, pos.y });
    
    movingObjects[0]->HandleInput(); 
    movingObjects[0]->Update(Vector2{velocity.x, velocity.y}, deltaTime); 

    tilemap.Update(Vector2{velocity.x, velocity.y}, deltaTime);

    Physics::Update(deltaTime); 
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode2D(camera.GetCamera());

    // DrawRectangle(0, 550, 750, 50, GRAY);
    tilemap.Draw();
    movingObjects[0]->Draw(); 
    Physics::world.DebugDraw(); 

    EndMode2D();
    EndDrawing();
}

void Game::Cleanup() {
    // Physics::world.DestroyBody(groundBody); 
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
