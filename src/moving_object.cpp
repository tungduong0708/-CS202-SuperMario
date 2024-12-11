#include "include.h"
#include "object.h"


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

// Bullet::Bullet() : MovingObject() {
//     damage = 0;
// }

// Bullet::Bullet(float damage, Vector2 size, float speed, float angle, vector<Image> imgs): 
//     MovingObject(size, speed, angle, imgs), damage(damage) {}


// Bullet::Bullet(const Bullet &b): MovingObject(b) {
//     damage = b.damage;
// }

// Bullet::~Bullet() {
//     damage = 0;
// }

// void Bullet::setDamage(float damage) {
//     this->damage = damage;
// }

// float Bullet::getDamage() {
//     return damage;
// }

// void Bullet::move() {
//     // move the bullet
// }

// void Bullet::jump() {
//     // jump the bullet
// }

// void Bullet::rotate() {
//     // rotate the bullet
// }

