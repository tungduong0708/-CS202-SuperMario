#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "scene_node.h"
#include "raylib.h"
#include <string>

class Background : public SceneNode {
private:
    Texture2D texture;
    Vector2 position;
    int numRepeated;
    float scrollingX = 0.0f;
public:
    Background(const Texture2D& texture, const Vector2& position, int numRepeated);
    ~Background();

    void Update(Vector2 playerPosition, float deltaTime);
    void Draw();
};

#endif // BACKGROUND_H