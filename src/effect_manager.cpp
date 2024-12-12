#include "effect_manager.h"

std::map<std::pair<int, int>, std::string> EffectManager::effectMap;
std::vector<AnimationEffect*> EffectManager::lowerEffects;
std::vector<AnimationEffect*> EffectManager::upperEffects;

EffectManager::~EffectManager()
 {
    for (auto* effect : lowerEffects) {
        delete effect;
    }
    lowerEffects.clear();
    for (auto* effect : upperEffects) {
        delete effect;
    }
    upperEffects.clear();
}

void EffectManager::AddLowerEffect(AnimationEffect *effect)
{
    lowerEffects.push_back(effect);
}

void EffectManager::AddUpperEffect(AnimationEffect *effect)
{
    upperEffects.push_back(effect);
}

void EffectManager::Update(float deltaTime)
 {
    for (auto it = lowerEffects.begin(); it != lowerEffects.end();) {
        if (*it == nullptr) {
            ++it;  // Skip null pointers
            continue;
        }
        
        if ((*it)->isActive()) {
            (*it)->Update(deltaTime);
            ++it;  // Move to the next element
        } else {
            delete *it;            // Free memory
            it = lowerEffects.erase(it); // Erase element and get a valid iterator to the next element
        }
    }
    for (auto it = upperEffects.begin(); it != upperEffects.end();) {
        if (*it == nullptr) {
            ++it;  // Skip null pointers
            continue;
        }
        
        if ((*it)->isActive()) {
            (*it)->Update(deltaTime);
            ++it;  // Move to the next element
        } else {
            delete *it;            // Free memory
            it = upperEffects.erase(it); // Erase element and get a valid iterator to the next element
        }
    }

}

void EffectManager::DrawLower()
{
    for (auto* effect : lowerEffects) {
        if (effect == nullptr) continue;
        effect->Draw();
    }
}

void EffectManager::DrawUpper()
{
    for (auto* effect : upperEffects) {
        if (effect == nullptr) continue;
        effect->Draw();
    }
}