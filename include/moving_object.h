#ifndef MOVING_OBJECT_H
#define MOVING_OBJECT_H

#include "include.h"
#include "scene_node.h"
#include "imagehandler.h"
#include "file_visitor.h"
#include <box2d/box2d.h>
// Forward declaration
class FireBall;
class Bullet;

class MovingObject : public SceneNode {
protected:
    Vector2 size; // size of the object
    Vector2 bodySize;  // size of the body
    float speed;  // max speed that the object can move
    float angle;  // initial angle of the object when moving
    float density; // density of the object, can determine the mass of the object
    vector<Animation> animations;
    float elapsedTime;
    float frameTime;
    b2Body* body = nullptr; 
    float restitution;
public:
    MovingObject();
    MovingObject(Vector2 size, float speed, float angle);
    MovingObject(const MovingObject &mo);
    virtual ~MovingObject();
    
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

    virtual void accept(FileVisitor* visitor) = 0;
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
    FireBall(float d, Vector2 size = {0, 0}, float s = 0, float a = 0);
    FireBall(const FireBall &ff);
    virtual ~FireBall();
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

    void accept(FileVisitor* visitor);
    MovingObject* copy() const;
};


class AttackBall : public MovingObject {
private:
    float damage;
    float span; // time span of the attackball
    bool flag; // indicate that the attackball is active or not

    // other attributes are inherited from the moving object class ---
public:
    AttackBall();
    AttackBall(float d, Vector2 size = {0, 0}, float s = 0, float a = 0);
    AttackBall(const AttackBall &ff);
    virtual ~AttackBall();
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
    void OnBeginContact(SceneNode* other, b2Vec2 normal);  
    void OnEndContact(SceneNode* other);

    void accept(FileVisitor* visitor);
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
    virtual ~ActiveItem();
    void setValue(int v);
    void setFlag(bool f);
    bool isAvailable();
    int getValue();

    virtual void Init(b2Vec2 position);
    virtual void Draw();
    virtual void Draw(Vector2 position, float angle = 0.0f);
    virtual void Update(Vector2 playerVelocity, float deltaTime);
    virtual void HandleInput();
    virtual void OnBeginContact(SceneNode* other, b2Vec2 normal) = 0;
    virtual void OnEndContact(SceneNode* other) = 0;

    virtual void accept(FileVisitor* visitor) = 0;
    virtual MovingObject* copy() const = 0;
};

class Mushroom : public ActiveItem {
public:
    Mushroom();
    Mushroom(int value, Vector2 size = {0, 0}, float speed = 0, float angle = 0);
    Mushroom(const Mushroom &m);
    virtual ~Mushroom();

    void Init(b2Vec2 position);
    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);

    void accept(FileVisitor* visitor);
    MovingObject* copy() const;
};

class FireFlower : public ActiveItem {
public:
    FireFlower();
    FireFlower(int value, Vector2 size = {0, 0}, float speed = 0, float angle = 0);
    FireFlower(const FireFlower &ff);
    virtual ~FireFlower();

    void Init(b2Vec2 position);
    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);

    void accept(FileVisitor* visitor);
    MovingObject* copy() const;
};

class Star : public ActiveItem {
public:
    Star();
    Star(int value, Vector2 size = {0, 0}, float speed = 0, float angle = 0);
    Star(const Star &s);
    virtual ~Star();

    void Init(b2Vec2 position);
    void Update(Vector2 playerVelocity, float deltaTime);
    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);

    void accept(FileVisitor* visitor);
    MovingObject* copy() const;
};

class MovingPlatform : public MovingObject {
private:
    Vector2 speed;
    float distance;
    float curDistance = 0;
    float direction;  
    string type;    

    Vector2 orbitCenter; 
    float orbitRadius;  
    float orbitAngle;   
    float orbitSpeed;    
public:
    MovingPlatform();
    MovingPlatform(Vector2 size, Vector2 speed, float distance, float angle, string _type);
    MovingPlatform(const MovingPlatform &mp);
    virtual ~MovingPlatform();

    void Init(b2Vec2 position) override;
    void InitOrbit(Vector2 center, float radius, float speed);
    void Update(Vector2 playerVelocity, float deltaTime) override;
    void HandleInput() override;
    void OnBeginContact(SceneNode *other, b2Vec2 normal) override;
    void OnEndContact(SceneNode *other) override;
    void Draw() override;
    void Draw(Vector2 position, float angle = 0.0f) override;
    void SetorbitSpeed(float speed){
        orbitSpeed = speed;
    }
    void SetorbitRadius(float speed){
        orbitRadius = speed;
    }
    void SetorbitCenter(Vector2 center){
        orbitCenter = center;
    }

    void accept(FileVisitor* visitor) override;
    MovingObject* copy() const override;
};

class Flag : public MovingObject {
public:
    Flag();
    Flag(const Flag &f);
    virtual ~Flag();

    void setPosition(Vector2 position);
    void Init(b2Vec2 position);
    void Update(Vector2 playerVelocity, float deltaTime);
    void HandleInput();
    void OnBeginContact(SceneNode *other, b2Vec2 normal);
    void OnEndContact(SceneNode *other);
    void Draw();
    void Draw(Vector2 position, float angle);

    void accept(FileVisitor* visitor);
    MovingObject* copy() const;
};

class Axe : public MovingObject {
private:
    std::vector<StaticTile*> tiles;
    Texture2D texture;
    bool activating = false;
    bool activated = false;
    float animationTime = 2.0f;
    float elapsedTime = 0.0f;
public:
    Axe();
    Axe(const Axe &a);
    virtual ~Axe() = default;

    void AddBridgeTile(StaticTile* tile);
    void Init(b2Vec2 position);
    void Update(Vector2 playerVelocity, float deltaTime);
    void HandleInput();
    void OnBeginContact(SceneNode *other, b2Vec2 normal);
    void OnEndContact(SceneNode *other);
    void Draw();
    void Draw(Vector2 position, float angle);

    void accept(FileVisitor* visitor);
    MovingObject* copy() const;
};

#endif
