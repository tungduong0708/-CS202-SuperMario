#pragma once
#include "include.h"
#include "scene_node.h"
#include "imagehandler.h"
#include <box2d/box2d.h>

#ifndef MOVING_OBJECT_H
#define MOVING_OBJECT_H

// Forward declaration
class FireBall;
class Bullet;

class MovingObject : public SceneNode {
protected:
    Vector2 size; // size of the object
    float speed;  // max speed that the object can move
    float angle;  // initial angle of the object when moving
    float density; // density of the object, can determine the mass of the object
    vector<Animation> animations;
    float elapsedTime;
    float frameTime;
    b2Body* body; 
    float restitution;
public:
    MovingObject();
    MovingObject(Vector2 size, float speed, float angle);
    MovingObject(const MovingObject &mo);
    ~MovingObject();
    
    void setSize(Vector2 size);
    void setSpeed(float speed);
    void setAngle(float angle);
    void setDensity(float density);
    void setElapsedTime(float et);
    void setFrameTime(float ft);
    void setRestitution(float r);
    void setStable();

    float getElapsedTime();
    float getFrameTime();   
    Vector2 getSize();
    float getSpeed();
    float getAngle();
    float getDensity();
    float getRestitution();
    Vector2 getPosition();
    b2Vec2 getVelocity();
    b2Body* getBody();
    vector<Animation> getAnimations();
    
    // virtual functions
    virtual MovingObject* copy() const = 0;  // Prototype design pattern
    virtual void Update(Vector2 playerVelocity, float deltaTime) = 0;
    virtual void Draw() = 0;  
    virtual void Draw(Vector2 position, float angle = 0.0f) = 0;
    virtual void Init(b2Vec2 position) = 0;
    virtual void HandleInput() = 0;
    virtual void OnBeginContact(SceneNode* other, b2Vec2 normal) = 0;
    virtual void OnEndContact(SceneNode* other) = 0;
};

class FireBall : public MovingObject {
private:
    float damage;
    float span; // time span of the fireball
    bool flag; // indicate that the fireball is active or not

    // angle = initial angle of the fire flower
    // other attributes are inherited from the moving object class ---
public:
    FireBall();
    FireBall(float d = 0, Vector2 size = {0, 0}, float s = 0, float a = 0);
    FireBall(const FireBall &ff);
    ~FireBall();
    void setDamage(float d);
    void setFlag(bool f);
    float getDamage();
    bool isActive();
    Animation getAnimation(bool flag);

    void Init(b2Vec2 position);
    void Draw();
    void Draw(Vector2 position, float angle = 0.0f);
    void Update(Vector2 playerVelocity, float deltaTime);
    void HandleInput();
    void ReloadAnimation();
    void OnBeginContact(SceneNode* other, b2Vec2 normal);  
    void OnEndContact(SceneNode* other);

    MovingObject* copy() const;
};


class ActiveItem : public MovingObject {
private:
    int value;
    bool flag; // indicate that the grow item is disappear or not
public:
    ActiveItem();
    ActiveItem(int value, Vector2 size = {0, 0}, float speed = 0, float angle = 0);
    ActiveItem(const ActiveItem &gi);
    ~ActiveItem();
    void setValue(int v);
    bool isAvailable();
    int getValue();

    virtual void Init(b2Vec2 position);
    void Draw();
    void Draw(Vector2 position, float angle = 0.0f);
    void Update(Vector2 playerVelocity, float deltaTime);
    void HandleInput();
    virtual void OnBeginContact(SceneNode* other, b2Vec2 normal) = 0;
    virtual void OnEndContact(SceneNode* other) = 0;
};

class Mushroom : public ActiveItem {
public:
    Mushroom();
    Mushroom(int value, Vector2 size = {0, 0}, float speed = 0, float angle = 0);
    Mushroom(const Mushroom &m);
    ~Mushroom();

    void Init(b2Vec2 position);
    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);
    MovingObject* copy() const;
};

class FireFlower : public ActiveItem {
public:
    FireFlower();
    FireFlower(int value, Vector2 size = {0, 0}, float speed = 0, float angle = 0);
    FireFlower(const FireFlower &ff);
    ~FireFlower();

    void Init(b2Vec2 position);
    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);
    MovingObject* copy() const;
};

class Star : public ActiveItem {
public:
    Star();
    Star(int value, Vector2 size = {0, 0}, float speed = 0, float angle = 0);
    Star(const Star &s);
    ~Star();

    void Init(b2Vec2 position);
    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);
    MovingObject* copy() const;
};

#endif
