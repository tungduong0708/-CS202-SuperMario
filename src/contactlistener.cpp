#include "contactlistener.h"
#include "scene_node.h"
#include "physics.h"
#include "kinematic_tile.h"
#include "moving_object.h"
#include "character.h"

void ContactListener::BeginContact(b2Contact *contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    
    SceneNode* nodeA = reinterpret_cast<SceneNode*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    SceneNode* nodeB = reinterpret_cast<SceneNode*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);
    b2Vec2 normal = worldManifold.normal;

    Character* nodeBcharacter = dynamic_cast<Character*>(nodeB);
    if (nodeBcharacter) {
        swap(nodeA, nodeB);
        normal = -normal;
    }
    
    // std::cout << "normal: " << normal.x << " " << normal.y << std::endl;
    if (nodeA) nodeA->OnBeginContact(nodeB, normal);
    if (nodeB) nodeB->OnBeginContact(nodeA, normal);
}

void ContactListener::EndContact(b2Contact *contact)
{
    if (!contact) return;

    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();

    SceneNode* nodeA = nullptr;
    SceneNode* nodeB = nullptr;

    if (bodyA && bodyA->GetUserData().pointer) {
        nodeA = reinterpret_cast<SceneNode*>(bodyA->GetUserData().pointer);
    }

    if (bodyB && bodyB->GetUserData().pointer) {
        nodeB = reinterpret_cast<SceneNode*>(bodyB->GetUserData().pointer);
    }

    if (nodeA) nodeA->OnEndContact(nodeB);
    if (nodeB) nodeB->OnEndContact(nodeA);
}
