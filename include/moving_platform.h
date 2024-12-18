#ifndef MOVING_PLATFORM_H
#define MOVING_PLATFORM_H

#include "moving_object.h"
#include <unordered_map>
#include <string>
#include <functional>

// PlatformCreator class handles registering and creating MovingPlatform objects
class PlatformCreator {
private:
    // Map for associating platform names with their creation functions
    static std::unordered_map<std::string, std::function<MovingPlatform*(Vector2)>> creators;

public:
    PlatformCreator() = default;
    ~PlatformCreator() = default;
    // Registers a platform type with a corresponding creation function
    static void RegisterPlatform(const std::string& name, std::function<MovingPlatform*(Vector2)> creator);

    // Initializes predefined platform types
    static void InitPlatforms();

    // Creates a platform based on the given name and position
    static MovingPlatform* CreatePlatform(const std::string& name, Vector2 position);
};

#endif 