#include "character_creator.h"
#include "character.h"

std::unordered_map<std::string, std::function<Character*(Vector2, int)>> CharacterCreator::creators;

void CharacterCreator::RegisterCharacter(const std::string& name, std::function<Character*(Vector2, int)> creator) {
    creators[name] = creator;
}

void CharacterCreator::InitCharacters() {
    RegisterCharacter("goomba", [](Vector2 pos, int difficulty) { 
        Goomba* goomba = new Goomba("goomba", 0.0f, true, false, 100, 100, difficulty, 100, Vector2{1.0f, 1.0f}, -2.0f, 0.0f); 
        goomba->Init(b2Vec2{pos.x, pos.y});
        goomba->setSpeed(-2.0f);
        return goomba;
    });
    RegisterCharacter("koopa", [](Vector2 pos, int difficulty) { 
        Koopa* koopa = new Koopa("koopa", 0.0f, true, false, 100, 100, difficulty, 100, Vector2{1.0f, 1.0f}, -2.0f, 0.0f); 
        koopa->Init(b2Vec2{pos.x, pos.y});
        koopa->setSpeed(-2.0f);
        return koopa;
    });
    RegisterCharacter("boss", [](Vector2 pos, int difficulty) { 
        Boss* boss = new Boss("boss", 0.0f, true, 100, 100, difficulty, 100, Vector2{1.0f, 1.0f}, 1.0f, 0.0f); 
        boss->Init(b2Vec2{pos.x, pos.y});
        boss->setSpeed(-2.0f);
        return boss;
    });
    RegisterCharacter("larvabubble", [](Vector2 pos, int difficulty) { 
        cout << "Creating larvabubble" << endl;
        LarvaBubble* larvaBubble = new LarvaBubble("larvabubble", 0.0f, true, 100, 100, difficulty, 100, Vector2{0.6f, 0.6f}); 
        larvaBubble->Init(b2Vec2{pos.x, pos.y});
        return larvaBubble;
    });
    RegisterCharacter("monsterflower", [](Vector2 pos, int difficulty) { 
        MonsterFlower* monsterFlower = new MonsterFlower("monsterflower", 0.0f, true, 100, 100, difficulty, 100, Vector2{1.0f, 1.0f}); 
        monsterFlower->Init(b2Vec2{pos.x, pos.y});
        return monsterFlower;
    });
    RegisterCharacter("princess", [](Vector2 pos, int difficulty) { 
        Princess* princess = new Princess("princess"); 
        princess->Init(b2Vec2{pos.x, pos.y});
        return princess;
    });
}

Character* CharacterCreator::CreateCharacter(const std::string& name, Vector2 position, int difficulty) {
    auto it = creators.find(name);
    if (it != creators.end()) {
        return it->second(position, difficulty);
    }
    return nullptr;
}