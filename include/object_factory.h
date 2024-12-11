#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include <raylib.h>
#include <string>

class ObjectFactory
{
    static void createObject(std::string type, Vector2 position);
};

#endif // OBJECT_FACTORY_H