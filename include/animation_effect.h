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

class MushroomEffect : public AnimationEffect {
private:
    float distance = 1.0f;
    Vector2 currentPosition;
    Vector2 size;
    Animation animation;
public:
    MushroomEffect(Vector2 pos);
    ~MushroomEffect() = default;
    void Update(float deltaTime);
    void Draw();
};

class StarEffect : public AnimationEffect {
private:
    float distance = 1.0f;
    Vector2 currentPosition;
    Vector2 size;
    Animation animation;
public:
    StarEffect(Vector2 pos);
    ~StarEffect() = default;
    void Update(float deltaTime);
    void Draw();
};

class FireFlowerEffect : public AnimationEffect {
private:
    float distance = 1.0f;
    Vector2 currentPosition;
    Vector2 size;
    Animation animation;
public:
    FireFlowerEffect(Vector2 pos);
    ~FireFlowerEffect() = default;
    void Update(float deltaTime);
    void Draw();
};

class FireballExplodeEffect : public AnimationEffect {
private:
    float alpha = 1.0f;
    float fadeTime = 0.0f;
    float fadeDuration = 0.1f;
    float scale = 0.5f;
    Vector2 currentPosition;
    Vector2 size;
    Animation animation;
public:
    FireballExplodeEffect(Vector2 pos);
    ~FireballExplodeEffect() = default;
    void Update(float deltaTime);
    void Draw();
};

class BrickExplodeEffect : public AnimationEffect {
private:
    Vector2 position;
    Vector2 size;
    Animation animation;
    bool isCreated = false;
    bool isApplyingForce = false;
    b2Body* pieces[4];
public:
    BrickExplodeEffect(Vector2 pos);
    ~BrickExplodeEffect() = default;
    void Update(float deltaTime);
    void Draw();
};

class DeadMarioEffect : public AnimationEffect {
private:
    Vector2 position;
    Vector2 size;
    Animation animation;
    b2Body* body = nullptr;
public:
    DeadMarioEffect(Vector2 pos);
    ~DeadMarioEffect() = default;
    void Update(float deltaTime);
    void Draw();
};

class DeadKoopaEffect : public AnimationEffect {
private:
    Vector2 position;
    Vector2 size;
    Animation animation;
    b2Body* body = nullptr;
public:
    DeadKoopaEffect(Vector2 pos);
    ~DeadKoopaEffect() = default;
    void Update(float deltaTime);
    void Draw();
};

class DeadGoombaEffect : public AnimationEffect {
private:
    Vector2 position;
    Vector2 size;
    Animation animation;
    b2Body* body = nullptr;
public:
    DeadGoombaEffect(Vector2 pos);
    ~DeadGoombaEffect() = default;
    void Update(float deltaTime);
    void Draw();
};

class GrowEffect : public AnimationEffect {
private:
    float effectTime = 1.5f;
    float totalTime = 0.0f;
    float appearTime = 0.3f;
    float elapsedTime = 0.0f;
    bool appear = true;
    Vector2 currentPostion;
    Vector2 position;
    Vector2 size;
    Animation animation;
public:
    GrowEffect(Vector2 pos);
    ~GrowEffect() = default;
    void Update(float deltaTime);
    void Draw();
};

class ShrinkEffect : public AnimationEffect {
private:
    float effectTime = 1.5f;
    float totalTime = 0.0f;
    float appearTime = 0.3f;
    float elapsedTime = 0.0f;
    bool appear = true;
    Vector2 currentPostion;
    Vector2 position;
    Vector2 size;
    Animation animation;
public:
    ShrinkEffect(Vector2 pos);
    ~ShrinkEffect() = default;
    void Update(float deltaTime);
    void Draw();
};

#endif // ANIMATION_EFFECT_H