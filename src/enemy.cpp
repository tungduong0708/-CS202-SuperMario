#include "include.h"
#include "object.h"
#include "character.h"

Enemy::Enemy() : Character() {
    type = "";
    range = 0;
    state = EnemyState::ENEMY_WALK;
}


Enemy::Enemy(string type, float range, bool alive, int health, int score, int level, 
             int strength, Vector2 size, float speed, float angle): 
    Character(type, health, score, level, strength, size, speed, angle), 
    type(type), 
    range(range)
{
}

Enemy::Enemy(const Enemy &e): Character(e) {
    type = e.type;
    range = e.range;
}

Enemy::~Enemy() {
    type = "";
    range = 0;
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

string Enemy::getType() {
    return type;
}

float Enemy::getRange() {
    return range;
}


void Enemy::Init(b2Vec2 position) {
    // define the texture
    state = EnemyState::ENEMY_WALK;
    animations = AnimationHandler::setAnimations(type);
    texture = animations[state].GetFrame();

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

void Enemy::Update(Vector2 playerVelocity, float deltaTime) {
    elapsedTime += deltaTime;
    b2Vec2 position = body->GetPosition();
    destRect.x = position.x;
    destRect.y = position.y;

    animations[state].Update(deltaTime);
    texture = animations[state].GetFrame();

    if (!alive) {
        state = EnemyState::ENEMY_DEAD;
        Dead();
    }
}

void Enemy::OnBeginContact(SceneNode *other, b2Vec2 normal) {
}

void Enemy::OnEndContact(SceneNode *other) {
}

void Enemy::HandleInput() {
}

void Enemy::Dead() {
    // dead...
}

void Enemy::Draw() {
    b2Vec2 pos = body->GetPosition();
    sourceRect = { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) };
    Vector2 drawPosition = { pos.x, pos.y };
    Renderer::DrawPro(texture, sourceRect, drawPosition, Vector2{size.x, size.y}, faceLeft);
}

void Enemy::Draw(Vector2 position, float angle) {
}

MovingObject* Enemy::copy() const {
    return new Enemy(*this);
}

Goomba::Goomba() : Enemy() {
}

Goomba::Goomba(string type, float range, bool alive, bool sit, int health, int score, int level, 
               int strength, Vector2 size, float speed, float angle): 
    Enemy(type, range, alive, health, score, level, strength, size, speed, angle)
{
}

Goomba::Goomba(const Goomba &g): Enemy(g) {
}

Goomba::~Goomba() {
}

void Goomba::OnBeginContact(SceneNode *other, b2Vec2 normal) {
    if (!other) return;
    Player* player = dynamic_cast<Player*>(other);
    Enemy* enemy = dynamic_cast<Enemy*>(other);
    if (normal.x > 0.5f) {
        setSpeed(-getSpeed());
        faceLeft = !faceLeft;
        if (player) {
            player->setHealth(player->getHealth() - getStrength());
        }
        else if (enemy) {
            return;
        }
    }
    else {
        setHealth(getHealth() - 100);
    }
}

void Goomba::OnEndContact(SceneNode *other) {
}

MovingObject* Goomba::copy() const {
    return new Goomba(*this);
}

Koopa::Koopa() : Enemy() {
}

Koopa::Koopa(string type, float range, bool alive, bool sit, int health, int score, int level, 
             int strength, Vector2 size, float speed, float angle): 
    Enemy(type, range, alive, health, score, level, strength, size, speed, angle)
{
}

Koopa::Koopa(const Koopa &k): Enemy(k) {
}

Koopa::~Koopa() {
}

void Koopa::OnBeginContact(SceneNode *other, b2Vec2 normal) {
    if (!other) return;
    Player* player = dynamic_cast<Player*>(other);
    Enemy* enemy = dynamic_cast<Enemy*>(other);
    if (normal.x > 0.5f) {
        setSpeed(-getSpeed());
        faceLeft = !faceLeft;
        if (player) {
            player->setHealth(player->getHealth() - getStrength());
        }
        else if (enemy) {
            return;
        }
    }
    else {
        if (state == EnemyState::ENEMY_WALK) {
            state = EnemyState::ENEMY_SHELL;
            setSpeed(0);
        }
        else if (state == EnemyState::ENEMY_SHELL) {
            state = EnemyState::ENEMY_SPIN;
            setSpeed(20.0f);
        }
        else if (state == EnemyState::ENEMY_SPIN) {
            state = EnemyState::ENEMY_SHELL;
            setSpeed(0);
        }
    }
}

void Koopa::OnEndContact(SceneNode *other) {
}

MovingObject* Koopa::copy() const {
    return new Koopa(*this);
}

