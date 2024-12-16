#include "moving_platform.h"
// Define the static member
std::unordered_map<std::string, std::function<MovingPlatform*(Vector2)>> PlatformCreator::creators;

// Registers a platform type with a corresponding creation function
void PlatformCreator::RegisterPlatform(const std::string& name, std::function<MovingPlatform*(Vector2)> creator) {
    creators[name] = creator;
}

// Initializes predefined platform types
void PlatformCreator::InitPlatforms() {
    // Register horizontal platform
    RegisterPlatform("movingplatform", [](Vector2 pos) {
        float boundaries[] = {0.0f, 0.0f, pos.x - 8.0f, pos.x + 8.0f}; // Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(MovementType::Horizontal, Vector2{1.0f, 4.0f}, 2.0f, 0.0f, boundaries);
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });
/*
    // Register vertical platform
    RegisterPlatform("vertical", [](Vector2 pos) {
        float boundaries[] = {pos.y + 3.0f, pos.y - 3.0f, 0.0f, 0.0f}; // Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(MovementType::Vertical, Vector2{2.0f, 0.5f}, 2.0f, 0.0f, boundaries);
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });

    // Additional platform types can be registered here
    */
}

// Creates a platform based on the given name and position
MovingPlatform* PlatformCreator::CreatePlatform(const std::string& name, Vector2 position) {
    auto it = creators.find(name);
    if (it != creators.end()) {
        return it->second(position);
    }
    return nullptr;
}