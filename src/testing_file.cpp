// #include <raylib.h>
// #include <box2d/box2d.h>
// #include <iostream>

// // Constants for the simulation
// const float SCALE = 30.0f; // Pixels per meter

// // Convert Box2D coordinates to screen coordinates
// Vector2 B2ToRaylib(const b2Vec2& vec) {
//     return Vector2{ vec.x * SCALE, GetScreenHeight() - (vec.y * SCALE) };
// }

// // Custom contact listener
// class ContactListener : public b2ContactListener {
// public:
//     void BeginContact(b2Contact* contact) override {
//         void* bodyUserDataA = (void*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
//         void* bodyUserDataB = (void*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

//         if (bodyUserDataA || bodyUserDataB) {
//             if (bodyUserDataA == (void*)1 || bodyUserDataB == (void*)1) {
//                 hit = true;
//             }
//         }
//     }

//     bool hit = false; // Flag to check if box is hit
// };

// int main() {
//     // Initialize Raylib
//     InitWindow(800, 600, "Bouncing Box in Mario Style");
//     SetTargetFPS(60);

//     // Box2D world setup
//     b2Vec2 gravity(0.0f, -9.8f);
//     b2World world(gravity);

//     // Create ground
//     b2BodyDef groundDef;
//     groundDef.position.Set(400.0f / SCALE, 50.0f / SCALE);
//     b2Body* ground = world.CreateBody(&groundDef);

//     b2PolygonShape groundShape;
//     groundShape.SetAsBox(400.0f / SCALE, 10.0f / SCALE);
//     ground->CreateFixture(&groundShape, 0.0f);

//     // Create player (e.g., Mario)
//     b2BodyDef playerDef;
//     playerDef.type = b2_dynamicBody;
//     playerDef.position.Set(400.0f / SCALE, 150.0f / SCALE);
//     b2Body* player = world.CreateBody(&playerDef);

//     b2PolygonShape playerShape;
//     playerShape.SetAsBox(15.0f / SCALE, 15.0f / SCALE);

//     b2FixtureDef playerFixture;
//     playerFixture.shape = &playerShape;
//     playerFixture.density = 1.0f;
//     playerFixture.friction = 0.3f;
//     player->CreateFixture(&playerFixture);

//     // Create box
//     b2BodyDef boxDef;
//     boxDef.type = b2_dynamicBody;
//     boxDef.position.Set(400.0f / SCALE, 300.0f / SCALE);
//     b2Body* box = world.CreateBody(&boxDef);
//     box->GetUserData().pointer = 1;

//     b2PolygonShape boxShape;
//     boxShape.SetAsBox(20.0f / SCALE, 10.0f / SCALE);

//     b2FixtureDef boxFixture;
//     boxFixture.shape = &boxShape;
//     boxFixture.density = 1.0f;
//     boxFixture.friction = 0.3f;
//     boxFixture.restitution = 0.0f; // No bounce on normal collision
//     box->CreateFixture(&boxFixture);

//     // Create a prismatic joint to constrain box movement
//     b2PrismaticJointDef jointDef;
//     jointDef.bodyA = ground;
//     jointDef.bodyB = box;
//     jointDef.collideConnected = false;
//     jointDef.localAxisA.Set(0.0f, 1.0f); // Constrain movement to vertical
//     jointDef.localAnchorA.Set(0.0f, 0.0f);
//     jointDef.localAnchorB.Set(0.0f, 0.0f);
//     jointDef.enableLimit = true;
//     jointDef.lowerTranslation = -0.1f; // Allow slight downward movement
//     jointDef.upperTranslation = 0.2f;  // Allow upward bounce
//     world.CreateJoint(&jointDef);

//     // Contact listener
//     ContactListener contactListener;
//     world.SetContactListener(&contactListener);

//     // Main game loop
//     while (!WindowShouldClose()) {
//         // Step the Box2D world
//         world.Step(1.0f / 60.0f, 8, 3);

//         // Check for box hit
//         if (contactListener.hit) {
//             box->ApplyLinearImpulseToCenter(b2Vec2(0.0f, 2.0f), true);
//             contactListener.hit = false; // Reset hit flag
//         }

//         // Start drawing
//         BeginDrawing();
//         ClearBackground(RAYWHITE);

//         // Draw ground
//         Vector2 groundPos = B2ToRaylib(ground->GetPosition());
//         DrawRectangle(groundPos.x - 400, groundPos.y - 10, 800, 20, DARKGRAY);

//         // Draw player
//         b2Vec2 playerPos = player->GetPosition();
//         Vector2 playerScreenPos = B2ToRaylib(playerPos);
//         DrawRectangle(playerScreenPos.x - 15, playerScreenPos.y - 15, 30, 30, RED);

//         // Draw box
//         b2Vec2 boxPos = box->GetPosition();
//         Vector2 boxScreenPos = B2ToRaylib(boxPos);
//         DrawRectangle(boxScreenPos.x - 20, boxScreenPos.y - 10, 40, 20, BLUE);

//         EndDrawing();
//     }

//     // Cleanup
//     CloseWindow();

//     return 0;
// }
