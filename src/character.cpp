#include "include.h"
#include "object.h"
#include "character.h"

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

void Character::changeMode(Mode mode)
{
    this->mode = mode;
    this->modeChanged = true;
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

void Character::UpdateMode(Mode mode)
{
    b2Vec2 position = body->GetPosition();
    this->mode = mode;
    currentImage = WALK;
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
    Physics::world.DestroyBody(body);
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
    if (modeChanged) {
        UpdateMode(mode);
        modeChanged = false;
    }
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