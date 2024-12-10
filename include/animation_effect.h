#ifndef ANIMATION_EFFECT_H
#define ANIMATION_EFFECT_H

#include "animation.h"
#include <utility>
#include <string>
#include <vector>
#include <raylib.h>

class AnimationEffect {
protected:
    bool active = true;
    Vector2 position;
    Texture2D texture;
public:    
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;
    virtual bool isActive();
};

class CoinEffect : public AnimationEffect {
private:
    float fadeTime = 0.0f;
    float fadeDuration = 1.0f;
    float alpha = 1.0f;
    Animation animation;
public:
    CoinEffect(Vector2 position);
    ~CoinEffect() = default;
    void Update(float deltaTime);
    void Draw();
};

#endif // ANIMATION_EFFECT_H