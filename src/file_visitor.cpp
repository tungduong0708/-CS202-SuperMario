#include "file_visitor.h"
#include "object.h"

void ExportFileVisitor::VisitFile(StaticTile *obj)
{
    ofstream file(filePath, ios::app);
    file << "StaticTile " << obj->getId() << std::endl;
    file  << obj->getTilesetPath() << std::endl;
    file << obj->getPosition().x << " " << obj->getPosition().y << std::endl;
    file << obj->getIsDestroyed() << " " << obj->getIsActivated() << std::endl;
    file.close();
}

void ExportFileVisitor::VisitFile(KinematicTile *obj)
{
    ofstream file(filePath, ios::app);
    file << "KinematicTile " << obj->getId() << std::endl;
    file  << obj->getTilesetPath() << std::endl;
    file << obj->getPosition().x << " " << obj->getPosition().y << std::endl;
    std::vector<std::pair<int, int>> frames = obj->getFrames();
    file << frames.size() << std::endl;
    for (auto frame : frames) {
        file << frame.first << " " << frame.second << std::endl;
    }
    file.close();
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
    file.close();
}

void ImportFileVisitor::VisitFile(StaticTile *obj)
{
    ifstream file(filePath);
    int id;
    string tilesetPath;
    float x, y;
    bool isDestroyed, isActivated;
    file >> id >> tilesetPath >> x >> y;
    file >> isDestroyed >> isActivated;
    if (!isDestroyed) {
        obj = new StaticTile(id, Vector2{x, y}, "", tilesetPath);
        obj->setIsDestroyed(isDestroyed);
        obj->setIsActivated(isActivated);
    }
    file.close();
}

void ImportFileVisitor::VisitFile(KinematicTile *obj)
{
    ifstream file(filePath);
    int id;
    string tilesetPath;
    float x, y;
    int frameSize;
    file >> id >> tilesetPath >> x >> y;
    file >> frameSize;
    std::vector<std::pair<int, int>> frames;
    for (int i = 0; i < frameSize; i++) {
        int x, y;
        file >> x >> y;
        frames.push_back({x, y});
    }
    obj = new KinematicTile(id, Vector2{x, y}, "", tilesetPath, frames);
    file.close();
}

void ImportFileVisitor::VisitFile(EffectManager *obj)
{
    ifstream file(filePath);
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
    obj = new EffectManager(effectMap, effectCount);
    file.close();
}
