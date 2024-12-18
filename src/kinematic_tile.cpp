#include "include.h"
#include "object.h"
#include "kinematic_tile.h"

const float BOUNCE_HEIGHT = 0.3f;

KinematicTile::KinematicTile(int id, std::string type, std::string tilesetName, const std::vector<std::pair<int, int>> &frames)
    : Tile(id, type, tilesetName), frames(frames)
{
}

KinematicTile::KinematicTile(int id, Vector2 pos, std::string type, std::string tilesetName, const std::vector<std::pair<int, int>> &frames)
    : Tile(id, pos, type, tilesetName), frames(frames)
{
}

KinematicTile::KinematicTile(KinematicTile& other) 
    : Tile(other), 
      currentFrameId(other.currentFrameId),
      elapsedTime(other.elapsedTime),
      frames(other.frames) 
{
    std::vector<b2Vec2> vertices = TilesetHandler::getBoxVertices(Tile::getTilesetPath(), getId());
    if (!vertices.empty()) {
        if (getType() == "blind_box") {
            Vector2 pos = getPosition();
            b2Body* solidBody = GetBody();

            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(pos.x, pos.y - BOUNCE_HEIGHT);
            bodyDef.fixedRotation = true;
            invisibleBody = Physics::world.CreateBody(&bodyDef);

            MyBoundingBox::createBody(solidBody, b2_dynamicBody, vertices, pos);
            solidBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
            SetBody(solidBody);

            b2PrismaticJointDef jointDef;
            jointDef.bodyB = solidBody;
            jointDef.bodyA = invisibleBody;
            jointDef.collideConnected = false;  
            jointDef.localAxisA = b2Vec2(0.0f, -1.0f);
            jointDef.localAnchorA = b2Vec2(0.5f, 0.5f);
            jointDef.localAnchorB = b2Vec2(0.5f, 0.5f);
            jointDef.enableLimit = true;
            jointDef.lowerTranslation = -BOUNCE_HEIGHT;
            jointDef.upperTranslation = 0.0f;
            joint = (b2PrismaticJoint*)Physics::world.CreateJoint(&jointDef);
        } else {
            b2Body* body = GetBody();
            MyBoundingBox::createBody(body, b2_staticBody, vertices, getPosition());

            if (getType() == "coin") {
                b2Fixture* fixture = body->GetFixtureList();
                fixture->SetSensor(true);
            }

            body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
            SetBody(body);
        }
    }
}

StaticObject *KinematicTile::clone()
{
    return new KinematicTile(*this);
}

KinematicTile::~KinematicTile()
{
    if (invisibleBody) {
        Physics::world.DestroyBody(invisibleBody);
        invisibleBody = nullptr;
    }
    if (getType() == "coin") {
        std::cout << "Coin deleted\n";
    }
}

std::vector<std::pair<int, int>> KinematicTile::getFrames()
{
    return frames;
}

void KinematicTile::setPosition(const Vector2 &position)
{
    Tile::setPosition(position);
}

void KinematicTile::Update(Vector2 playerVelocity, float deltaTime)
{
    if (frames.empty()) return;

    elapsedTime += deltaTime;
    std::pair<int, int> currentFrame = frames[currentFrameId];

    if (elapsedTime >= currentFrame.second / 1000.0f) { // Duration in milliseconds
        elapsedTime = 0.0f;
        currentFrameId = (currentFrameId + 1) % frames.size(); // Loop back to start
    }

    b2Body* body = GetBody();
    Vector2 pos = getPosition();
    std::string type = getType();
    if (!animation) {
        if (type == "blind_box") {
            if (std::fmod(pos.y, 1.0f) == 0.0f && frames.size() == 1 && body->GetType() == b2_dynamicBody)  {
                body->SetType(b2_staticBody);
            }
        } else if (type == "coin") {
            frames.clear();
        }
    }
}

