#include "moving_object.h"
#include "imagehandler.h"
#include "physics.h"


MovingObject::MovingObject() {
    size = {0, 0};
    speed = 0;
    angle = 0;
    images = {};
    elapsedTime = 0.0f;
}

MovingObject::MovingObject(Vector2 size, float speed, float angle, vector<Image> images): 
    size(size), speed(speed), angle(angle), images(images) {}

MovingObject::MovingObject(const MovingObject &mo) {
    size = mo.size;
    speed = mo.speed;
    angle = mo.angle;
    ImageHandler::ImageVectorCopy(mo.images, images);
    body = mo.body;
}

MovingObject::~MovingObject() {}

void MovingObject::setSize(Vector2 size)
{
    this->size = size;
}

void MovingObject::setSpeed(float speed) {
    this->speed = speed;
}

void MovingObject::setAngle(float angle) {
    this->angle = angle;
}

void MovingObject::setImage(const Image &img) {
    images.push_back(img);
}

void MovingObject::setDensity(float density) {
    this->density = density;
}

void MovingObject::setElapsedTime(float et) {
    elapsedTime = et;
}

void MovingObject::setFrameTime(float ft) {
    frameTime = ft;
}

float MovingObject::getElapsedTime() {
    return elapsedTime;
}

float MovingObject::getFrameTime() {
    return frameTime;
}

Vector2 MovingObject::getSize() {
    return size;
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

b2Vec2 MovingObject::getPosition()
{
    return body->GetPosition();
}

b2Vec2 MovingObject::getVelocity()
{
    return body->GetLinearVelocity();
}

b2Body *MovingObject::getBody()
{
    return body;
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

Character::Character(int) : MovingObject()
{
    health = 0;
    score = 0;
    level = 0;
    strength = 0;
    type = "";
}

Character::Character(int health, int score, int level, int strength, 
                     Vector2 size, float speed, float angle, vector<Image> imgs, string type): 
    MovingObject(size, speed, angle, imgs), 
    health(health), 
    score(score), 
    level(level), 
    strength(strength), 
    type(type) {
    isOnGround = false;
    faceLeft = false;
    currentImage = IDLE;
    previousImage = IDLE;
    images = ImageHandler::setImages(type);
    curAnim = Animation();
}

Character::Character(const Character &c)
    : MovingObject(c),                
      health(c.health),
      score(c.score),
      level(c.level),
      strength(c.strength),
      type(c.type),

      sourceRect(c.sourceRect),
      destRect(c.destRect),
      origin(c.origin),
      frameWidth(c.frameWidth),
      frameHeight(c.frameHeight),
      currentFrame(c.currentFrame),
      frameTime(c.frameTime),
      frameSpeed(c.frameSpeed),
      isOnGround(c.isOnGround),
      currentImage(c.currentImage),
      previousImage(c.previousImage),
      faceLeft(c.faceLeft)
{
}

Character::~Character() {
    health = 0;
    score = 0;
    level = 0;
    strength = 0;
    type = "";
    sourceRect = {};
    destRect = {};
    origin = {};
    frameWidth = 0;
    frameHeight = 0;
    currentFrame = 0;
    frameTime = 0;
    frameSpeed = 0;
    isOnGround = false;
    currentImage = IDLE;
    previousImage = IDLE;
    faceLeft = false;
    for (auto img : images) {
        UnloadImage(img);
    }
}

void Character::setHealth(int health) {
    health = health;
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

bool Character::onGround() {
    return isOnGround;
}

bool Character::isLeft() { 
    return faceLeft;
}

void Character::move() {
    // move the character
}

void Character::jump() {
}

void Character::rotate() {
    // rotate the character
}

void Character::InitCharacter(b2Vec2 position, ImageSet imageSet) {
    currentImage = imageSet;
    animations = AnimationHandler::setAnimations(type);
    curAnim = animations[currentImage];

    texture = curAnim.GetFrame();

    frameWidth = texture.width;
    frameHeight = texture.height;
    sourceRect = {0, 0, (float)frameWidth, (float)frameHeight};

    float ratio = (float)frameWidth / (float)frameHeight;
    if (ratio < 1.0f) 
        size = {1.0f * ratio, 1.0f};
    else 
        size = {1.0f, 1.0f / ratio};

    destRect = {position.x, position.y, size.x, size.y};

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x, position.y);
    body = Physics::world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(destRect.width / 2.0f, destRect.height / 2.0f, b2Vec2(destRect.width / 2.0f, destRect.height / 2.0f), 0.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);

    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    body->SetFixedRotation(true); 

    cout << "Character: " << fixtureDef.filter.categoryBits << " " << fixtureDef.filter.maskBits << endl;
}

void Character::Update(Vector2 playerVelocity, float deltaTime) {
    b2Vec2 position = body->GetPosition();
    destRect.x = position.x;
    destRect.y = position.y;
    // std::cout << position.x << " " << position.y << std::endl;
    // std::cout << previousImage << " " << currentImage << std::endl;
    curAnim = animations[currentImage];
    // cout << deltaTime << endl;
    // cout << curAnim.size() << endl;
    // cout << curAnim.getCurrentIndex() << endl;
    curAnim.Update(deltaTime);
    texture = curAnim.GetFrame();
}

void Character::Draw() {
    b2Vec2 pos = body->GetPosition();
    Renderer::DrawPro(texture, sourceRect, Vector2{pos.x, pos.y}, Vector2{size.x, size.y}, faceLeft);
}

void Character::HandleInput() {
}

void Character::OnBeginContact(SceneNode* other)
{
}

void Character::OnEndContact(SceneNode* other)
{
}

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
    if (isOnGround && currentImage == JUMP) {
        currentImage = IDLE;
    }

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        if (!isOnGround) return;
        body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -15.0f), true);
        currentImage = JUMP;
    }

    // Handle character input
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        body->SetLinearVelocity(b2Vec2(8.0f, body->GetLinearVelocity().y));
        if (currentImage != JUMP) currentImage = WALK;   
        faceLeft = false;
    } else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        body->SetLinearVelocity(b2Vec2(-8.0f, body->GetLinearVelocity().y));
        if (currentImage != JUMP) currentImage = WALK;
        faceLeft = true;
    }
    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));
        if (currentImage != JUMP) currentImage = DUCK;
    }
    else {
        body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));
        if (currentImage != JUMP) currentImage = IDLE;
    }

    if (!isOnGround && body->GetPosition().y >= 550) {
        currentImage = JUMP;
    }

}

