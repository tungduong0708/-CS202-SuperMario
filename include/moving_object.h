#include "include.h"

enum ImageSet {
    IDLE,
    DUCK,
    WALK1,
    WALK2,
    JUMP,
    FALL,
    PIPE,
    SHOOT,
    DEAD,
    VICTORY
};

// [-----declaration of the moving object base class -------------------]

class MovingObject {
private:
    // moving object attributes
    float height, width; // size of the object
    float speed;  // max speed that the object can move
    float angle;  // initial angle of the object when moving
    float density; // density of the object, can determine the mass of the object
    vector<Image> images;
public:
    MovingObject(int);
    MovingObject(float h = 0, float w = 0, float s = 0, float a = 0, vector<Image> images);
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
    void move();
    void jump();
    void rotate();
};

// [-----declaration of Character classes derived from moving object class -------------------]

class Character : public MovingObject {
private:
    // character attributes
    int health;
    int score;
    int level;
    int strength;
    // other attributes are inherited from the moving object class ---
public:
    Character(int);
    Character(int h = 0, int s = 0, int l = 0, int st = 0, float h1 = 0, float w1 = 0, float s1 = 0, float a1 = 0, vector<Image> images);
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
    void move();
    void jump();
    void rotate();
};

class Player : public Character {
private:
    // player attributes
    string name;
    float coins;
    float range; // max range if the player can shoot
    bool alive;
    bool sit;
    // speed = max speed that the player can move
    // other attributes are inherited from the character class ---
public:
    Player();
    Player(string n = "", float c = 0, float r = 0, bool ia = true, bool s = false, int h = 0, int s1 = 0, int l = 0, int st = 0, float h1 = 0, float w1 = 0, float s2 = 0, float a1 = 0, vector<Image> images);
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
    void move();
    void jump();
    void rotate();
    void shoot();
};


class Enemy : public Character {
private:
    // enemy attributes
    string type;
    float range; // max range if the enemy can shoot
    bool alive;
    bool sit;
    // speed = max speed that the enemy can move
    // other attributes are inherited from the character class ---
public:
    Enemy();
    Enemy(string t = "", float r = 0, bool ia = true, bool s = false, int h = 0, int s1 = 0, int l = 0, int st = 0, float h1 = 0, float w1 = 0, float s2 = 0, float a1 = 0, vector<Image> images);
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
};

// [-----declaration of moving items, derived from moving object class -------------------]

class FireFlower : public MovingObject {
private:
    // fire flower attributes
    float damage;
    // angle = initial angle of the fire flower
    // other attributes are inherited from the moving object class ---
public:
    FireFlower();
    FireFlower(float d = 0, float h = 0, float w = 0, float s = 0, float a = 0, vector<Image> images);
    FireFlower(const FireFlower &ff);
    ~FireFlower();
    void setDamage(float d);
    float getDamage();
    void move();
    void jump();
    void rotate();
};

class Bullet : public MovingObject {
private:
    // bullet attributes
    float damage;
    // angle = initial angle of the bullet
    // other attributes are inherited from the moving object class ---
public:
    Bullet();
    Bullet(float d = 0, float h = 0, float w = 0, float s = 0, float a = 0, vector<Image> images);
    Bullet(const Bullet &b);
    ~Bullet();
    void setDamage(float d);
    float getDamage();
    void move();
    void jump();
    void rotate();
};

class Coin : public MovingObject {
private:
    // coin attributes
    float value;
    // angle = initial angle of the coin
    // other attributes are inherited from the moving object class ---
public:
    Coin();
    Coin(float v = 0, float h = 0, float w = 0, float s = 0, float a = 0, vector<Image> images);
    Coin(const Coin &c);
    ~Coin();
    void setValue(float v);
    float getValue();
    void jump();
    void rotate();
};

