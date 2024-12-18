#include "include.h"
#include "object.h"
#include "moving_object.h"


MovingObject::MovingObject() {
    size = {0, 0};
    speed = 0;
    angle = 0;
    elapsedTime = 0.0f;
    restitution = 0.0f;
}

MovingObject::MovingObject(Vector2 size, float speed, float angle): 
    size(size), speed(speed), angle(angle), elapsedTime(0), restitution(0) {}

MovingObject::MovingObject(const MovingObject &mo) {
    size = mo.size;
    speed = mo.speed;
    angle = mo.angle;
    body = mo.body;
    elapsedTime = mo.elapsedTime;
    frameTime = mo.frameTime;
    density = mo.density;
    restitution = mo.restitution;
}

MovingObject::~MovingObject() {
    size = {0, 0};
    speed = 0;
    angle = 0;
    elapsedTime = 0.0f;
    restitution = 0.0f;
    if (body) {
        Physics::world.DestroyBody(body);
        body = nullptr;
    }
}

void MovingObject::setSize(Vector2 size) {
    this->size = size;
}

void MovingObject::setSpeed(float speed) {
    this->speed = speed;
    body->SetLinearVelocity(b2Vec2(speed, 0.0f));
}

void MovingObject::setAngle(float angle) {
    this->angle = angle;
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

void MovingObject::setRestitution(float r) {
    restitution = r;
}

void MovingObject::setStable() {
    float gravityScale = body->GetGravityScale();
    float mass = body->GetMass();
    b2Vec2 gravity = Physics::world.GetGravity();
    b2Vec2 antiGravityForce = -mass * gravityScale * gravity;

    body->ApplyForceToCenter(antiGravityForce, true);
    
    elapsedTime = 0.0f;
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

float MovingObject::getSpeed() {
    return speed;
}

float MovingObject::getAngle() {
    return angle;
}

float MovingObject::getDensity() {
    return density;
}

float MovingObject::getRestitution() {
    return restitution;
}

Vector2 MovingObject::getPosition()
{
    b2Vec2 pos = body->GetPosition();
    return Vector2{pos.x, pos.y};
}

b2Vec2 MovingObject::getVelocity()
{
    if (body == nullptr) return b2Vec2{0, 0};
    return body->GetLinearVelocity();
}

b2Body *MovingObject::getBody()
{
    return body;
}

vector<Animation> MovingObject::getAnimations() {
    return animations;
}


FireBall::FireBall() : MovingObject() {
    damage = 0;
    flag = false;
}

FireBall::FireBall(float d, Vector2 size, float speed, float angle): 
    MovingObject(size, speed, angle), damage(d), flag(false) {
}


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

void FireBall::setFlag(bool flag) {
    this->flag = flag;
}

float FireBall::getDamage() {
    return damage;
}

bool FireBall::isActive() {
    return flag;
}

Animation FireBall::getAnimation(bool flag) {
    return animations[flag];
}

MovingObject* FireBall::copy() const {
    return new FireBall(*this);
}

void FireBall::Init(b2Vec2 position) {
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
    restitution = 1.0f;
    MyBoundingBox::createBody(body, b2_dynamicBody, vertices, Vector2{position.x, position.y}, restitution);

    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    b2Fixture* fixture = body->GetFixtureList();
    b2Filter filter = fixture->GetFilterData();
    filter.maskBits = ~CATEGORY_PLAYER;
    fixture->SetFilterData(filter);
}

void FireBall::Update(Vector2 playerVelocity, float deltaTime) {
    if (!body) return;
    elapsedTime += deltaTime;
    span += deltaTime;
    if (span >= 1.5f) {
        flag = true;
    }
    angle += 5.0f;

}

void FireBall::HandleInput() {

}

void FireBall::ReloadAnimation() {
    animations = AnimationHandler::setAnimations("fireball");
}

void FireBall::OnBeginContact(SceneNode *other, b2Vec2 normal) {
    if (!other || !body) return;

    Enemy *enemy = dynamic_cast<Enemy*>(other);
    if (enemy || normal.x != 0) {
        if (enemy) enemy->setHealth(enemy->getHealth() - damage);
        AnimationEffect* effect = AnimationEffectCreator::CreateAnimationEffect("fireball_explode", Vector2{body->GetPosition().x, body->GetPosition().y});

        EffectManager* effectManager = Tilemap::getInstance()->GetEffectManager();
        effectManager->AddUpperEffect(effect);

        Physics::bodiesToDestroy.push_back(body);
        body = nullptr;
        animations.clear();
    }
}

void FireBall::OnEndContact(SceneNode *other) {
    if (!other) return;
}

void FireBall::Draw() {
    if (!body) return;
    b2Vec2 pos = body->GetPosition();
    Texture text = animations[0].GetFrame();
    Rectangle sourceRect = { 0, 0, static_cast<float>(text.width), static_cast<float>(text.height) };
    Renderer::DrawPro(text, sourceRect, Vector2{pos.x, pos.y}, Vector2{size.x, size.y}, false, angle);
}

void FireBall::Draw(Vector2 position, float angle) {
}


MovingPlatform::MovingPlatform() 
    : MovingObject(), movementType(MovementType::Horizontal), direction(1.0f),
      topBoundary(0), bottomBoundary(0), leftBoundary(0), rightBoundary(0) {}

MovingPlatform::MovingPlatform(MovementType type, Vector2 size, float speed, float angle, float boundaries[4]) 
    : MovingObject(size, speed, angle), movementType(type), direction(1.0f) {
    topBoundary = boundaries[0];
    bottomBoundary = boundaries[1];
    leftBoundary = boundaries[2];
    rightBoundary = boundaries[3];
}

MovingPlatform::MovingPlatform(const MovingPlatform &mp)
    : MovingObject(mp), movementType(mp.movementType), direction(mp.direction),
      topBoundary(mp.topBoundary), bottomBoundary(mp.bottomBoundary),
      leftBoundary(mp.leftBoundary), rightBoundary(mp.rightBoundary) {}

MovingPlatform::~MovingPlatform() {}

void MovingPlatform::Init(b2Vec2 position) {
    animations = AnimationHandler::setAnimations("movingplatform");
    Texture texture = animations[0].GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};

    std::vector<b2Vec2> vertices = {
        b2Vec2{0.0f, 0.0f},
        b2Vec2{size.x, 0.0f},
        b2Vec2{size.x, size.y},
        b2Vec2{0.0f, size.y}
    };
    restitution = 0.0f;
    MyBoundingBox::createBody(body, b2_dynamicBody, vertices, Vector2{position.x, position.y}, restitution);

    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    
}

void MovingPlatform::Update(Vector2 playerVelocity, float deltaTime) {
    elapsedTime += deltaTime;
    b2Vec2 velocity = body->GetLinearVelocity();

    if (movementType == MovementType::Vertical) {
        velocity.y = speed * direction;
        body->SetLinearVelocity(velocity);

        // Check boundaries for vertical movement
        float posY = body->GetPosition().y;
        if (posY <= bottomBoundary || posY >= topBoundary) {
            direction = -direction; // Reverse direction
        }
    } else if (movementType == MovementType::Horizontal) {
        velocity.x = speed * direction;
        body->SetLinearVelocity(velocity);

        // Check boundaries for horizontal movement
        float posX = body->GetPosition().x;
        if (posX <= leftBoundary || posX >= rightBoundary) {
            direction = -direction; // Reverse direction
        }
    }
    animations[0].Update(deltaTime);
}

void MovingPlatform::HandleInput() {
    // Platforms generally do not require input handling
}

void MovingPlatform::OnBeginContact(SceneNode *other, b2Vec2 normal) {
    // Handle collisions if needed, e.g., with the player
}

void MovingPlatform::OnEndContact(SceneNode *other) {
    // Handle end of collision logic if needed
}

void MovingPlatform::Draw() {
    b2Vec2 pos = body->GetPosition();
    Texture text = animations[0].GetFrame();
    Rectangle sourceRect = { 0, 0, static_cast<float>(text.width), static_cast<float>(text.height) };
    Renderer::DrawPro(text, sourceRect, Vector2{pos.x, pos.y}, Vector2{size.x, size.y}, false, angle);
}

void MovingPlatform::Draw(Vector2 position, float angle) {
    Texture text = animations[0].GetFrame();
    Rectangle sourceRect = { 0, 0, static_cast<float>(text.width), static_cast<float>(text.height) };
    Renderer::DrawPro(text, sourceRect, position, Vector2{size.x, size.y}, false, angle);
}

MovingObject* MovingPlatform::copy() const {
    return new MovingPlatform(*this);
}

AttackBall::AttackBall() {
    damage = 0;
    flag = false;
    span = 0.0f;
}

AttackBall::AttackBall(float d, Vector2 size, float speed, float angle): 
    MovingObject(size, speed, angle), damage(d), flag(false), span(0.0f) {
}

AttackBall::AttackBall(const AttackBall &ff): MovingObject(ff) {
    damage = ff.damage;
    flag = ff.flag;
    span = ff.span;
}

AttackBall::~AttackBall() {
    damage = 0;
    flag = false;
    span = 0.0f;
}

void AttackBall::setDamage(float damage) {
    this->damage = damage;
}

void AttackBall::setFlag(bool flag) {
    this->flag = flag;
}

float AttackBall::getDamage() {
    return damage;
}

bool AttackBall::isActive() {
    return flag;
}

Animation AttackBall::getAnimation(bool flag) {
    return animations[0];
}

MovingObject* AttackBall::copy() const {
    return new AttackBall(*this);
}

void AttackBall::Init(b2Vec2 position) {
    animations = AnimationHandler::setAnimations("attackball");
    Texture texture = animations[0].GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};

    std::vector<b2Vec2> vertices = {
        b2Vec2{0.0f, 0.0f},
        b2Vec2{size.x, 0.0f},
        b2Vec2{size.x, size.y},
        b2Vec2{0.0f, size.y}
    };
    restitution = 0.0f;
    MyBoundingBox::createBody(body, b2_dynamicBody, vertices, Vector2{position.x, position.y}, restitution);

    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    b2Fixture* fixture = body->GetFixtureList();
    b2Filter filter = fixture->GetFilterData();
    filter.maskBits = CATEGORY_PLAYER;
    fixture->SetFilterData(filter);

    setSpeed(6.0f);
}

