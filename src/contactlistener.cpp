#include "contactlistener.h"
#include "scene_node.h"
#include "physics.h"
#include "kinematic_tile.h"
#include "moving_object.h"

void ContactListener::BeginContact(b2Contact *contact)
{
    SceneNode* nodeA = reinterpret_cast<SceneNode*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    SceneNode* nodeB = reinterpret_cast<SceneNode*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    if (nodeA) nodeA->OnBeginContact(nodeB);
    if (nodeB) nodeB->OnBeginContact(nodeA);
}

void ContactListener::EndContact(b2Contact *contact)
{
    SceneNode* nodeA = reinterpret_cast<SceneNode*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    SceneNode* nodeB = reinterpret_cast<SceneNode*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    if (nodeA) nodeA->OnEndContact(nodeB);
    if (nodeB) nodeB->OnEndContact(nodeA);
}