void Player::rotate() {
    // rotate the player
}

void Player::shoot() {
    // shoot the player
}

void Player::OnBeginContact(SceneNode *other)
{
    isOnGround = true;
}

void Player::OnEndContact(SceneNode *other)
{
    isOnGround = false;
}

MovingObject* Player::copy() const {
    return new Player(*this);
}

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

void Enemy::HandleInput()
{
}

MovingObject* Enemy::copy() const {
    return new Enemy(*this);
}


// [-----implementation of moving items, derived from moving object class -------------------]

FireFlower::FireFlower() : MovingObject() {
    damage = 0;
}

FireFlower::FireFlower(float d, Vector2 size, float speed, float angle, vector<Image> imgs): 
    MovingObject(size, speed, angle, imgs), damage(d) {}


FireFlower::FireFlower(const FireFlower &ff): MovingObject(ff) {
    damage = ff.damage;
}

FireFlower::~FireFlower() {
    damage = 0;
}

void FireFlower::setDamage(float damage) {
    this->damage = damage;
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

Bullet::Bullet(float damage, Vector2 size, float speed, float angle, vector<Image> imgs): 
    MovingObject(size, speed, angle, imgs), damage(damage) {}


Bullet::Bullet(const Bullet &b): MovingObject(b) {
    damage = b.damage;
}

Bullet::~Bullet() {
    damage = 0;
}

void Bullet::setDamage(float damage) {
    this->damage = damage;
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


Coin::Coin(float value, Vector2 size, float speed, float angle, vector<Image> imgs): 
    MovingObject(size, speed, angle, imgs), value(value) {}


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





