#include "effect_manager.h"

std::map<std::pair<int, int>, std::string> EffectManager::effectMap;
std::vector<AnimationEffect*> EffectManager::effects;

EffectManager::~EffectManager()
 {
    for (auto effect : effects) {
        delete effect;
    }
    effects.clear();
}

void EffectManager::AddEffect(AnimationEffect *effect)
{
    effects.push_back(effect);
}

void EffectManager::Update(float deltaTime)
 {
    for (auto it = effects.begin(); it != effects.end();) {
        if (*it == nullptr) {
            ++it;  // Skip null pointers
            continue;
        }
        
        if ((*it)->isActive()) {
            (*it)->Update(deltaTime);
            ++it;  // Move to the next element
        } else {
            delete *it;            // Free memory
            it = effects.erase(it); // Erase element and get a valid iterator to the next element
        }
    }

}

void EffectManager::Draw()
{
    for (auto* effect : effects) {
        if (effect == nullptr) continue;
        effect->Draw();
    }
}
