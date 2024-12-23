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

void FireBall::accept(FileVisitor *visitor) {
    visitor->VisitFile(this);
}

MovingObject *FireBall::copy() const
{
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
    : MovingObject() {}

MovingPlatform::MovingPlatform(Vector2 size, Vector2 speed, float distance, float angle, string _type) 
    : MovingObject(size, speed.x, angle), distance(distance), speed(speed), type(_type) 
    {
}

MovingPlatform::MovingPlatform(const MovingPlatform &mp)
    : MovingObject(mp) {}

MovingPlatform::~MovingPlatform() {
    if (body) {
        Physics::world.DestroyBody(body);
        body = nullptr;
    }
    animations.clear();
}

void MovingPlatform::Init(b2Vec2 position) {
    if (type=="movingplatform") animations = AnimationHandler::setAnimations("movingplatform");
    else if(type=="rotatingblaze") animations = AnimationHandler::setAnimations("rotatingball");
    Texture texture = animations[0].GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};

    std::vector<b2Vec2> vertices = {
        b2Vec2{0.0f, 0.0f},
        b2Vec2{size.x, 0.0f},
        b2Vec2{size.x, size.y},
        b2Vec2{0.0f, size.y}
    };
    restitution = 0.0f;
    MyBoundingBox::createBody(body, b2_kinematicBody, vertices, Vector2{position.x, position.y}, restitution);
    body->SetLinearVelocity(b2Vec2(speed.x, speed.y));
    b2Fixture* fixture = body->GetFixtureList();
    fixture->SetFriction(10.0f);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
}
void MovingPlatform::InitOrbit(Vector2 center, float radius, float speed) {
    animations = AnimationHandler::setAnimations("rotatingball");
    Texture texture = animations[0].GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};

    std::vector<b2Vec2> vertices = {
        b2Vec2{0.0f, 0.0f},
        b2Vec2{size.x, 0.0f},
        b2Vec2{size.x, size.y},
        b2Vec2{0.0f, size.y}
    };
    restitution = 0.0f;
    MyBoundingBox::createBody(body, b2_kinematicBody, vertices, center, restitution);
    //b2Fixture* fixture = body->GetFixtureList();
    //fixture->SetFriction(10.0f);
    //body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    orbitCenter = center;
    orbitRadius = radius;
    orbitAngle = 0.0f;    
    orbitSpeed = speed;   
}
void MovingPlatform::Update(Vector2 playerVelocity, float deltaTime) {
    if(type == "movingplatform") {
    if (!body) return;
    if (speed.x != 0) curDistance += abs(speed.x) * deltaTime;
    else if (speed.y != 0) curDistance += abs(speed.y) * deltaTime;
    if (distance == 0) {
        Tilemap* tilemap = Tilemap::getInstance();
        float heightMap = (float)tilemap->GetHeight();
        b2Vec2 pos = body->GetPosition();
        if (pos.y < 0) {
            pos.y = heightMap;
        }
        else if (pos.y > heightMap) {
            pos.y = 0;
        }
        body->SetTransform(b2Vec2(pos.x, pos.y), body->GetAngle());
    }
    else {
        if (curDistance >= distance) {
            speed.x = -speed.x;
            speed.y = -speed.y;
            curDistance = 0;
            body->SetLinearVelocity(b2Vec2(speed.x, speed.y));
        }
    }
    }
    else if (type == "rotatingblaze") {
       orbitAngle += orbitSpeed * deltaTime;

        // Đảm bảo góc quay không reset, chỉ giới hạn trong khoảng [0, 360)
        if (orbitAngle >= 360.0f) {
            orbitAngle -= 360.0f;
        }
        float radianAngle = orbitAngle * (M_PI / 180.0f);
        body->SetTransform(b2Vec2(orbitCenter.x, orbitCenter.y), radianAngle);
    }
}

void MovingPlatform::HandleInput() {
    // Platforms generally do not require input handling
}

void MovingPlatform::OnBeginContact(SceneNode *other, b2Vec2 normal) {
    if(type=="rotatingblaze"){
    if (!other) return;
    Player* player = dynamic_cast<Player*>(other);
    if (player) {
       // player->setHealth(player->getHealth() - 1000);
    }
    }
}

