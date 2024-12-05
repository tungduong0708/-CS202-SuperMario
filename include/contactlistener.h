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
    void BeginContact(b2Contact* contact) override;

    void EndContact(b2Contact* contact) override;
};

#endif // CONTACTLISTENER_H