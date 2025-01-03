#include "include.h"
#include "game.h"
#include "object.h"
#include "character.h"

Player::Player() : Character()
{
    name = "";
    currentMap = "";
    immortal = false;
    allowInput = true;  
    coins = 0;
    lives = 0;
    time = 0;
    force = -12.0f;
    bulletSpeed = 9.0f;
    bulletFreq = 0.75f;
    mode = Mode::SMALL;
}

Player::Player(string type, string name, float coins, int lives, int health, 
               int score, int level, int strength, Vector2 size, float speed, 
               float angle): 
    Character(type, health, score, level, strength, size, speed, angle), 
    name(name), 
    coins(coins),  
    lives(lives)
{
    this->currentMap = "";
    this->alive = true;
    this->mode = Mode::SMALL;
    this->elapsedTime = 0.0f;
    this->allowInput = true;
    this->jump = false;

    if (type == "mario") {
        this->time = 300.0f;
        this->speed = 8.5f;
        force = -27.0f;
        bulletSpeed = 9.0f;
        bulletFreq = 0.37f;
    }
    else {
        this->time = 300.0f;
        this->speed = 7.5f;
        force = -29.0f;
        bulletSpeed = 8.0f;
        bulletFreq = 0.30f;
    }

}

Player::Player(const Player &p): 
    Character(p), 
    name(p.name), 
    coins(p.coins), 
    lives(p.lives),
    currentMap(p.currentMap),
    time(p.time)
{
}

Player::~Player() {
    if (this) {
        name = "";
        coins = 0;
        lives = 0;
        time = 0;
    }
}

void Player::setAddScore(int s)
{
    addScore = s;
}

void Player::setPositon(b2Vec2 pos)
{
    // if (body) body->SetTransform(pos, body->GetAngle());
    position = Vector2{pos.x, pos.y};
}

void Player::setPositionBody(b2Vec2 pos)
{
    if (body) body->SetTransform(pos, body->GetAngle());
    position = Vector2{pos.x, pos.y};
}

void Player::setName(string n) {
    name = n;
}

void Player::setCoins(float c) {
    coins = c;
}

void Player::setLives(int lives) {
    this->lives = lives;
}

void Player::setImmortal(bool im) {
    immortal = im;
}

void Player::setImmortalTime(float it)
{
    immortalTime = it;
}

void Player::setCurrentMap(string map) {
    currentMap = map;
}

void Player::setTime(float t) {
    time = t;
}

void Player::setForce(float f) {
    force = f;
}

void Player::setBulletSpeed(float bs) {
    bulletSpeed = bs;
}

void Player::setBulletFreq(float bf) {
    bulletFreq = bf;
}

void Player::setInitialPosition(Vector2 pos)
{
    initialPosition = pos;
}

void Player::setAllowInput(bool state) {
    allowInput = state;
}

void Player::setJump(bool j) {
    jump = j;
}

void Player::impulseForce(Vector2 force) { 
    body->ApplyLinearImpulseToCenter(b2Vec2(force.x, force.y), true);
}

void Player::updateScore(int s) {
    score += s;
}

void Player::updateScore()
{
    score += addScore;
    addScore = 0;
}
void Player::SetIsOnGround(bool state) { 
    isOnGround = state; 
}

void Player::SetWalkingOnPlatform(bool state){ 
    isWalkingOnPlatform = state; 
}

int Player::getAddScore()
{
    return addScore;
}

int Player::getLives() {
    return lives;
}

string Player::getName()
{
    return name;
}

float Player::getCoins() {
    return coins;
}

string Player::getCurrentMap() {
    return currentMap;
}

Vector2 Player::getInitialPosition() {
    return initialPosition;
}

float Player::getTime() {
    return time;
}

bool Player::isJump() {
    return jump;
}

float Player::getForce() {
    return force;
}

float Player::getBulletSpeed() {
    return bulletSpeed;
}

float Player::getBulletFreq() {
    return bulletFreq;
}

bool Player::isImmortal() {
    return immortal;
}

bool Player::isAllowInput() {
    return allowInput;
}

