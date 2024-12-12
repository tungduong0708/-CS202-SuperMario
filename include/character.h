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
    Character(string type, int h = 0, int s = 0, int l = 0, int st = 0, Vector2 size = {0, 0}, float s1 = 0, float a1 = 0, vector<Image> images = {});
    Character(const Character &c);
    ~Character();

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
    void changeMode(Mode mode);
    Mode getMode();

    void move();
    void jump();
    void rotate();

    //MovingObject* copy() const;  // Prototype design pattern
    // default image = IDLE
    void Init(b2Vec2 position, ImageSet imageSet = IDLE);  
    virtual void UpdateMode(Mode mode);
    virtual void Update(Vector2 playerVelocity, float deltaTime);
    virtual void Draw();  
    virtual void Draw(Vector2 position, float angle = 0.0f) = 0;
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
    bool alive;
    bool sit;
    bool immortal;
    string currentMap;
public:
    Player();
    Player(string type, string name = "", float coins = 0.0f, float range = 0.0f, bool alive = true, bool sit = false, int health = 0, int score = 0, int level = 0, int strength = 0, Vector2 size = {0, 0}, float speed = 0.0f, 
               float angle = 0.0f, vector<Image> imgs = {});
    Player(const Player &p);
    ~Player();

    void setName(string n);
    void setCoins(float c);
    void setRange(float r);
    void setIsAlive(bool ia);
    void setSit(bool s);
    void setImmortal(bool im);
    void setCurrentMap(string map);
    void updateScore(int s);
    string getName();
    float getCoins();
    float getRange();
    string getCurrentMap();
    bool isAlive();
    bool isSitting();
    bool isImmortal();

    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);
    void HandleInput();
    void Update(Vector2 playerVelocity, float deltaTime);
    void UpdateAnimation();
    void Draw();
    void Draw(Vector2 position, float angle = 0.0f);
    MovingObject* copy() const;
};


class Enemy : public Character {
private:
    string type;
    float range; // max range if the enemy can shoot
    bool alive;
    bool sit;
    // speed = max speed that the enemy can move
public:
    Enemy();
    Enemy(string t = "", float r = 0, bool ia = true, bool s = false, int h = 0, int s1 = 0, int l = 0, int st = 0, Vector2 size = {0, 0}, float s2 = 0, float a1 = 0, vector<Image> images = {});
    Enemy(const Enemy &e);
    ~Enemy();
    void setType(string t);
    void setRange(float r);
    void setIsAlive(bool ia);
    void setSit(bool s);
    string getType();
    float getRange();
    bool isAlive();
    bool isSitting();
    void move();
    void jump();
    void rotate();
    void shoot();

    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);
    void HandleInput();
    void Draw(Vector2 position, float angle = 0.0f);

    MovingObject* copy() const;
};


class Goomba : public Enemy {
public:
    Goomba();
    Goomba(string type, float range, bool alive, bool sit, int health, int score, int level, int strength, Vector2 size, float speed, float angle, vector<Image> images);
    Goomba(const Goomba &g);
    ~Goomba();

    void Init(b2Vec2 position, ImageSet imageSet);
    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);
    MovingObject* copy() const;
};

class Koopa : public Enemy {
public:
    Koopa();
    Koopa(string type, float range, bool alive, bool sit, int health, int score, int level, int strength, Vector2 size, float speed, float angle, vector<Image> images);
    Koopa(const Koopa &k);
    ~Koopa();

    void Init(b2Vec2 position, ImageSet imageSet);
    void OnBeginContact(SceneNode* other, b2Vec2 normal);
    void OnEndContact(SceneNode* other);
    MovingObject* copy() const; 
};  

#endif
