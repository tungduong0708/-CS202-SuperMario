#include "animation_effect.h"
#include "imagehandler.h"
#include "tilemap.h"
#include "moving_object.h"


bool AnimationEffect::isActive()
{
    return active;
}

CoinEffect::CoinEffect(Vector2 pos)
{
    animation = AnimationHandler::setAnimations("coin")[0];
    position = pos;
}

void CoinEffect::Update(float deltaTime)
{
    if (!active) return;
    fadeTime += deltaTime;
    if (fadeTime > fadeDuration) {
        float elapsed = fadeTime - fadeDuration;
        alpha = 1.0f -  10 * elapsed / fadeDuration;
        if (alpha <= 0.0f) active = false;
    }
    if (curHeight < height)
    {
        position.y -= deltaTime * 7;
        curHeight += deltaTime * 7;
    }    
    animation.Update(deltaTime);
    texture = animation.GetFrame();
    size = {(float)texture.width / texture.height, 1.0f};

    
}

void CoinEffect::Draw()
{
    if (!active) return;
    Color color = { 255, 255, 255, (unsigned char)(alpha * 255) };
    DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, (float)texture.height}, Rectangle{position.x, position.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, color);
}

MushroomEffect::MushroomEffect(Vector2 pos)
{
    animation = AnimationHandler::setAnimations("mushroom")[0];
    position = pos;
    currentPosition = pos;
    currentPosition.y += 1.0f;
}

void MushroomEffect::Update(float deltaTime)
{
    if (!active) return;
    if (currentPosition.y > position.y)
    {
        currentPosition.y -= deltaTime * 2;
    }
    else {
        active = false;
        Tilemap* tilemap = Tilemap::getInstance();
        Mushroom* mushroom = new Mushroom();
        mushroom->Init(b2Vec2{position.x, position.y}, ImageSet{});
        mushroom->setSpeed(1.0f);
        tilemap->addNode(mushroom);
    }
}

void MushroomEffect::Draw()
{
    if (!active) return;
    texture = animation.GetFrame();
    size = {(float)texture.width / texture.height, 1.0f};
    DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, (float)texture.height}, Rectangle{currentPosition.x, currentPosition.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}

StarEffect::StarEffect(Vector2 pos)
{
    animation = AnimationHandler::setAnimations("star")[0];
    position = pos;
    currentPosition = pos;
    currentPosition.y += 1.0f;
}

void StarEffect::Update(float deltaTime)
{
    if (!active) return;
    if (currentPosition.y > position.y)
    {
        currentPosition.y -= deltaTime * 2;
    }
    else {
        active = false;
        Tilemap* tilemap = Tilemap::getInstance();
        Star* star = new Star();
        star->Init(b2Vec2{position.x, position.y}, ImageSet{});
        star->setSpeed(1.0f);
        tilemap->addNode(star);
    }
}

void StarEffect::Draw() 
{
    if (!active) return;
    texture = animation.GetFrame();
    size = {(float)texture.width / texture.height, 1.0f};
    DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, (float)texture.height}, Rectangle{currentPosition.x, currentPosition.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}

FireFlowerEffect::FireFlowerEffect(Vector2 pos)
{
    animation = AnimationHandler::setAnimations("fireflower")[0];
    position = pos;
    currentPosition = pos;
    currentPosition.y += 1.0f;
}

void FireFlowerEffect::Update(float deltaTime)
{
    if (!active) return;
    if (currentPosition.y > position.y)
    {
        currentPosition.y -= deltaTime * 2;
    }
    else {
        active = false;
        Tilemap* tilemap = Tilemap::getInstance();
        FireFlower* fireFlower = new FireFlower();
        fireFlower->Init(b2Vec2{position.x, position.y}, ImageSet{});
        fireFlower->setSpeed(1.0f);
        tilemap->addNode(fireFlower);
    }
}

void FireFlowerEffect::Draw()
{
    if (!active) return;
    texture = animation.GetFrame();
    size = {(float)texture.width / texture.height, 1.0f};
    DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, (float)texture.height}, Rectangle{currentPosition.x, currentPosition.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}
