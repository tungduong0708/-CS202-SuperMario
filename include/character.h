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
    Character(int);
    Character(int h = 0, int s = 0, int l = 0, int st = 0, Vector2 size = {0, 0}, float s1 = 0, float a1 = 0, vector<Image> images = {}, string type = "");
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
    vector<FireBall> fireBalls;  
    vector<DelayedTexture> delayedTextures;
    // speed = max speed that the player can move
public:
    Player();
    Player(string n, float c, float r, bool iarue, bool salse, int h, int s1, int l, int st, Vector2 size, float s2, float a1, vector<Image> images, string type);
    Player(const Player &p);
    ~Player();

    void setName(string n);
    void setCoins(float c);
    void setRange(float r);
    void setIsAlive(bool ia);
    void setSit(bool s);
    void updateScore(int s);
    string getName();
    float getCoins();
    float getRange();
    bool isAlive();
    bool isSitting();

    // Move, jump, rotate and shoot the player methods
    void move();
    void jump();
    void rotate();
    void shoot();

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

#endif
