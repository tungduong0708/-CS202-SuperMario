#include "include.h"
#include "object.h"
#include "character.h"

Player::Player() : Character()
{
    name = "";
    coins = 0;
    lives = 0;
    time = 0;
    immortal = false;
    currentMap = "";
    force = -12.0f;
    bulletSpeed = 9.0f;
    bulletFreq = 0.75f;
    mode = Mode::SMALL;
}

Player::Player(string type, string name, float coins, int lives, int health, 
               int score, int level, int strength, Vector2 size, float speed, 
               float angle): 
    Character(type, health, score, level, strength, size, speed, angle), 
    name(name), 
    coins(coins),  
    lives(lives)
{
    time = 0;
    immortal = false;
    currentMap = "";
    alive = true;

    speed = 8.0f;
    force = -24.0f;
    bulletSpeed = 9.0f;
    bulletFreq = 0.75f;
    mode = Mode::SMALL;
}

Player::Player(const Player &p): 
    Character(p), 
    name(p.name), 
    coins(p.coins), 
    lives(p.lives),
    immortal(p.immortal),
    currentMap(p.currentMap),
    time(p.time)
{
}

Player::~Player() {
    name = "";
    coins = 0;
    lives = 0;
    time = 0;
}

void Player::setPositon(b2Vec2 pos)
{
    body->SetTransform(pos, body->GetAngle());
}

void Player::setName(string n) {
    name = n;
}

void Player::setCoins(float c) {
    coins = c;
}

void Player::setLives(int lives) {
    this->lives = lives;
}

void Player::setImmortal(bool im) {
    immortal = im;
}

void Player::setCurrentMap(string map) {
    currentMap = map;
}

void Player::setTime(float t) {
    time = t;
}

void Player::impulseForce(Vector2 force) { 
    body->ApplyLinearImpulseToCenter(b2Vec2(force.x, force.y), true);
}

void Player::updateScore(int s) {
    score += s;
}

string Player::getName() {
    return name;
}

float Player::getCoins() {
    return coins;
}

string Player::getCurrentMap() {
    return currentMap;
}

float Player::getTime() {
    return time;
}

bool Player::isImmortal() {
    return immortal;
}

void Player::HandleInput() {
    if (isAlive() == false) {
        return;
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        body->SetLinearVelocity(b2Vec2(speed, body->GetLinearVelocity().y));
        if (currentImage != JUMP) {
            previousImage = currentImage;
            currentImage = ImageSet::WALK;   
        }
        faceLeft = false;

    } else 
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        body->SetLinearVelocity(b2Vec2(-speed, body->GetLinearVelocity().y));
        if (currentImage != JUMP) {
            previousImage = currentImage;
            currentImage = ImageSet::WALK;
        }
        faceLeft = true;

    }
    else 
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));
        if (currentImage != JUMP) {
            previousImage = currentImage;
            currentImage = DUCK;
        }

    }
    else {
        body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));
        if (currentImage != JUMP) {
            previousImage = currentImage;
            currentImage = IDLE;
        }
    }

    if (isOnGround && currentImage == JUMP) {
        previousImage = currentImage;
        currentImage = IDLE;
    }

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        if (isOnGround) {
            if (mode == SMALL) {
                body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, force), true);
            }
            else if (mode == BIG or mode == FIRE) {
                body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, force * 1.5f), true);
            }
            previousImage = currentImage;
            currentImage = JUMP;
            isOnGround = false;
        }
    }

    // frequency of the bullet: 0.75 seconds
    if ((IsKeyPressed(KEY_E) || IsKeyPressed(KEY_ENTER)) && mode == FIRE) {
        previousImage = currentImage;
        currentImage = HOLD;
        animations[currentImage].setTimer();
        if (elapsedTime >= bulletFreq) {
            // create a fireball
            FireBall* fireball = new FireBall(10.0f, {0.5f, 0.5f}, 5.0f, 0.0f);
            fireball->Init(body->GetPosition() + b2Vec2(!faceLeft * ((float)texture.width/16 + 0.1f), texture.height/32));
            fireball->setSpeed(15.0f * (faceLeft ? -1 : 1));

            Tilemap* tilemap = Tilemap::getInstance();
            tilemap->addNode(fireball);
            elapsedTime = 0.0f;
        }
    }

    if (!isOnGround) {
        previousImage = currentImage;
        currentImage = JUMP;
    }
}

void Player::Update(Vector2 playerVelocity, float deltaTime) {
    Character::Update(playerVelocity, deltaTime);
    if (time <= 0) {
        alive = false;
        time = 300.0f;
    }
    if (health <= 0) {
        alive = false;
        health = 100;
    }
    if (alive) {
        time -= deltaTime;
    }
    else {
        Dead();
    }
}

void Player::Dead() {
    // dead....
}

void Player::UpdateAnimation() {
    if (mode == SMALL) {
        animations = AnimationHandler::setAnimations("small" + name);
    }
    else if (mode == BIG) {
        animations = AnimationHandler::setAnimations("big" + name);
    }
    else if (mode == FIRE) {
        animations = AnimationHandler::setAnimations("fire" + name);
    }
}

void Player::Draw() {
    Character::Draw();
}

void Player::Draw(Vector2 position, float angle) {
    TextHelper::DrawPackage(lives, score, coins, currentMap, time, position, 12, WHITE);
}

void Player::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
    if (!other) return;

    if (normal.y < -0.5f) {
        isOnGround = true;
    }
}

void Player::Init(b2Vec2 position) {
    Character::Init(position);
}

void Player::OnEndContact(SceneNode *other) {
    
}

MovingObject* Player::copy() const {
    return new Player(*this);
}