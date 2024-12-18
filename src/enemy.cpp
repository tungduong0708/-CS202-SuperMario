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

void Enemy::setState(EnemyState s) {
    state = s;
}

string Enemy::getType() {
    return type;
}

float Enemy::getRange() {
    return range;
}

EnemyState Enemy::getState() {
    return state;
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
    b2Fixture* fixture = body->GetFixtureList();
    b2Filter filter = fixture->GetFilterData();
    filter.categoryBits = CATEGORY_ENEMY;
    filter.maskBits = MASK_ENEMY;
    fixture->SetFilterData(filter);
    // set the category bits


    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    setSpeed(-2.0f);
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
        Init(b2Vec2(position.x, position.y));

        b2Fixture* fixture = body->GetFixtureList();
        b2Filter filter = fixture->GetFilterData();
        filter.categoryBits = CATEGORY_DEFAULT;
        filter.maskBits = MASK_DEFAULT;
        fixture->SetFilterData(filter);
        fixtureChange = false;
    }

    body->SetLinearVelocity(b2Vec2(speed, body->GetLinearVelocity().y));

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

void Enemy::Dead() {}

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


        if (deadByPlayer) {
            EffectManager* effectManager = Tilemap::getInstance()->GetEffectManager();
            effectManager->AddUpperEffect(AnimationEffectCreator::CreateAnimationEffect("squash_dead_goomba", Vector2{pos.x, pos.y + bodySize.y}));
        }
        else {
            EffectManager* effectManager = Tilemap::getInstance()->GetEffectManager();
            effectManager->AddUpperEffect(AnimationEffectCreator::CreateAnimationEffect("dead_goomba", Vector2{pos.x, pos.y}));
        }
        
    }
    else {
        return;
    }
}

void Goomba::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
    if (!other) return;
    if (!alive) return;
    Player* player = dynamic_cast<Player*>(other);
    Enemy* enemy = dynamic_cast<Enemy*>(other);
    FireBall* fireball = dynamic_cast<FireBall*>(other);
    if (fireball || (player && player->isImmortal())) {
        setHealth(getHealth() - 100);
        if (!alive) {
            state = EnemyState::ENEMY_DEAD;
            if (!deadByPlayer and !deadByFireball) {
                deadByFireball = true;
                Dead();
            }
            Tilemap::getInstance()->GetPlayer()->updateScore(100);
        }
    }
    else if (player || enemy) {
        if (abs(normal.x) > 0.75f) {
            if (player) {
                if (player->isInvisible()) {
                    return;
                }
                else if (player->getMode() == Mode::SMALL) {
                    player->setHealth(player->getHealth() - getStrength());
                }
                else if (player->getMode() == Mode::BIG or player->getMode() == Mode::FIRE) {
                    player->changeMode(Mode::SMALL);
                    player->setInvisibleTime(1.5f);
                    b2Body* playerBody = player->getBody();
                    Vector2 pos = player->getPosition();
                    player->setPositon(b2Vec2{pos.x, pos.y});
                    playerBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                }
            }
            else if (enemy) {
                return;
            }
        }
        else if (player) {
            setHealth(getHealth() - 100);
            player->impulseForce(Vector2{0, -20.0f});
            if (!alive) {
                state = EnemyState::ENEMY_DEAD;
                if (!deadByPlayer and !deadByFireball) {
                    size = Vector2{size.x, size.y/4};
                    deadByPlayer = true; 
                }
                Tilemap::getInstance()->GetPlayer()->updateScore(100);
            }
        }
    }
    else if (fireball) {
        setHealth(getHealth() - 100);
        if (!alive) {
            state = EnemyState::ENEMY_DEAD;
            if (!deadByPlayer and !deadByFireball) {
                Dead();
                deadByFireball = true;
            }
            Tilemap::getInstance()->GetPlayer()->updateScore(100);
        }
    }
    else {
        if ((normal.x) < -0.9f) {
            setSpeed(abs(speed));
            faceLeft = false;
        }
        else if ((normal.x) > 0.9f) {
            setSpeed(-abs(speed));
            faceLeft = true;
        }
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
    delay = 0.2f;
    isDelay = false;
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

        playSoundEffect(SoundEffect::HIT_ENEMY);

        EffectManager* effectManager = Tilemap::getInstance()->GetEffectManager();
        effectManager->AddUpperEffect(AnimationEffectCreator::CreateAnimationEffect("dead_koopa", Vector2{pos.x, pos.y}));
    }
    else {
        return;
    }
}

