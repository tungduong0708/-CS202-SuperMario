#ifndef FILE_VISITOR_H
#define FILE_VISITOR_H

#include "scene_node.h"
#include "moving_object.h"
#include "character.h"

class FileVisitor {
public:
    virtual void VisitFile(Mushroom* obj)   = 0;
    virtual void VisitFile(Star* obj)       = 0;
    virtual void VisitFile(FireFlower* obj) = 0;
    virtual void VisitFile(Goomba* obj)     = 0;
    virtual void VisitFile(Koopa* obj)      = 0;
    virtual void VisitFile(Boss* obj)       = 0;
    virtual void VisitFile(AttackBall* obj) = 0;
    virtual void VisitFile(FireBall* obj)   = 0;
    virtual void VisitFile(Player* obj)     = 0;
};

class ExportFileVisitor : public FileVisitor {
private:
    const string filePath = "resources/save/save.txt";
    static ExportFileVisitor* instance;
    ExportFileVisitor() = default;
public:
    static ExportFileVisitor* getInstance();
    ~ExportFileVisitor() = default;

    void VisitFile(Mushroom* obj) override;
    void VisitFile(Star* obj) override;
    void VisitFile(FireFlower* obj) override;
    void VisitFile(Goomba* obj) override;
    void VisitFile(Koopa* obj) override;
    void VisitFile(Boss* obj) override;
    void VisitFile(AttackBall* obj) override;
    void VisitFile(FireBall* obj) override;
    void VisitFile(Player* obj) override;
};


class ImportFileVisitor : public FileVisitor {
private:
    string filePath;
    static ImportFileVisitor* instance;
    ImportFileVisitor() = default;
public:
    static ImportFileVisitor* getInstance();
    ~ImportFileVisitor() = default;

    static void setFilePath(string path);

    void VisitFile(Mushroom* obj) override;
    void VisitFile(Star* obj) override;
    void VisitFile(FireFlower* obj) override;
    void VisitFile(Goomba* obj) override;
    void VisitFile(Koopa* obj) override;
    void VisitFile(Boss* obj) override;
    void VisitFile(AttackBall* obj) override;
    void VisitFile(FireBall* obj) override;
    void VisitFile(Player* obj) override;
};

#endif
