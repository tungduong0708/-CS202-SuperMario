#ifndef CHARACTER_CREATOR_H
#define CHARACTER_CREATOR_H

#include "character.h"
#include <unordered_map>
#include <functional>
#include <raylib.h>

class CharacterCreator {
private:
    static std::unordered_map<std::string, std::function<Character*(Vector2, int)>> creators;

public:
    CharacterCreator() = default;
    ~CharacterCreator() = default;

    static void RegisterCharacter(const std::string& name, std::function<Character*(Vector2, int)> creator);
    static void InitCharacters();
    static Character* CreateCharacter(const std::string& name, Vector2 position, int difficulty);
};

#endif // CHARACTER_CREATOR_H