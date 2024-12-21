#include "include.h"
#include "object.h"

ExportFileVisitor* ExportFileVisitor::instance;
ImportFileVisitor* ImportFileVisitor::instance;

ExportFileVisitor *ExportFileVisitor::getInstance() {
    if (instance == nullptr) {
        instance = new ExportFileVisitor();
    }
    return instance;
}

void ExportFileVisitor::setFilePath(std::string path)
{
    file << path << std::endl;
}


void ExportFileVisitor::openFile(bool overwrite)
{
    if (overwrite) {
        file.open(filePath, std::ios::out);  
    } else {
        file.open(filePath, std::ios::app);  
    }

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }

    std::cout << "File " << filePath << " opened in " 
              << (overwrite ? "overwrite" : "append") << " mode.\n";
}

void ExportFileVisitor::closeFile() {
    if (file.is_open()) {
        file.close();
    }
}

void ExportFileVisitor::VisitFile(StaticTile *obj)
{
    file << "StaticTile " << obj->getId() << std::endl;
    file << obj->getTilesetPath() << std::endl;
    file << obj->getPosition().x << " " << obj->getPosition().y << std::endl;
    file << obj->getIsDestroyed() << " " << obj->getIsActivated() << std::endl;
}

void ExportFileVisitor::VisitFile(KinematicTile *obj)
{
    int id = obj->getId();
    file << "KinematicTile " << id << std::endl;
    file << obj->getTilesetPath() << std::endl;
    file << obj->getPosition().x << " " << obj->getPosition().y << std::endl;
    file << obj->isAnimation() << std::endl;
    std::vector<std::pair<int, int>> frames = obj->getFrames();
    file << frames.size() << std::endl;
    for (auto frame : frames) {
        file << frame.first << " " << frame.second << std::endl;
    }
}

void ExportFileVisitor::VisitFile(MovingPlatform *obj) {

}

void ExportFileVisitor::VisitFile(Mushroom * obj) {
    file << "Mushroom" << std::endl;
    file << obj->getPosition().x << " " << obj->getPosition().y << std::endl;
}

void ExportFileVisitor::VisitFile(Star *obj) {
    file << "Star" << std::endl;
    file << obj->getPosition().x << " " << obj->getPosition().y << std::endl;
}

void ExportFileVisitor::VisitFile(FireFlower *obj) {
    file << "FireFlower" << std::endl;
    file << obj->getPosition().x << " " << obj->getPosition().y << std::endl;
}

void ExportFileVisitor::VisitFile(Goomba *obj) {
    file << "Goomba" << std::endl;
    file << obj->getPosition().x << " " << obj->getPosition().y << std::endl;
    file << obj->getLevel() << std::endl;
    file << obj->getHealth() << std::endl;
}

void ExportFileVisitor::VisitFile(Koopa *obj) {
    file << "Koopa" << std::endl;
    file << obj->getPosition().x << " " << obj->getPosition().y << std::endl;
    file << obj->getLevel() << std::endl;
    file << obj->getHealth() << std::endl;
}

void ExportFileVisitor::VisitFile(Boss *obj) {
    file << "Boss" << std::endl;
    file << obj->getPosition().x << " " << obj->getPosition().y << std::endl;
    file << obj->getLevel() << std::endl;
    file << obj->getHealth() << std::endl;
}

void ExportFileVisitor::VisitFile(LarvaBubble *obj) {
    file << "LarvaBubble" << std::endl;
    file << obj->getPosition().x << " " << obj->getPosition().y << std::endl;
    file << obj->getLevel() << std::endl;
    file << obj->getHealth() << std::endl;
}

void ExportFileVisitor::VisitFile(MonsterFlower *obj) {
    file << "MonsterFlower" << std::endl;
    b2Vec2 size = {obj->getSize().x, obj->getSize().y};
    b2Vec2 diff = b2Vec2{-0.5f*size.x, 0};
    b2Vec2 pos = b2Vec2{obj->getPosition().x, obj->getPosition().y} - diff;
    file << pos.x  << " " << pos.y << std::endl;
    file << obj->getLevel() << std::endl;
    file << obj->getHealth() << std::endl;
}

