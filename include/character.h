#pragma once
#include "include.h"
#include "scene_node.h"
#include "imagehandler.h"
#include "file_visitor.h"
#include "moving_object.h"

#ifndef CHARACTER_H
#define CHARACTER_H

// Forward declaration
class FireBall;
class Bullet;

class Character : public MovingObject {
protected:
    int health;
    int score;
    int level;
    int strength;
    int colorIndex = 0;
    int frameWidth, frameHeight;
    int currentFrame;

    bool alive;
    bool appear = true;
    bool invincible = false;
    bool immortal;
    bool isOnGround;          // Is character on the ground
    bool modeChanged;
    bool faceLeft;            // Is character facing left

    float blinkTime;
    float immortalTime;
    float appearTimer = 0.0f;
    float colorChangeTimer = 0.0f; 
    float frameTime, frameSpeed;

    string type;
    Mode mode;
    Texture texture;            // Character texture
    Animation curAnim;          // Character sprite sheet              
    Rectangle sourceRect;       // Sprite frame to draw
    Rectangle destRect;         // Scaled drawing rectangle
    Vector2 origin;             // Sprite origin
    Vector2 position;
    ImageSet currentImage;
    ImageSet previousImage;

public:
    Character();
    Character(string type, int health = 0, int score = 0, int level = 0, int strength = 0, Vector2 size = {0, 0}, 
              float speed = 0, float angle = 0);
    Character(const Character &c);
    virtual ~Character();

    void setCurrentImage(ImageSet image);
    void setHealth(int h);
    void setScore(int s);
    void setLevel(int l);
    void setStrength(int st);
    void setMode(Mode mode);
    void setOnGround(bool og);
    void setAppear(bool ap);
    void setInvisibleTime(float it);
    void changeMode(Mode mode);

    int getHealth();
    int getScore();
    int getLevel();
    int getStrength();
    bool onGround();
    bool isLeft();
    bool isAlive();
    bool isInvisible();
    string getType();
    Mode getMode();

    virtual void Init(b2Vec2 position);  
    virtual void UpdateMode(Mode mode, b2Vec2 position);
    virtual void Update(Vector2 playerVelocity, float deltaTime);
    virtual void Draw();  
    virtual void Draw(Vector2 position, float angle = 0.0f) = 0;
    virtual void Dead();
    virtual void OnBeginContact(SceneNode* other);
    virtual void OnEndContact(SceneNode* other);
    virtual void HandleInput();

    virtual void accept(FileVisitor* visitor) = 0;
    virtual MovingObject* copy() const = 0;
};

class Player : public Character {
private:
    int lives;
    int addScore = 0;
    float coins;
    float time; // time allotted for the player to complete the map
    float force;
    float bulletSpeed;
    float bulletFreq;
    bool isWalkingOnPlatform;
    bool allowInput; // allow to handle input
    string name;
    string currentMap;
    Vector2 initialPosition;
public:
    Player();
    Player(string type, string name = "", float coins = 0.0f, int lives = 3, int health = 100, 
          int score = 0, int level = 0, int strength = 0, Vector2 size = {0, 0}, float speed = 0.0f, float angle = 0.0f);
    Player(const Player &p);
    virtual ~Player();

    void setAddScore(int s);
    void setPositon(b2Vec2 pos);
    void setPositionBody(b2Vec2 pos);
    void setName(string n);
    void setCoins(float c);
    void setLives(int lives);
    void setImmortal(bool im);
    void setImmortalTime(float it);
    void setCurrentMap(string map);
    void setTime(float t);
    void setForce(float f);
    void setBulletSpeed(float bs);
    void setBulletFreq(float bf);
    void setInitialPosition(Vector2 pos);
    void setAllowInput(bool state);
    void impulseForce(Vector2 force);
    void updateScore(int s);
    void updateScore();
    void SetIsOnGround(bool state);
    void SetWalkingOnPlatform(bool state);

    int getAddScore();
    int getLives();
    float getCoins();
    float getForce();
    float getBulletSpeed();
    float getBulletFreq();
    float getTime();
    bool isImmortal();
    bool isAllowInput();
    Vector2 getInitialPosition();
    string getCurrentMap();
    string getName();

    void Init(b2Vec2 position);
    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);
    void HandleInput();
    void Update(Vector2 playerVelocity, float deltaTime);
    void Dead();
    void UpdateAnimation();
    void Draw();
    void Draw(Vector2 position, float angle = 0.0f);

    void accept(FileVisitor* visitor);
    MovingObject* copy() const;
};


