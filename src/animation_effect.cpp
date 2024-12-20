#include "animation_effect.h"
#include "imagehandler.h"
#include "tilemap.h"
#include "moving_object.h"
#include "physics.h"

bool AnimationEffect::isActive()
{
    return active;
}

ScoreEffect::ScoreEffect(Vector2 pos)
{
    position = pos;
    Tilemap* tilemap = Tilemap::getInstance();
    Player* player = tilemap->GetPlayer();
    score = std::to_string(player->getAddScore());
    std::cout << "Score: " << score << std::endl;
}

void ScoreEffect::Update(float deltaTime)
{
    if (!active) return;
    fadeTime += deltaTime;
    if (fadeTime > fadeDuration) {
        float elapsed = fadeTime - fadeDuration;
        alpha = 1.0f -  2 * elapsed / fadeDuration;
        if (alpha <= 0.0f) active = false;
    }
    position.y -= deltaTime * 2;   
}

void ScoreEffect::Draw()
{
    if (!active) return;
    Color color = { 255, 255, 255, (unsigned char)(alpha * 255) };
    TextHelper::Draw(score, position, 6, color);
}


CoinEffect::CoinEffect(Vector2 pos)
{
    animation = AnimationHandler::setAnimations("coin")[0];
    position = pos;
}

void CoinEffect::Update(float deltaTime)
{
    if (!active) return;
    fadeTime += deltaTime;
    if (fadeTime > fadeDuration) {
        float elapsed = fadeTime - fadeDuration;
        alpha = 1.0f -  10 * elapsed / fadeDuration;
        if (alpha <= 0.0f) active = false;
    }
    if (curHeight < height)
    {
        position.y -= deltaTime * 7;
        curHeight += deltaTime * 7;
    }    
    animation.Update(deltaTime);
    texture = animation.GetFrame();
    size = {(float)texture.width / texture.height, 1.0f};

    
}

void CoinEffect::Draw()
{
    if (!active) return;
    Color color = { 255, 255, 255, (unsigned char)(alpha * 255) };
    DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, (float)texture.height}, Rectangle{position.x, position.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, color);
}

MushroomEffect::MushroomEffect(Vector2 pos)
{
    animation = AnimationHandler::setAnimations("mushroom")[0];
    position = pos;
    currentPosition = pos;
    currentPosition.y += 1.0f;
}

void MushroomEffect::Update(float deltaTime)
{
    if (!active) return;
    if (currentPosition.y > position.y)
    {
        currentPosition.y -= deltaTime * 2;
    }
    else {
        active = false;
        Tilemap* tilemap = Tilemap::getInstance();
        Mushroom* mushroom = new Mushroom();
        mushroom->Init(b2Vec2{position.x, position.y});
        tilemap->addNode(mushroom);
    }
}

void MushroomEffect::Draw()
{
    if (!active) return;
    texture = animation.GetFrame();
    size = {(float)texture.width / texture.height, 1.0f};
    DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, (float)texture.height}, Rectangle{currentPosition.x, currentPosition.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}

StarEffect::StarEffect(Vector2 pos)
{
    animation = AnimationHandler::setAnimations("star")[0];
    position = pos;
    currentPosition = pos;
    currentPosition.y += 1.0f;
}

void StarEffect::Update(float deltaTime)
{
    if (!active) return;
    if (currentPosition.y > position.y)
    {
        currentPosition.y -= deltaTime * 2;
    }
    else {
        active = false;
        Tilemap* tilemap = Tilemap::getInstance();
        Star* star = new Star();
        star->Init(b2Vec2{position.x, position.y});
        tilemap->addNode(star);
    }
}

void StarEffect::Draw() 
{
    if (!active) return;
    texture = animation.GetFrame();
    size = {(float)texture.width / texture.height, 1.0f};
    DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, (float)texture.height}, Rectangle{currentPosition.x, currentPosition.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}

FireFlowerEffect::FireFlowerEffect(Vector2 pos)
{
    animation = AnimationHandler::setAnimations("fireflower")[0];
    position = pos;
    currentPosition = pos;
    currentPosition.y += 1.0f;
}

