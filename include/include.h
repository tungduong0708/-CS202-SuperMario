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
using json = nlohmann::json;
using namespace std;


#include "renderer.h"

enum ImageSet {
    IDLE,
    DUCK,
    WALK,
    WALK2,
    JUMP,
    FALL,
    PIPE,
    HOLD,
    DEAD,
    VICTORY
};