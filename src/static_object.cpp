#include "static_object.h"
#include "character.h"
#include "string.h"
#include "tilemap.h"
#include "game.h"

StaticObject::StaticObject() : body(nullptr) {
}

StaticObject::StaticObject(b2Body *body) : body(body) {
}

StaticObject::StaticObject(std::string type) : body(nullptr), type(type) {
}

StaticObject::StaticObject(const StaticObject &other) : body(nullptr), type(other.type)
{
}

StaticObject::~StaticObject()
{
    if (body)
    {
        Physics::bodiesToDestroy.push_back(body);
        // Physics::world.DestroyBody(body);
        body->GetUserData().pointer = reinterpret_cast<uintptr_t>(nullptr);
        body = nullptr;
    }
}

StaticObject *StaticObject::clone()
{
    return new StaticObject(*this);
}

std::string StaticObject::getType()
{
    return type;
}

b2Body *StaticObject::GetBody()
{
    return body;
}

Vector2 StaticObject::getPosition()
{
    b2Vec2 pos = body->GetPosition();
    return Vector2{pos.x, pos.y};
}

void StaticObject::SetBody(b2Body *body)
{
    this->body = body;
}

void StaticObject::SetType(std::string type)
{
    this->type = type;
}

void StaticObject::Update(Vector2 playerVelocity, float deltaTime)
{
}

void StaticObject::Draw()
{
}

void StaticObject::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
}

void StaticObject::OnEndContact(SceneNode *other)
{
}

void StaticObject::accept(FileVisitor *visitor)
{
}

Gate::Gate() : StaticObject()
{
    elapsedTime = 0.0f;
    delay = 6.5f;
    start = false;
    prevVolume = 0.0f;
    prevSpeed = 0.0f;
}

Gate::Gate(b2Body *body, std::string addressNext) : StaticObject(body), addressNext(addressNext)
{
    elapsedTime = 0.0f;
    delay = 6.5f;
    start = false;
}

Gate::~Gate()
{
    // bnojln
}

std::string Gate::getAddressNext() const
{
    return addressNext;
}

void Gate::setAddressNext(std::string addressNext)
{
    this->addressNext = addressNext;
}

void Gate::Init(std::vector<b2Vec2> vertices, Vector2 position, std::string addressNext)
{
    this->addressNext = addressNext;
    b2Body* body = GetBody();
    MyBoundingBox::createBody(body, b2_staticBody, vertices, Vector2{position.x, position.y});
    b2Fixture* fixture = body->GetFixtureList();
    fixture->SetSensor(true);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    SetBody(body);
}

void Gate::Update(Vector2 playerVelocity, float deltaTime)
{
    if (!start) return;
    elapsedTime += deltaTime;
    Player* player = Tilemap::getInstance()->GetPlayer();
    if (elapsedTime > delay) {
        elapsedTime = 0.0f;
        Tilemap::getInstance()->SetNewMapPath(addressNext);
        if (StageStateHandler::GetInstance().GetState() == StageState::STAGE_CLEAR) {
            StageStateHandler::GetInstance().SetState(StageState::NEW_STAGE);
        }
        else if (StageStateHandler::GetInstance().GetState() == StageState::WORLD_CLEAR) {
            StageStateHandler::GetInstance().SetState(StageState::NEW_WORLD);
        }

        player->setAllowInput(true);
        player->setAppear(true);
        Game* game = Game::getInstance();
        game->getSettings().volume = prevVolume;
        player->setSpeed(prevSpeed);
        start = false;

        Game::getInstance()->changeState(Game::getInstance()->changeStageState.get());
    }
    else {
        float ratio = player->getTime() / (delay) * deltaTime;
        if (player->getTime() > 10 and start) {
            player->setTime(player->getTime() - ratio*5);
            player->setScore(player->getScore() + ratio*100);
        }
        cout << "Time: " << player->getTime() << endl;
    }
}

void Gate::Draw()
{
}

void Gate::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
    Player* player = dynamic_cast<Player*>(other);
    if (player != nullptr) {
        Tilemap* tilemap = Tilemap::getInstance();
        if (addressNext[7] == '-' && tilemap->GetCurrentMapPath()[7] == '-') {
            tilemap->SetNewMapPath(addressNext);
            // StageStateHandler::GetInstance().SetState(StageState::WORLD_CLEAR);
            // playSoundEffect(SoundEffect::WORLD_CLEAR);
        } else {
            start = true;
            elapsedTime = 0.0f;

            StageStateHandler::GetInstance().SetState(StageState::STAGE_CLEAR);
            playSoundEffect(SoundEffect::STAGE_CLEAR);

            player->setAllowInput(false);
            player->setAppear(false);
            prevSpeed = player->getSpeed();
            player->setSpeed(0.0f);

            Game* game = Game::getInstance();
            prevVolume = game->getSettings().volume;
            game->getSettings().volume = 0.0f;
        }
    }
}

void Gate::OnEndContact(SceneNode *other)
{
}

