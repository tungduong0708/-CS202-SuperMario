#include "include.h"
#include "object.h"
#include "tilemap.h"

Tilemap* Tilemap::instance = nullptr;

Tilemap::Tilemap() {
    effectManager = new EffectManager();
    player = nullptr;
}

Tilemap::Tilemap(const std::string& filePath) {
    effectManager = new EffectManager();
    player = nullptr;
    LoadMapFromJson(filePath);
    
}

Tilemap::~Tilemap() {
    clearMap();
    delete effectManager;
    delete player;
}

Tilemap* Tilemap::getInstance()
 {
    if (instance == nullptr) {
        instance = new Tilemap();
    }
    return instance;
}

void Tilemap::clearMap()
{
    std::cout << Physics::world.GetBodyCount() << " bodies in the world before clear.\n";
    for (auto& layer : nodes) {
        for (auto& node : layer) {
            delete node;
        }
    }
    nodes.clear();
    tilesets.clear();
    effectManager->clearEffects();
    std::cout << Physics::world.GetBodyCount() << " bodies in the world after clear.\n";
}

void Tilemap::changeMap(const std::string &filePath)
{
    if (!isChangingMap) {
        clearMap();
        isChangingMap = true;
    }
    else {
        LoadMapFromJson(filePath);
        isChangingMap = false;
    }
}

