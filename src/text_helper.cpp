#include "text_helper.h"
#include "imagehandler.h"

Font TextHelper::font;
Texture TextHelper::coin;

void TextHelper::loadTexture(string texturePath) {
    vector<Texture> txts = ImageHandler::setTextures(texturePath);
    coin = txts[0];
}

void TextHelper::loadFont(string fontPath, string mode)
{
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

void TextHelper::DrawPackage(string name, int score, int coins, Vector2 position, int fontSize, Color color) {
    Draw(name, position, fontSize, color);
    Draw(to_string(score), Vector2{position.x, position.y + 1.0f}, fontSize, color);
    Rectangle destRect = {position.x + 8.25f, position.y, 5.0f, 4.5f};
    Rectangle sourceRect = {0, 0, (float)coin.width, (float)coin.height};
    Renderer::DrawPro(coin, sourceRect, Vector2(position.x + 8.25f, position.y), Vector2{1.0f, 1.0f}, false);
    DrawTextEx(font, ("x " + to_string(coins)).c_str(), {position.x + 9.2f, position.y + 0.15f}, (float)fontSize/16, 0, color);
}