void Gate::accept(FileVisitor *visitor)
{
}

DeadLine::DeadLine(b2Body *body) : StaticObject(body)
{
}

void DeadLine::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
    Player *player = dynamic_cast<Player *>(other);
    if (player != nullptr)
    {
        player->setHealth(player->getHealth() - 1000);
    }
}

void DeadLine::OnEndContact(SceneNode *other)
{
}

void DeadLine::accept(FileVisitor *visitor)
{
}

Pole::Pole() : StaticObject()
{
    activated = false;
    activating = false;
    flagOnGround = false;
    speed = 1.0f;
    height = 0.0f;
}

Pole::Pole(b2Body *body) : StaticObject(body)
{
}

void Pole::addFlag(Flag *flag)
{
    this->flag = flag;
}

void Pole::Init(std::vector<b2Vec2> vertices, b2Vec2 position)
{
    height = vertices[2].y;
    b2Body *body = GetBody();
    MyBoundingBox::createBody(body, b2_staticBody, vertices, Vector2{position.x, position.y});
    b2Fixture *fixture = body->GetFixtureList();
    fixture->SetSensor(true);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    SetBody(body);
}

void Pole::Update(Vector2 playerVelocity, float deltaTime)
{
    if (activated) return;
    if (activating && (!flagOnGround || !playerOnGround))
    {
        Vector2 flagPos = flag->getPosition();
        Vector2 polePos = getPosition();
        if (playerTouching != NULL) {
            Player* player = playerTouching;
            Vector2 playerPos = player->getPosition();
            if (playerPos.y + player->getSize().y + 0.5f >= polePos.y + height)
            {
                playerOnGround = true;
            }
            else if (!playerOnGround) {
                player->setPositionBody(b2Vec2{playerPos.x, playerPos.y});
            }
            if (flagPos.y + flag->getSize().y + 0.5f >= polePos.y + height)
            {
                flagOnGround = true;
            }
            else if (!flagOnGround) {
                flag->setPosition(Vector2{flagPos.x, flagPos.y + 5.0f * speed * deltaTime});
            }
        }

        if (playerTouching2 != NULL) {
            Player* player = playerTouching;
            Vector2 playerPos = player->getPosition();
            if (playerPos.y + player->getSize().y + 0.5f >= polePos.y + height)
            {
                playerOnGround = true;
            }
            else if (!playerOnGround) {
                player->setPositionBody(b2Vec2{playerPos.x, playerPos.y});
            }
            if (flagPos.y + flag->getSize().y + 0.5f >= polePos.y + height)
            {
                flagOnGround = true;
            }
            else if (!flagOnGround) {
                flag->setPosition(Vector2{flagPos.x, flagPos.y + 5.0f * speed * deltaTime});
            }
        }
        if (flagOnGround && playerOnGround) {
            activating = false;
            activated = true;
            if (playerTouching != NULL) {
                playerTouching->setCurrentImage(ImageSet::WALK);
                playerTouching->setAllowInput(true);
                playerTouching->getBody()->SetGravityScale(1.0f);
                playerTouching->setSpeed(prevPlayerSpeed);
                playerTouching = NULL;
            }
            if (playerTouching2 != NULL) {
                playerTouching2->setCurrentImage(ImageSet::WALK);
                playerTouching2->setAllowInput(true);
                playerTouching2->getBody()->SetGravityScale(1.0f);
                playerTouching2->setSpeed(prevPlayerSpeed);
                playerTouching2 = NULL;
            }
        }
    }
}

void Pole::OnBeginContact(SceneNode *other, b2Vec2 normal)
{
    if (activated) return;   
    Player *player = dynamic_cast<Player *>(other);
    if (player != nullptr)
    {
        if (playerTouching == NULL) {
            playerTouching = player;
        }
        else if (playerTouching2 == NULL) {
            playerTouching2 = player;
        }
        if (!activating) {
            playSoundEffect(SoundEffect::FLAG_POLE_DOWN);
        }
        activating = true;
        Vector2 playerPos = player->getPosition();
        Vector2 polePos = getPosition();
        polePos.y += height;
        float scoreRatio = 5000.0f / height;
        player->setAddScore(int((polePos.y - playerPos.y) * scoreRatio)/ 10 * 10);
        EffectManager* effectManager = Tilemap::getInstance()->GetEffectManager();
        effectManager->AddUpperEffect(AnimationEffectCreator::CreateAnimationEffect("score", Vector2{polePos.x, polePos.y}));
        player->updateScore();
        player->setAllowInput(false);
        player->setCurrentImage(ImageSet::HOLD);

        float gravity = player->getBody()->GetGravityScale();
        player->getBody()->SetGravityScale(0.3f);
        prevPlayerSpeed = player->getSpeed();
        player->setSpeed(0.0f);
    }
}

void Pole::OnEndContact(SceneNode *other)
{
}

void Pole::accept(FileVisitor *visitor)
{
}
