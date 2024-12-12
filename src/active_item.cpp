#include "include.h"
#include "object.h"
#include "moving_object.h"
#include <box2d/box2d.h>

ActiveItem::ActiveItem() : MovingObject() {
    value = 0;
    flag = false;
}  

ActiveItem::ActiveItem(int value, Vector2 size, float speed, float angle, vector<Image> images): 
    MovingObject(size, speed, angle, images), value(value), flag(false) {}

ActiveItem::ActiveItem(const ActiveItem &gi): MovingObject(gi) {
    value = gi.value;
}

ActiveItem::~ActiveItem() {
    value = 0;
}

void ActiveItem::setValue(int v) {
    value = v;
}

bool ActiveItem::isAvailable() {
    return !flag;
}

int ActiveItem::getValue() {
    return value;
}

void ActiveItem::Init(b2Vec2 position, ImageSet imageSet) {
    // initialize the grow item
    // animations initialization....
    Texture texture = animations[0].GetFrame();
    size = {static_cast<float>(texture.width) / IMAGE_WIDTH, static_cast<float>(texture.height) / IMAGE_WIDTH};
    // create the body
    std::vector<b2Vec2> vertices = {
        b2Vec2{0.0f, 0.0f},
        b2Vec2{size.x, 0.0f},
        b2Vec2{size.x, size.y},
        b2Vec2{0.0f, size.y}
    };
    MyBoundingBox::createBody(body, b2_dynamicBody, vertices, Vector2{position.x, position.y}, restitution);
}

void ActiveItem::Draw() {
    // draw the grow item
    b2Vec2 pos = body->GetPosition();
    Texture texture = animations[0].GetFrame();
    Rectangle sourceRect = { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) };
    Renderer::DrawPro(texture, sourceRect, Vector2{pos.x, pos.y}, Vector2{size.x, size.y}, false, angle);
}

void ActiveItem::Draw(Vector2 position, float angle) {
    // draw the grow item
}

void ActiveItem::Update(Vector2 playerVelocity, float deltaTime) {
    elapsedTime += deltaTime;
    animations[0].Update(deltaTime);
}

void ActiveItem::HandleInput() {}

void ActiveItem::OnBeginContact(SceneNode* other, b2Vec2 normal) {
    // handle the begin contact of the grow item
}

void ActiveItem::OnEndContact(SceneNode* other) {
    // handle the end contact of the grow item
}


Mushroom::Mushroom() : ActiveItem() {}

Mushroom::Mushroom(int value, Vector2 size, float speed, float angle, vector<Image> images): 
    ActiveItem(value, size, speed, angle, images) {}

Mushroom::Mushroom(const Mushroom &m): ActiveItem(m) {}

Mushroom::~Mushroom() {}

void Mushroom::Init(b2Vec2 position, ImageSet imageSet) {
    // initialize the mushroom
    animations = AnimationHandler::setAnimations("mushroom");
    ActiveItem::Init(position, imageSet);
}

void Mushroom::OnBeginContact(SceneNode* other, b2Vec2 normal) {
    // handle the begin contact of the mushroom
    Player* player = dynamic_cast<Player*>(other);
    if (player != nullptr) {
        player->changeMode(BIG);
        Physics::bodiesToDestroy.push_back(body);
        animations.clear();
        images.clear();
    }
}

void Mushroom::OnEndContact(SceneNode* other) {
    // handle the end contact of the mushroom
}

MovingObject* Mushroom::copy() const {
    return new Mushroom(*this);
}

FireFlower::FireFlower() : ActiveItem() {}

FireFlower::FireFlower(int value, Vector2 size, float speed, float angle, vector<Image> images): 
    ActiveItem(value, size, speed, angle, images) {}

FireFlower::FireFlower(const FireFlower &ff): ActiveItem(ff) {}

FireFlower::~FireFlower() {}

void FireFlower::Init(b2Vec2 position, ImageSet imageSet) {
    // initialize the fire flower
    animations = AnimationHandler::setAnimations("fireflower");
    ActiveItem::Init(position, imageSet);
}


void FireFlower::OnBeginContact(SceneNode* other, b2Vec2 normal) {
    Player* player = dynamic_cast<Player*>(other);
    if (player != nullptr) {
        player->changeMode(FIRE);
        Physics::bodiesToDestroy.push_back(body);
        animations.clear();
        images.clear();
    }
}

void FireFlower::OnEndContact(SceneNode* other) {
    // handle the end contact of the fire flower
}

MovingObject* FireFlower::copy() const {
    return new FireFlower(*this);
}

Star::Star() : ActiveItem() {}

Star::Star(int value, Vector2 size, float speed, float angle, vector<Image> images): 
    ActiveItem(value, size, speed, angle, images) {
        restitution = 1.0f;
    }

Star::Star(const Star &s): ActiveItem(s) {}

Star::~Star() {}

void Star::Init(b2Vec2 position, ImageSet imageSet) {
    // initialize the star
    animations = AnimationHandler::setAnimations("star");
    ActiveItem::Init(position, imageSet);
}

void Star::OnBeginContact(SceneNode* other, b2Vec2 normal) {
    // handle the begin contact of the star
}

void Star::OnEndContact(SceneNode* other) {
    // handle the end contact of the star
}

MovingObject* Star::copy() const {
    return new Star(*this);
}