void AttackBall::Draw() {
    if (!body) return;
    b2Vec2 pos = body->GetPosition();
    Texture text = animations[0].GetFrame();
    Rectangle sourceRect = { 0, 0, static_cast<float>(text.width), static_cast<float>(text.height) };
    Renderer::DrawPro(text, sourceRect, Vector2{pos.x, pos.y}, Vector2{size.x, size.y}, false, angle);
}

void AttackBall::Draw(Vector2 position, float angle) {
};

void AttackBall::Update(Vector2 playerVelocity, float deltaTime) {
    elapsedTime += deltaTime;
    span += deltaTime;
    if (span >= 1.5f) {
        flag = true;
    }
    angle += 5.0f;
}

void AttackBall::HandleInput() {
    // Attack balls generally do not require input handling
}

void AttackBall::OnBeginContact(SceneNode *other, b2Vec2 normal) {
    if (!other || !body) return;

    Player *player = dynamic_cast<Player*>(other);
    if (player) {
        if (player->isImmortal()) return;
        if (player->getMode() == Mode::SMALL) {
            player->setHealth(player->getHealth() - damage);
        } else if (player->getMode() == Mode::BIG || player->getMode() == Mode::FIRE) {
            b2Body* playerBody = player->getBody();
            Vector2 playerSize = player->getSize();
            Vector2 pos = player->getPosition();
            player->setPositon(b2Vec2{pos.x, pos.y});
            player->changeMode(Mode::SMALL);
            player->setImmortal(true);
            player->setImmortalTime(1.5f);

            EffectManager* effectManager = Tilemap::getInstance()->GetEffectManager();
            effectManager->AddUpperEffect(AnimationEffectCreator::CreateAnimationEffect("shrink_mario", Vector2{pos.x, pos.y + playerSize.y}));
            effectManager->setActivePlayerEffect(true);
            playerBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        }
    }
}

void AttackBall::OnEndContact(SceneNode *other) {
    // Handle end of collision logic if needed
}




