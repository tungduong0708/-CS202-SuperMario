#include "effect_manager.h"

EffectManager::EffectManager(std::map<std::pair<int, int>, std::string> effectMap, std::map<std::pair<int, int>, int> effectCount) : effectMap(effectMap), effectCount(effectCount)
{
}

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

void EffectManager::AddEffectPosition(std::pair<int, int> pos, std::string name)
{
    effectMap[pos] = name;
}

void EffectManager::AddEffectCount(std::pair<int, int> pos, int count)
{
    effectCount[pos] = count;
}

void EffectManager::AddLowerEffect(AnimationEffect *effect)
{
    lowerEffects.push_back(effect);
}

void EffectManager::AddUpperEffect(AnimationEffect *effect)
{
    upperEffects.push_back(effect);
}

bool EffectManager::isActivePlayerEffect()
{
    return activePlayerEffect;
}

std::string EffectManager::GetEffectName(std::pair<int, int> pos)
{
    return effectMap[pos];
}

int EffectManager::GetEffectCount(std::pair<int, int> pos)
{
    return effectCount[pos];
}

std::map<std::pair<int, int>, std::string> EffectManager::getEffectMap()
{
    return effectMap;
}

std::map<std::pair<int, int>, int> EffectManager::getEffectCountMap()
{
    return effectCount;
}

void EffectManager::setActivePlayerEffect(bool active)
{
    activePlayerEffect = active;
}

bool EffectManager::UpdateEffectCount(std::pair<int, int> pos)
{
    effectCount[pos]--;
    if (effectCount[pos] == 0) {
        effectMap.erase(pos);
        return true;
    }
    return false;
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
            DeadMarioEffect* deadMario = dynamic_cast<DeadMarioEffect*>(*it);
            DeadLuigiEffect* deadLuigiEffect = dynamic_cast<DeadLuigiEffect*>(*it);
            GrowMarioEffect* growMarioEffect = dynamic_cast<GrowMarioEffect*>(*it);
            GrowLuigiEffect* growLuigiEffect = dynamic_cast<GrowLuigiEffect*>(*it);
            if (deadMario || deadLuigiEffect || growMarioEffect || growLuigiEffect) {
                activePlayerEffect = false;
            }
            delete *it;            // Free memory
            it = upperEffects.erase(it); // Erase element and get a valid iterator to the next element
        }
    }

}

void EffectManager::DrawLower() const
{
    for (auto* effect : lowerEffects) {
        if (effect == nullptr) continue;
        effect->Draw();
    }
}

void EffectManager::DrawUpper() const
{
    for (auto* effect : upperEffects) {
        if (effect == nullptr) continue;
        effect->Draw();
    }
}

void EffectManager::clearEffects()
{
    for (auto* effect : lowerEffects) {
        delete effect;
    }
    lowerEffects.clear();
    for (auto* effect : upperEffects) {
        delete effect;
    }
    upperEffects.clear();
    effectCount.clear();
    effectMap.clear();
}

void EffectManager::accept(FileVisitor *v)
{
    v->VisitFile(this);
}
