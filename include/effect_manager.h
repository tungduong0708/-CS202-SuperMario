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
public:
    static std::map<std::pair<int, int>, std::string> effectMap; // Map effect id to effect name
    static std::vector<AnimationEffect*> lowerEffects; // Manage all effects
    static std::vector<AnimationEffect*> upperEffects; // Manage all effects

    ~EffectManager();
    static void AddLowerEffect(AnimationEffect* effect);
    static void AddUpperEffect(AnimationEffect* effect);
    static void Update(float deltaTime);
    static void DrawLower();
    static void DrawUpper();
};

#endif // EFFECT_MANAGER_H
