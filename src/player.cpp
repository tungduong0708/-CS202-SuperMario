#include "include.h"
#include "object.h"
#include "character.h"

Player::Player() : Character()
{
    name = "";
    coins = 0;
    range = 0;
    alive = true;
    sit = false;
}

Player::Player(string type, string name, float coins, float range, bool alive, bool sit, int health, 
               int score, int level, int strength, Vector2 size, float speed, 
               float angle, vector<Image> imgs): 
    Character(type, health, score, level, strength, size, speed, angle, imgs), 
    name(name), 
    coins(coins), 
    range(range), 
    alive(alive), 
    sit(sit) {}

Player::Player(const Player &p): 
    Character(p), 
    name(p.name), 
    coins(p.coins), 
    range(p.range), 
    alive(p.alive), 
    sit(p.sit) 
{
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

void Player::setImmortal(bool im) {
    immortal = im;
}

void Player::setCurrentMap(string map) {
    currentMap = map;
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

float Player::getRange() {
    return range;
}

string Player::getCurrentMap() {
    return currentMap;
}

bool Player::isAlive() {
    return alive;
}

bool Player::isSitting() {
    return sit;
}

bool Player::isImmortal() {
    return immortal;
}

void Player::HandleInput() {
    //ResizeBody(size.x, size.y);
    // Handle character input
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        body->SetLinearVelocity(b2Vec2(8.0f, body->GetLinearVelocity().y));
        if (currentImage != JUMP) {
            previousImage = currentImage;
            currentImage = WALK;   
        }
        faceLeft = false;
        // cout << "state: " << currentImage << endl;
    } else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        body->SetLinearVelocity(b2Vec2(-8.0f, body->GetLinearVelocity().y));
        if (currentImage != JUMP) {
            previousImage = currentImage;
            currentImage = WALK;
        }
        faceLeft = true;
        // cout << "state: " << currentImage << endl;
    }
    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));
        if (currentImage != JUMP) {
            previousImage = currentImage;
            currentImage = DUCK;
            //ResizeBody(size.x, size.y / 2.0f);
        }
        // cout << "state: " << currentImage << endl;
    }
    else {
        body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));
        if (currentImage != JUMP) {
            previousImage = currentImage;
            currentImage = IDLE;
        }
        //cout << "state: " << currentImage << endl;
    }

    if (isOnGround && currentImage == JUMP) {
        previousImage = currentImage;
        currentImage = IDLE;
        //cout << "state: " << currentImage << endl;
    }

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        if (isOnGround) {
            if (mode == SMALL) {
                body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -24.0f), true);
            }
            else if (mode == BIG or mode == FIRE) {
                body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -36.0f), true);
            }
            previousImage = currentImage;
            currentImage = JUMP;
            isOnGround = false;
        }
        // cout << "state: " << currentImage << endl;
    }

    if ((IsKeyPressed(KEY_E) || IsKeyPressed(KEY_ENTER)) && mode == FIRE) {
        // shoot
        // freq = 0.75 second
        previousImage = currentImage;
        currentImage = HOLD;
        animations[currentImage].setTimer();
        if (elapsedTime >= 0.75f) {
            // shoot the bullet
            FireBall* fireball = new FireBall(10.0f, {0.5f, 0.5f}, 5.0f, 0.0f, ImageHandler::setImages("fireball"));
            fireball->Init(body->GetPosition() + b2Vec2(!faceLeft * ((float)texture.width/16 + 0.1f), texture.height/32), IDLE);
            fireball->setSpeed(15.0f * (faceLeft ? -1 : 1));
            Tilemap* tilemap = Tilemap::getInstance();
            tilemap->addNode(fireball);
            elapsedTime = 0.0f;
        }
    }

    if (!isOnGround) {
        previousImage = currentImage;
        currentImage = JUMP;
        //cout << "state: " << currentImage << endl;
    }

    // cout << isOnGround << endl;
}

void Player::Update(Vector2 playerVelocity, float deltaTime) {
    Character::Update(playerVelocity, deltaTime);
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
    TextHelper::DrawPackage(name, score, coins, position, 12, WHITE);
}

void Player::OnBeginContact(SceneNode *other, b2Vec2 normal) 
{
    if (!other) return;

    if (normal.y < -0.5f) {
        // groundContacts.insert(other);
        isOnGround = true;
    }
}

void Player::OnEndContact(SceneNode *other)
{
}

MovingObject* Player::copy() const {
    return new Player(*this);
}