void Player::HandleInput() {
    if (!isAlive() or !allowInput) {
        return;
    }
    if (IsKeyDown(inputSet[PlayerInput::RIGHT])) {
        body->SetLinearVelocity(b2Vec2(speed, body->GetLinearVelocity().y));
        if (currentImage != JUMP) {
            previousImage = currentImage;
            currentImage = ImageSet::WALK;   
        }
        faceLeft = false;

    } 
    else 
    if (IsKeyDown(inputSet[PlayerInput::LEFT])) {
        body->SetLinearVelocity(b2Vec2(-speed, body->GetLinearVelocity().y));
        if (currentImage != JUMP) {
            previousImage = currentImage;
            currentImage = ImageSet::WALK;
        }
        faceLeft = true;

    }
    else 
    if (IsKeyDown(inputSet[PlayerInput::DOWN])) {
        body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));
        if (currentImage != JUMP) {
            previousImage = currentImage;
            currentImage = DUCK;
        }

    }
    else {
        body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));
        if (currentImage != JUMP) {
            previousImage = currentImage;
            currentImage = IDLE;
        }
    }

    if (isOnGround && currentImage == JUMP) {
        previousImage = currentImage;
        currentImage = IDLE;
    }

    if (jump == true) {
        jump = false;
    }
    
    if (IsKeyPressed(inputSet[PlayerInput::UP]) || IsKeyPressed(inputSet[PlayerInput::UP2])) {
        if (isOnGround) {
            if (mode == SMALL) {
                playSoundEffect(SoundEffect::JUMP);
                body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, force), true);
            }
            else if (mode == BIG or mode == FIRE) {
                playSoundEffect(SoundEffect::JUMP_SUPER);
                body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, force * 1.5f), true);
            }
            previousImage = currentImage;
            currentImage = ImageSet::JUMP;
            isOnGround = false;
            jump = true;
        }
    }



    // default frequency of the bullet: 0.4 seconds
    if (IsKeyPressed(inputSet[PlayerInput::SHOOT]) && mode == Mode::FIRE) {
        previousImage = currentImage;
        currentImage = HOLD;
        animations[currentImage].setTimer();
        if (elapsedTime >= bulletFreq) {
            // sound effect
            playSoundEffect(SoundEffect::SHOOT_FIREBALL);
            // create a fireball
            FireBall* fireball = new FireBall(10.0f, {0.5f, 0.5f}, 5.0f, 0.0f);
            fireball->Init(body->GetPosition() + b2Vec2(!faceLeft * ((float)texture.width/16 + 0.1f), texture.height/32));
            b2Fixture* fixture = fireball->getBody()->GetFixtureList();
            fixture->SetDensity(2.0f);
            fireball->setSpeed(10.0f * (faceLeft ? -1 : 1) + body->GetLinearVelocity().x);

            Tilemap* tilemap = Tilemap::getInstance();
            tilemap->addNode(fireball);
            elapsedTime = 0.0f;

            fireball->setPlayerShot(this);
        }
    }

    if (!isOnGround) {
        previousImage = currentImage;
        currentImage = ImageSet::JUMP;
    }
}

void Player::Update(Vector2 playerVelocity, float deltaTime) {
    if (body) {
        Character::Update(playerVelocity, deltaTime);
        b2Vec2 vel = body->GetLinearVelocity();
        if (vel.x != 0) {
            if (isOnGround && currentImage != JUMP) {
                previousImage = currentImage;
                currentImage = WALK;
            }
        }
        
        if (vel.x > 0) {
            faceLeft = false;
        }
        else if (vel.x < 0) {
            faceLeft = true;
        }
    }    

    if (time <= 0) {
        cout << "Time's up!" << endl;
        alive = false;
        time = 300.0f;
    }

    if (health <= 0) {
        alive = false;
        health = 100;
    }

    if (body) position = Vector2{body->GetPosition().x, body->GetPosition().y};
    if (Tilemap::getInstance()->GetHeight() < position.y) {
        alive = false;
    }

    if (coins >= 100) {
        lives++;
        coins = coins - 100;
    }

    if (alive) {
        if (StageStateHandler::GetInstance().GetState() != StageState::VICTORY_STATE) 
            time -= deltaTime;
    }
    else {
        Dead();
    }
}

