#ifndef STATIC_OBJECT_H
#define STATIC_OBJECT_H

#include "scene_node.h"
#include "physics.h"
#include <box2d/box2d.h>

class StaticObject : public SceneNode {
private:
    b2Body* body;
public:
    StaticObject();
    StaticObject(b2Body* body);
    ~StaticObject();

    virtual b2Body* GetBody();
    virtual Vector2 getPosition();

    void SetBody(b2Body* body);

    virtual void Update(Vector2 playerVelocity, float deltaTime);
    virtual void Draw();
    virtual void OnBeginContact(SceneNode* other);
    virtual void OnEndContact(SceneNode* other);
};

#endif // STATIC_OBJECT_H