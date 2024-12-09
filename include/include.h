#pragma once

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <box2d/box2d.h>
#include <raylib.h>
#include <filesystem>
#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unordered_set>
using json = nlohmann::json;
using namespace std;


#include "renderer.h"

enum ImageSet {
    IDLE,
    DUCK,
    WALK,
    JUMP,
    FALL,
    PIPE,
    DEAD,
    HOLD,
    VICTORY
};


enum Mode {
    SMALL,
    BIG,
    FIRE
};


enum CollisionCategory {
    CATEGORY_PLAYER = 0x0001,
    CATEGORY_FIREBALL = 0x0001 << 1,
    CATEGORY_SCENERY = 0x0001 << 2,
    // Add more categories as needed
};



// bool collide = (nodeA.getCategoryBits() & nodeB.getMaskBits()) != 0 &&
//               (nodeA.getMaskBits() & nodeB.getCategoryBits()) != 0;