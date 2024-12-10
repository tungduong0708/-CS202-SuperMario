#include "text_helper.h"

Font TextHelper::font;

void TextHelper::loadFont(string fontPath, string mode) {
    if (fontPath == "") return;
    if (fontPath == "highway_gothic") {
        fontPath = "resources/fonts/HWYGWDE.TTF";
    }
    font = LoadFont(fontPath.c_str());
    if (mode == "default") font = GetFontDefault();
}  

void TextHelper::Draw(const string &text, Vector2 position, int fontSize, Color color) {
    // capitalize all letters
    string t = "";
    for (int i = 0; i < text.size(); i++) {
        t += toupper(text[i]);
    }
    DrawTextEx(font, t.c_str(), {(float)position.x, (float)position.y}, (float)fontSize/16.0, 0, color);
}

void TextHelper::Draw(const string &text, Vector2 position, int fontSize, Color color, string fontPath) {
    loadFont(fontPath);
    string t = "";
    for (int i = 0; i < text.size(); i++) {
        t += toupper(text[i]);
    }
    DrawTextEx(font, text.c_str(), {position.x/16, position.y/16}, (float)fontSize/16.0, 0, color);
}