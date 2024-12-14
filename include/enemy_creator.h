#ifndef ENEMY_CREATOR_H
#define ENEMY_CREATOR_H

#include "character.h"
#include <unordered_map>
#include <functional>
#include <raylib.h>

class EnemyCreator {
private:
    static std::unordered_map<std::string, std::function<Enemy*(Vector2)>> creators;

public:
    EnemyCreator() = default;
    ~EnemyCreator() = default;

    static void RegisterEnemy(const std::string& name, std::function<Enemy*(Vector2)> creator);
    static void InitEnemies();
    static Enemy* CreateEnemy(const std::string& name, Vector2 position);
};

#endif // ENEMY_CREATOR_H