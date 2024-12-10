#include "game.h"
#include "include.h"
#include "physics.h"
#include "contactlistener.h"
#include "moving_object.h"
#include "imagehandler.h"
#include "tileset_handler.h"
#include "animation_effect_creator.h"
#include "tilemap.h"
#include "camera.h"
#include "text_helper.h"
#include <raylib.h>
#include <box2d/box2d.h>
Game* Game::instance = nullptr;

const int screenWidth = 800;
const int screenHeight = 600;

Player player("Player", 0, 0, true, false, 0, 0, 0, 0, Vector2{0, 0}, 0, 0, {}, "mario");
MyCamera camera;
Tilemap tilemap;

Game::Game(){
    Init();
}

void Game::Init() {
    InitWindow(screenWidth, screenHeight, "Super Mario Bros");
    SetTargetFPS(60);

    Physics::Init(); 
    AnimationEffectCreator::InitEffects();
    TextHelper::loadFont("highway_gothic", "");
    TextHelper::loadTexture("coin");


    ImageSet idleImageSet = IDLE;
    movingObjects.push_back(player.copy());

    movingObjects[0]->Init(b2Vec2(10.0f, 12.0f), idleImageSet);

    TilesetHandler::Init();
    tilemap.LoadMapFromJson("resources/tilemaps/map-1-1.json");
    camera = MyCamera(38.0f, Vector2{ (float)tilemap.GetWidth(), (float)tilemap.GetHeight() }, screenWidth, screenHeight);
}

void Game::Update(float deltaTime) {
    Physics::Update(deltaTime); 
    if (GetMouseWheelMove() > 0) camera.SetZoom(camera.GetZoom() * 1.1f);
    if (GetMouseWheelMove() < 0) camera.SetZoom(camera.GetZoom() / 1.1f);


    for (int i = 0; i < movingObjects.size(); i++) {
        Vector2 pos = movingObjects[i]->getPosition();
        b2Vec2 vel = movingObjects[i]->getVelocity();
        movingObjects[i]->HandleInput();
        movingObjects[i]->Update(Vector2{vel.x, vel.y}, deltaTime);
    }

    b2Vec2 velocity = movingObjects[0]->getVelocity();
    Vector2 position = movingObjects[0]->getPosition();
    //cout << "player position: " << position.x << " " << position.y << endl;
    camera.Update(movingObjects[0]->getPosition());  
    tilemap.Update(Vector2{velocity.x, velocity.y}, deltaTime);


    // Vector2 pos = movingObjects[0]->getPosition();
    // b2Vec2 velocity = movingObjects[0]->getVelocity();
    // camera.Update(pos);
    // movingObjects[0]->HandleInput(fireFlowers); 
    // movingObjects[0]->Update(Vector2{velocity.x, velocity.y}, deltaTime); 


}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode2D(camera.GetCamera());

    tilemap.Draw();
    for (int i = 0; i < movingObjects.size(); i++) {
        movingObjects[i]->Draw();
    }

    Vector2 cameraTarget = camera.GetCameraTarget();
    movingObjects[0]->Draw(Vector2(cameraTarget.x - 9.0f, cameraTarget.y - 7.0f), 0.0f);
    
    Physics::world.DebugDraw(); 

    EndMode2D();
    EndDrawing();
}

void Game::Cleanup() {
    // Physics::world.DestroyBody(groundBody); 
    TilesetHandler::Clear();
    CloseWindow();

    for (int i = 0; i < movingObjects.size(); i++) {
        delete movingObjects[i];
    } 
}

void Game::Run() {
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        Update(deltaTime); 
        Draw();         
    }
    Cleanup(); 
}
