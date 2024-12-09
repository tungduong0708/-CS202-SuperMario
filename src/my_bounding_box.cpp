#include "my_bounding_box.h"

void MyBoundingBox::createBody(b2Body*& body, b2BodyType type, const std::vector<b2Vec2> &vertices, const Vector2 &position, float restitution)
{
    b2BodyDef bodyDef;
    bodyDef.type = type;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.fixedRotation = true;

    body = Physics::world.CreateBody(&bodyDef);

    b2PolygonShape polygonShape;
    polygonShape.Set(vertices.data(), vertices.size());

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polygonShape;
    fixtureDef.density = 1.0f; 
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = restitution;
    body->CreateFixture(&fixtureDef);
}

void MyBoundingBox::copyBody(b2Body*& body, b2Body* otherBody, const Vector2 &position) {
    b2BodyDef bodyDef;
    bodyDef.type = otherBody->GetType();
    bodyDef.position.Set(position.x, position.y);
    bodyDef.angle = otherBody->GetAngle();
    bodyDef.linearVelocity = otherBody->GetLinearVelocity();
    bodyDef.angularVelocity = otherBody->GetAngularVelocity();
    bodyDef.gravityScale = otherBody->GetGravityScale();
    body = Physics::world.CreateBody(&bodyDef);

    b2Fixture* otherFixture = otherBody->GetFixtureList();
    if (otherFixture) {
        b2FixtureDef fixtureDef;
        fixtureDef.density = otherFixture->GetDensity();
        fixtureDef.friction = otherFixture->GetFriction();
        fixtureDef.restitution = otherFixture->GetRestitution();
        fixtureDef.isSensor = otherFixture->IsSensor();
        fixtureDef.filter = otherFixture->GetFilterData();

        const b2Shape* otherShape = otherFixture->GetShape();
        if (otherShape->GetType() == b2Shape::e_polygon) {
            const b2PolygonShape* otherPolygon = static_cast<const b2PolygonShape*>(otherShape);
            b2PolygonShape* polygonShape = new b2PolygonShape();
            polygonShape->Set(otherPolygon->m_vertices, otherPolygon->m_count);
            fixtureDef.shape = polygonShape;

            body->CreateFixture(&fixtureDef);
            delete polygonShape;
        } else {
            std::cerr << "Warning: Unsupported shape type in MyBoundingBox copy constructor." << std::endl;
        }
    }
}

void MyBoundingBox::updatePosition(b2Body*& body, const b2Vec2 &position)
{
    body->SetTransform(position, body->GetAngle());
}

void MyBoundingBox::updateFixture(b2Body*& body, const std::vector<b2Vec2> &vertices)
{
    b2Fixture* fixture = body->GetFixtureList();
    if (fixture) {
        body->DestroyFixture(fixture);
        fixture = nullptr;
    }
    if (vertices.empty()) return;

    b2PolygonShape polygonShape;
    polygonShape.Set(vertices.data(), vertices.size());

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polygonShape;
    body->CreateFixture(&fixtureDef);
}