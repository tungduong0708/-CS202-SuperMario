// Game.cpp
#include "Game.h"

Game::Game(const std::string& jsonFilePath) : filePath(jsonFilePath) {
    // Load the JSON data
    LoadData();
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
    InitWindow(800, 600, "JSON Data Display with raylib");
    SetTargetFPS(60);

    // Initialize Box2D world
    b2Vec2 gravity(0.0f, -10.0f);
    world = new b2World(gravity);

    // Create the ground body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    groundBody = world->CreateBody(&groundBodyDef);

    // Create a ground box shape
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Create a dynamic body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);
    dynamicBody = world->CreateBody(&bodyDef);

    // Create a box shape for the dynamic body
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Attach the box shape to the dynamic body
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;
    dynamicBody->CreateFixture(&fixtureDef);
}

void Game::UpdatePhysics() {
    world->Step(1.0f / 60.0f, 8, 3); // Step the physics world
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Display data using raylib's drawing functions
    DrawText(("Name: " + j["name"].get<std::string>()).c_str(), 50, 50, 20, BLACK);
    DrawText(("Age: " + std::to_string(j["age"].get<int>())).c_str(), 50, 80, 20, BLACK);
    DrawText(("Is Student: " + std::string(j["is_student"].get<bool>() ? "Yes" : "No")).c_str(), 50, 110, 20, BLACK);
    DrawText(("Math Grade: " + std::to_string(j["grades"]["math"].get<int>())).c_str(), 50, 140, 20, BLACK);
    DrawText(("Science Grade: " + std::to_string(j["grades"]["science"].get<int>())).c_str(), 50, 170, 20, BLACK);
    
    // Draw dynamic body (as a rectangle)
    b2Vec2 position = dynamicBody->GetPosition();
    DrawRectangle(static_cast<int>(position.x * 20), 600 - static_cast<int>(position.y * 20), 40, 40, BLUE);

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