std::pair<std::string, int> Tilemap::GetTilesetInfo(int tileIdx) const
{
    int l = 0;
    int r = tilesets.size() - 1;
    while (l <= r) {
        int mid = (l + r)/2;
        if (tilesets[mid].second <= tileIdx) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    return tilesets[r];
}

void Tilemap::addNode(SceneNode *node)
{
    nodes.back().push_back(node);
}

void Tilemap::LoadMapFromJson(const std::string &filePath)
{
    std::string mapPath = "resources/tilemaps/" + filePath;
    std::cout << "Loading map " << mapPath << std::endl;
    std::ifstream file(mapPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open map file!" << std::endl;
        return;
    }

    json j;
    file >> j;
    
    width = j["width"];
    height = j["height"];
    tileSize = j["tilewidth"];

    // Define boundary
    vector<b2Vec2> vertices = {
        b2Vec2{0.0f, height},
        b2Vec2{0.0f, 0.0f},
        b2Vec2{width, 0.0f},
        b2Vec2{width, height}
    };

    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(0.0f, 0.0f);
    b2Body* lineBody = Physics::world.CreateBody(&bodyDef);

    for (int i = 0; i < 3; i++) {
        b2EdgeShape edge;
        edge.SetTwoSided(vertices[i], vertices[i + 1]);
        lineBody->CreateFixture(&edge, 1.0f);
    }

    b2Body* deadLine;
    b2EdgeShape edge;
    edge.SetTwoSided(vertices[0], vertices[3]);
    lineBody->CreateFixture(&edge, 0.0f);
    DeadLine* deadLineNode = new DeadLine(lineBody);
    lineBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(deadLineNode);   
    std::vector<SceneNode*> deadLineLayer;
    deadLineLayer.push_back(deadLineNode);
    nodes.push_back(deadLineLayer);

    for (const auto& tileset : j["tilesets"]) {
        std::string tilesetPath = tileset["source"].get<std::string>();
        size_t pos = tilesetPath.find_last_of('/');
        if (pos != std::string::npos) {
            tilesetPath = tilesetPath.substr(pos + 1);
        }
        tilesetPath = "resources/tilesets/" + tilesetPath;
        tilesets.push_back({tilesetPath, tileset["firstgid"].get<int>()});
    }
    
    for (const auto& layer : j["layers"]) {
        std::vector<SceneNode*> nodeLayer;
        if (layer["type"] == "imagelayer" && layer["name"] != "Effect") {
            std::cout << "Image layer found!" << std::endl;
            std::string imagePath = layer["image"].get<std::string>();
            size_t pos = imagePath.find_last_of('/');
            if (pos != std::string::npos) {
                imagePath = imagePath.substr(pos + 1);
            }
            Texture2D source = LoadTexture(("resources/background/" + imagePath).c_str());
            source.width /= 16;
            source.height /= 16;
            int numRepeated = width / source.width + 1;
            const auto& x = layer["x"].get<float>();
            const auto& y = layer["y"].get<float>();
            Background* background = new Background(source, Vector2{x, y}, numRepeated); 
            nodeLayer.push_back(background);
        }
        else if (layer["type"] == "tilelayer" && layer.contains("data")) {
            std::cout << "Tile layer found!" << std::endl;
            const auto& data = layer["data"];

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    int id = data[y * width + x].get<int>();  
                    if (id == 0) {
                        continue;  
                    }
                    const auto& [tilesetPath, firstGid] = GetTilesetInfo(id);
                    id -= firstGid;
                    Vector2 pos = {static_cast<float>(x), static_cast<float>(y)};

                    Tile* tile = TilesetHandler::getTile(tilesetPath, id);
                    if (tile == nullptr) {
                        std::cerr << "Tilemap::Tile with ID: " << id << "not exist" << std::endl;
                        std::cout << "Map id: " << data[y * width + x] << std::endl;
                        continue;  
                    }
                    tile->setPosition(pos);
                    nodeLayer.push_back(tile->clone());
                }
            }
        }
        else if (layer["type"] == "objectgroup" && layer.contains("objects")) {
            std::cout << "Object layer found!" << std::endl;
            for (const auto& object : layer["objects"]) {
                float width = object["width"].get<float>() / tileSize;
                float height = object["height"].get<float>() / tileSize;
                float x = object["x"].get<float>() / tileSize;
                float y = object["y"].get<float>() / tileSize;
                if (width == 0 && height == 0) {
                    if (object.contains("polygon")) {
                        std::cout << "Polygon object found!" << std::endl;
                        std::vector<b2Vec2> vertices;
                        for (const auto& vertex : object["polygon"]) {
                            vertices.push_back(b2Vec2{vertex["x"].get<float>() / tileSize, vertex["y"].get<float>() / tileSize});
                        }
                        b2Body* body;
                        MyBoundingBox::createBody(body, b2_staticBody, vertices, Vector2{x, y});
                        StaticObject* obj = new StaticObject(body);
                        nodeLayer.push_back(obj);
                    }
                    else {
                        if (object.contains("type") && object["type"] == "player") {
                            if (!player) {
                                std::string name = object["name"].get<std::string>();
                                player = new Player(name);
                                player->Init(b2Vec2{x, y});
                                //cout << player->getSpeed() << endl;
                                player->setSpeed(8.0f);
                                player->setInitialPosition(Vector2{x, y});
                                player->setName(name);
                                player->setHealth(100);
                                player->setLives(3);
                                player->setTime(300.0f);
                                player->setCurrentMap(filePath);
                            }
                            else {
                                player->setPositon(b2Vec2{x, y});
                                player->setCurrentMap(filePath);
                                player->setElapsedTime(0.0f);
                                player->setInitialPosition(Vector2{x, y});
                            }
                        }
                        else if (object.contains("type") && object["type"] == "enemy") {
                            std::string enemyName = object["name"].get<std::string>();
                            Enemy* enemy = EnemyCreator::CreateEnemy(enemyName, Vector2{x, y});
                            if (enemy != nullptr) {
                                nodeLayer.push_back(enemy);
                            }

                        }
                        else if (object.contains("type") && object["type"] == "platform") {
                            std::string platformName = object["name"].get<std::string>();
                            MovingPlatform* platform = PlatformCreator::CreatePlatform(platformName, Vector2{x, y});
                            if (platform != nullptr) {
                                nodeLayer.push_back(platform);
                            }
}
                        else if (object.contains("name") && object["name"].is_string()) {
                            std::string effectName = object["name"].get<std::string>();
                            effectManager->AddEffectPosition(std::make_pair((int)x, (int)y), effectName);
                            if (object.contains("type") && object["type"] != "") {
                                effectManager->AddEffectCount({(int)x, (int)y}, std::stoi(object["type"].get<std::string>()));
                            }
                            else {
                                effectManager->AddEffectCount({(int)x, (int)y}, 1);
                            }
                        } else {
                            std::cerr << "Error: Invalid 'name' key in object\n";
                        }
                    }
                }
                else {
                    std::vector<b2Vec2> vertices = {
                        b2Vec2{0.0f, 0.0f},
                        b2Vec2{width, 0.0f},
                        b2Vec2{width, height},
                        b2Vec2{0.0f, height}
                    };
                    
                    if (object.contains("name") && object["name"] == "gate") {
                        b2Body* body;
                        MyBoundingBox::createBody(body, b2_staticBody, vertices, Vector2{x, y});
                        b2Fixture* fixture = body->GetFixtureList();
                        fixture->SetSensor(true);

                        std::string addressNext = object["type"].get<std::string>();
                        Gate* gate = new Gate(body, addressNext);
                        body->GetUserData().pointer = reinterpret_cast<uintptr_t>(gate);
                        nodeLayer.push_back(gate);
                    }
                    else {
                        b2Body* body;
                        MyBoundingBox::createBody(body, b2_staticBody, vertices, Vector2{x, y});
                        StaticObject* obj = new StaticObject(body);
                        body->GetUserData().pointer = reinterpret_cast<uintptr_t>(obj);
                        nodeLayer.push_back(obj);
                    }
                }
            }
        }
        nodes.push_back(nodeLayer);
    }
    file.close();
    camera = MyCamera(38.0f, player->getPosition(), Vector2{ (float)width, (float)height }, screenWidth, screenHeight);
    std::cout << "Map loaded successfully!" << std::endl;
    std::cout << Physics::world.GetBodyCount() << " bodies in the world after loading.\n";

}

