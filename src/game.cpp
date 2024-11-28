// Game.cpp
#include "Game.h"
#include "include.h"
#include "contactlistener.h"
#include "moving_object.h"
#include "image.h"

Game::Game(){
    // Initialize the game window and Box2D world
    Init();
}

void Game::LoadData() {
    std::ifstream input_file(filePath);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        exit(1); // Exit if the file cannot be opened
    }

    input_file >> j; // Parse the JSON
}

void Game::Init() {
    // Initialize raylib
    InitWindow(800, 600, "Character Animation with Ground Detection");
    SetTargetFPS(60);

    // Initialize Box2D world
    b2Vec2 gravity(0.0f, -10.0f);
    world = new b2World(gravity);
    ContactListener contactListener;
    world->SetContactListener(&contactListener);

    // Create the ground body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    groundBody = world->CreateBody(&groundBodyDef);

    // Create a ground box shape
    b2PolygonShape groundBox;
    groundBox.SetAsBox(10.0f, 5.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Create a character body
    Player player("Player", 0, 0, true, false, 0, 0, 0, 0, 0, 0, 0, 0, {}, "smallmario");
    ImageSet idleImageSet = IDLE;
    player.InitCharacter(*world, b2Vec2(0.0f, 0.0f), idleImageSet);
    player.SetOnGround(true);
    groundBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(&player);

    movingObjects.push_back(&player);
}

void Game::UpdatePhysics() {
    world->Step(1.0f / 60.0f, 8, 3); // Step the physics world
    movingObjects[0]->Update(); // Update the player    
    movingObjects[0]->HandleInput(); // Handle player input
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw ground body (as a rectangle)
    DrawRectangle(0, 550, 750, 50, GRAY);
    
    movingObjects[0]->Render(); // Render the player

    EndDrawing();
}

void Game::Cleanup() {
    // Close the window and clean up resources
    delete world; // Free the Box2D world
    CloseWindow(); // Close the raylib window
}

void Game::Run() {
    while (!WindowShouldClose()) {
        UpdatePhysics(); // Update physics
        Draw();          // Draw everything
    }
    Cleanup(); // Cleanup resources
}
