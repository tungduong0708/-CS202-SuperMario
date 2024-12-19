#include "enemy_creator.h"
#include "character.h"

std::unordered_map<std::string, std::function<Enemy*(Vector2)>> EnemyCreator::creators;

void EnemyCreator::RegisterEnemy(const std::string& name, std::function<Enemy*(Vector2)> creator) {
    creators[name] = creator;
}

void EnemyCreator::InitEnemies() {
    RegisterEnemy("goomba", [](Vector2 pos) { 
        Goomba* goomba = new Goomba("goomba", 0.0f, true, false, 100, 100, 1, 100, Vector2{1.0f, 1.0f}, -2.0f, 0.0f); 
        goomba->Init(b2Vec2{pos.x, pos.y});
        goomba->setSpeed(-2.0f);
        return goomba;
    });
    RegisterEnemy("koopa", [](Vector2 pos) { 
        Koopa* koopa = new Koopa("koopa", 0.0f, true, false, 100, 100, 1, 100, Vector2{1.0f, 1.0f}, -2.0f, 0.0f); 
        koopa->Init(b2Vec2{pos.x, pos.y});
        koopa->setSpeed(-2.0f);
        return koopa;
    });
    RegisterEnemy("boss", [](Vector2 pos) { 
        Boss* boss = new Boss("boss", 0.0f, true, 100, 100, 1, 100, Vector2{1.0f, 1.0f}, 1.0f, 0.0f); 
        boss->Init(b2Vec2{pos.x, pos.y});
        boss->setSpeed(-2.0f);
        return boss;
    });
    RegisterEnemy("larvabubble", [](Vector2 pos) { 
        cout << "Creating larvabubble" << endl;
        LarvaBubble* larvaBubble = new LarvaBubble("larvabubble", 0.0f, true, 100, 100, 1, 100, Vector2{0.6f, 0.6f}); 
        larvaBubble->Init(b2Vec2{pos.x, pos.y});
        return larvaBubble;
    });
    RegisterEnemy("monsterflower", [](Vector2 pos) { 
        MonsterFlower* monsterFlower = new MonsterFlower("monsterflower", 0.0f, true, 100, 100, 1, 100, Vector2{1.0f, 1.0f}); 
        monsterFlower->Init(b2Vec2{pos.x, pos.y});
        return monsterFlower;
    });
}

Enemy* EnemyCreator::CreateEnemy(const std::string& name, Vector2 position) {
    auto it = creators.find(name);
    if (it != creators.end()) {
        return it->second(position);
    }
    return nullptr;
}