void Tilemap::Update(float deltaTime) {
    if (newMapPath != "") {
        changeMap(newMapPath);
        if (!isChangingMap) newMapPath = "";
    }
    else {
        b2Vec2 playerVelocity = player->getVelocity();
        for (auto& layer : nodes) {
            if (layer.empty()) {
                effectManager->Update(deltaTime);
                continue;
            }
            for (auto& node : layer) {
                node->Update(Vector2{playerVelocity.x, playerVelocity.y}, deltaTime);
            }
        }
        if (!effectManager->isActivePlayerEffect()) {
            if (player->isAlive()) camera.Update(player->getPosition());  
            player->HandleInput();
            player->Update(Vector2{playerVelocity.x, playerVelocity.y}, deltaTime);
        }
    }
}

void Tilemap::Draw() const {
    if (isChangingMap) {
        return;
    }
    BeginMode2D(camera.GetCamera());
    for (int i = 0; i < nodes.size() - 1; ++i) {
        if (nodes[i].empty()) {
            effectManager->DrawLower();
            continue;
        }
        for (auto& node : nodes[i]) {
            node->Draw();
        }
    }
    if (!effectManager->isActivePlayerEffect()) player->Draw();
    for (auto& node : nodes.back()) {
        node->Draw();
    }

    Vector2 cameraTarget = camera.GetCameraTarget();
    player->Draw(Vector2{cameraTarget.x - 9.5f, cameraTarget.y - 7.0f}, 0.0f);
    effectManager->DrawUpper();
    EndMode2D();

}

void Tilemap::SetNewMapPath(const std::string &path)
{
    newMapPath = path;
}

EffectManager* Tilemap::GetEffectManager()
{
    return effectManager;
}

Player *Tilemap::GetPlayer()
{
    return player;
}

Vector2 Tilemap::GetPlayerPosition() const
{
    return player->getPosition();
}

int Tilemap::GetWidth() const
{
    return width;
}

int Tilemap::GetHeight() const {
    return height;
}

int Tilemap::GetTileSize() const {
    return tileSize;
}
