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

Vector2 Background::getPosition()
{
    return position;
}

void Background::Update(Vector2 playerVelocity, float deltaTime)
{
    scrollingX -= playerVelocity.x * 0.2 * deltaTime;
    if (scrollingX < -texture.width) scrollingX = 0.0f;
    position.x = scrollingX;
}

void Background::Draw()
{
    position.x -= texture.width;
    for (int i = 0; i < numRepeated + 1; i++) {
        DrawTextureEx(texture, position, 0.0f, 1.0f, WHITE);
        position.x += texture.width;
    }

}

void Background::OnBeginContact(SceneNode* other, b2Vec2 normal)
{
}

void Background::OnEndContact(SceneNode* other)
{
}

void Background::accept(FileVisitor *visitor) {
    
}
