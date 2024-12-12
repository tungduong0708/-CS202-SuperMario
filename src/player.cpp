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

Player::Player(string name, float coins, float range, bool alive, bool sit, int health, 
               int score, int level, int strength, Vector2 size, float speed, 
               float angle, vector<Image> imgs, string type): 
    Character(health, score, level, strength, size, speed, angle, imgs, type), 
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
            body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -24.0f), true);
            previousImage = currentImage;
            currentImage = JUMP;
            isOnGround = false;
        }
        // cout << "state: " << currentImage << endl;
    }

    if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_ENTER)) {
        // shoot
        // freq = 0.75 second
        previousImage = currentImage;
        currentImage = HOLD;
        animations[currentImage].setTimer();
        if (elapsedTime >= 0.75f) {
            // shoot the bullet
            fireBalls.push_back(FireBall(10.0f, {0.5f, 0.5f}, 5.0f, 0.0f, ImageHandler::setImages("fireball")));
            for (int i = 0; i < fireBalls.size()-1; i++) {
                fireBalls[i].ReloadAnimation();
            }
            fireBalls.back().Init(body->GetPosition() + b2Vec2(!faceLeft * (texture.width + 0.1f)/16, texture.height/32), IDLE);
            fireBalls.back().setSpeed(9.0f * (faceLeft ? -1 : 1));
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
    vector<int> posFireBalls;
    vector<int> posDelayedTextures;

    Vector2 position = getPosition();

    //cout << tilemap.GetWidth() << " " << tilemap.GetHeight() << endl;
    for (int i = 0; i < fireBalls.size(); i++) {
        fireBalls[i].Update(Vector2{0, 0}, deltaTime);
        Vector2 pos = fireBalls[i].getPosition();
        // if the flower is out of the screen, remove it from the vector
        if (pos.x < position.x - GetScreenWidth()/IMAGE_WIDTH || pos.x > position.x + GetScreenWidth()/IMAGE_WIDTH  
            || pos.y < position.y - GetScreenHeight()/IMAGE_WIDTH || pos.y > position.y + GetScreenHeight()/IMAGE_WIDTH) {
            posFireBalls.push_back(i);
        }
        if (fireBalls[i].isActive()) {
            posFireBalls.push_back(i);
            Texture txt = ImageHandler::setTextures("active")[0];
            delayedTextures.push_back(DelayedTexture(txt, 1.0f, pos));
        }
    }


    for (int i = 0; i < delayedTextures.size(); i++) {
        delayedTextures[i].Update(deltaTime);
        if (delayedTextures[i].isActive()) {
            posDelayedTextures.push_back(i);
        }
    }

    //cout << delayedTextures.size() << " " << posDelayedTextures.size() << endl;

    for (int i = posFireBalls.size() - 1; i >= 0; i--) {
        fireBalls.erase(fireBalls.begin() + posFireBalls[i]);
    }
    for (int i = posDelayedTextures.size() - 1; i >= 0; i--) {
        delayedTextures.erase(delayedTextures.begin() + posDelayedTextures[i]);
    }
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
    for (auto &fireBall : fireBalls) {
        fireBall.Draw();
    }
    for (auto &delayedTexture : delayedTextures) {
        delayedTexture.Draw();
    }
}

void Player::Draw(Vector2 position, float angle) {
    TextHelper::DrawPackage(name, score, coins, position, 12, WHITE);
}

void Player::rotate() {
    // rotate the player
}

void Player::shoot() {
    // shoot the player
}

void Player::OnBeginContact(SceneNode *other, b2Vec2 normal) 
{
    if (!other) return;

    if (normal.y > 0.5f) {
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