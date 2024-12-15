#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"

class Renderer {
private:
public:
    static void Draw(Texture2D texture, const Vector2& position, const Vector2& size);
    static void DrawPro(Texture2D texture, const Rectangle& srcRect, const Vector2& position, const Vector2& size, bool isLeft, float rotation = 0.0f, Vector2 bodySize = {-1, -1}, Color color = WHITE);
};

#endif // RENDERER_H