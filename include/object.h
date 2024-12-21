#pragma once

// Header files for project
// APCS K23, VNU-HCM University of Science.
// Course: CS202 - Programming Systems.
// WARNING: This project is for reference only. Do not copy it for your own project.
// Group 3, Dec 2024.
// -----------------------------------

// Helper package: Headers for utility classes
#include "include.h"
#include "imagehandler.h"
#include "renderer.h"
#include "text_helper.h"
#include "string_mode.h"
#include "file_check_helper.h"

// Visitor pattern for file handling
#include "file_visitor.h"


// Physics package: Headers for box2d physics engine
#include "physics.h"
#include "contactlistener.h"
#include "my_bounding_box.h"


// Effect package: Headers for animation effects
#include "animation_effect_creator.h"
#include "effect_manager.h"


// Tilemap package: Headers for tilemap and tileset
#include "tileset_handler.h"
#include "tilemap.h"
#include "tileset.h"
#include "tile.h"
#include "static_tile.h"
#include "kinematic_tile.h"


// Camera package: Headers for game objects
#include "camera.h"


// Object package: Headers for game objects:
// - SceneNode: Base class for all game objects
// - Background: Background object
// - StaticObject: Static object
// - MovingObject: Moving object
// - Character: Character object, including player and enemies
#include "scene_node.h"
#include "background.h"
#include "static_object.h"
#include "moving_object.h"
#include "character.h"
#include "enemy_creator.h"
#include "object_creator.h"