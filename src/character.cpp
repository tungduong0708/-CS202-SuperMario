#include "game.h"
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

void Character::setCurrentImage(ImageSet image)
{
    previousImage = currentImage;
    currentImage = image;
}

void Character::setHealth(int health)
{
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

void Character::setOnGround(bool og) {
    isOnGround = og;
}

void Character::setAppear(bool iv) {
    appear = iv;
}

void Character::setInvisibleTime(float it)
{
    invincible = true;
    blinkTime = it;
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

bool Character::isInvisible()
{
    return invincible;
}

string Character::getType() {
    return type;
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
    if (this->mode < mode){
        playSoundEffect(SoundEffect::POWER_UP);
    }
    else if (this->mode > mode){
        playSoundEffect(SoundEffect::POWER_DOWN);
    }
    this->mode = mode;
    this->modeChanged = true;
}

void Character::Init(b2Vec2 position) {
    this->position = Vector2{position.x, position.y};  
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
    b2Fixture* fixture = body->GetFixtureList();
    fixture->SetFriction(0.3f);
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
    else if (invincible) {
        b2Fixture* fixture = body->GetFixtureList();
        b2Filter filter = fixture->GetFilterData();
        filter.categoryBits = CATEGORY_PLAYER;
        filter.maskBits = MASK_ENEMY;
        fixture->SetFilterData(filter);
        appearTimer += deltaTime;
        blinkTime -= deltaTime;
        if (blinkTime <= 0.0f) {
            invincible = false;
            blinkTime = 0.0f;
            appear = true;
            filter.maskBits = MASK_DEFAULT;
            fixture->SetFilterData(filter);
            return;
        }
        if (appearTimer >= 0.1f) {
            appearTimer = 0.0f;
            appear = !appear;
        }
    }
}

void Character::Draw() {
    if (!appear) return;
    b2Vec2 pos = body->GetPosition();
    sourceRect = { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) };
    Vector2 drawPosition = { pos.x, pos.y };
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
    if (immortal) {
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

Princess::Princess()
{
    type = "princess";
    health = 0;
    score = 0;
    level = 0;
    strength = 0;
    size = {0.6f, 0.6f};
    speed = 0.0f;
    angle = 0.0f;
    appear = true;
    invincible = false;
    immortal = false;
    isOnGround = false;
    modeChanged = false;
    faceLeft = false;
    blinkTime = 0.0f;
    immortalTime = 0.0f;
    appearTimer = 0.0f;
    colorChangeTimer = 0.0f;
    frameTime = 0.0f;
    frameSpeed = 0.0f;
    currentImage = IDLE;
    previousImage = IDLE;
}

Princess::Princess(string type, int health, int score, int level, int strength, Vector2 size, float speed, float angle)
    : Character(type, health, score, level, strength, size, speed, angle)
{
    appear = true;
    invincible = false;
    immortal = false;
    isOnGround = false;
    modeChanged = false;
    faceLeft = false;
    blinkTime = 0.0f;
    immortalTime = 0.0f;
    appearTimer = 0.0f;
    colorChangeTimer = 0.0f;
    frameTime = 0.0f;
    frameSpeed = 0.0f;
    currentImage = IDLE;
    previousImage = IDLE;
    isFree = false;
}


Princess::Princess(const Princess &p)
    : Character(p)
{
    appear = p.appear;
    invincible = p.invincible;
    immortal = p.immortal;
    isOnGround = p.isOnGround;
    modeChanged = p.modeChanged;
    faceLeft = p.faceLeft;
    blinkTime = p.blinkTime;
    immortalTime = p.immortalTime;
    appearTimer = p.appearTimer;
    colorChangeTimer = p.colorChangeTimer;
    frameTime = p.frameTime;
    frameSpeed = p.frameSpeed;
    currentImage = p.currentImage;
    previousImage = p.previousImage;
}

Princess::~Princess()
{
    appear = false;
    invincible = false;
    immortal = false;
    isOnGround = false;
    modeChanged = false;
    faceLeft = false;
    blinkTime = 0.0f;
    immortalTime = 0.0f;
    appearTimer = 0.0f;
    colorChangeTimer = 0.0f;
    frameTime = 0.0f;
    frameSpeed = 0.0f;
    currentImage = IDLE;
    previousImage = IDLE;
}

void Princess::Init(b2Vec2 position)
{
    this->position = Vector2{position.x, position.y};  
    alive = true;
    currentImage = ImageSet::IDLE;
    animations = AnimationHandler::setAnimations("princess");
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
    b2Fixture* fixture = body->GetFixtureList();
    fixture->SetFriction(0.3f);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
}

void Princess::Update(Vector2 playerVelocity, float deltaTime)
{
    if (!body) return;
    setSpeed(0.0f);
    if (isFree) {
        elapsedTime += deltaTime;
        if (elapsedTime >= 7.0f) {
            Game* game = Game::getInstance();
            game->changeState(game->victoryState.get());
            game->getSettings().volume = prevVolume;
        }
        else {
            Player* player = Tilemap::getInstance()->GetPlayer();
            string type = player->getType();
            string text = "Thank you " + type + "!";
            int pos = min(int(round(16.0f * elapsedTime / 2.0f)), (int)text.size());
            cout << text.substr(0, pos) << endl;
            cout << getPosition().x << " " << getPosition().y << endl;

            TextHelper::Draw(text.substr(0, pos), Vector2{getPosition().x - 4.0f, getPosition().y - 4.0f}, 9, RAYWHITE);
            float ratio = player->getTime() / (7.0f) * deltaTime;
            if (player->getTime() > 0) {
                player->setTime(player->getTime() - ratio*5);
                player->setScore(player->getScore() + ratio*100);
            }
        }
    }
}

void Princess::OnBeginContact(SceneNode* other, b2Vec2 normal)
{
    if (!other) return;
    Player* player = dynamic_cast<Player*>(other);
    if (player) {
        player->setAllowInput(false);
        player->setSpeed(0.0f);
        isFree = true;
        elapsedTime = 0.0f;

        playSoundEffect(SoundEffect::WORLD_CLEAR);
        Game* game = Game::getInstance();
        prevVolume = game->getSettings().volume;
        game->getSettings().volume = 0.0f;
    }
}

void Princess::OnEndContact(SceneNode* other)
{
}

void Princess::HandleInput()
{
}

void Princess::Dead()
{
}

void Princess::Draw()
{
    if (!appear) return;
    b2Vec2 pos = body->GetPosition();
    sourceRect = { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) };
    Vector2 drawPosition = { pos.x, pos.y };
    Renderer::DrawPro(texture, sourceRect, drawPosition, Vector2{size.x, size.y}, faceLeft);
}

void Princess::Draw(Vector2 position, float angle)
{
}

void Princess::accept(FileVisitor* visitor) {
    visitor->VisitFile(this);
}

MovingObject* Princess::copy() const {
    return new Princess(*this);
}
