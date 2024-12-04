#include "my_bounding_box.h"

MyBoundingBox::MyBoundingBox() : body(nullptr), fixture(nullptr)
{
}

MyBoundingBox::MyBoundingBox(const std::vector<b2Vec2> &vertices, const Vector2 &position)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(position.x, position.y);
    body = Physics::world.CreateBody(&bodyDef);

    b2PolygonShape polygonShape;
    polygonShape.Set(vertices.data(), vertices.size());

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polygonShape;
    fixtureDef.density = 0.0f;
    fixture = body->CreateFixture(&fixtureDef);
}

MyBoundingBox::MyBoundingBox(const MyBoundingBox& other, const Vector2 &position) {
    b2BodyDef bodyDef;
    bodyDef.type = other.body->GetType();
    bodyDef.position.Set(position.x, position.y);
    bodyDef.angle = other.body->GetAngle();
    bodyDef.linearVelocity = other.body->GetLinearVelocity();
    bodyDef.angularVelocity = other.body->GetAngularVelocity();
    bodyDef.gravityScale = other.body->GetGravityScale();
    this->body = Physics::world.CreateBody(&bodyDef);

    if (other.fixture) {
        b2FixtureDef fixtureDef;
        fixtureDef.density = other.fixture->GetDensity();
        fixtureDef.friction = other.fixture->GetFriction();
        fixtureDef.restitution = other.fixture->GetRestitution();
        fixtureDef.isSensor = other.fixture->IsSensor();
        fixtureDef.filter = other.fixture->GetFilterData();

        const b2Shape* otherShape = other.fixture->GetShape();
        if (otherShape->GetType() == b2Shape::e_polygon) {
            const b2PolygonShape* otherPolygon = static_cast<const b2PolygonShape*>(otherShape);
            b2PolygonShape* polygonShape = new b2PolygonShape();
            polygonShape->Set(otherPolygon->m_vertices, otherPolygon->m_count);
            fixtureDef.shape = polygonShape;

            this->fixture = body->CreateFixture(&fixtureDef);

            delete polygonShape;
        } else {
            std::cerr << "Warning: Unsupported shape type in MyBoundingBox copy constructor." << std::endl;
        }
    }
    else {
        fixture = nullptr;
    }
}

b2Body* MyBoundingBox::getBody() const { return body; }
b2Fixture* MyBoundingBox::getFixture() const { return fixture; }

void MyBoundingBox::updatePosition(const b2Vec2 &position)
{
    body->SetTransform(position, body->GetAngle());
}

void MyBoundingBox::updateFixture(const std::vector<b2Vec2> &vertices)
{
    if (fixture) {
        body->DestroyFixture(fixture);
        fixture = nullptr;
    }
    if (vertices.empty()) return;

    b2PolygonShape polygonShape;
    polygonShape.Set(vertices.data(), vertices.size());

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polygonShape;
    fixture = body->CreateFixture(&fixtureDef);
}

void MyBoundingBox::print() const
{
    std::cout << "Bounding Box Fixture attached to Body at position ("
              << body->GetPosition().x << ", " << body->GetPosition().y << ")\n";

    if (fixture) {
        const b2Shape* shape = fixture->GetShape();
        switch (shape->GetType()) {
            case b2Shape::e_circle:
                std::cout << "Shape type: Circle\n";
                break;
            case b2Shape::e_polygon:
                std::cout << "Shape type: Polygon\n";
                break;
            case b2Shape::e_edge:
                std::cout << "Shape type: Edge\n";
                break;
            case b2Shape::e_chain:
                std::cout << "Shape type: Chain\n";
                break;
            default:
                std::cout << "Shape type: Unknown\n";
                break;
        }
    } else {
        std::cout << "No fixture attached to the body\n";
    }
}
