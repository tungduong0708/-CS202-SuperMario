#include "animation_effect_creator.h"
#include "animation_effect.h"

std::unordered_map<std::string, std::function<AnimationEffect*(Vector2)>> AnimationEffectCreator::creators;

void AnimationEffectCreator::RegisterEffect(const std::string &name, std::function<AnimationEffect *(Vector2)> creator)
{
    creators[name] = creator;
}

void AnimationEffectCreator::InitEffects()
{
    RegisterEffect("coin", [](Vector2 position) {
        return new CoinEffect(position);
    });
    RegisterEffect("mushroom", [](Vector2 position) {
        return new MushroomEffect(position);
    });
    RegisterEffect("star", [](Vector2 position) {
        return new StarEffect(position);
    });
    RegisterEffect("fireflower", [](Vector2 position) {
        return new FireFlowerEffect(position);
    });
    RegisterEffect("fireball_explode", [](Vector2 position) {
        return new FireballExplodeEffect(position);
    });
}

AnimationEffect *AnimationEffectCreator::CreateAnimationEffect(const std::string& name, Vector2 position)
{
    if (creators.find(name) != creators.end()) {
        return creators[name](position); 
    }
    return nullptr; 
}
