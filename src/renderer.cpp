#include "renderer.h"

void Renderer::Draw(Texture2D texture, const Vector2& position, const Vector2& size) {
    Vector2 scale = { size.x / texture.width, size.y / texture.height };
    texture.width = size.x * scale.x;
    texture.height = size.y * scale.y;

    DrawTexture(texture, position.x, position.y, WHITE);
}

void Renderer::DrawPro(Texture2D texture, const Rectangle &srcRect, const Vector2& position, const Vector2 &size)
{
    Rectangle destRect = { position.x, position.y, size.x, size.y };
    DrawTexturePro(texture, srcRect, destRect, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}