void FireFlowerEffect::Update(float deltaTime)
{
    if (!active) return;
    if (currentPosition.y > position.y)
    {
        currentPosition.y -= deltaTime * 2;
    }
    else {
        active = false;
        Tilemap* tilemap = Tilemap::getInstance();
        FireFlower* fireFlower = new FireFlower();
        fireFlower->Init(b2Vec2{position.x, position.y});
        tilemap->addNode(fireFlower);
    }
}

void FireFlowerEffect::Draw()
{
    if (!active) return;
    texture = animation.GetFrame();
    size = {(float)texture.width / texture.height, 1.0f};
    DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, (float)texture.height}, Rectangle{currentPosition.x, currentPosition.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}

FireballExplodeEffect::FireballExplodeEffect(Vector2 pos)
{
    position = pos;
    animation = AnimationHandler::setAnimations("fireball_explode")[0];
}

void FireballExplodeEffect::Update(float deltaTime)
{
    if (!active) return;
    fadeTime += deltaTime;
    if (fadeTime > fadeDuration) {
        float elapsed = fadeTime - fadeDuration;
        alpha = 1.0f - 2 * elapsed / fadeDuration;
        if (alpha <= 0.0f) active = false;
    }
    scale += deltaTime * 0.25f; // Increase the scale over time
    animation.Update(deltaTime);
    texture = animation.GetFrame();
    size = {(float)texture.width / texture.height * scale, scale};
}

void FireballExplodeEffect::Draw()
{
    if (!active) return;
    Color color = { 255, 255, 255, (unsigned char)(alpha * 255) };
    DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, (float)texture.height}, Rectangle{position.x, position.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, color);
}

BrickExplodeEffect::BrickExplodeEffect(Vector2 pos)
{
    position = pos;
    animation = AnimationHandler::setAnimations("piece_brick")[0];
    texture = animation.GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};
}

void BrickExplodeEffect::Update(float deltaTime)
{
    if (!active) return;
    if (!isCreated) {
        std::vector<b2Vec2> vertices = {
            b2Vec2{0.0f, 0.0f},
            b2Vec2{size.x, 0.0f},
            b2Vec2{0.0f, size.y},
            b2Vec2{size.x, size.y}
        };
        for (int i = 0; i < 4; i++)
        {
            MyBoundingBox::createBody(pieces[i], b2_dynamicBody, vertices, position);
            b2Fixture* fixture = pieces[i]->GetFixtureList();
            fixture->SetSensor(true);
        }
        pieces[0]->ApplyLinearImpulseToCenter(b2Vec2{-3.0f, -15.0f}, true);
        pieces[1]->ApplyLinearImpulseToCenter(b2Vec2{3.0f, -15.0f}, true);
        pieces[2]->ApplyLinearImpulseToCenter(b2Vec2{-3.0f, -10.0f}, true);
        pieces[3]->ApplyLinearImpulseToCenter(b2Vec2{3.0f, -10.0f}, true);
        isCreated = true;
    }
    
    int cnt = 0;
    Tilemap* tilemap = Tilemap::getInstance();
    int heightMap  = tilemap->GetHeight();
    for (int i = 0; i < 4; i++)
    {
        if (pieces[i] != nullptr)
        {
            b2Vec2 pos = pieces[i]->GetPosition();
            if (pos.y > heightMap)
            {
                Physics::world.DestroyBody(pieces[i]);
                pieces[i] = nullptr;
            }
            cnt++;
        }
    }
    if (cnt == 0) active = false;
}

void BrickExplodeEffect::Draw()
{
    if (!active) return;
    int cnt = 0;
    for (int i = 0; i < 4; i++)
    {
        if (pieces[i] != nullptr)
        {
            b2Vec2 pos = pieces[i]->GetPosition();
            DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, (float)texture.height}, Rectangle{pos.x, pos.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
            cnt++;
        }
    }
    if (cnt == 0) active = false;
}

DeadMarioEffect::DeadMarioEffect(Vector2 pos)
{
    position = pos;
    animation = AnimationHandler::setAnimations("smallmario")[6];
    texture = animation.GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};
}

