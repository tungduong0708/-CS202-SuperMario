#pragma once
#include "include.h"
#include "scene_node.h"
#include "imagehandler.h"
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
    bool alive;
    string type;
    Mode mode;

    Texture texture;            // Character texture
    Animation curAnim;          // Character sprite sheet              
    Rectangle sourceRect;       // Sprite frame to draw
    Rectangle destRect;         // Scaled drawing rectangle
    Vector2 origin;             // Sprite origin
    Vector2 position;
    bool appear = true;
    bool invincible = false;
    bool immortal;
    float blinkTime;
    float immortalTime;
    float appearTimer = 0.0f;
    float colorChangeTimer = 0.0f; 
    int colorIndex = 0;
    int frameWidth, frameHeight;
    int currentFrame;
    float frameTime, frameSpeed;
    bool isOnGround;          // Is character on the ground
    bool modeChanged;
    ImageSet currentImage;
    ImageSet previousImage;
    bool faceLeft;            // Is character facing left
public:
    Character();
    Character(string type, int health = 0, int score = 0, int level = 0, int strength = 0, Vector2 size = {0, 0}, 
              float speed = 0, float angle = 0);
    Character(const Character &c);
    virtual ~Character();

    void setHealth(int h);
    void setScore(int s);
    void setLevel(int l);
    void setStrength(int st);
    void setMode(Mode mode);
    void setOnGround(bool og);
    void setInvisibleTime(float it);

    int getHealth();
    int getScore();
    int getLevel();
    int getStrength();
    bool onGround();
    bool isLeft();
    bool isAlive();
    void changeMode(Mode mode);
    Mode getMode();
    bool isInvisible();

    // default image = IDLE
    virtual void Init(b2Vec2 position);  
    virtual void UpdateMode(Mode mode, b2Vec2 position);
    virtual void Update(Vector2 playerVelocity, float deltaTime);
    virtual void Draw();  
    virtual void Draw(Vector2 position, float angle = 0.0f) = 0;
    virtual void Dead();
    void ResizeBody(float newWidth, float newHeight);
    virtual void OnBeginContact(SceneNode* other);
    virtual void OnEndContact(SceneNode* other);
    virtual void HandleInput();
    virtual MovingObject* copy() const = 0;
};

class Player : public Character {
private:
    string name;
    float coins;
    int lives;
    int addScore = 0;
    string currentMap;
    float time; // time allotted for the player to complete the map
    float force;
    float bulletSpeed;
    float bulletFreq;
    Vector2 initialPosition;
public:
    Player();
    Player(string type, string name = "", float coins = 0.0f, int lives = 0, int health = 0, 
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
    void impulseForce(Vector2 force);
    void updateScore(int s);
    void updateScore();


    int getAddScore();
    string getName();
    float getCoins();
    float getForce();
    float getBulletSpeed();
    float getBulletFreq();
    string getCurrentMap();
    float getTime();
    bool isImmortal();

    void Init(b2Vec2 position);
    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);
    void HandleInput();
    void Update(Vector2 playerVelocity, float deltaTime);
    void Dead();
    void UpdateAnimation();
    void Draw();
    void Draw(Vector2 position, float angle = 0.0f);
    MovingObject* copy() const;
};


class Enemy : public Character {
protected:
    string type;
    float range;
    EnemyState state;
    bool fixtureChange;
    bool deadByPlayer;
    bool deadByFireball;
    bool isBodyChanged;
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
    float getRange();
    EnemyState getState();

    virtual void Init(b2Vec2 position);  
    virtual void Update(Vector2 playerVelocity, float deltaTime);
    virtual void OnBeginContact(SceneNode* other, b2Vec2 normal);
    virtual void OnEndContact(SceneNode* other);
    virtual void HandleInput();
    virtual void Dead();
    virtual void Draw();
    virtual void Draw(Vector2 position, float angle = 0.0f);

    MovingObject* copy() const;
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
    MovingObject* copy() const; 
}; 


class Boss: public Enemy {
private:
    float bulletFreq;
    float bulletSpeed;
    bool attackFire;
    BossState bossState;
    float timer; // total time of 3 attack frames
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
    // void Draw();
    // void Draw(Vector2 position, float angle = 0.0f);
    MovingObject* copy() const;
};

#endif
