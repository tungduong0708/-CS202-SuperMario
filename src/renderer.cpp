#include "renderer.h"

void Renderer::Draw(Texture2D texture, const Vector2& position, const Vector2& size) {
    Vector2 scale = { size.x / texture.width, size.y / texture.height };
    texture.width = size.x * scale.x;
    texture.height = size.y * scale.y;

    DrawTexture(texture, position.x, position.y, WHITE);
}

void Renderer::DrawPro(Texture2D texture, const Rectangle &srcRect, const Vector2& position, const Vector2 &size, bool isLeft, float rotation) {
    Rectangle destRect = { position.x, position.y, size.x, size.y };
    Rectangle sourceRect = srcRect;
    if (!isLeft) {
        sourceRect.width *= -1; // Flip horizontally
        sourceRect.x += srcRect.width; // Adjust the x position to flip correctly
    }
    DrawTexturePro(texture, sourceRect, destRect, Vector2{0.0f, 0.0f}, rotation, WHITE);
}

// void Renderer::DrawPro2(Texture2D texture, const Rectangle &srcRect, const Vector2& position, const Vector2 &size, bool isLeft) {
//     // Destination rectangle where the texture will be drawn, resized to 'size'
//     Rectangle destRect = { position.x, position.y, size.x, size.y };

//     // Calculate the aspect ratio of the source and destination rectangles
//     float srcAspectRatio = srcRect.width / srcRect.height;
//     float destAspectRatio = size.x / size.y;

//     // Adjust the source rectangle to maintain the aspect ratio
//     Rectangle sourceRect = srcRect;
//     if (srcAspectRatio > destAspectRatio) {
//         // Source is wider than destination, adjust width
//         sourceRect.width = srcRect.height * destAspectRatio;
//         sourceRect.x += (srcRect.width - sourceRect.width) / 2;
//     } else if (srcAspectRatio < destAspectRatio) {
//         // Source is taller than destination, adjust height
//         sourceRect.height = srcRect.width / destAspectRatio;
//         sourceRect.y += (srcRect.height - sourceRect.height) / 2;
//     }

//     // Flip the source rectangle horizontally if 'isLeft' is false
//     if (!isLeft) {
//         sourceRect.width *= -1; // Flip horizontally
//         sourceRect.x += srcRect.width; // Adjust the x position to flip correctly
//     }

//     // Draw the texture with the specified source and destination rectangles
//     DrawTexturePro(texture, sourceRect, destRect, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
// }
