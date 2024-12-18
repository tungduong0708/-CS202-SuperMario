#ifndef FILE_VISITOR_H
#define FILE_VISITOR_H

#include "scene_node.h"
#include "moving_object.h"
#include "character.h"
#include <fstream>

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
    virtual void VisitFile(AttackBall* obj)     = 0;
    virtual void VisitFile(FireBall* obj)       = 0;
    virtual void VisitFile(Player* obj)         = 0;
    virtual void VisitFile(EffectManager* obj)  = 0;
};

class ExportFileVisitor : public FileVisitor {
private:
    const string filePath = "resources/save/save.txt";
    ofstream file;
    static ExportFileVisitor* instance;
    ExportFileVisitor() = default;
public:
    static ExportFileVisitor* getInstance();
    ~ExportFileVisitor() = default;
    void openFile();
    void closeFile();

    void VisitFile(StaticTile* obj) override;
    void VisitFile(KinematicTile *obj) override;
    void VisitFile(MovingPlatform* obj) override;
    void VisitFile(Mushroom* obj) override;
    void VisitFile(Star* obj) override;
    void VisitFile(FireFlower* obj) override;
    void VisitFile(Goomba* obj) override;
    void VisitFile(Koopa* obj) override;
    void VisitFile(Boss* obj) override;
    void VisitFile(AttackBall* obj) override;
    void VisitFile(FireBall* obj) override;
    void VisitFile(Player* obj) override;
    void VisitFile(EffectManager* obj) override;
};


class ImportFileVisitor : public FileVisitor {
private:
    string filePath;
    static ImportFileVisitor* instance;
    ifstream file;
    ImportFileVisitor() = default;
public:
    static ImportFileVisitor* getInstance();
    ~ImportFileVisitor() = default;

    void setFilePath(string path);
    void openFile();
    void closeFile();

    void VisitFile(StaticTile* obj) override;
    void VisitFile(KinematicTile *obj) override;
    void VisitFile(MovingPlatform* obj) override;
    void VisitFile(Mushroom* obj) override;
    void VisitFile(Star* obj) override;
    void VisitFile(FireFlower* obj) override;
    void VisitFile(Goomba* obj) override;
    void VisitFile(Koopa* obj) override;
    void VisitFile(Boss* obj) override;
    void VisitFile(AttackBall* obj) override;
    void VisitFile(FireBall* obj) override;
    void VisitFile(Player* obj) override;
    void VisitFile(EffectManager* obj) override;
};

#endif
