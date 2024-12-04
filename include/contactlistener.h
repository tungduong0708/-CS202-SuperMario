#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include "include.h"
#include "moving_object.h"
#include <iostream>
#include <box2d/box2d.h>
#include <raylib.h>
using namespace std;

class ContactListener : public b2ContactListener {
public:
    void BeginContact(b2Contact* contact) override {
        void* userDataA = reinterpret_cast<void*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
        void* userDataB = reinterpret_cast<void*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

        if (userDataA) {
            Character* character = static_cast<Character*>(userDataA);
            character->SetOnGround(true); 
        }

        if (userDataB) {
            Character* character = static_cast<Character*>(userDataB);
            character->SetOnGround(true);
        }
    }

    void EndContact(b2Contact* contact) override {
        void* userDataA = reinterpret_cast<void*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
        void* userDataB = reinterpret_cast<void*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

        if (userDataA) {
            Character* character = static_cast<Character*>(userDataA);
            character->SetOnGround(true); 
        }

        if (userDataB) {
            Character* character = static_cast<Character*>(userDataB);
            character->SetOnGround(true);
        }
    }
};

#endif // CONTACTLISTENER_H