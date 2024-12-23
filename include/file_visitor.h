#ifndef FILE_VISITOR_H
#define FILE_VISITOR_H

#include <string>
#include <fstream>

// Forward declaration
class StaticTile;
class KinematicTile;
class MovingPlatform;
class Mushroom;
class Star;
class FireFlower;
class Goomba;
class Koopa;
class Boss;
class LarvaBubble;
class MonsterFlower;
class AttackBall;
class FireBall;
class Player;
class Princess;
class EffectManager;

class FileVisitor {
public:
    virtual ~FileVisitor() = default;
    virtual void VisitFile(StaticTile* obj)     = 0;
    virtual void VisitFile(KinematicTile* obj)  = 0;
    virtual void VisitFile(MovingPlatform* obj) = 0;  
    virtual void VisitFile(Mushroom* obj)       = 0;
    virtual void VisitFile(Star* obj)           = 0;
    virtual void VisitFile(FireFlower* obj)     = 0;
    virtual void VisitFile(Goomba* obj)         = 0;
    virtual void VisitFile(Koopa* obj)          = 0;
    virtual void VisitFile(Boss* obj)           = 0;
    virtual void VisitFile(LarvaBubble* obj)    = 0;
    virtual void VisitFile(MonsterFlower* obj)  = 0;
    virtual void VisitFile(AttackBall* obj)     = 0;
    virtual void VisitFile(FireBall* obj)       = 0;
    virtual void VisitFile(Player* obj)         = 0;
    virtual void VisitFile(Princess* obj)       = 0;
    virtual void VisitFile(EffectManager* obj)  = 0;
};

class ExportFileVisitor : public FileVisitor {
private:
    std::string filePath;
    std::ofstream file;
    static ExportFileVisitor* instance;
    ExportFileVisitor() = default;
public:
    static ExportFileVisitor* getInstance();
    ~ExportFileVisitor() = default;
    void setFilePath(std::string path);
    void openFile(bool overwrite = false);
    void closeFile();

    void exportMapPath(std::string path);
    void VisitFile(StaticTile* obj) override;
    void VisitFile(KinematicTile *obj) override;
    void VisitFile(MovingPlatform* obj) override;
    void VisitFile(Mushroom* obj) override;
    void VisitFile(Star* obj) override;
    void VisitFile(FireFlower* obj) override;
    void VisitFile(Goomba* obj) override;
    void VisitFile(Koopa* obj) override;
    void VisitFile(Boss* obj) override;
    void VisitFile(LarvaBubble* obj) override;
    void VisitFile(MonsterFlower* obj) override;
    void VisitFile(AttackBall* obj) override;
    void VisitFile(FireBall* obj) override;
    void VisitFile(Player* obj) override;
    void VisitFile(Princess* obj) override;
    void VisitFile(EffectManager* obj) override;
};


class ImportFileVisitor : public FileVisitor {
private:
    std::string filePath;
    static ImportFileVisitor* instance;
    std::ifstream file;
    ImportFileVisitor() = default;
public:
    static ImportFileVisitor* getInstance();
    ~ImportFileVisitor() = default;

    void setFilePath(std::string path);
    std::string getFilePath() const;
    void openFile();
    void closeFile();
    
    std::ifstream& getFile();

    void VisitFile(StaticTile* obj) override;
    void VisitFile(KinematicTile *obj) override;
    void VisitFile(MovingPlatform* obj) override;
    void VisitFile(Mushroom* obj) override;
    void VisitFile(Star* obj) override;
    void VisitFile(FireFlower* obj) override;
    void VisitFile(Goomba* obj) override;
    void VisitFile(Koopa* obj) override;
    void VisitFile(Boss* obj) override;
    void VisitFile(LarvaBubble* obj) override;
    void VisitFile(MonsterFlower* obj) override;
    void VisitFile(AttackBall* obj) override;
    void VisitFile(FireBall* obj) override;
    void VisitFile(Player* obj) override;
    void VisitFile(Princess* obj) override;
    void VisitFile(EffectManager* obj) override;
};

#endif
