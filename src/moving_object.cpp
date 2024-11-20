#include "moving_object.h"

// [-----implementation of the moving object base class -------------------]

MovingObject::MovingObject(int) {
    height = 0;
    width = 0;
    speed = 0;
    angle = 0;
    images = {};
}

MovingObject::MovingObject(float h, float w, float s, float a, vector<Image> imgs): 
    height(h), width(w), speed(s), angle(a), images(imgs) {}

MovingObject::MovingObject(const MovingObject &mo) {
    height = mo.height;
    width = mo.width;
    speed = mo.speed;
    angle = mo.angle;
    images = mo.images;
}

MovingObject::~MovingObject() {}

void MovingObject::setHeight(float h) {
    height = h;
}

void MovingObject::setWidth(float w) {
    width = w;
}

void MovingObject::setSpeed(float s) {
    speed = s;
}

void MovingObject::setAngle(float a) {
    angle = a;
}

void MovingObject::setImage(const Image &img) {
    images.push_back(img);
}

void MovingObject::setDensity(float d) {
    density = d;
}

float MovingObject::getHeight() {
    return height;
}

float MovingObject::getWidth() {
    return width;
}

float MovingObject::getSpeed() {
    return speed;
}

float MovingObject::getAngle() {
    return angle;
}

float MovingObject::getDensity() {
    return density;
}

vector<Image> MovingObject::getImages() {
    return images;
}

void MovingObject::move() {
    // move the object
}

void MovingObject::jump() {
    // jump the object
}

void MovingObject::rotate() {
    // rotate the object
}

// [-----implementation of Character classes derived from moving object class -------------------]

Character::Character(int): MovingObject() {
    health = 0;
    score = 0;
    level = 0;
    strength = 0;
}

Character::Character(int h, int s, int l, int st, float h1, float w1, float s1, float a1, vector<Image> imgs): 
    MovingObject(h1, w1, s1, a1, imgs), health(h), score(s), level(l), strength(st) {}

Character::Character(const Character &c): MovingObject(c) {
    health = c.health;
    score = c.score;
    level = c.level;
    strength = c.strength;
}

Character::~Character() {
    health = 0;
    score = 0;
    level = 0;
    strength = 0;
}

void Character::setHealth(int h) {
    health = h;
}

void Character::setScore(int s) {
    score = s;
}

void Character::setLevel(int l) {
    level = l;
}

void Character::setStrength(int st) {
    strength = st;
}

int Character::getHealth() {
    return health;
}

int Character::getScore() {
    return score;
}

int Character::getLevel() {
    return level;
}

int Character::getStrength() {
    return strength;
}

void Character::move() {
    // move the character
}

void Character::jump() {
    // jump the character
}

void Character::rotate() {
    // rotate the character
}

// [-----implementation of moving items, derived from moving object class -------------------]

Player::Player() : Character() {
    name = "";
    coins = 0;
    range = 0;
    alive = true;
    sit = false;
}

Player::Player(string n, float c, float r, bool ia, bool s, int h, int s1, int l, int st, float h1, float w1, float s2, float a1, vector<Image> imgs): 
    Character(h, s1, l, st, h1, w1, s2, a1, imgs), name(n), coins(c), range(r), alive(ia), sit(s) {}    

Player::Player(const Player &p): Character(p) {
    name = p.name;
    coins = p.coins;
    range = p.range;
    alive = p.alive;
    sit = p.sit;
}

Player::~Player() {
    name = "";
    coins = 0;
    range = 0;
    alive = true;
    sit = false;
}

void Player::setName(string n) {
    name = n;
}

void Player::setCoins(float c) {
    coins = c;
}

void Player::setRange(float r) {
    range = r;
}

void Player::setIsAlive(bool ia) {
    alive = ia;
}

void Player::setSit(bool s) {
    sit = s;
}

string Player::getName() {
    return name;
}

float Player::getCoins() {
    return coins;
}

bool Player::isAlive() {
    return alive;
}

bool Player::isSitting() {
    return sit;
}

void Player::move() {
    // move the player
}

void Player::jump() {
    // jump the player
}

void Player::rotate() {
    // rotate the player
}

void Player::shoot() {
    // shoot the player
}

Enemy::Enemy() : Character() {
    type = "";
    range = 0;
    alive = true;
    sit = false;
}

Enemy::Enemy(string t, float r, bool ia, bool s, int h, int s1, int l, int st, float h1, float w1, float s2, float a1, vector<Image> imgs): 
    Character(h, s1, l, st, h1, w1, s2, a1, imgs), type(t), range(r), alive(ia), sit(s) {}

Enemy::Enemy(const Enemy &e): Character(e) {
    type = e.type;
    range = e.range;
    alive = e.alive;
    sit = e.sit;
}

Enemy::~Enemy() {
    type = "";
    range = 0;
    alive = true;
    sit = false;
}

void Enemy::setType(string t) {
    type = t;
}

void Enemy::setRange(float r) {
    range = r;
}

void Enemy::setIsAlive(bool ia) {
    alive = ia;
}

void Enemy::setSit(bool s) {
    sit = s;
}

string Enemy::getType() {
    return type;
}

float Enemy::getRange() {
    return range;
}

bool Enemy::isAlive() {
    return alive;
}

bool Enemy::isSitting() {
    return sit;
}

void Enemy::move() {
    // move the enemy
}

void Enemy::jump() {
    // jump the enemy
}

void Enemy::rotate() {
    // rotate the enemy
}


// [-----implementation of moving items, derived from moving object class -------------------]

FireFlower::FireFlower() : MovingObject() {
    damage = 0;
}

FireFlower::FireFlower(float d, float h, float w, float s, float a, vector<Image> imgs): 
    MovingObject(h, w, s, a, imgs), damage(d) {}

FireFlower::FireFlower(const FireFlower &ff): MovingObject(ff) {
    damage = ff.damage;
}

FireFlower::~FireFlower() {
    damage = 0;
}

void FireFlower::setDamage(float d) {
    damage = d;
}

float FireFlower::getDamage() {
    return damage;
}

void FireFlower::move() {
    // move the fire flower
}

void FireFlower::jump() {
    // jump the fire flower
}

void FireFlower::rotate() {
    // rotate the fire flower
}


Bullet::Bullet() : MovingObject() {
    damage = 0;
}

Bullet::Bullet(float d, float h, float w, float s, float a, vector<Image> imgs): 
    MovingObject(h, w, s, a, imgs), damage(d) {}


Bullet::Bullet(const Bullet &b): MovingObject(b) {
    damage = b.damage;
}

Bullet::~Bullet() {
    damage = 0;
}

void Bullet::setDamage(float d) {
    damage = d;
}

float Bullet::getDamage() {
    return damage;
}

void Bullet::move() {
    // move the bullet
}

void Bullet::jump() {
    // jump the bullet
}

void Bullet::rotate() {
    // rotate the bullet
}

Coin::Coin() : MovingObject() {
    value = 0;
}

Coin::Coin(float v, float h, float w, float s, float a, vector<Image> imgs): 
    MovingObject(h, w, s, a, imgs), value(v) {}

Coin::Coin(const Coin &c): MovingObject(c) {
    value = c.value;
}

Coin::~Coin() {
    value = 0;
}

void Coin::setValue(float v) {
    value = v;
}

float Coin::getValue() {
    return value;
}

void Coin::jump() {
    // jump the coin
}

void Coin::rotate() {
    // rotate the coin
}



