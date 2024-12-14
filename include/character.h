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
              float spêed = 0, float angle = 0);
    Character(const Character &c);
    virtual ~Character();

    void setHealth(int h);
    void setScore(int s);
    void setLevel(int l);
    void setStrength(int st);
    void setMode(Mode mode);

    int getHealth();
    int getScore();
    int getLevel();
    int getStrength();
    bool onGround();
    bool isLeft();
    bool isAlive();
    void changeMode(Mode mode);
    Mode getMode();

    // default image = IDLE
    virtual void Init(b2Vec2 position);  
    virtual void UpdateMode(Mode mode);
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
    float range; // max range if the player can shoot
    int lives;
    bool sit;
    bool immortal;
    string currentMap;
    float time; // time allotted for the player to complete the map
public:
    Player();
    Player(string type, string name = "", float coins = 0.0f, float range = 0.0f, int lives = 0, bool sit = false, 
            int health = 0, int score = 0, int level = 0, int strength = 0, Vector2 size = {0, 0}, float speed = 0.0f, float angle = 0.0f);
    Player(const Player &p);
    virtual ~Player();

    void setPositon(b2Vec2 pos);
    void setName(string n);
    void setCoins(float c);
    void setRange(float r);
    void setLives(int lives);
    void setSit(bool s);
    void setImmortal(bool im);
    void setCurrentMap(string map);
    void setTime(float t);
    void updateScore(int s);

    string getName();
    float getCoins();
    float getRange();
    string getCurrentMap();
    float getTime();
    bool isSitting();
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
public:
    Enemy();
    Enemy(string type, float range = 0, bool alive = true, int health = 0, int score = 0, int level = 0, int strength = 0, Vector2 size = {0, 0}, float speed = 0.0f, float angle = 0.0f);
    Enemy(const Enemy &e);
    virtual ~Enemy();
    void setType(string t);
    void setRange(float r);
    void setIsAlive(bool ia);

    string getType();
    float getRange();

    void Init(b2Vec2 position);  
    virtual void Update(Vector2 playerVelocity, float deltaTime);
    virtual void OnBeginContact(SceneNode* other, b2Vec2 normal);
    virtual void OnEndContact(SceneNode* other);
    void HandleInput();
    void Dead();
    void Draw();
    void Draw(Vector2 position, float angle = 0.0f);

    MovingObject* copy() const;
};


class Goomba : public Enemy {
public:
    Goomba();
    Goomba(string type, float range = 0, bool alive = true, bool sit = false, int health = 0, int score = 0, int level = 0, int strength = 0, Vector2 size = {0, 0}, float speed = 0.0f, float angle = 0.0f);
    Goomba(const Goomba &g);
    virtual ~Goomba();

    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);
    MovingObject* copy() const;
};

class Koopa : public Enemy {
public:
    Koopa();
    Koopa(string type, float range = 0, bool alive = true, bool sit = false, int health = 0, int score = 0, int level = 0, int strength = 0, Vector2 size = {0, 0}, float speed = 0.0f, float angle = 0.0f);
    Koopa(const Koopa &k);
    virtual ~Koopa();

    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);
    MovingObject* copy() const; 
};  

#endif
