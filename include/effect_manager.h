#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "object.h"
#include "animation_effect.h"
#include <vector>
#include <map>
#include <utility>
#include <string>
#include <raylib.h>


class EffectManager {
private:
    bool loadedFromMap = false;
    bool activePlayerEffect = false;
    std::map<std::pair<int, int>, std::string> effectMap; // Map effect position
    std::map<std::pair<int, int>, int> effectCount;
    std::vector<AnimationEffect*> lowerEffects; // Manage all effects
    std::vector<AnimationEffect*> upperEffects; // Manage all effects

    Player* playerUpdated = NULL;     // Player updated the effect
    Player* playerUpdated2 = NULL;    // Player updated the effect, in case there is already a player updated
public:
    EffectManager() = default;
    EffectManager(std::map<std::pair<int, int>, std::string> effectMap, std::map<std::pair<int, int>, int> effectCount);
    EffectManager(const EffectManager& other);
    ~EffectManager();
    void AddEffectPosition(std::pair<int, int> pos, std::string name);
    void AddEffectCount(std::pair<int, int> pos, int count);
    void AddLowerEffect(AnimationEffect* effect);
    void AddUpperEffect(AnimationEffect* effect);
    bool isActivePlayerEffect(Player* player);
    bool isLoadedFromMap();
    std::string GetEffectName(std::pair<int, int> pos);
    int GetEffectCount(std::pair<int, int> pos);
    std::map<std::pair<int, int>, std::string> getEffectMap();
    std::map<std::pair<int, int>, int> getEffectCountMap();
    void setActivePlayerEffect(Player* player, bool active);
    void setLoadedFromMap(bool loaded);
    bool UpdateEffectCount(std::pair<int, int> pos);
    void Update(float deltaTime);
    void DrawLower() const;
    void DrawUpper() const;
    void clearEffects();
    void accept(FileVisitor* v);
};

#endif // EFFECT_MANAGER_H
