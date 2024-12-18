#include "moving_platform.h"

std::unordered_map<std::string, std::function<MovingPlatform*(Vector2)>> PlatformCreator::creators;


void PlatformCreator::RegisterPlatform(const std::string& name, std::function<MovingPlatform*(Vector2)> creator) {
    creators[name] = creator;
}


void PlatformCreator::InitPlatforms() {
    
    RegisterPlatform("movingplatform", [](Vector2 pos) {
        float boundaries[] = {0.0f, 0.0f, pos.x - 3.0f, pos.x + 3.0f}; // Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(MovementType::Horizontal, Vector2{1.0f, 4.0f}, 1.0f, 0.0f, boundaries);
        platform->Init(b2Vec2{pos.x, pos.y});
        //platform->setSpeed(2.0f);
        return platform;
    });

    RegisterPlatform("verticalmovingplatform", [](Vector2 pos) {
     float boundaries[] = {pos.y + 3.0f, pos.y - 3.0f, 0.0f, 0.0f};// Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(MovementType::Vertical, Vector2{1.0f, 4.0f}, 1.0f, 0.0f, boundaries);
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });

    RegisterPlatform("upverticalmovingplatform1", [](Vector2 pos) {
     float boundaries[] = {pos.y + 3.0f, pos.y - 3.0f, 0.0f, 0.0f};// Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(MovementType::UpVertical, Vector2{1.0f, 4.0f}, 1.0f, 0.0f, boundaries);
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });

    RegisterPlatform("upverticalmovingplatform2", [](Vector2 pos) {
     float boundaries[] = {pos.y + 3.0f, pos.y - 3.0f, 0.0f, 0.0f};// Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(MovementType::UpVertical, Vector2{1.0f, 4.0f}, 1.0f, 0.0f, boundaries);
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });

    RegisterPlatform("downverticalmovingplatform1", [](Vector2 pos) {
     float boundaries[] = {pos.y + 3.0f, pos.y - 3.0f, 0.0f, 0.0f};// Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(MovementType::DownVertical, Vector2{1.0f, 4.0f}, 1.0f, 0.0f, boundaries);
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });

    RegisterPlatform("downverticalmovingplatform2", [](Vector2 pos) {
     float boundaries[] = {pos.y + 3.0f, pos.y - 3.0f, 0.0f, 0.0f};// Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(MovementType::DownVertical, Vector2{1.0f, 4.0f}, 1.0f, 0.0f, boundaries);
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });
}

// Creates a platform based on the given name and position
MovingPlatform* PlatformCreator::CreatePlatform(const std::string& name, Vector2 position) {
    auto it = creators.find(name);
    if (it != creators.end()) {
        return it->second(position);
    }
    return nullptr;
}