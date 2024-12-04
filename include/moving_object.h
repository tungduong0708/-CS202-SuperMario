#pragma once
#include "include.h"

#define IMAGE_WIDTH 16 

#ifndef MOVING_OBJECT_H
#define MOVING_OBJECT_H


class MovingObject {
protected:
    float height, width; // size of the object
    float speed;  // max speed that the object can move
    float angle;  // initial angle of the object when moving
    float density; // density of the object, can determine the mass of the object
    vector<Image> images;
    b2Body* body; 
public:
    MovingObject(int);
    MovingObject(float h = 0, float w = 0, float s = 0, float a = 0, vector<Image> images = {});
    MovingObject(const MovingObject &mo);
    ~MovingObject();
    void setHeight(float h);
    void setWidth(float w);
    void setSpeed(float s);
    void setAngle(float a);
    void setDensity(float d);
    void setImage(const Image &img);

    float getHeight();
    float getWidth();
    float getSpeed();
    float getAngle();
    float getDensity();
    vector<Image> getImages();
    b2Vec2 getPosition();
    b2Vec2 getVelocity();

    void move();
    void jump();
    void rotate();
    
    // virtual functions
    virtual MovingObject* copy() const = 0;  // Prototype design pattern
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;  
    virtual void InitCharacter(b2Vec2 position, ImageSet imageSet) = 0;
    virtual void HandleInput() = 0;
};


class Character : public MovingObject {
protected:
    int health;
    int score;
    int level;
    int strength;
    string type;

    Texture2D texture;          // Character sprite sheet              
    Rectangle sourceRect;       // Sprite frame to draw
    Rectangle destRect;         // Scaled drawing rectangle
    Vector2 origin;             // Sprite origin
    int frameWidth, frameHeight;
    int currentFrame;
    float frameTime, frameSpeed;
    bool isOnGround;          // Is character on the ground
    ImageSet currentImage;
    ImageSet previousImage;
    bool faceLeft;            // Is character facing left
public:
    Character(int);
    Character(int h = 0, int s = 0, int l = 0, int st = 0, float h1 = 0, float w1 = 0, float s1 = 0, float a1 = 0, vector<Image> images = {}, string type = "");
    Character(const Character &c);
    ~Character();

    void setHealth(int h);
    void setScore(int s);
    void setLevel(int l);
    void setStrength(int st);
    int getHealth();
    int getScore();
    int getLevel();
    int getStrength();
    bool onGround();
    bool isLeft();

    void move();
    void jump();
    void rotate();

    MovingObject* copy() const;  // Prototype design pattern
    // default image = IDLE
    void InitCharacter(b2Vec2 position, ImageSet imageSet = IDLE);  
    void Update(float deltaTime);
    void Render();
    void HandleInput();
    void SetOnGround(bool onGround); // Setter for ground state
};

class Player : public Character {
private:
    string name;
    float coins;
    float range; // max range if the player can shoot
    bool alive;
    bool sit;
    // speed = max speed that the player can move
public:
    Player();
    Player(string n = "", float c = 0, float r = 0, bool ia = true, bool s = false, int h = 0, int s1 = 0, int l = 0, int st = 0, float h1 = 0, float w1 = 0, float s2 = 0, float a1 = 0, vector<Image> images = {}, string type = "");
    Player(const Player &p);
    ~Player();

    void setName(string n);
    void setCoins(float c);
    void setRange(float r);
    void setIsAlive(bool ia);
    void setSit(bool s);
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
    Enemy(string t = "", float r = 0, bool ia = true, bool s = false, int h = 0, int s1 = 0, int l = 0, int st = 0, float h1 = 0, float w1 = 0, float s2 = 0, float a1 = 0, vector<Image> images = {});
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

    MovingObject* copy() const;
};


class FireFlower : public MovingObject {
private:
    float damage;
    // angle = initial angle of the fire flower
    // other attributes are inherited from the moving object class ---
public:
    FireFlower();
    FireFlower(float d = 0, float h = 0, float w = 0, float s = 0, float a = 0, vector<Image> images = {});
    FireFlower(const FireFlower &ff);
    ~FireFlower();
    void setDamage(float d);
    float getDamage();
    void move();
    void jump();
    void rotate();

    //MovingObject* copy() const;
};

class Bullet : public MovingObject {
private:
    // bullet attributes
    float damage;
    // angle = initial angle of the bullet
    // other attributes are inherited from the moving object class ---
public:
    Bullet();
    Bullet(float d = 0, float h = 0, float w = 0, float s = 0, float a = 0, vector<Image> images = {});
    Bullet(const Bullet &b);
    ~Bullet();
    void setDamage(float d);
    float getDamage();
    void move();
    void jump();
    void rotate();

    //MovingObject* copy() const;
};

class Coin : public MovingObject {
private:
    // coin attributes
    float value;
    // angle = initial angle of the coin
    // other attributes are inherited from the moving object class ---
public:
    Coin();
    Coin(float v = 0, float h = 0, float w = 0, float s = 0, float a = 0, vector<Image> images = {});
    Coin(const Coin &c);
    ~Coin();
    void setValue(float v);
    float getValue();
    void jump();
    void rotate();

    //MovingObject* copy() const;
};


#endif

