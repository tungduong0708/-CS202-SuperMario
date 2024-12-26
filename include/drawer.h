#pragma once

#include "game_state.h" // For Button
#include "raylib.h"

// Forward declaration
class Game;

// Function prototypes
void DrawButton(const Button& button, const Game& game); // Customized button drawing with game's font
void DrawImageButton(const Game& game, const PlayerInfo& player); // Customized image button drawing
void DrawDifficultyButton(const ImageButton& button, const string& difficulty);  // Customized difficulty button drawing
void DrawYesNoButton(const Button& button); // Draw Yes and No buttons

bool IsButtonClicked(const Button& button);             // Handle button click detection
bool IsButtonClicked(const ImageButton& button);         // Handle image button click detection
void DrawMarioSlider(Rectangle rect, int& value, const float& minValue, const float& maxValue, const Font& font, const char* label); // Customized slider drawing

void DrawPlayerImageIcon(Player* player, Vector2 position);  // Draw only the player image
