#ifndef MOVING_PLATFORM_H
#define MOVING_PLATFORM_H

#include "moving_object.h"
#include <unordered_map>
#include <string>
#include <functional>

// PlatformCreator class handles registering and creating MovingPlatform objects
class ObjectCreator {
private:
    // Map for associating platform names with their creation functions
    std::unordered_map<std::string, std::function<SceneNode*(Vector2)>> ObjectCreator::creators;

public:
    ObjectCreator() = default;
    ~ObjectCreator() = default;
    // Registers a platform type with a corresponding creation function
    static void RegisterObject(const std::string& name, std::function<SceneNode*(Vector2)> creator);

    // Initializes predefined platform types
    static void InitObjects();

    // Creates a platform based on the given name and position
    static SceneNode* CreateObject(const std::string& name, Vector2 position);
};

#endif 