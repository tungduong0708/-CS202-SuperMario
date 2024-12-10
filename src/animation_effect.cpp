#include "animation_effect.h"
#include "imagehandler.h"

const int TILE_SIZE = 16;

CoinEffect::CoinEffect(Vector2 position)
{
    this->animation = AnimationHandler::setAnimations("coin")[0];
    this->position = position;
}

void CoinEffect::Update(float deltaTime)
{
    if (!active) return;
    fadeTime += deltaTime;
    if (fadeTime > fadeDuration) {
        float fadeElapsed = fadeTime - fadeDuration;
        alpha -= 2 * fadeElapsed / fadeDuration;
        std::cout << fadeTime << " " << fadeDuration << " " << fadeElapsed << std::endl;
        std::cout << alpha << std::endl;
        if (fadeElapsed >= 0.2) alpha = 0.0f;
        if (alpha <= 0.0f) active = false;
    }
    position.y -= deltaTime / 50;
    animation.Update(deltaTime);
    texture = animation.GetFrame();
    texture.width /= texture.height - 6;
    texture.height /= texture.height;
    
}

void CoinEffect::Draw()
{
    if (!active) return;
    Color color = { 255, 255, 255, (unsigned char)(alpha * 255) };
    DrawTexture(texture, position.x, position.y, color);
}

bool AnimationEffect::isActive()
{
    return active;
}