void KinematicTile::Draw()
{
    if (frames.empty()) return;
    std::string tilesetPath = Tile::getTilesetPath();
    int columns = TilesetHandler::getColumns(tilesetPath);
    int spacing = TilesetHandler::getSpacing(tilesetPath);
    int id = frames[currentFrameId].first;
    int src_x = (id % columns) * (TILE_SIZE + spacing);
    int src_y = (id / columns) * (TILE_SIZE + spacing);

    if (getType() != "blind_box") {
        b2Body* body = GetBody();
        if (getType() == "coin") {
            MyBoundingBox::updateFixture(body, TilesetHandler::getBoxVertices(tilesetPath, id), true);
        } else {
            MyBoundingBox::updateFixture(body, TilesetHandler::getBoxVertices(tilesetPath, id));
        }
    }
    
    Rectangle srcRect = { static_cast<float>(src_x), static_cast<float>(src_y), 
                        static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) };

    Renderer::DrawPro(TilesetHandler::getTexture(tilesetPath), srcRect, getPosition(), Vector2{ 1, 1 }, true);
}

void KinematicTile::OnBeginContact(SceneNode* other, b2Vec2 normal)
{
    if (!other) return;
    Vector2 pos = getPosition();
    Player* playerPtr = dynamic_cast<Player*>(other); 
    if (playerPtr != nullptr && animation) {
        if (getType() == "blind_box") {
            if (normal.y < -0.5f) {
                pos.y--;
                EffectManager* effectManager = Tilemap::getInstance()->GetEffectManager();
                std::string effectName = effectManager->GetEffectName({pos.x, pos.y});
                if (effectName == "mushroom_fireflower") {
                    playSoundEffect(SoundEffect::POWER_UP_APPEAR);
                    if (playerPtr->getMode() == Mode::SMALL) {
                        effectManager->AddLowerEffect(AnimationEffectCreator::CreateAnimationEffect("mushroom", pos));
                    } else if (playerPtr->getMode() == Mode::BIG || playerPtr->getMode() == Mode::FIRE) {
                        effectManager->AddLowerEffect(AnimationEffectCreator::CreateAnimationEffect("fireflower", pos));
                    }
                }
                else effectManager->AddLowerEffect(AnimationEffectCreator::CreateAnimationEffect(effectName, pos));

                if (effectName == "coin") {
                    playSoundEffect(SoundEffect::COIN_GRAB);
                    playerPtr->setAddScore(200);
                    playerPtr->setCoins(playerPtr->getCoins() + 1);
                    effectManager->AddUpperEffect(AnimationEffectCreator::CreateAnimationEffect("score", pos));
                    playerPtr->updateScore();
                }
                
                if (effectManager->UpdateEffectCount({pos.x, pos.y})) {
                    Tile::setTilesetPath("resources/tilesets/OverWorld.json");
                    Tile::setId(2);
                    frames.clear();
                    frames.push_back({2, 0});
                    animation = false;

                    std::cout << "Write to file done\n";
                    ExportFileVisitor* visitor = ExportFileVisitor::getInstance();
                    visitor->openFile();
                    accept(visitor);
                    visitor->closeFile();
                }
            }
        }
        else if (getType() == "coin") {
            playSoundEffect(SoundEffect::COIN_GRAB);
            b2Fixture* fixture = GetBody()->GetFixtureList();
            if (fixture->IsSensor()) {
                animation = false;
                Physics::bodiesToDestroy.push_back(GetBody());
                SetBody(nullptr);
                playerPtr->setAddScore(100);
                playerPtr->setCoins(playerPtr->getCoins() + 1);
                EffectManager* effectManager = Tilemap::getInstance()->GetEffectManager();
                effectManager->AddUpperEffect(AnimationEffectCreator::CreateAnimationEffect("score", pos));
                playerPtr->updateScore();
            }
        }
    }
    
}

void KinematicTile::OnEndContact(SceneNode* other)
{
}

void KinematicTile::accept(FileVisitor *v)
{
    v->VisitFile(this);
}
