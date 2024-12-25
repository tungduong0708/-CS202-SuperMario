#include "include.h"
#include "object.h"
#include "tilemap.h"

Tilemap* Tilemap::instance = nullptr;

Tilemap::Tilemap() {
    effectManager = new EffectManager();
    player = nullptr;
}

Tilemap::Tilemap(const std::string& filePath, int difficulty) {
    effectManager = new EffectManager();
    player = nullptr;
    LoadMapFromJson(filePath, difficulty);
    
}

Tilemap::~Tilemap() {
    clearMap();
    delete effectManager;
    effectManager = nullptr;
    delete player;
    player = nullptr;
    instance = nullptr;
    std::cout << "Tilemap destroyed.\n";
    std::cout << Physics::world.GetBodyCount() << " bodies in the world after destruction.\n";
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
    for (int i = nodes.size() - 1; i >= 0; i--) {
        for (auto& node : nodes[i]) {
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
        effectManager = new EffectManager();
        changedTiles.clear();
        isChangingMap = true;
    }
    else {
        LoadMapFromJson(filePath, difficulty);
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

void Tilemap::addChangedTile(Tile *tile)
{
    changedTiles.push_back(tile);
}

void Tilemap::LoadMapFromJson(const std::string &filePath, int difficulty)
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
    
    this->filePath = filePath;
    this->difficulty = difficulty;
    width = j["width"];
    height = j["height"];
    tileSize = j["tilewidth"];

    // Define boundary
    vector<b2Vec2> vertices = {
        b2Vec2{0.0f, (float)height},
        b2Vec2{0.0f, 0.0f},
        b2Vec2{(float)width, 0.0f},
        b2Vec2{(float)width, (float)height}
    };

    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(0.0f, 0.0f);
    b2Body* lineBody = Physics::world.CreateBody(&bodyDef);

    for (int i = 0; i < 3; i++) {
        if (i == 1) continue;
        b2EdgeShape edge;
        edge.SetTwoSided(vertices[i], vertices[i + 1]);
        lineBody->CreateFixture(&edge, 1.0f);
    }
    StaticObject* lineNode = new StaticObject(lineBody);

    b2Body* deadLine;
    deadLine = Physics::world.CreateBody(&bodyDef);
    b2EdgeShape edge;
    edge.SetTwoSided(vertices[0], vertices[3]);
    deadLine->CreateFixture(&edge, 0.0f);
    DeadLine* deadLineNode = new DeadLine(deadLine);
    
    b2Fixture* fixture = deadLine->GetFixtureList();
    b2Filter filter = fixture->GetFilterData();
    filter.categoryBits = CATEGORY_ENEMY;
    filter.maskBits = MASK_ENEMY;
    fixture->SetFilterData(filter);

    deadLine->GetUserData().pointer = reinterpret_cast<uintptr_t>(deadLineNode);   

    std::vector<SceneNode*> boundaryLayer;
    boundaryLayer.push_back(deadLineNode);
    boundaryLayer.push_back(lineNode);
    nodes.push_back(boundaryLayer);

    for (const auto& tileset : j["tilesets"]) {
        std::string tilesetPath = tileset["source"].get<std::string>();
        size_t pos = tilesetPath.find_last_of('/');
        if (pos != std::string::npos) {
            tilesetPath = tilesetPath.substr(pos + 1);
        }
        tilesetPath = "resources/tilesets/" + tilesetPath;
        tilesets.push_back({tilesetPath, tileset["firstgid"].get<int>()});
    }
    
    Pole* pole = nullptr;
    Axe* axe = nullptr;;
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
                    if (activatedTiles.find({x, y}) != activatedTiles.end()) {
                        continue;
                    }
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
                    StaticObject* tileCopy = tile->clone();
                    nodeLayer.push_back(tileCopy);
                    if (tile->getType() == "bridge") {
                        axe->AddBridgeTile(static_cast<StaticTile*>(tileCopy));
                    }
                }
            }

            if (activatedTiles.size() > 0) {
                activatedTiles.clear();
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
                            if (playerLoaded) {
                                playerLoaded = false;
                                continue;
                            }
                            playerPosition = Vector2{x, y};
                            if (player != nullptr) {
                                player->setPositionBody(b2Vec2{playerPosition.x, playerPosition.y});
                                player->setInitialPosition(playerPosition);
                                string fPath = filePath.substr(4,3);
                                player->setCurrentMap(fPath);
                                player->setElapsedTime(0.0f);
                                player->setTime(300.0f);
                            }
                        }
                        else if (object.contains("type") && object["type"] == "character") {
                            if (!loadedNodes.empty()) {
                                continue;
                            }
                            std::string characterName = object["name"].get<std::string>();
                            Character* character = CharacterCreator::CreateCharacter(characterName, Vector2{x, y}, difficulty);
                            if (character != nullptr) {
                                nodeLayer.push_back(character);
                            }
                        }
                        else if (object.contains("type") && object["type"] == "object") {
                            std::string objName = object["name"].get<std::string>();
                            SceneNode* node = ObjectCreator::CreateObject(objName, Vector2{x, y});
                            nodeLayer.push_back(node);
                            if (objName == "flag") {
                                pole->addFlag(static_cast<Flag*>(node));
                            } else if (objName == "axe") {
                                axe = static_cast<Axe*>(node);
                            }
                        }
                        else if (object.contains("name") && object["name"].is_string()) {
                            if (effectManager->isLoadedFromMap()) {
                                continue;
                            }
                            std::string effectName = object["name"].get<std::string>();
                            if (effectName == "") {
                                continue;
                            }
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
                    
                    if (object.contains("type") && object["type"] == "object") {
                        std::string objName = object["name"].get<std::string>();
                        SceneNode* node = ObjectCreator::CreateObject(objName, Vector2{x, y});
                        if (objName == "pole") {
                            pole = static_cast<Pole*>(node);
                            pole->Init(vertices, b2Vec2{x, y});
                        }
                        else if (objName == "gate") {
                            std::string addressNext = object["properties"][0]["value"].get<std::string>();
                            SceneNode* gate = ObjectCreator::CreateObject("gate", Vector2{x, y});
                            static_cast<Gate*>(node)->Init(vertices, Vector2{x, y}, addressNext);
                        }
                        nodeLayer.push_back(node);
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

            if (loadedNodes.size() > 0) {
                nodes.push_back(loadedNodes);
                loadedNodes.clear();
            }
        }
        nodes.push_back(nodeLayer);
    }
    
    camera = MyCamera(46.875f, playerPosition, Vector2{ (float)width, (float)height });
    file.close();
    std::cout << "Map loaded successfully!" << std::endl;
    std::cout << Physics::world.GetBodyCount() << " bodies in the world after loading.\n";

}

