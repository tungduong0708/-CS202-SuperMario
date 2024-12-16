#pragma once

#include "included_library.h"

#include "renderer.h"
#include "scene_node.h"
#include "text_helper.h"
#include "delayed_texture.h"

#include "sound_effect_handler.h"

#define IMAGE_WIDTH 16

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

enum EnemyState {
    ENEMY_WALK,
    ENEMY_DEAD,
    ENEMY_SHELL,
    ENEMY_SPIN
};


enum Mode {
    SMALL,
    BIG,
    FIRE,
};


enum CollisionCategory {
    CATEGORY_PLAYER = 0x0001 << 1,
    CATEGORY_FIREBALL = 0x0001 << 2,
    CATEGORY_SCENERY = 0x0001 << 3,
    // Add more categories as needed
};



// bool collide = (nodeA.getCategoryBits() & nodeB.getMaskBits()) != 0 &&
//               (nodeA.getMaskBits() & nodeB.getCategoryBits()) != 0;

