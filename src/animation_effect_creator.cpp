#include "animation_effect_creator.h"
#include "animation_effect.h"

std::unordered_map<std::string, std::function<AnimationEffect*(Vector2)>> AnimationEffectCreator::creators;

void AnimationEffectCreator::RegisterEffect(const std::string &name, std::function<AnimationEffect *(Vector2)> creator)
{
    creators[name] = creator;
}

void AnimationEffectCreator::InitEffects()
{
    RegisterEffect("score", [](Vector2 position) {
        return new ScoreEffect(position);
    });
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
    RegisterEffect("brick_explode", [](Vector2 position) {
        return new BrickExplodeEffect(position);
    });
    RegisterEffect("dead_mario", [](Vector2 position) {
        return new DeadMarioEffect(position);
    });
    RegisterEffect("dead_luigi", [](Vector2 position) {
        return new DeadLuigiEffect(position);
    });
    RegisterEffect("dead_koopa", [](Vector2 position) {
        return new DeadKoopaEffect(position);
    });
    RegisterEffect("dead_goomba", [](Vector2 position) {
        return new DeadGoombaEffect(position);
    });
    RegisterEffect("dead_boss", [](Vector2 position) {
        return new DeadBossEffect(position);
    });
    RegisterEffect("squash_dead_goomba", [](Vector2 position) {
        return new SquashDeadGoombaEffect(position);
    });
    RegisterEffect("grow_mario", [](Vector2 position) {
        return new GrowMarioEffect(position);
    });
    RegisterEffect("grow_luigi", [](Vector2 position) {
        return new GrowLuigiEffect(position);
    });
}

AnimationEffect *AnimationEffectCreator::CreateAnimationEffect(const std::string& name, Vector2 position)
{
    if (creators.find(name) != creators.end()) {
        return creators[name](position); 
    }
    return nullptr; 
}