void ExportFileVisitor::VisitFile(AttackBall *obj) {
    file << "AttackBall" << std::endl;
    file << obj->getPosition().x << " " << obj->getPosition().y << std::endl;
    file << obj->getDamage() << std::endl;
}

void ExportFileVisitor::VisitFile(FireBall *obj) {
    file << "FireBall" << std::endl;
    file << obj->getPosition().x << " " << obj->getPosition().y << std::endl;
}

void ExportFileVisitor::VisitFile(Player *obj) {
    file << "Player" << std::endl;
    file << obj->getPosition().x << " " << obj->getPosition().y << std::endl;
    file << obj->getInitialPosition().x << " " << obj->getInitialPosition().y << std::endl;
    file << obj->getMode() << std::endl;
    file << obj->getType() << std::endl;
    file << obj->getScore() << std::endl;
    file << obj->getCoins() << std::endl;
    file << obj->getLives() << std::endl;
    file << obj->getTime() << std::endl;
    file << obj->getCurrentMap() << std::endl;
}

void ExportFileVisitor::VisitFile(EffectManager *obj)
{
    ofstream file(filePath, ios::app);
    file << "EffectManager" << std::endl;
    std::map<std::pair<int, int>, std::string> effectMap = obj->getEffectMap();
    file << effectMap.size() << std::endl;
    for (auto effect : effectMap) {
        file << effect.first.first << " " << effect.first.second << " " << effect.second << std::endl;
    }
    std::map<std::pair<int, int>, int> effectCount = obj->getEffectCountMap();
    file << effectCount.size() << std::endl;
    for (auto effect : effectCount) {
        if (effect.second > 0) {
            file << effect.first.first << " " << effect.first.second << " " << effect.second << std::endl;
        }
    }
}

void ImportFileVisitor::openFile() {
    file.open(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file");
    }
}

ImportFileVisitor *ImportFileVisitor::getInstance() {
    if (instance == nullptr) {
        instance = new ImportFileVisitor();
    }
    return instance;
}

void ImportFileVisitor::setFilePath(string path)
{
    filePath = path;
}

void ImportFileVisitor::closeFile() {
    if (file.is_open()) file.close();
}

std::ifstream& ImportFileVisitor::getFile()
{
    return file;
}

void ImportFileVisitor::VisitFile(StaticTile *obj)
{
    int id;
    string tilesetPath;
    float x, y;
    bool isDestroyed, isActivated;
    file >> id >> tilesetPath >> x >> y;
    file >> isDestroyed >> isActivated;
    if (!isDestroyed) {
        *obj = StaticTile(id, Vector2{x, y}, "", tilesetPath);
        obj->setIsDestroyed(isDestroyed);
        obj->setIsActivated(isActivated);
        obj->createBody();
    } else {
        obj = nullptr; // StaticTile remains destroyed
    }
}

void ImportFileVisitor::VisitFile(KinematicTile *obj)
{
    int id;
    string tilesetPath;
    float x, y;
    int frameSize;
    bool animation;
    file >> id >> tilesetPath >> x >> y;
    file >> animation >> frameSize;
    std::vector<std::pair<int, int>> frames;
    for (int i = 0; i < frameSize; i++) {
        int frameId, frameTime;
        file >> frameId >> frameTime;
        frames.push_back({frameId, frameTime});
    }
    *obj = KinematicTile(id, Vector2{x, y}, "", tilesetPath, frames);
    obj->setAnimation(animation);
    if (frameSize > 0) {
        obj->createBody();
    }
}

void ImportFileVisitor::VisitFile(MovingPlatform *obj) {

}

void ImportFileVisitor::VisitFile(Mushroom *obj)
{
    float x, y;
    file >> x >> y;
    *obj = Mushroom();
    obj->Init(b2Vec2{x, y});
}

void ImportFileVisitor::VisitFile(Star *obj)
{
    float x, y;
    file >> x >> y;
    *obj = Star();
    obj->Init(b2Vec2{x, y});
}

void ImportFileVisitor::VisitFile(FireFlower *obj)
{
    float x, y;
    file >> x >> y;
    *obj = FireFlower();
    obj->Init(b2Vec2{x, y});
}

