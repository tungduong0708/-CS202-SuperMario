#include "background.h"
#include "raylib.h"
#include "renderer.h"
#include "physics.h"
#include "tilemap.h"
#include <iostream>

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
    Tilemap *tilemap = Tilemap::getInstance();
    Camera2D camera = tilemap->getCamera();

    // Parallax effect: Background moves slower relative to the camera target
    float parallaxFactor = 0.3f; // Adjust this factor for the desired background speed
    scrollingX = camera.target.x * parallaxFactor;

    // Ensure seamless wrapping of the background texture
    if (scrollingX < -texture.width) scrollingX += texture.width;
    if (scrollingX > texture.width) scrollingX -= texture.width;

    // Update the position of the background
    position.x = -scrollingX;

}

void Background::Draw()
{
    position.x -= texture.width;
    for (int i = 0; i < numRepeated + 4; i++) {
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