void DeadMarioEffect::Update(float deltaTime)
{
    if (!active) return;
    if (!body) {
        std::vector<b2Vec2> vertices = {
            b2Vec2{0.0f, 0.0f},
            b2Vec2{size.x, 0.0f},
            b2Vec2{0.0f, size.y},
            b2Vec2{size.x, size.y}
        };
        MyBoundingBox::createBody(body, b2_dynamicBody, vertices, position);
        b2Fixture* fixture = body->GetFixtureList();
        fixture->SetSensor(true);
        body->ApplyLinearImpulseToCenter(b2Vec2{0.0f, -20.0f}, true);
    }
    else {
        int heightMap = Tilemap::getInstance()->GetHeight();
        b2Vec2 pos = body->GetPosition();
        if (pos.y > heightMap)
        {
            Physics::world.DestroyBody(body);
            body = nullptr;
            active = false;
        }
    }
}

void DeadMarioEffect::Draw()
{
    if (!active || !body) return;
    b2Vec2 pos = body->GetPosition();
    DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, (float)texture.height}, Rectangle{pos.x, pos.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}

DeadLuigiEffect::DeadLuigiEffect(Vector2 pos)
{
    position = pos;
    animation = AnimationHandler::setAnimations("smallluigi")[6];
    texture = animation.GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};
}

void DeadLuigiEffect::Update(float deltaTime)
{
    if (!active) return;
    if (!body) {
        std::vector<b2Vec2> vertices = {
            b2Vec2{0.0f, 0.0f},
            b2Vec2{size.x, 0.0f},
            b2Vec2{0.0f, size.y},
            b2Vec2{size.x, size.y}
        };
        MyBoundingBox::createBody(body, b2_dynamicBody, vertices, position);
        b2Fixture* fixture = body->GetFixtureList();
        fixture->SetSensor(true);
        body->ApplyLinearImpulseToCenter(b2Vec2{0.0f, -20.0f}, true);
    }
    else {
        int heightMap = Tilemap::getInstance()->GetHeight();
        b2Vec2 pos = body->GetPosition();
        if (pos.y > heightMap)
        {
            Physics::world.DestroyBody(body);
            body = nullptr;
            active = false;
        }
    }
}

void DeadLuigiEffect::Draw()
{
    if (!active || !body) return;
    b2Vec2 pos = body->GetPosition();
    DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, (float)texture.height}, Rectangle{pos.x, pos.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}

DeadKoopaEffect::DeadKoopaEffect(Vector2 pos)
{
    position = pos;
    animation = AnimationHandler::setAnimations("koopa")[1];
    texture = animation.GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};
}

void DeadKoopaEffect::Update(float deltaTime)
{
    if (!active) return;
    if (!body) {
        std::vector<b2Vec2> vertices = {
            b2Vec2{0.0f, 0.0f},
            b2Vec2{size.x, 0.0f},
            b2Vec2{0.0f, size.y},
            b2Vec2{size.x, size.y}
        };
        MyBoundingBox::createBody(body, b2_dynamicBody, vertices, position);
        b2Fixture* fixture = body->GetFixtureList();
        fixture->SetSensor(true);
        body->ApplyLinearImpulseToCenter(b2Vec2{0.0f, -15.0f}, true);
    }
    else {
        int heightMap = Tilemap::getInstance()->GetHeight();
        b2Vec2 pos = body->GetPosition();
        if (pos.y > heightMap)
        {
            Physics::world.DestroyBody(body);
            body = nullptr;
            active = false;
        }
    }
}

void DeadKoopaEffect::Draw()
{
    if (!active || !body) return;
    b2Vec2 pos = body->GetPosition();
    DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, -(float)texture.height}, Rectangle{pos.x, pos.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}

DeadGoombaEffect::DeadGoombaEffect(Vector2 pos)
{
    position = pos;
    animation = AnimationHandler::setAnimations("goomba")[1];
    texture = animation.GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};
}

void DeadGoombaEffect::Update(float deltaTime)
{
    if (!active) return;
    if (!body) {
        std::vector<b2Vec2> vertices = {
            b2Vec2{0.0f, 0.0f},
            b2Vec2{size.x, 0.0f},
            b2Vec2{0.0f, size.y},
            b2Vec2{size.x, size.y}
        };
        MyBoundingBox::createBody(body, b2_dynamicBody, vertices, position);
        b2Fixture* fixture = body->GetFixtureList();
        fixture->SetSensor(true);
        body->ApplyLinearImpulseToCenter(b2Vec2{0.0f, -15.0f}, true);
    }
    else {
        int heightMap = Tilemap::getInstance()->GetHeight();
        b2Vec2 pos = body->GetPosition();
        if (pos.y > heightMap)
        {
            Physics::world.DestroyBody(body);
            body = nullptr;
            active = false;
        }
    }
}

