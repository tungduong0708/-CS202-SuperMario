#ifndef SCENE_NODE_H
#define SCENE_NODE_H
#include <raylib.h>

class SceneNode {
private:
public:
    SceneNode() = default;
    virtual ~SceneNode() = default;

    virtual void Update(Vector2 playerVelocity, float deltaTime) = 0;
    virtual void Draw() = 0;
    virtual void OnBeginContact(SceneNode* other) = 0;
    virtual void OnEndContact(SceneNode* other) = 0;
};

#endif // SCENE_NODE_H