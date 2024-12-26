#pragma once
#include <raylib.h>
#include <iostream>
#include <string>
#include <box2d/box2d.h>
#include <vector>
using namespace std;

#ifndef TEXT_HELPER_H
#define TEXT_HELPER_H

class TextHelper {
public:
    static void loadTexture(string texturePath, string playerPath = "");

    static void loadFont(string fontPath, string mode = "");

    static void Draw(const string &text, Vector2 position = {0.0f, 0.0f}, int fontSize = 13, Color color = WHITE);

    static void Draw(const string &text, Vector2 position, int fontSize, Color color, string fontPath);

    static void DrawPackage(int lives, int score, int coins, string currentMap, float timeLeft, Vector2 position, int fontSize, Color color);

    static void DrawPlayerImageIcon(Texture playerTexture, Vector2 position);  //Draw only the player image

private:
    static Font font;
    static Texture coin;
    static Texture player;
};

#endif // TEXT_HELPER_H