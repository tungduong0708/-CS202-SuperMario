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
    float height = 1.0f;
    float curHeight = 0.0f;
    float fadeTime = 0.0f;
    float fadeDuration = 0.6f;
    float alpha = 1.0f;
    Vector2 size;
    Animation animation;
public:
    CoinEffect(Vector2 pos);
    ~CoinEffect() = default;
    void Update(float deltaTime);
    void Draw();
};

#endif // ANIMATION_EFFECT_H