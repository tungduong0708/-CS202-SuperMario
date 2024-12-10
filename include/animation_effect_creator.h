#ifndef ANIMATION_EFFECT_CREATOR_H
#define ANIMATION_EFFECT_CREATOR_H

#include "animation_effect.h"
#include <raylib.h>

class AnimationEffectCreator {
private:
    static std::unordered_map<std::string, std::function<AnimationEffect*(Vector2)>> creators;
public:
    AnimationEffectCreator() = default;
    ~AnimationEffectCreator() = default;
    static void RegisterEffect(const std::string& name, std::function<AnimationEffect*(Vector2)> creator);
    static void InitEffects();
    static AnimationEffect* CreateAnimationEffect(const std::string& name, Vector2 position);
};

#endif // ANIMATION_EFFECT_CREATOR_H