void Tilemap::LoadSaveGame(const std::string &filePath)
{
    std::cout << "Loading save " << filePath << std::endl;
    ImportFileVisitor* visitor = ImportFileVisitor::getInstance();
    visitor->setFilePath(filePath);
    visitor->openFile();
    ifstream& file = visitor->getFile();
    std::string obj, mapPath;
    int difficulty;
    file >> mapPath >> difficulty;

    while (file >> obj) {
        SceneNode* newNode = nullptr;

        if (obj == "StaticTile") {
            auto tile = std::make_unique<StaticTile>();
            tile->accept(visitor);
            changedTiles.push_back(tile.get());
            Vector2 pos = tile->getPosition();
            activatedTiles.insert({static_cast<int>(pos.x), static_cast<int>(pos.y)});
            newNode = tile.release();
        } 
        else if (obj == "KinematicTile") {
            auto tile = std::make_unique<KinematicTile>();
            tile->accept(visitor);
            changedTiles.push_back(tile.get());
            Vector2 pos = tile->getPosition();
            activatedTiles.insert({static_cast<int>(pos.x), static_cast<int>(pos.y)});
            newNode = tile.release();
        } 
        else if (obj == "MovingPlatform") {
            auto platform = std::make_unique<MovingPlatform>();
            platform->accept(visitor);
            newNode = platform.release();
        } 
        else if (obj == "Mushroom") {
            auto mushroom = std::make_unique<Mushroom>();
            mushroom->accept(visitor);
            newNode = mushroom.release();
        } 
        else if (obj == "Star") {
            auto star = std::make_unique<Star>();
            star->accept(visitor);
            newNode = star.release();
        } 
        else if (obj == "FireFlower") {
            auto flower = std::make_unique<FireFlower>();
            flower->accept(visitor);
            newNode = flower.release();
        } 
        else if (obj == "Goomba") {
            auto goomba = std::make_unique<Goomba>();
            goomba->accept(visitor);
            newNode = goomba.release();
        } 
        else if (obj == "Koopa") {
            auto koopa = std::make_unique<Koopa>();
            koopa->accept(visitor);
            newNode = koopa.release();
        } 
        else if (obj == "Boss") {
            auto boss = std::make_unique<Boss>();
            boss->accept(visitor);
            newNode = boss.release();
        } 
        else if (obj == "AttackBall") {
            auto ball = std::make_unique<AttackBall>();
            ball->accept(visitor);
            newNode = ball.release();
        } 
        else if (obj == "FireBall") {
            auto ball = std::make_unique<FireBall>();
            ball->accept(visitor);
            newNode = ball.release();
        } 
        else if (obj == "Player") {
            player = new Player();
            player->accept(visitor);
            playerLoaded = true;
        } 
        else if (obj == "LarvaBubble") {
            auto lbubble = std::make_unique<LarvaBubble>();
            lbubble->accept(visitor);
            newNode = lbubble.release();
        }
        else if (obj == "MonsterFlower") {
            auto mflower = std::make_unique<MonsterFlower>();
            mflower->accept(visitor);
            newNode = mflower.release();
        }
        else if (obj == "EffectManager") {
            effectManager->accept(visitor);
            effectManager->setLoadedFromMap(true);
        }
        else if (obj == "Princess") {
            auto princess = std::make_unique<Princess>();
            princess->accept(visitor);
            newNode = princess.release();
        }

        if (newNode) {
            loadedNodes.push_back(newNode);
        } 
    }
    visitor->closeFile();
    LoadMapFromJson(mapPath, difficulty);
}