void Koopa::Update(Vector2 playerVelocity, float deltaTime) {
    Enemy::Update(playerVelocity, deltaTime);
    if (isDelay) {
        delay -= deltaTime;
        if (delay <= 0) {
            isDelay = false;
            delay = 0.2f;
        }
    }
}

void Koopa::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
    if (isDelay || !other || !alive || other == this) return;
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
        if (abs(normal.x) > 0.75f) {
            if (player) {
                if (player->isInvisible()) {
                    return;
                }
                else if (player->getMode() == Mode::SMALL) {
                    player->setHealth(player->getHealth() - getStrength());
                }
                else if (player->getMode() == Mode::BIG or player->getMode() == Mode::FIRE) {
                    player->changeMode(Mode::SMALL);
                    player->setInvisibleTime(1.5f);
                    b2Body* playerBody = player->getBody();
                    Vector2 pos = player->getPosition();
                    player->setPositon(b2Vec2{pos.x, pos.y});
                    playerBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                }
            }
            else if (enemy) {
                if (state == ENEMY_SPIN) {
                    enemy->setHealth(enemy->getHealth() - 100);
                    if (!enemy->isAlive()) {
                        enemy->setState(EnemyState::ENEMY_DEAD);
                        enemy->Dead();
                    }
                }
                else {
                    return;
                }
            }
        }
        else {
            player->setOnGround(true);
            // player->impulseForce(Vector2{0, -30.0f});
            if (state == EnemyState::ENEMY_WALK) {
                playSoundEffect(SoundEffect::HIT_ENEMY);
                state = EnemyState::ENEMY_SHELL;
                fixtureChange = true;
                texture = animations[state].GetFrame();
                size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};
                setSpeed(0);
                isDelay = true;
                return;
            }
            if (state == EnemyState::ENEMY_SHELL) {
                state = EnemyState::ENEMY_SPIN;
                setSpeed(15.0f);
                return;
            }
            if (state == EnemyState::ENEMY_SPIN) {
                state = EnemyState::ENEMY_SHELL;
                setSpeed(0);
                return;
            }
        }
    }
    else {
        if (normal.x > 0.9f) {
            setSpeed(-abs(speed));
            faceLeft = true;
        }
        if (normal.x < -0.9f) {
            setSpeed(+abs(speed));
            faceLeft = false;
        }
    }
}

void Koopa::OnEndContact(SceneNode *other) {
}

MovingObject* Koopa::copy() const {
    return new Koopa(*this);
}

Boss::Boss() : Enemy() {
    bulletFreq = 0.0f;
    bulletSpeed = 0.0f;
    player = nullptr;
    bossState = BossState::BOSS_IDLE;
    attackFire = false;
    timer = 0.0f;
}

Boss::Boss(string type, float range, bool alive, int health, int score, int level, 
           int strength, Vector2 size, float speed, float angle): 
    Enemy(type, range, alive, health, score, level, strength, size, speed, angle)
{
    bulletFreq = 0.0f;
    bulletSpeed = 0.0f;
    player = nullptr;
    bossState = BossState::BOSS_IDLE;
    attackFire = true;
    timer = 0.0f;
}

Boss::Boss(const Boss &b): Enemy(b) {
    bulletFreq = b.bulletFreq;
    bulletSpeed = b.bulletSpeed;
    bossState = b.bossState;
    attackFire = b.attackFire;
    player = dynamic_cast<Player*>(b.player->copy());
    timer = b.timer;
}

Boss::~Boss() {
    player = nullptr;
}

void Boss::setBulletFreq(float bf) {
    bulletFreq = bf;
}

