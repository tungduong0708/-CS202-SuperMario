#include "object_creator.h"
#include "static_object.h"

std::unordered_map<std::string, std::function<SceneNode*(Vector2)>> ObjectCreator::creators;


void ObjectCreator::RegisterObject(const std::string& name, std::function<SceneNode*(Vector2)> creator) {
    creators[name] = creator;
}


void ObjectCreator::InitObjects() {
    RegisterObject("horizontalmovingplatform", [](Vector2 pos) {
        float boundaries[] = {0.0f, 0.0f, pos.x - 3.0f, pos.x + 3.0f}; // Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(Vector2{1.0f, 4.0f}, Vector2{1.0f, 0.0f}, 2.0f);
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });

    RegisterObject("verticalmovingplatform", [](Vector2 pos) {
        float boundaries[] = {pos.y + 3.0f, pos.y - 3.0f, 0.0f, 0.0f};// Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(Vector2{1.0f, 4.0f}, Vector2{0.0f, 1.0f}, 2.0f);
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });

    RegisterObject("upverticalmovingplatform", [](Vector2 pos) {
        float boundaries[] = {pos.y + 20.0f, pos.y - 3.0f, 0.0f, 0.0f};// Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(Vector2{1.0f, 4.0f}, Vector2{0.0f, -1.0f}, 0.0f);
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });

    RegisterObject("downverticalmovingplatform", [](Vector2 pos) {
        float boundaries[] = {pos.y + 3.0f, pos.y - 20.0f, 0.0f, 0.0f};// Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(Vector2{1.0f, 4.0f}, Vector2{0.0f, 1.0f}, 0.0f);
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });

    RegisterObject("flag", [](Vector2 pos) {
        Flag* flag = new Flag();
        flag->Init(b2Vec2{pos.x, pos.y});
        return flag;
    });

    RegisterObject("pole", [](Vector2 pos) {
        StaticObject* pole = new Pole();
        return pole;
    });

    RegisterObject("gate", [](Vector2 pos) {
        Gate* gate = new Gate();
        return gate;
    });

    RegisterObject("axe", [](Vector2 pos) {
        Axe* axe = new Axe();
        axe->Init(b2Vec2{pos.x, pos.y});
        return axe;
    });
}

SceneNode* ObjectCreator::CreateObject(const std::string& name, Vector2 position) {
    auto it = creators.find(name);
    if (it != creators.end()) {
        return it->second(position);
    }
    return nullptr;
}