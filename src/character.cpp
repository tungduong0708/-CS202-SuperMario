#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <box2d/box2d.h>
#include <raylib.h>
#include <filesystem>
using json = nlohmann::json;
using namespace std;


class World {
private:
    b2World* world;
    b2Body* groundBody;
    b2Body* dynamicBody;

    void init() {
        InitWindow(800, 600, "Raylib test");
        SetTargetFPS(60);

        // create world
        world = new b2World(b2Vec2(0.0f, -10.0f));

        // create ground body
        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(50.0f, 10.0f);
        groundBody = world->CreateBody(&groundBodyDef);

        // create dynamic body
        b2BodyDef dBodyDef;
        dBodyDef.type = b2_dynamicBody; // if u want the body move in response to forces 
        dBodyDef.position.Set(0.0f, 4.0f);
        dynamicBody = world->CreateBody(&dBodyDef);

        // create a shape and attach it with the dynamic body
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(1.0f, 1.0f);

        // fixture def
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        
        dynamicBody->CreateFixture(&fixtureDef);
    }

    void physics() {
        float timeStep = 1.0f/ 60.0f;
        int velIters = 6; // velocity iteration
        int posIters = 2; // position iteration
        world->Step(timeStep, velIters, posIters);
    }

    void draw() {
        // begin drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // draw
        // dynamic body
        b2Vec2 pos = dynamicBody->GetPosition();
        DrawRectangle(pos.x * 100, pos.y * 100, 100, 100, RED);

        EndDrawing();
    }

    void cleanup() {
        delete world;
        CloseWindow();
    }

public:
    void run() {
        while (!WindowShouldClose()) {
            physics();
            draw();
        }
        cleanup();
    }
};