void Tilemap::SaveGame(std::string filePath) const
{
    ExportFileVisitor* visitor = ExportFileVisitor::getInstance();
    visitor->setFilePath(filePath);
    visitor->openFile(true);
    visitor->exportMapInfo(this->filePath, this->difficulty);
    for (auto& tile : changedTiles) {
        StaticTile* staticTile = dynamic_cast<StaticTile*>(tile);
        KinematicTile* kinematicTile = dynamic_cast<KinematicTile*>(tile);
        if (staticTile) {
            staticTile->accept(visitor);
        }
        else if (kinematicTile) {
            kinematicTile->accept(visitor);
        }
    }
    for (auto& layer : nodes) {
        for (auto& node : layer) {
            Enemy* enemy = dynamic_cast<Enemy*>(node);
            ActiveItem* item = dynamic_cast<ActiveItem*>(node);
            if (enemy && enemy->isAlive()) {
                enemy->accept(visitor);
            }
            else if (item && item->isAvailable()) {
                item->accept(visitor);
            }
        }
    }
    player->accept(visitor);
    effectManager->accept(visitor);
    visitor->closeFile();
}

void Tilemap::Update(float deltaTime) {
    if (newMapPath != "") {
        changeMap(newMapPath);
        if (!isChangingMap) newMapPath = "";
    }
    else {
        b2Vec2 playerVelocity = player->getVelocity();
        for (int i = 0; i < nodes.size(); ++i) {
            for (int j = 0; j < nodes[i].size(); ++j) {
                nodes[i][j]->Update(Vector2{playerVelocity.x, playerVelocity.y}, deltaTime);
            }
        }
        effectManager->Update(deltaTime);
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

    for (int i = 0; i < nodes.size(); ++i) {
        if (nodes[i].empty()) {
            effectManager->DrawLower();
            continue;
        }
        for (auto& node : nodes[i]) {
            node->Draw();
        }
    }
    Vector2 cameraTarget = camera.GetCameraTarget();
    if (!effectManager->isActivePlayerEffect()) {
        player->Draw();
    }
    player->Draw(Vector2{cameraTarget.x - 9.5f, cameraTarget.y - 7.0f}, 0.0f);
    effectManager->DrawUpper();
    EndMode2D();

}

void Tilemap::setPlayer(const std::string name)
{
    player = new Player(name);
    TextHelper::loadTexture("coin", "small" + name);
    player->Init(b2Vec2{playerPosition.x, playerPosition.y});
    player->setPositionBody(b2Vec2{playerPosition.x, playerPosition.y});
    player->setInitialPosition(playerPosition);
    string fPath = filePath.substr(4,3);
    player->setCurrentMap(fPath);
}

void Tilemap::SetNewMapPath(const std::string &path)
{
    newMapPath = path;
}

std::string Tilemap::GetCurrentMapPath() const
{
    return filePath;
}

std::string Tilemap::GetNewMapPath() const
{
    return newMapPath;
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