void MovingPlatform::OnEndContact(SceneNode *other) {
}

void MovingPlatform::Draw() {
    if(type == "movingplatform"){
    b2Vec2 pos = body->GetPosition();
    Texture text = animations[0].GetFrame();
    Rectangle sourceRect = { 0, 0, static_cast<float>(text.width), static_cast<float>(text.height) };
    Renderer::DrawPro(text, sourceRect, Vector2{pos.x, pos.y}, Vector2{size.x, size.y}, false, angle);
    }
    else if(type == "rotatingblaze")
    {
    if (!body) return;

    b2Vec2 pos = body->GetPosition();
  
    //float radianAngle = orbitAngle * (M_PI / 180.0f);

    Texture text = animations[0].GetFrame();
    Rectangle sourceRect = { 0, 0, static_cast<float>(text.width), static_cast<float>(text.height) };

    Renderer::DrawPro(text, sourceRect, Vector2{pos.x, pos.y}, Vector2{size.x, size.y}, false, orbitAngle);
    }
}

void MovingPlatform::Draw(Vector2 position, float angle) {
    Texture text = animations[0].GetFrame();
    Rectangle sourceRect = { 0, 0, static_cast<float>(text.width), static_cast<float>(text.height) };
    Renderer::DrawPro(text, sourceRect, position, Vector2{size.x, size.y}, false, angle);
}

void MovingPlatform::accept(FileVisitor *visitor) {
    visitor->VisitFile(this);
}

MovingObject *MovingPlatform::copy() const
{
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
    if (body) {
        Physics::world.DestroyBody(body);
        body = nullptr;
    }
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

void AttackBall::accept(FileVisitor *visitor) {
    visitor->VisitFile(this);
}

MovingObject *AttackBall::copy() const
{
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
    body->SetGravityScale(0.0f);
    // b2Fixture* fixture = body->GetFixtureList();
    // b2Filter filter = fixture->GetFilterData();
    // filter.maskBits = CATEGORY_PLAYER;
    // fixture->SetFilterData(filter);
}

void AttackBall::Draw() {
    if (!body) return;
    bool faceLeft = speed < 0;
    b2Vec2 pos = body->GetPosition();
    Texture text = animations[0].GetFrame();
    Rectangle sourceRect = { 0, 0, static_cast<float>(text.width), static_cast<float>(text.height) };
    Renderer::DrawPro(text, sourceRect, Vector2{pos.x, pos.y}, Vector2{size.x, size.y}, faceLeft, angle);
}

void AttackBall::Draw(Vector2 position, float angle) {
};

void AttackBall::Update(Vector2 playerVelocity, float deltaTime) {
    if (!body) return;
    setSpeed(speed);
    elapsedTime += deltaTime;
    span += deltaTime;
    if (span >= 1.5f) {
        flag = true;
    }
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
            if (player->getHealth() <= 0) {
                // player->setAlive(false);
            }
        } else if (player->getMode() == Mode::BIG || player->getMode() == Mode::FIRE) {
            b2Body* playerBody = player->getBody();
            Vector2 playerSize = player->getSize();
            Vector2 pos = player->getPosition();
            player->setPositon(b2Vec2{pos.x, pos.y});
            player->changeMode(Mode::SMALL);
            player->setImmortal(true);
            player->setImmortalTime(1.5f);
        }
    }
    Physics::bodiesToDestroy.push_back(body);
    body = nullptr;
    animations.clear();
}

void AttackBall::OnEndContact(SceneNode *other) {
    // Handle end of collision logic if needed
}

Flag::Flag()
{
    elapsedTime = 0.0f;
}

Flag::Flag(const Flag &f)
{
    elapsedTime = f.elapsedTime;
}

Flag::~Flag()
{
    elapsedTime = 0.0f;
}

void Flag::setPosition(Vector2 position)
{
    if (body) body->SetTransform(b2Vec2{position.x, position.y}, body->GetAngle());
}

