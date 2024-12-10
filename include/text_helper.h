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
    static void loadFont(string fontPath, string mode = "");

    static void Draw(const string &text, Vector2 position = {0.0f, 0.0f}, int fontSize = 13, Color color = WHITE);

    static void Draw(const string &text, Vector2 position, int fontSize, Color color, string fontPath);
private:
    static Font font;
};

#endif // TEXT_HELPER_H