#ifndef STATIC_OBJECT_H
#define STATIC_OBJECT_H

#include "scene_node.h"
#include "physics.h"
#include <string>
#include <box2d/box2d.h>

class StaticObject : public SceneNode {
private:
    b2Body* body;
    std::string type;
public:
    StaticObject();
    StaticObject(b2Body* body);
    StaticObject(std::string type);;
    StaticObject(const StaticObject& other);
    virtual ~StaticObject();

    virtual StaticObject* clone();
    virtual std::string getType();
    virtual b2Body* GetBody();
    virtual Vector2 getPosition();

    void SetBody(b2Body* body);
    void SetType(std::string type);

    virtual void Update(Vector2 playerVelocity, float deltaTime);
    virtual void Draw();
    virtual void OnBeginContact(SceneNode* other, b2Vec2 normal);
    virtual void OnEndContact(SceneNode* other);
};

class Gate : public StaticObject {
private:
    std::string addressNext;
public:
    Gate(b2Body* body, std::string addressNext);
    virtual ~Gate() = default;

    std::string getAddressNext() const;
    void Update(Vector2 playerVelocity, float deltaTime);
    void Draw();
    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);
};

class DeadLine : public StaticObject {
public:
    DeadLine(b2Body* body);
    virtual ~DeadLine() = default;

    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);
};

#endif // STATIC_OBJECT_H