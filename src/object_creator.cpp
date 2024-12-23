#include "object_creator.h"
#include "static_object.h"

std::unordered_map<std::string, std::function<SceneNode*(Vector2)>> ObjectCreator::creators;


void ObjectCreator::RegisterObject(const std::string& name, std::function<SceneNode*(Vector2)> creator) {
    creators[name] = creator;
}


void ObjectCreator::InitObjects() {
    RegisterObject("horizontalmovingplatform", [](Vector2 pos) {
        float boundaries[] = {0.0f, 0.0f, pos.x - 3.0f, pos.x + 3.0f}; // Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(Vector2{0.6875f, 3.0f}, Vector2{1.0f, 0.0f}, 2.0f, 0.0f, "movingplatform");
        platform->Init(b2Vec2{pos.x, pos.y});
        b2Fixture* fixture = platform->getBody()->GetFixtureList();
        fixture->SetFriction(10.0f);
        return platform;
    });

    RegisterObject("verticalmovingplatform", [](Vector2 pos) {
        float boundaries[] = {pos.y + 3.0f, pos.y - 3.0f, 0.0f, 0.0f};// Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(Vector2{0.6875f, 3.0f}, Vector2{0.0f, 1.0f}, 2.0f, 0.0f, "movingplatform");
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });

    RegisterObject("upverticalmovingplatform", [](Vector2 pos) {
        float boundaries[] = {pos.y + 20.0f, pos.y - 3.0f, 0.0f, 0.0f};// Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(Vector2{0.6875f, 3.0f}, Vector2{0.0f, -1.0f}, 0.0f, 0.0f, "movingplatform");
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });

    RegisterObject("downverticalmovingplatform", [](Vector2 pos) {
        float boundaries[] = {pos.y + 3.0f, pos.y - 20.0f, 0.0f, 0.0f};// Adjust boundaries as needed
        MovingPlatform* platform = new MovingPlatform(Vector2{0.6875f, 3.0f}, Vector2{0.0f, 1.0f}, 0.0f, 0.0f, "movingplatform");
        platform->Init(b2Vec2{pos.x, pos.y});
        return platform;
    });

    RegisterObject("rotatingblaze", [](Vector2 pos) {
        FireBlaze* fireblaze = new FireBlaze(pos, 2.0f, 0.0f, 120.0f);
        fireblaze->SetorbitCenter(pos);
        //platform->Init(b2Vec2{pos.x, pos.y});
        fireblaze->InitOrbit(pos, 2.0f, 120.0f);

        // Thiết lập tốc độ quay
        fireblaze->SetorbitSpeed(120.0f); // 90 độ/giây// Tâm quỹ đạo
        fireblaze->SetorbitRadius(2.0f);     
        return fireblaze;
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