#include "text_helper.h"
#include "imagehandler.h"

Font TextHelper::font;
Texture TextHelper::coin;
Texture TextHelper::player;

void TextHelper::loadTexture(string texturePath, string playerPath) {
    vector<Texture> txts = ImageHandler::setTextures(texturePath);
    coin = txts[0];
    if (playerPath != "") {
        vector<Texture> txts = ImageHandler::setTextures(playerPath);
        player = txts[0];
    }
}

void TextHelper::loadFont(string fontPath, string mode)
{
    if (fontPath == "") return;
    if (fontPath == "highway_gothic") {
        fontPath = "resources/fonts/HWYGWDE.TTF";
    }
    else if (fontPath == "Mario256") {
        fontPath = "resources/fonts/SuperMario256.ttf";
    }
    else if (fontPath == "MarioKart") {
        fontPath = "resources/fonts/Mario-Kart-DS.ttf";
    }
    else if (fontPath == "PressStart") {
        fontPath = "resources/fonts/PressStart2P-Regular.ttf";
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

void TextHelper::DrawPackage(int lives, int score, int coins, string currentMap, float timeLeft, Vector2 position, int fontSize, Color color) {
    // Playerimage x lives
    // 00100 (score)
    Rectangle destRect = {position.x, position.y, 15.0f, 19.0f};
    Rectangle sourceRect = {0, 0, (float)player.width, (float)player.height};
    Renderer::DrawPro(player, sourceRect, {position.x, position.y - 0.25f}, Vector2{1.0f, 1.0f}, false);
    DrawTextEx(font, ("x " + to_string(lives)).c_str(), {position.x + 1.5f, position.y}, (float)fontSize/16, 0, color);
    Draw(to_string(score), Vector2{position.x + 0.15f, position.y + 1.0f}, fontSize, color);

    // coins: image x10
    destRect = {position.x + 6.0f, position.y, 5.0f, 4.5f};
    sourceRect = {0, 0, (float)coin.width, (float)coin.height};
    Renderer::DrawPro(coin, sourceRect, Vector2{position.x + 5.2f, position.y - 0.25f}, Vector2{1.0f, 1.0f}, false);
    DrawTextEx(font, ("x " + to_string(coins)).c_str(), {position.x + 6.5f, position.y}, (float)fontSize/16, 0, color);

    //world
    //1-1 (currentMap)
    DrawTextEx(font, "WORLD", {position.x + 10.5f, position.y}, (float)fontSize/16, 0, color);
    //cout << currentMap << endl;
    DrawTextEx(font, currentMap.c_str(), {position.x + 10.5f, position.y + 0.9f}, (float)fontSize/16, 0, color);

    //time
    //001 (timeLeft)
    DrawTextEx(font, "TIME", {position.x + 16.0f, position.y}, (float)fontSize/16, 0, color);
    DrawTextEx(font, to_string((int)timeLeft).c_str(), {position.x + 16.1f, position.y + 0.9f}, (float)fontSize/16, 0, color);
}