void ImportFileVisitor::VisitFile(Goomba *obj)
{
    float x, y;
    int level, health;
    file >> x >> y;
    file >> level >> health;
    *obj = Goomba("goomba", 0.0f, true, false, 100, 100, level, 100, Vector2{1.0f, 1.0f}, -2.0f, 0.0f);
    obj->Init(b2Vec2{x, y});
    obj->setLevel(level);
    obj->setHealth(health);
}

void ImportFileVisitor::VisitFile(Koopa *obj)
{
    float x, y;
    int level, health;
    file >> x >> y;
    file >> level >> health;
    *obj = Koopa("koopa", 0.0f, true, false, 100, 100, level, 100, Vector2{1.0f, 1.0f}, -2.0f, 0.0f);
    obj->Init(b2Vec2{x, y});
    obj->setLevel(level);
    obj->setHealth(health);
}

void ImportFileVisitor::VisitFile(Boss *obj)
{
    float x, y;
    int level, health;
    file >> x >> y;
    file >> level >> health;
    *obj = Boss("boss", 0.0f, true, 100, 100, level, 100, Vector2{1.0f, 1.0f}, 1.0f, 0.0f);
    obj->Init(b2Vec2{x, y});
    obj->setLevel(level);
    obj->setHealth(health);
}

void ImportFileVisitor::VisitFile(LarvaBubble *obj)
{
    float x, y;
    int level, health;
    file >> x >> y;
    file >> level >> health;
    *obj = LarvaBubble("larvabubble", 0.0f, true, 100, 100, 1, 100, Vector2{0.6f, 0.6f});
    obj->Init(b2Vec2{x, y});
    obj->setLevel(level);
    obj->setHealth(health);
}

void ImportFileVisitor::VisitFile(MonsterFlower *obj)
{
    float x, y;
    int level, health;
    file >> x >> y;
    file >> level >> health;
    *obj = MonsterFlower("monsterflower", 0.0f, true, 100, 100, 1, 100, Vector2{1.0f, 1.0f}, 1.0f, 0.0f);
    obj->Init(b2Vec2{x, y});
    obj->setLevel(level);
    obj->setHealth(health);
}

void ImportFileVisitor::VisitFile(AttackBall *obj)
{
    float x, y;
    float damage;
    file >> x >> y;
    file >> damage;
    *obj = AttackBall(damage, Vector2{0.5f, 0.5f}, -6.0f, 0.0f);
    obj->Init(b2Vec2{x, y});
    obj->setDamage(damage);
}

void ImportFileVisitor::VisitFile(FireBall *obj)
{
    float x, y;
    file >> x >> y;
    *obj = FireBall(100, Vector2{0.5f, 0.5f}, 6.0f, 0.0f);
    obj->Init(b2Vec2{x, y});
}

void ImportFileVisitor::VisitFile(Player *obj)
{
    float x, y;
    float initialX, initialY;
    int mode, score, coins, lives;
    float time;
    string type;
    string currentMap;
    file >> x >> y;
    file >> initialX >> initialY;
    file >> mode;
    file >> type;
    file >> score >> coins >> lives;
    file >> time;
    file >> currentMap;
    *obj = Player(type, "player", coins, lives, 100, score, 0, 0, Vector2{1.0f, 1.0f});
    TextHelper::loadTexture("coin", "small" + type);
    obj->Init(b2Vec2{x, y});
    obj->setInitialPosition(Vector2{initialX, initialY});
    if (mode == 0) {
        obj->setMode(Mode::SMALL);
    }
    else if (mode == 1) {
        obj->changeMode(Mode::BIG);
    }
    else {
        obj->changeMode(Mode::FIRE);
    }
    obj->setTime(time);
    obj->setCurrentMap(currentMap);
}

void ImportFileVisitor::VisitFile(EffectManager *obj)
{
    int effectSize;
    file >> effectSize;
    std::map<std::pair<int, int>, std::string> effectMap;
    for (int i = 0; i < effectSize; i++) {
        int x, y;
        string name;
        file >> x >> y >> name;
        effectMap[{x, y}] = name;
    }
    int effectCountSize;
    file >> effectCountSize;
    std::map<std::pair<int, int>, int> effectCount;
    for (int i = 0; i < effectCountSize; i++) {
        int x, y, count;
        file >> x >> y >> count;
        effectCount[{x, y}] = count;
    }
    *obj = EffectManager(effectMap, effectCount);
}
