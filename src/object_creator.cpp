#include "object_creator.h"

std::unordered_map<std::string, std::function<SceneNode*(Vector2)>> ObjectCreator::creators;


void ObjectCreator::RegisterObject(const std::string& name, std::function<SceneNode*(Vector2)> creator) {
    creators[name] = creator;
}



void ObjectCreator::InitObjects() {
    
    RegisterObject("horizontalmovingplatform", [](Vector2 pos) {
        float boundaries[] = {0.0f, 0.0f, pos.x - 3.0f, pos.x + 3.0f}; // Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(Vector2{1.0f, 4.0f}, Vector2{1.0f, 0.0f}, 2.0f, 0.0f, "movingplatform");
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });

    RegisterObject("verticalmovingplatform", [](Vector2 pos) {
     float boundaries[] = {pos.y + 3.0f, pos.y - 3.0f, 0.0f, 0.0f};// Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(Vector2{1.0f, 4.0f}, Vector2{0.0f, 1.0f}, 2.0f, 0.0f, "movingplatform");
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });

    RegisterObject("upverticalmovingplatform", [](Vector2 pos) {
     float boundaries[] = {pos.y + 20.0f, pos.y - 3.0f, 0.0f, 0.0f};// Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(Vector2{1.0f, 4.0f}, Vector2{0.0f, -1.0f}, 0.0f, 0.0f, "movingplatform");
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });

    RegisterObject("downverticalmovingplatform", [](Vector2 pos) {
     float boundaries[] = {pos.y + 3.0f, pos.y - 20.0f, 0.0f, 0.0f};// Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(Vector2{1.0f, 4.0f}, Vector2{0.0f, 1.0f}, 0.0f, 0.0f, "movingplatform");
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });

    RegisterObject("rotatingblaze", [](Vector2 pos) {
    MovingPlatform* platform = new MovingPlatform(Vector2{1.0f, 4.0f}, Vector2{0.0f, 1.0f}, 0.0f, 1.0f, "rotating");
    platform->Init(b2Vec2{pos.x, pos.y});

    for (int i = 0; i < 4; ++i) { 
        FireBall* fireball = new FireBall( 1.0f,Vector2{0.5f, 0.5f}); 
        fireball->Init(b2Vec2{pos.x, pos.y}); 
        platform->AddFireBall(fireball);
    }

    return platform;
});
}

// Creates a platform based on the given name and position
SceneNode* ObjectCreator::CreateObject(const std::string& name, Vector2 position) {
    auto it = creators.find(name);
    if (it != creators.end()) {
        return it->second(position);
    }
    return nullptr;
}