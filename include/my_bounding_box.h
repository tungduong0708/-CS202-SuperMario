#ifndef MYBOUNDINGBOX_H
#define MYBOUNDINGBOX_H

#include "physics.h"
#include <iostream>
#include <vector>
#include <box2d/box2d.h>
#include <raylib.h>

class MyBoundingBox {
private:
public:
    static void createBody(b2Body*& body, b2BodyType type, const std::vector<b2Vec2> &vertices, const Vector2 &position);
    static void copyBody(b2Body*& body, b2Body* otherBody, const Vector2 &position);

    static void updatePosition(b2Body*& body, const b2Vec2& position);
    static void updateFixture(b2Body*& body, const std::vector<b2Vec2>& vertices);
};

#endif  // BOUNDINGBOX_H
