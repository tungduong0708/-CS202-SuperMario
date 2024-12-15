#include "include.h"
#include "object.h"
#include "character.h"

std::vector<Color> colors = {LIME, YELLOW, RAYWHITE, GOLD};

Character::Character() : MovingObject()
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
    modeChanged = false;
    currentImage = IDLE;
    faceLeft = false;
    alive = true;
    mode = Mode::SMALL;
}

Character::Character(string type, int health, int score, int level, int strength, 
                     Vector2 size, float speed, float angle): 
    MovingObject(size, speed, angle), 
    health(health), 
    score(score), 
    level(level), 
    strength(strength), 
    type(type) 
{
    isOnGround = false;
    modeChanged = false;
    faceLeft = false;
    currentImage = IDLE;
    previousImage = IDLE;
    curAnim = Animation();
    mode = Mode::SMALL;
    alive = true;
    immortal = false;
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
      mode(c.mode),
      alive(c.alive),
      immortal(c.immortal)
{
    modeChanged = false;
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
    mode = SMALL;
    alive = false;
    immortal = false;
}

void Character::setHealth(int health) {
    this->health = health;
    if (health <= 0) {
        alive = false;
    }
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

bool Character::isAlive() {
    return alive;
}

void Character::changeMode(Mode mode)
{
    this->mode = mode;
    this->modeChanged = true;
}

void Character::Init(b2Vec2 position) {
    alive = true;
    currentImage = ImageSet::WALK;
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
    currentImage = IDLE;
}

void Character::UpdateMode(Mode mode, b2Vec2 position)
{
    this->mode = mode;
    currentImage = ImageSet::WALK;
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
void Character::Dead()
{
}

void Character::ResizeBody(float newWidth, float newHeight)
{
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
    EffectManager* effectManager = Tilemap::getInstance()->GetEffectManager();
    if (modeChanged && !effectManager->isActivePlayerEffect()) {
        UpdateMode(mode, b2Vec2{position.x, position.y});
        modeChanged = false;
    }
    if (!body) return;
    elapsedTime += deltaTime;
    b2Vec2 position = body->GetPosition();
    destRect.x = position.x;
    destRect.y = position.y;

    curAnim = animations[currentImage];
    Animation prevAnim = animations[previousImage];
    if (previousImage == HOLD && prevAnim.getTimer() <= prevAnim.getFrameTime(0)) {
        currentImage = HOLD; // delay the hold animation 
    }

    animations[currentImage].Update(deltaTime);
    texture = animations[currentImage].GetFrame();
    if (immortal) {
        b2Vec2 vel = body->GetLinearVelocity();
        if (vel.x > 0) {
            body ->SetLinearVelocity(b2Vec2{vel.x + 3.0f, vel.y});
        }
        else if (vel.x < 0) {
            body ->SetLinearVelocity(b2Vec2{vel.x - 3.0f, vel.y});
        }
        immortalTime -= deltaTime;
        colorChangeTimer += deltaTime;

        // Change color every 1 second
        if (colorChangeTimer >= 0.05f) {
            colorIndex = (colorIndex + 1) % colors.size();
            colorChangeTimer = 0.0f;  // Reset the timer
        }

        // End immortal state
        if (immortalTime <= 0) {
            immortal = false;
        }
    }
}

void Character::Draw() {
    b2Vec2 pos = body->GetPosition();
    sourceRect = { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) };
    Vector2 drawPosition = { pos.x, pos.y };
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
    if (immortal) {
        // colors[colorIndex].a = 50;
        Renderer::DrawPro(texture, sourceRect, drawPosition, Vector2{size.x, size.y}, faceLeft, 0.0f, Vector2{-1.0f, -1.0f}, colors[colorIndex]);
    }
    else {
        Renderer::DrawPro(texture, sourceRect, drawPosition, Vector2{size.x, size.y}, faceLeft);
    }
}

void Character::HandleInput() {
}

void Character::OnBeginContact(SceneNode* other)
{
}

void Character::OnEndContact(SceneNode* other)
{
}