void Player::Dead() {
    cout << "Spawn at: " << spawnPosition.x << ", " << spawnPosition.y << endl;
    EffectManager* effectManager = Tilemap::getInstance()->GetEffectManager();
    if (effectManager->isActivePlayerEffect(this)) {
        return;
    }
    if (!effectManager->isActivePlayerEffect(this)) {
        if (body) {
            b2Vec2 pos = body->GetPosition();
            Physics::world.DestroyBody(body);
            body = nullptr;
            alive = false;
            lives--;
            std::string effectName = "dead_" + type;
            effectManager->AddUpperEffect(AnimationEffectCreator::CreateAnimationEffect(effectName, Vector2{pos.x, pos.y}));
            effectManager->setActivePlayerEffect(this, true);
        }
        else {
            if (lives <= 0) {
                // game over
                Game* game = Game::getInstance();
                StageStateHandler::GetInstance().SetState(StageState::GAME_OVER);
            }
            else if (StageStateHandler::GetInstance().GetState() == StageState::NORMAL_STATE) {
                StageStateHandler::GetInstance().SetState(StageState::PLAYER_DEAD);
            }
            else if (StageStateHandler::GetInstance().GetState() == StageState::PLAYER_DEAD) {
                Init(b2Vec2{spawnPosition.x, spawnPosition.y});
                StageStateHandler::GetInstance().SetState(StageState::NORMAL_STATE);
            }
        }
    }
}

void Player::UpdateAnimation() {
    if (mode == SMALL) {
        animations = AnimationHandler::setAnimations("small" + name);
    }
    else if (mode == BIG) {
        animations = AnimationHandler::setAnimations("big" + name);
    }
    else if (mode == FIRE) {
        animations = AnimationHandler::setAnimations("fire" + name);
    }
}

void Player::Draw() {
    if (body) {
        Vector2 position = Vector2{body->GetPosition().x, body->GetPosition().y};
        if (position.x == spawnPosition.x && position.y == spawnPosition.y) {
            return;
        }
        Character::Draw();
    }
}

void Player::Draw(Vector2 position, float angle) {
    TextHelper::DrawPackage(lives, score, coins, currentMap, max(time - 10, 0.0f), position, 9, WHITE);
}

void Player::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
    if (!other) return;

    if (normal.y > 0.5f) {
        isOnGround = true;
    }

    FireFlower* fireFlower = dynamic_cast<FireFlower*>(other);
    Mushroom* mushroom = dynamic_cast<Mushroom*>(other);
    if (fireFlower || mushroom) {
        if (fireFlower) {
            if (mode == BIG) mode = FIRE;
            else if (mode == SMALL) changeMode(FIRE);
        }
        else if (mushroom) {
            if (mode == FIRE) mode = BIG;
            else if (mode == SMALL) changeMode(BIG);
        }
        if (modeChanged) {            
            b2Vec2 pos = body->GetPosition();
            position = Vector2{pos.x, pos.y - 0.5f};

            EffectManager* effectManager = Tilemap::getInstance()->GetEffectManager();
            std::string effectName = "grow_" + type;
            effectManager->AddUpperEffect(AnimationEffectCreator::CreateAnimationEffect(effectName, Vector2{pos.x, pos.y + size.y}));
            effectManager->setActivePlayerEffect(this, true);
            body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        }
    }
}

void Player::Init(b2Vec2 position) {
    Character::Init(position);
    cout << "Player spawned at: " << position.x << ", " << position.y << endl;
    SetSpawnPosition(Vector2{position.x, position.y});
}

void Player::OnEndContact(SceneNode *other) {
}

void Player::accept(FileVisitor *visitor) {
    visitor->VisitFile(this);
}

MovingObject *Player::copy() const
{
    return new Player(*this);
}

void Player::SetInputSet(vector<int> inputSet) {
    this->inputSet = inputSet;
}

void Player::SetSpawnPosition(Vector2 spawnPos) {
    spawnPosition = spawnPos;
}

void Player::accept(MultiplayerHandlerVisitor *visitor) {
    visitor->VisitPlayer(this);
}

b2Body* Player::getBody() {
    return body;
}

void Player::DrawImageIcon(Vector2 position, float angle) {
    TextHelper::DrawPlayerImageIcon(getTexture(), position);
}

void Player::setScore(int score) {
    this->score = score;
}

int Player::getScore() {
    return score;
}

Texture Player::getTexture() {
    string fileName = "../resources/images/smallmario/idle.png";
    Texture texture = LoadTexture(fileName.c_str());            
    return texture;
}

bool Player::getActiveEffectOnThisPlayer() {
    return isActiveEffectOnThisPlayer;
}

void Player::setActiveEffectOnThisPlayer(bool active) {
    isActiveEffectOnThisPlayer = active;
}