class Enemy : public Character {
protected:
    float range;
    bool fixtureChange;
    bool deadByPlayer;
    bool deadByFireball;
    bool isBodyChanged;
    string type;
    EnemyState state;
public:
    Enemy();
    Enemy(string type, float range = 0, bool alive = true, int health = 0, int score = 0, int level = 0, int strength = 0, 
          Vector2 size = {0, 0}, float speed = 0.0f, float angle = 0.0f);
    Enemy(const Enemy &e);
    virtual ~Enemy();
    void setType(string t);
    void setRange(float r);
    void setIsAlive(bool ia);
    void setState(EnemyState s);

    string getType();
    EnemyState getState();
    float getRange();

    virtual void Init(b2Vec2 position);  
    virtual void Update(Vector2 playerVelocity, float deltaTime);
    virtual void OnBeginContact(SceneNode* other, b2Vec2 normal);
    virtual void OnEndContact(SceneNode* other);
    virtual void HandleInput();
    virtual void Dead();
    virtual void Draw();
    virtual void Draw(Vector2 position, float angle = 0.0f);

    virtual void accept(FileVisitor* visitor) = 0;
    virtual MovingObject* copy() const = 0;
};


class Goomba : public Enemy {
public:
    Goomba();
    Goomba(string type, float range = 0, bool alive = true, bool sit = false, int health = 0, int score = 0, 
           int level = 0, int strength = 0, Vector2 size = {0, 0}, float speed = 0.0f, float angle = 0.0f);
    Goomba(const Goomba &g);
    virtual ~Goomba();

    void Dead();
    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);

    void accept(FileVisitor* visitor);
    MovingObject* copy() const;
};

class Koopa : public Enemy {
private:
    float delay;
    bool isDelay;
public:
    Koopa();
    Koopa(string type, float range = 0, bool alive = true, bool sit = false, int health = 0, int score = 0, 
          int level = 0, int strength = 0, Vector2 size = {0, 0}, float speed = 0.0f, float angle = 0.0f);
    Koopa(const Koopa &k);
    virtual ~Koopa();

    void Dead();
    void Update(Vector2 playerVelocity, float deltaTime);
    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);

    void accept(FileVisitor* visitor);
    MovingObject* copy() const; 
}; 


class Boss: public Enemy {
private:
    float bulletFreq;
    float bulletSpeed;
    float timer; // total time of 3 attack frames
    bool attackFire;
    BossState bossState;
public:
    Boss();
    Boss(string type, float range = 0, bool alive = true, int health = 0, int score = 0, int level = 0, 
         int strength = 0, Vector2 size = {0, 0}, float speed = 0.0f, float angle = 0.0f);
    Boss(const Boss &b);
    virtual ~Boss();

    void setBulletFreq(float bf);
    void setBulletSpeed(float bs);
    void setBossState(BossState bs);

    BossState getBossState();
    float getBulletFreq();
    float getBulletSpeed();

    void Init(b2Vec2 position);
    void Update(Vector2 playerVelocity, float deltaTime);
    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);
    void Dead();

    void accept(FileVisitor* visitor);
    MovingObject* copy() const;
};

class LarvaBubble : public Enemy {
public:
    LarvaBubble();
    LarvaBubble(string type, float range = 0, bool alive = true, int health = 0, int score = 0, int level = 0, 
                int strength = 0, Vector2 size = {0.6f, 0.6f}, float speed = -15.0f, float angle = 0.0f, float gravity = 15.0f, float waitTime = 1.0f);
    LarvaBubble(const LarvaBubble &lb);
    virtual ~LarvaBubble();

    void Init(b2Vec2 position);
    void Explode();
    void Splash();
    void Update(Vector2 playerVelocity, float deltaTime);
    void OnBeginContact(SceneNode *other, b2Vec2 normal);
    void OnEndContact(SceneNode *other);

    void accept(FileVisitor *visitor);
    MovingObject* copy() const;

    void Draw();

private:
    float gravity;
    float initialSpeed;
    float waitTime;
    float elapsedTime = 0.0f;
};


class MonsterFlower : public Enemy {
private:
    float waitTime;
    float elapsedTime = 0.0f;
    float delayTime;
    Texture2D pipe;
    b2Vec2 initialPosition;
public:
    MonsterFlower();
    MonsterFlower(string type, float range = 0, bool alive = true, int health = 0, int score = 0, int level = 0, 
                  int strength = 0, Vector2 size = {1.0f, 1.0f}, float speed = -5.0f, float angle = 0.0f, float waitTime = 1.0f, float delayTime = 0.5f);
    MonsterFlower(const MonsterFlower &mf);
    virtual ~MonsterFlower();

    void Init(b2Vec2 position);
    void Update(Vector2 playerVelocity, float deltaTime);
    void OnBeginContact(SceneNode *other, b2Vec2 normal);
    void OnEndContact(SceneNode *other);

    void accept(FileVisitor *visitor);
    MovingObject* copy() const;

    void Draw();
};

#endif
