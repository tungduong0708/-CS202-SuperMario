#include "include.h"
#include "object.h"
#include "character.h"

Enemy::Enemy() : Character() {
    type = "";
    range = 0;
    state = EnemyState::ENEMY_WALK;
    fixtureChange = false;
    deadByPlayer = false;
    deadByFireball = false;
}


Enemy::Enemy(string type, float range, bool alive, int health, int score, int level, 
             int strength, Vector2 size, float speed, float angle): 
    Character(type, health, score, level, strength, size, speed, angle), 
    type(type), 
    range(range),
    fixtureChange(false)
{
    state = EnemyState::ENEMY_WALK;
    deadByPlayer = false;
    deadByFireball = false;
}

Enemy::Enemy(const Enemy &e): Character(e) {
    type = e.type;
    range = e.range;
    state = e.state;
    fixtureChange = e.fixtureChange;
    deadByPlayer = e.deadByPlayer;
    deadByFireball = e.deadByFireball;
}

Enemy::~Enemy() {
    type = "";
    range = 0;
    state = EnemyState::ENEMY_WALK;
    fixtureChange = false;
    deadByPlayer = false;
    deadByFireball = false;
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
    animations = AnimationHandler::setAnimations(type);
    texture = animations[state].GetFrame();

    frameWidth = texture.width;
    frameHeight = texture.height;
    sourceRect = {0, 0, (float)frameWidth, (float)frameHeight};

    size = {(float)frameWidth / IMAGE_WIDTH, (float)frameHeight / IMAGE_WIDTH};
    bodySize = size;
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
    if (!body) return;
    elapsedTime += deltaTime;

    b2Vec2 position = body->GetPosition();
    destRect.x = position.x;
    destRect.y = position.y;

    if (fixtureChange) {
        Physics::bodiesToDestroy.push_back(body);
        body = nullptr;
        Init(position);
        fixtureChange = false;
    }


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
}

void Enemy::Draw() {
    if (!body) return;
    b2Vec2 pos = body->GetPosition();
    sourceRect = { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) };
    Vector2 drawPosition = { pos.x, pos.y };
    Renderer::DrawPro(texture, sourceRect, drawPosition, Vector2{size.x, size.y}, faceLeft, 0.0f, bodySize);
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

void Goomba::Dead()
{
    if (body) {
        b2Vec2 pos = body->GetPosition();
        Physics::bodiesToDestroy.push_back(body);
        body = nullptr;
        animations.clear();

        EffectManager* effectManager = Tilemap::getInstance()->GetEffectManager();
        effectManager->AddUpperEffect(AnimationEffectCreator::CreateAnimationEffect("dead_goomba", Vector2{pos.x, pos.y}));
    }
    else {
        return;
    }
}

void Goomba::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
    if (!other) return;
    Player* player = dynamic_cast<Player*>(other);
    Enemy* enemy = dynamic_cast<Enemy*>(other);
    FireBall* fireball = dynamic_cast<FireBall*>(other);
    if (fireball || (player && player->isImmortal())) {
        setHealth(getHealth() - 100);
        if (!alive) {
            state = EnemyState::ENEMY_DEAD;
            if (!deadByPlayer and !deadByFireball) {
                Dead();
                deadByFireball = true;
            }
        }
    }
    else if (player || enemy) {
        if (abs(normal.x) > 0.5f) {
            if (player) {
                player->setHealth(player->getHealth() - getStrength());
            }
            else if (enemy) {
                return;
            }
        }
        else {
            setHealth(getHealth() - 100);
            player->impulseForce(Vector2{0, -10.0f});
            if (!alive) {
                state = EnemyState::ENEMY_DEAD;
                if (!deadByPlayer and !deadByFireball) {
                    size = Vector2{size.x, size.y/4};
                    deadByPlayer = true; 
                }
            }
        }
    }
    else {
        if ((normal.x) > 0.9f) {
            setSpeed(abs(speed));
            faceLeft = false;
        }
        else if ((normal.x) < -0.9f) {
            setSpeed(-abs(speed));
            faceLeft = true;
        }
    }

    if (!alive) {
        player->updateScore(100);
    }

    if (!alive) {
        player->updateScore(100);
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
    faceLeft = true;
}

Koopa::Koopa(const Koopa &k): Enemy(k) {
}

Koopa::~Koopa() {
}

void Koopa::Dead()
{
    if (body) {
        b2Vec2 pos = body->GetPosition();
        Physics::bodiesToDestroy.push_back(body);
        body = nullptr;
        animations.clear();

        EffectManager* effectManager = Tilemap::getInstance()->GetEffectManager();
        effectManager->AddUpperEffect(AnimationEffectCreator::CreateAnimationEffect("dead_koopa", Vector2{pos.x, pos.y}));
    }
    else {
        return;
    }
}

void Koopa::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
    if (!other) return;
    Player* player = dynamic_cast<Player*>(other);
    Enemy* enemy = dynamic_cast<Enemy*>(other);
    FireBall* fireball = dynamic_cast<FireBall*>(other);
    if (fireball || (player && player->isImmortal())) {
        setHealth(getHealth() - 100);
        if (!alive) {
            state = EnemyState::ENEMY_DEAD;
            Dead();
        }
    }
    else if (player || enemy) {
        if (abs(normal.x) > 0.5f) {
            if (player) {
                player->setHealth(player->getHealth() - getStrength());
            }
            else if (enemy) {
                return;
            }
        }
        else {
            player->impulseForce(Vector2{0, -10.0f});
            if (state == EnemyState::ENEMY_WALK) {
                state = EnemyState::ENEMY_SHELL;
                fixtureChange = true;
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
    else {
        if ((normal.x) > 0.9f) {
            setSpeed(abs(speed));
            faceLeft = false;
        }
        else if ((normal.x) < -0.9f) {
            setSpeed(-abs(speed));
            faceLeft = true;
        }
    }

    if (!alive) {
        player->updateScore(100);
    }
}

void Koopa::OnEndContact(SceneNode *other) {
}

MovingObject* Koopa::copy() const {
    return new Koopa(*this);
}

