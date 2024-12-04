#ifndef MYBOUNDINGBOX_H
#define MYBOUNDINGBOX_H

#include "physics.h"
#include <iostream>
#include <vector>
#include <box2d/box2d.h>
#include <raylib.h>

class MyBoundingBox {
private:
    b2Body* body;  
    b2Fixture* fixture;

public:
    MyBoundingBox();
    MyBoundingBox(const std::vector<b2Vec2> &vertices, const Vector2 &position);
    MyBoundingBox(const MyBoundingBox& other, const Vector2 &position);

    b2Body* getBody() const;
    b2Fixture* getFixture() const;

    void updatePosition(const b2Vec2& position);
    void updateFixture(const std::vector<b2Vec2>& vertices);
    void print() const;
};

#endif  // BOUNDINGBOX_H
