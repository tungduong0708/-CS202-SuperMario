#include "include.h"
#include "object.h"
#include "moving_object.h"


MovingObject::MovingObject() {
    size = {0, 0};
    speed = 0;
    angle = 0;
    images = {};
    elapsedTime = 0.0f;
    restitution = 0.0f;
}

MovingObject::MovingObject(Vector2 size, float speed, float angle, vector<Image> images): 
    size(size), speed(speed), angle(angle), images(images), elapsedTime(0), restitution(0) {}

MovingObject::MovingObject(const MovingObject &mo) {
    size = mo.size;
    speed = mo.speed;
    angle = mo.angle;
    ImageHandler::ImageVectorCopy(mo.images, images);
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
    images.clear();
    elapsedTime = 0.0f;
    restitution = 0.0f;
    // if (body) {
    //     Physics::world.DestroyBody(body);
    //     body = nullptr;
    // }
}

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

float MovingObject::getAngle() {
    return angle;
}

float MovingObject::getDensity() {
    return density;
}

float MovingObject::getRestitution() {
    return restitution;
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

// [-----implementation of moving items, derived from moving object class -------------------]

FireBall::FireBall() : MovingObject() {
    damage = 0;
    flag = false;
}

FireBall::FireBall(float d, Vector2 size, float speed, float angle, vector<Image> imgs): 
    MovingObject(size, speed, angle, imgs), damage(d), flag(false) {
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
    restitution = 1.0f;
    MyBoundingBox::createBody(body, b2_dynamicBody, vertices, Vector2{position.x, position.y}, restitution);

    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    b2Fixture* fixture = body->GetFixtureList();
    b2Filter filter = fixture->GetFilterData();
    filter.maskBits = ~CATEGORY_PLAYER;
    fixture->SetFilterData(filter);
}

void FireBall::Update(Vector2 playerVelocity, float deltaTime) {
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
    if (!other) return;

    Enemy *enemy = dynamic_cast<Enemy*>(other);
    if (enemy || normal.x != 0) {
        if (enemy) enemy->setHealth(enemy->getHealth() - damage);
        Physics::bodiesToDestroy.push_back(body);
        animations.clear();
        images.clear();
    }
}

void FireBall::OnEndContact(SceneNode *other) {
    if (!other) return;
}

void FireBall::Draw() {
    b2Vec2 pos = body->GetPosition();
    Texture text = animations[0].GetFrame();
    Rectangle sourceRect = { 0, 0, static_cast<float>(text.width), static_cast<float>(text.height) };
    Renderer::DrawPro(text, sourceRect, Vector2{pos.x, pos.y}, Vector2{size.x, size.y}, false, angle);
}

void FireBall::Draw(Vector2 position, float angle) {
}
