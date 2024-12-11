#include "include.h"
#include "object.h"
#include "character.h"

Enemy::Enemy() : Character() {
    type = "";
    range = 0;
    alive = true;
    sit = false;
}


Enemy::Enemy(string type, float range, bool alive, bool sit, int health, int score, int level, 
             int strength, Vector2 size, float speed, float angle, vector<Image> imgs): 
    Character(health, score, level, strength, size, speed, angle, imgs, type), 
    type(type), 
    range(range), 
    alive(alive), 
    sit(sit)
{
}


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

void Enemy::shoot() {
    // shoot the enemy
}

void Enemy::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
}
void Enemy::OnEndContact(SceneNode *other)
{
}
void Enemy::HandleInput()
{
}

void Enemy::Draw(Vector2 position, float angle)
{
}

MovingObject* Enemy::copy() const {
    return new Enemy(*this);
}