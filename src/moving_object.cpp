#include "moving_object.h"
#include "imagehandler.h"
#include "physics.h"
#include "my_bounding_box.h"
#include "include.h"
#include "string_mode.h"


MovingObject::MovingObject() {
    size = {0, 0};
    speed = 0;
    angle = 0;
    images = {};
    elapsedTime = 0.0f;
}

MovingObject::MovingObject(Vector2 size, float speed, float angle, vector<Image> images): 
    size(size), speed(speed), angle(angle), images(images), elapsedTime(0) {}

MovingObject::MovingObject(const MovingObject &mo) {
    size = mo.size;
    speed = mo.speed;
    angle = mo.angle;
    ImageHandler::ImageVectorCopy(mo.images, images);
    body = mo.body;
    elapsedTime = mo.elapsedTime;
}

MovingObject::~MovingObject() {}

void MovingObject::setSize(Vector2 size)
{
    this->size = size;
}

void MovingObject::setSpeed(float speed) {
    this->speed = speed;
    body->SetLinearVelocity(b2Vec2(speed, 0.0f));
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

Vector2 MovingObject::getPosition()
{
    b2Vec2 pos = body->GetPosition();
    return Vector2{pos.x, pos.y};
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

vector<Animation> MovingObject::getAnimations() {
    // get the animation of the object
    return animations;
}

Character::Character(int) : MovingObject()
{
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
    faceLeft = false;
    mode = Mode::SMALL;
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
      faceLeft(c.faceLeft),
      mode(c.mode)
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
    mode = SMALL;
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

void Character::setMode(Mode mode) {
    this->mode = mode;
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

Mode Character::getMode() {
    return mode;
}

bool Character::onGround() {
    return isOnGround;
}

bool Character::isLeft() { 
    return faceLeft;
}

bool Character::hitWall()
{
    return isHitWall;
}

void Character::move() {
    // move the character
}

void Character::jump() {
}

void Character::rotate() {
    // rotate the character
}

void Character::Init(b2Vec2 position, ImageSet imageSet) {
    currentImage = WALK;
    mode = SMALL;
    animations = AnimationHandler::setAnimations(StringMode::getMode(mode) + type);
    curAnim = animations[currentImage];

    texture = curAnim.GetFrame();

    frameWidth = texture.width;
    frameHeight = texture.height;
    sourceRect = {0, 0, (float)frameWidth, (float)frameHeight};

    size = {(float)frameWidth / IMAGE_WIDTH, (float)frameHeight / IMAGE_WIDTH};
    destRect = {position.x, position.y, size.x, size.y};

    std::vector<b2Vec2> vertices = {
        b2Vec2{0.0f, 0.0f},
        b2Vec2{size.x, 0.0f},
        b2Vec2{size.x - 0.2f, size.y},
        b2Vec2{size.x, size.y - 0.05f},
        b2Vec2{0.0f, size.y - 0.05f},
        b2Vec2{0.0f + 0.2f, size.y}
    };
    MyBoundingBox::createBody(body, b2_dynamicBody, vertices, Vector2{position.x, position.y});

    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
}

void Character::ResizeBody(float newWidth, float newHeight) {
    // Destroy the existing fixture
    b2Fixture* fixture = body->GetFixtureList();
    while (fixture != nullptr) {
        b2Fixture* next = fixture->GetNext();
        body->DestroyFixture(fixture);
        fixture = next;
    }

    // Define a new shape with the desired size
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(newWidth / 2.0f, newHeight / 2.0f); // Box2D uses half-widths and half-heights

    // Define a new fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;

    // Create the new fixture on the body
    body->CreateFixture(&fixtureDef);
}

void Character::Update(Vector2 playerVelocity, float deltaTime) {
    elapsedTime += deltaTime;
    b2Vec2 position = body->GetPosition();
    destRect.x = position.x;
    destRect.y = position.y;

    curAnim = animations[currentImage];
    Animation prevAnim = animations[previousImage];
    //cout << prevAnim.getTimer() << " " << prevAnim.getFrameTime(0) << endl;
    if (previousImage == HOLD && prevAnim.getTimer() <= prevAnim.getFrameTime(0)) {
        currentImage = HOLD; // delay the hold animation 
    }

    //cout << previousImage << " " << currentImage << endl;
    animations[currentImage].Update(deltaTime);
    texture = animations[currentImage].GetFrame();
    //size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};
}

void Character::Draw() {
    b2Vec2 pos = body->GetPosition();
    sourceRect = { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) };
    //cout << pos.x << " " << pos.y << endl;
    Vector2 drawPosition = { pos.x, pos.y };

    Renderer::DrawPro(texture, sourceRect, drawPosition, Vector2{size.x, size.y}, faceLeft);
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
        }
    }


    for (int i = posFireBalls.size() - 1; i >= 0; i--) {
        fireBalls.erase(fireBalls.begin() + posFireBalls[i]);
    }
}

void Player::Draw() {
    Character::Draw();
    for (auto &fireBall : fireBalls) {
        fireBall.Draw();
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

void Player::OnBeginContact(SceneNode *other)
{
    if (!other) return;
    b2Vec2 pos = body->GetPosition();
    Vector2 otherPos = other->getPosition();
    float playBottom = pos.y + size.y;
    float otherTop = otherPos.y;
    float diff = playBottom - otherTop;

    std::cout << "diff: " << diff << std::endl;
    
    if (abs(diff) < 0.4f) {
        groundContacts.insert(other);
        isOnGround = true;
    }
}

void Player::OnEndContact(SceneNode *other)
{
    if (!other) return;

    if (groundContacts.find(other) != groundContacts.end()) {
        groundContacts.erase(other);
    }

    // If there are no more ground contacts, set isOnGround to false
    if (groundContacts.empty()) {
        isOnGround = false;
    }
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

void Enemy::Draw(Vector2 position, float angle)
{
}

MovingObject* Enemy::copy() const {
    return new Enemy(*this);
}


// [-----implementation of moving items, derived from moving object class -------------------]

FireBall::FireBall() : MovingObject() {
    damage = 0;
    flag = false;
}

FireBall::FireBall(float d, Vector2 size, float speed, float angle, vector<Image> imgs): 
    MovingObject(size, speed, angle, imgs), damage(d), flag(false) {}


FireBall::FireBall(const FireBall &ff): MovingObject(ff) {
    damage = ff.damage;
    flag = ff.flag;
}

FireBall::~FireBall() {
    damage = 0;
    flag = false;
}

void FireBall::setDamage(float damage) {
    this->damage = damage;
}

float FireBall::getDamage() {
    return damage;
}

bool FireBall::isActive() {
    return flag;
}

void FireBall::move() {
    // move the fire flower
}

void FireBall::jump() {
    // jump the fire flower
}

void FireBall::rotate() {
    // rotate the fire flower
    angle += 90.0f;
}


MovingObject* FireBall::copy() const {
    return new FireBall(*this);
}

void FireBall::Init(b2Vec2 position, ImageSet imageSet) {
    span = 0.0f;
    animations = AnimationHandler::setAnimations("fireball");
    Texture texture = animations[0].GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};

    std::vector<b2Vec2> vertices = {
        b2Vec2{0.0f, 0.0f},
        b2Vec2{size.x, 0.0f},
        b2Vec2{size.x, size.y},
        b2Vec2{0.0f, size.y}
    };
    MyBoundingBox::createBody(body, b2_dynamicBody, vertices, Vector2{position.x, position.y}, 1.0f);
}

void FireBall::Update(Vector2 playerVelocity, float deltaTime) {
    elapsedTime += deltaTime;
    span += deltaTime;
    if (span >= 1.5f) {
        flag = true;
    }
    angle += 5.0f;

    b2Vec2 position = body->GetPosition();
    //cout << "fireball position: " << position.x << " " << position.y << endl;
}

void FireBall::HandleInput() {

}

void FireBall::ReloadAnimation() {
    animations = AnimationHandler::setAnimations("fireball");
}

void FireBall::OnBeginContact(SceneNode *other) {
    if (!other) return;

    // if (dynamic_cast<Enemy*>(other)) {
    //     // if the fireball hits the enemy, destroy the fireball
    //     flag = true;
    //     //other->setHealth(other->getHealth() - damage);
    // }
    // if hit vertical wall, destroy the fireball
    Vector2 otherPos = other->getPosition();
    Vector2 posLeft = Vector2{size.x/2 + getPosition().x, size.y/2 + getPosition().y};
    cout << posLeft.x - otherPos.x << " " << size.x/2 + 0.1f << endl;
    if (abs(posLeft.x - otherPos.x) < size.x/2 + 0.1f) {
        flag = true;
    }
}

void FireBall::OnEndContact(SceneNode *other) {
    if (!other) return;
}

void FireBall::Draw() {
    b2Vec2 pos = body->GetPosition();
    Texture texture = animations[0].GetFrame();
    Rectangle sourceRect = { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) };
    Renderer::DrawPro(texture, sourceRect, Vector2{pos.x, pos.y}, Vector2{size.x, size.y}, false, angle);
}

void FireBall::Draw(Vector2 position, float angle) {
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