void Boss::setBulletSpeed(float bs) {
    bulletSpeed = bs;
}

void Boss::setPlayer(Player* p) {
    player = p;
}

void Boss::setBossState(BossState bs) {
    bossState = bs;
}

float Boss::getBulletFreq() {
    return bulletFreq;
}

float Boss::getBulletSpeed() {
    return bulletSpeed;
}

Player* Boss::getPlayer() {
    return player;
}

BossState Boss::getBossState() {
    return bossState;
}

void Boss::Init(b2Vec2 position) {
    bossState = BossState::BOSS_IDLE;
    animations = AnimationHandler::setAnimations(type);
    Animation attack = animations[BossState::BOSS_ATTACK];
    // for (int i = 0; i < 3; i++) {
    //     timer += attack.getFrameTime(i);
    // }
    timer = 1.0f;

    texture = animations[bossState].GetFrame();
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
    b2Fixture* fixture = body->GetFixtureList();
    b2Filter filter = fixture->GetFilterData();
    filter.categoryBits = CATEGORY_ENEMY;
    filter.maskBits = MASK_ENEMY;
    fixture->SetFilterData(filter);
}

void Boss::Update(Vector2 playerVelocity, float deltaTime) {
    player = Tilemap::getInstance()->GetPlayer();
    if (!body || !player->isAlive()) return;
    Vector2 playerPos = player->getPosition();
    elapsedTime += deltaTime;
    b2Vec2 position = body->GetPosition();
    destRect.x = position.x;
    destRect.y = position.y;

    float diff = position.x - playerPos.x;
    if (abs(diff) > 5.0f) {
        bossState = BossState::BOSS_ATTACK;
    }
    else {
        bossState = BossState::BOSS_WALK;
    }

    if (diff > 0.5f) {
        faceLeft = true;
        setSpeed(-abs(speed));
    }
    else {
        faceLeft = false;
        setSpeed(abs(speed));
    }


    if (bossState == BossState::BOSS_ATTACK) {
        if (elapsedTime >= timer) {
            // assemble the attack ball
            AttackBall* atkball = new AttackBall(10.0f, {0.5f, 0.5f}, 5.0f, 0.0f);
            atkball->Init(body->GetPosition() + b2Vec2(!faceLeft * ((float)texture.width/16 + 0.1f), 0.15f));
            atkball->setSpeed(6.0f * (faceLeft ? -1 : 1));

            Tilemap* tilemap = Tilemap::getInstance();
            tilemap->addNode(atkball);
            elapsedTime = 0.0f;
        }
    }
    else {
        body->SetLinearVelocity(b2Vec2(speed, body->GetLinearVelocity().y));
    }

    animations[bossState].Update(deltaTime);
    texture = animations[bossState].GetFrame();

    if (!alive) {
        bossState = BossState::BOSS_DEAD;
        Dead();
    }
}

void Boss::OnBeginContact(SceneNode *other, b2Vec2 normal) {
    if (!other) return;
    if (!alive) return;
    Player* player = dynamic_cast<Player*>(other);
    FireBall* fireball = dynamic_cast<FireBall*>(other);
    if (player) {
        if (player->isImmortal()) return;
        if (player->getMode() == Mode::SMALL) {
            player->setHealth(player->getHealth() - getStrength());
        }
        else if (player->getMode() == Mode::BIG or player->getMode() == Mode::FIRE) {
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
    else if (fireball) {
        setHealth(getHealth() - 100);
        if (!alive) {
            bossState = BossState::BOSS_DEAD;
            Dead();
        }
    }
    else {
        if (normal.x > 0.9f) {
            setSpeed(-abs(speed));
            faceLeft = true;
        }
        if (normal.x < -0.9f) {
            setSpeed(+abs(speed));
            faceLeft = false;
        }
    }
}

void Boss::OnEndContact(SceneNode *other) {
}

void Boss::Dead() {

}

// void Boss::Draw() {

// }

// void Boss::Draw(Vector2 position, float angle) {

// }

MovingObject *Boss::copy() const {
    return new Boss(*this);
}
