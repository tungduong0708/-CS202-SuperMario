#include "background.h"
#include "raylib.h"
#include "renderer.h"
#include "physics.h"

Background::Background(const Texture2D &texture, const Vector2 &position, int numRepeated) : texture(texture), position(position), numRepeated(numRepeated)
{
}

Background::~Background()
{
}

void Background::Update(Vector2 playerVelocity, float deltaTime)
{
    scrollingX -= playerVelocity.x * 0.2 * deltaTime;
    if (scrollingX < -texture.width) scrollingX = 0.0f;
    position.x = scrollingX;
}

void Background::Draw()
{
    for (int i = 0; i < numRepeated; i++) {
        DrawTextureEx(texture, position, 0.0f, 1.0f, WHITE);
        position.x += texture.width;
    }

}
