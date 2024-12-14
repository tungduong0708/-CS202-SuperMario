#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "animation_effect.h"
#include <vector>
#include <map>
#include <utility>
#include <string>
#include <raylib.h>


class EffectManager {
private:
    std::map<std::pair<int, int>, std::string> effectMap; // Map effect position
    std::map<std::pair<int, int>, int> effectCount;
    std::vector<AnimationEffect*> lowerEffects; // Manage all effects
    std::vector<AnimationEffect*> upperEffects; // Manage all effects
public:
    ~EffectManager();
    void AddEffectPosition(std::pair<int, int> pos, std::string name);
    void AddEffectCount(std::pair<int, int> pos, int count);
    void AddLowerEffect(AnimationEffect* effect);
    void AddUpperEffect(AnimationEffect* effect);
    std::string GetEffectName(std::pair<int, int> pos);
    int GetEffectCount(std::pair<int, int> pos);
    bool UpdateEffectCount(std::pair<int, int> pos);
    void Update(float deltaTime);
    void DrawLower() const;
    void DrawUpper() const;
    void clearEffects();
};

#endif // EFFECT_MANAGER_H