void DeadGoombaEffect::Draw()
{
    if (!active || !body) return;
    b2Vec2 pos = body->GetPosition();
    DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, -(float)texture.height}, Rectangle{pos.x, pos.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}

DeadBossEffect::DeadBossEffect(Vector2 pos)
{
    position = pos;
    animation = AnimationHandler::setAnimations("boss")[0];
    texture = animation.GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};
}

void DeadBossEffect::Update(float deltaTime)
{
    if (!active) return;
    if (!body) {
        std::vector<b2Vec2> vertices = {
            b2Vec2{0.0f, 0.0f},
            b2Vec2{size.x, 0.0f},
            b2Vec2{0.0f, size.y},
            b2Vec2{size.x, size.y}
        };
        MyBoundingBox::createBody(body, b2_dynamicBody, vertices, position);
        b2Fixture* fixture = body->GetFixtureList();
        fixture->SetSensor(true);
        body->ApplyLinearImpulseToCenter(b2Vec2{0.0f, -15.0f}, true);
    }
    else {
        int heightMap = Tilemap::getInstance()->GetHeight();
        b2Vec2 pos = body->GetPosition();
        if (pos.y > heightMap)
        {
            Physics::world.DestroyBody(body);
            body = nullptr;
            active = false;
        }
    }
}

void DeadBossEffect::Draw()
{
    if (!active || !body) return;
    b2Vec2 pos = body->GetPosition();
    DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, -(float)texture.height}, Rectangle{pos.x, pos.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}


SquashDeadGoombaEffect::SquashDeadGoombaEffect(Vector2 pos)
{
    position = pos;
    animation = AnimationHandler::setAnimations("goomba")[1];
    texture = animation.GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / (IMAGE_WIDTH * 2)};
    position.y -= size.y;
}

void SquashDeadGoombaEffect::Update(float deltaTime)
{
    if (!active) return;
    totalTime += deltaTime;
    if (totalTime > effectTime) {
        active = false;
        return;
    }
}

void SquashDeadGoombaEffect::Draw()
{
    if (!active) return;
    DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, (float)texture.height}, Rectangle{position.x, position.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}


GrowMarioEffect::GrowMarioEffect(Vector2 pos)
{
    // Lower left origin
    position = pos;
    currentPostion = pos;
    animation = AnimationHandler::setAnimations("grow_mario")[0];
    texture = animation.GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};
}

void GrowMarioEffect::Update(float deltaTime)
{
    if (!active) return;
    elapsedTime += deltaTime;
    totalTime += deltaTime;
    if (totalTime > effectTime) {
        active = false;
        return;
    }
    if (elapsedTime > appearTime) {
        appear = !appear;
        elapsedTime = 0.0f;
    }
    animation.Update(deltaTime);
    texture = animation.GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};
    position.y = currentPostion.y - size.y;
}

void GrowMarioEffect::Draw()
{
    if (!active || !appear) return;
    DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, (float)texture.height}, Rectangle{position.x, position.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}

GrowLuigiEffect::GrowLuigiEffect(Vector2 pos)
{
    // Lower left origin
    position = pos;
    currentPostion = pos;
    animation = AnimationHandler::setAnimations("grow_luigi")[0];
    texture = animation.GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};
}

void GrowLuigiEffect::Update(float deltaTime)
{
    if (!active) return;
    elapsedTime += deltaTime;
    totalTime += deltaTime;
    if (totalTime > effectTime) {
        active = false;
        return;
    }
    if (elapsedTime > appearTime) {
        appear = !appear;
        elapsedTime = 0.0f;
    }
    animation.Update(deltaTime);
    texture = animation.GetFrame();
    size = {(float)texture.width / IMAGE_WIDTH, (float)texture.height / IMAGE_WIDTH};
    position.y = currentPostion.y - size.y;
}

void GrowLuigiEffect::Draw()
{
    if (!active || !appear) return;
    DrawTexturePro(texture, Rectangle{0, 0, (float)texture.width, (float)texture.height}, Rectangle{position.x, position.y, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}