void Flag::Init(b2Vec2 position) 
{
    animations = AnimationHandler::setAnimations("flag");
    Texture texture = animations[0].GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};

    std::vector<b2Vec2> vertices = {
        b2Vec2{0.0f, 0.0f},
        b2Vec2{size.x, 0.0f},
        b2Vec2{size.x, size.y},
        b2Vec2{0.0f, size.y}
    };
    restitution = 0.0f;
    MyBoundingBox::createBody(body, b2_staticBody, vertices, Vector2{position.x, position.y}, restitution);
    b2Fixture *fixture = body->GetFixtureList();
    fixture->SetSensor(true);
}

void Flag::Update(Vector2 playerVelocity, float deltaTime)
{
}

void Flag::HandleInput()
{
}

void Flag::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
}

void Flag::OnEndContact(SceneNode *other)
{
}

void Flag::Draw()
{
    if (!body) return;
    b2Vec2 pos = body->GetPosition();
    Texture text = animations[0].GetFrame();
    Rectangle sourceRect = {0, 0, static_cast<float>(text.width), static_cast<float>(text.height)};
    Renderer::DrawPro(text, sourceRect, Vector2{pos.x, pos.y}, Vector2{size.x, size.y}, true, 0);
}

void Flag::Draw(Vector2 position, float angle)
{
}

void Flag::accept(FileVisitor *visitor)
{
}

MovingObject *Flag::copy() const
{
    return new Flag(*this);
}

Axe::Axe() : MovingObject()
{
}

Axe::Axe(const Axe &a) : MovingObject(a)
{
}

void Axe::AddBridgeTile(StaticTile *tile)
{
    tiles.push_back(tile);
}

void Axe::Init(b2Vec2 position)
{
    animations = AnimationHandler::setAnimations("axe");
    Texture texture = animations[0].GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};

    std::vector<b2Vec2> vertices = {
        b2Vec2{0.0f, 0.0f},
        b2Vec2{size.x, 0.0f},
        b2Vec2{size.x, size.y},
        b2Vec2{0.0f, size.y}
    };
    restitution = 0.0f;
    MyBoundingBox::createBody(body, b2_staticBody, vertices, Vector2{position.x, position.y}, restitution);
    b2Fixture *fixture = body->GetFixtureList();
    fixture->SetSensor(true);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
}

void Axe::Update(Vector2 playerVelocity, float deltaTime)
{
    if (activated) return;
    if (!activating && !activated) {
        animations[0].Update(deltaTime);
        texture = animations[0].GetFrame();
        return;
    }
    elapsedTime += deltaTime;
    float tilesSize = tiles.size();
    float activateTime = 1.0f / tilesSize;
    if (elapsedTime >= activateTime)
    {
        elapsedTime = 0.0f;
        if (tiles.size() > 0)
        {
            StaticTile *tile = tiles.back();
            tile->setIsDestroyed(true);
            tile->setSensorBody(true);
            tiles.pop_back();
        }
    }

    if (tiles.size() == 0)
    {
        activated = true;
        Tilemap *tilemap = Tilemap::getInstance();
        Player *player = tilemap->GetPlayer();
        b2Body *playerBody = player->getBody();
        playerBody->SetGravityScale(1.0f);
        player->setSpeed(8.5f);
    }
}

void Axe::HandleInput()
{
}

void Axe::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
    Player *player = dynamic_cast<Player *>(other);
    if (player)
    {
        activating = true;
        Physics::bodiesToDestroy.push_back(body);
        body = nullptr;
        animations.clear();

        b2Body *playerBody = player->getBody();
        playerBody->SetGravityScale(0.0f);
        player->setSpeed(0.0f);
    }
}

void Axe::OnEndContact(SceneNode *other)
{
}

void Axe::Draw()
{
    if (!body) return;
    b2Vec2 pos = body->GetPosition();
    Texture text = animations[0].GetFrame();
    Rectangle sourceRect = {0, 0, static_cast<float>(text.width), static_cast<float>(text.height)};
    Renderer::DrawPro(text, sourceRect, Vector2{pos.x, pos.y}, Vector2{size.x, size.y}, false, 0);
}

void Axe::Draw(Vector2 position, float angle)
{
}

void Axe::accept(FileVisitor *visitor)
{
}

MovingObject *Axe::copy() const
{
    return new Axe(*this);
}
