#include "animation_effect.h"
#include "imagehandler.h"

const int TILE_SIZE = 16;

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

bool AnimationEffect::isActive()
{
    return active;
}
