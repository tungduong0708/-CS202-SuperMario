#include "tilemap.h"
#include "static_tile.h"
#include "tileset_handler.h"
#include "renderer.h"
#include "scene_node.h"
#include "moving_object.h"
#include "my_bounding_box.h"
#include "animation_effect_creator.h"
#include "effect_manager.h"
#include "character.h"

const float BOUNCE_HEIGHT = 0.3f;

StaticTile::StaticTile(int id, std::string type, std::string tilesetName) : Tile(id, type, tilesetName)
{
}

StaticTile::StaticTile(int id, Vector2 pos, std::string type, std::string tilesetName)
    : Tile(id, pos, type, tilesetName)
{
}

StaticTile::StaticTile(StaticTile& other) : Tile(other)
{
    std::vector<b2Vec2> vertices = TilesetHandler::getBoxVertices(Tile::getTilesetPath(), getId());
    if (!vertices.empty()) {
        std::string type = getType();
        if (type == "brick" || type == "blind_box") {
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
            isActivated = false;
        }
        else {
            b2Body* body = GetBody();
            MyBoundingBox::createBody(body, b2_staticBody, vertices, getPosition());
            body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
            SetBody(body);
        }
    }
}

StaticObject *StaticTile::clone()
{
    return new StaticTile(*this);
}

StaticTile::~StaticTile()
{
    if (invisibleBody) {
        Physics::world.DestroyBody(invisibleBody);
        invisibleBody = nullptr;
    }
    
}

void StaticTile::setPosition(const Vector2& position)
{
    Tile::setPosition(position);
}

void StaticTile::Update(Vector2 playerVelocity, float deltaTime)
{
    if (isActivated && getType() == "brick") {
        b2Body* body = GetBody();
        Vector2 pos = getPosition();
        if (std::fmod(pos.y, 1.0f) == 0.0f && body->GetType() == b2_dynamicBody)  {
            body->SetType(b2_staticBody);
        }
    }
}

void StaticTile::Draw()
{
    if (!isDestroyed) return;
    std::string tilesetPath = Tile::getTilesetPath();
    int columns = TilesetHandler::getColumns(tilesetPath);
    int spacing = TilesetHandler::getSpacing(tilesetPath);
    int src_x = (getId() % columns) * (TILE_SIZE + spacing);
    int src_y = (getId() / columns) * (TILE_SIZE + spacing);
    
    Rectangle srcRect = { static_cast<float>(src_x), static_cast<float>(src_y), 
                        static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) };
    Renderer::DrawPro(TilesetHandler::getTexture(tilesetPath), srcRect, getPosition(), Vector2{ 1, 1 }, true);
	// Physics::DebugDraw();
}

void StaticTile::OnBeginContact(SceneNode* other, b2Vec2 normal)
{
    if (!other) return;
    Player* playerPtr = dynamic_cast<Player*>(other); 
    if (playerPtr != nullptr) {
        if (getType() == "brick") {
            if (playerPtr->getMode() == Mode::FIRE || playerPtr->getMode() == Mode::BIG || playerPtr->isImmortal()) {
                if (normal.y > 0.5f) {
                    EffectManager* effectManager = Tilemap::getInstance()->GetEffectManager();
                    effectManager->AddUpperEffect(AnimationEffectCreator::CreateAnimationEffect("brick_explode", getPosition()));
                    Physics::bodiesToDestroy.push_back(GetBody());
                    SetBody(nullptr);
                    isDestroyed = false;
                }
            }
            else {
                if (normal.y > 0.5f && !isActivated) {
                    Vector2 pos = getPosition();
                    pos.y--;
                    EffectManager* effectManager = Tilemap::getInstance()->GetEffectManager();
                    std::string effectName = effectManager->GetEffectName({pos.x, pos.y});
                    effectManager->AddLowerEffect(AnimationEffectCreator::CreateAnimationEffect(effectName, pos));
                    if (effectName == "coin") {
                        playerPtr->updateScore(200);
                        playerPtr->setCoins(playerPtr->getCoins() + 1);
                        
                        if (effectManager->UpdateEffectCount({pos.x, pos.y})) {
                            Tile::setTilesetPath("resources/tilesets/OverWorld.json");
                            Tile::setId(2);
                            isActivated = true;
                        }
                    }
                }
            }
        }
    }
}

void StaticTile::OnEndContact(SceneNode* other)
{
}
