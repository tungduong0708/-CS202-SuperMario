#include "drawer.h"
#include <game.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


void DrawButton(const Button& button, const Game& game)
{
    // Define Mario-themed colors
    Color baseColor = button.isHovered ? Fade(RED, 0.9f) : Fade(GOLD, 0.9f); // Red when hovered, gold otherwise
    Color borderColor = button.isHovered ? Fade(WHITE, 1.0f) : Fade(DARKBLUE, 0.9f); // Border changes color
    Color shadowColor = (Color){242, 107, 15, 255};       // Shadow color
    Color highlightColor = Fade(YELLOW, 0.7f);   // Highlight color on top

    // --- Shadow Effect ---
    DrawRectangleRounded(
        {button.rect.x + 4, button.rect.y + 4, button.rect.width, button.rect.height},
        0.3f, 6, shadowColor);

    // --- Button Body ---
    // Draw main button with rounded corners
    DrawRectangleRounded(button.rect, 0.3f, 6, baseColor);

    // Draw highlight (top gradient effect)
    Rectangle highlightRect = {button.rect.x, button.rect.y, button.rect.width, button.rect.height / 2};
    DrawRectangleRounded(highlightRect, 0.3f, 6, highlightColor);

    // --- Border ---
    DrawRectangleRoundedLines(button.rect, 0.3f, 6, 6, borderColor);

    // --- Text with Shadow ---
    Vector2 textSize = MeasureTextEx(game.getFont(), button.text.c_str(), 30, 1);

    // Text shadow for depth
    DrawTextEx(
        game.getFont(),
        button.text.c_str(),
        {button.rect.x + button.rect.width / 2 - textSize.x / 2 + 2.5f,
         button.rect.y + button.rect.height / 2 - textSize.y / 2},
        30, 1, shadowColor);
    DrawTextEx(
        game.getFont(),
        button.text.c_str(),
        {button.rect.x + button.rect.width / 2 - textSize.x / 2 - 2.5f,
         button.rect.y + button.rect.height / 2 - textSize.y / 2},
        30, 1, shadowColor);
    DrawTextEx(
        game.getFont(),
        button.text.c_str(),
        {button.rect.x + button.rect.width / 2 - textSize.x / 2,
         button.rect.y + button.rect.height / 2 - textSize.y / 2 + 2.5f},
        30, 1, shadowColor);
    DrawTextEx(
        game.getFont(),
        button.text.c_str(),
        {button.rect.x + button.rect.width / 2 - textSize.x / 2,
         button.rect.y + button.rect.height / 2 - textSize.y / 2 - 2.5f},
        30, 1, shadowColor);

    // Text with vibrant color
    DrawTextEx(
        game.getFont(),
        button.text.c_str(),
        {button.rect.x + button.rect.width / 2 - textSize.x / 2,
         button.rect.y + button.rect.height / 2 - textSize.y / 2},
        30, 1, WHITE);
}

bool IsButtonClicked(const Button& button)
{
    return button.isHovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
}

bool IsButtonClicked(const ImageButton& button)
{
    return button.isHovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
}

// Clamp function
template <typename T>
T Clamp(T value, T min, T max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Customized slider drawing, returns true if the slider value was changed
void DrawMarioSlider(Rectangle rect, int& value, const float& minValue, const float& maxValue, const Font& font, const char* label)
{
    // Define Colors with RGBA for customization
    Color baseColor = (Color){200, 200, 200, 255};        // Light gray for background
    Color borderColor = (Color){120, 120, 120, 255};      // Darker gray for border
    Color shadowColor = (Color){0, 0, 0, 50};             // Semi-transparent black for shadow
    Color trackColor = (Color){70, 130, 180, 255};        // Steel blue for track
    Color knobColor = (Color){255, 215, 0, 255};          // Gold for the knob
    Color hoverColor = (Color){255, 165, 0, 255};         // Orange when hovered
    Color textColor = (Color){255, 255, 255, 255};        // Pure white for text
    Color knobShadowColor = (Color){0, 0, 0, 100};        // Dark shadow under the knob

    // --- Shadow for the slider background ---
    DrawRectangleRounded(
        {rect.x + 3, rect.y + 3, rect.width, rect.height}, 0.3f, 6, shadowColor);

    // --- Slider Background ---
    DrawRectangleRounded(rect, 0.3f, 6, baseColor);
    DrawRectangleRoundedLines(rect, 0.3f, 6, 2, borderColor);

    // --- Track Bar ---
    Rectangle bar = {
        rect.x + 10, rect.y + rect.height / 2 - 10, rect.width - 30, 20};
    DrawRectangleRounded(bar, 0.5f, 6, trackColor);

    // --- Knob ---
    float knobX = bar.x + ((value - minValue) / (maxValue - minValue)) * bar.width;
    Vector2 mousePos = GetMousePosition();
    bool isHovered = CheckCollisionPointCircle(mousePos, {knobX, rect.y + rect.height / 2}, 15);

    // Draw knob shadow
    DrawCircle(static_cast<int>(knobX) + 3, static_cast<int>(rect.y + rect.height / 2) + 3, 15, knobShadowColor);

    // Draw the knob itself
    DrawCircle(static_cast<int>(knobX), static_cast<int>(rect.y + rect.height / 2), 15, isHovered ? hoverColor : knobColor);

    // Draw a subtle border for the knob
    DrawCircleLines(static_cast<int>(knobX), static_cast<int>(rect.y + rect.height / 2), 15, borderColor);

    // --- Label and Value ---
    const char* sliderText = TextFormat("%s: %d", label, value);
    Vector2 textSize = MeasureTextEx(font, sliderText, 30, 1);

    // Draw shadow for text
    DrawTextEx(font, sliderText, {rect.x + (rect.width - textSize.x) / 2 + 2, rect.y - 30 + 2}, 30, 1, shadowColor);

    // Draw main text
    DrawTextEx(font, sliderText, {rect.x + (rect.width - textSize.x) / 2, rect.y - 30}, 30, 1, textColor);

    // --- Interaction Logic ---
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, bar))
    {
        float mouseX = mousePos.x;
        value = static_cast<int>(minValue + ((mouseX - bar.x) / bar.width) * (maxValue - minValue));
        value = Clamp(value, static_cast<int>(minValue), static_cast<int>(maxValue));
    }
}

// Customized image button drawing
void DrawImageButton(const Game& game, const PlayerInfo& player)
{
    // Define Mario-themed colors
    Color shadowColor = Fade(BLACK, 0.4f);            // Shadow color
    Color borderColor = player.button.isHovered ? Fade(WHITE, 1.0f) : Fade(DARKBLUE, 0.9f); // Border changes color
    Color baseColor = player.button.isHovered
                         ? (player.name == "Mario" ? PINK : DARKGREEN) // Red for Mario, Green for Luigi
                         : YELLOW;                                             // Gold background when not hovered
    Vector2 mousePos = GetMousePosition();

    // --- Check Hover State ---
    bool isHovered = CheckCollisionPointRec(mousePos, player.button.rect);

    // --- Shadow Effect ---
    DrawRectangleRounded(
        {player.button.rect.x + 4, player.button.rect.y + 4, player.button.rect.width, player.button.rect.height},
        0.3f, 6, shadowColor);

    // --- Button Background ---
    DrawRectangleRounded(player.button.rect, 0.3f, 6, baseColor);
    DrawRectangleRoundedLines(player.button.rect, 0.3f, 6, 2, borderColor);

    // --- Texture on the Left ---
    float texturePaddingX = 50.0f; // Padding for the texture
    float texturePaddingY = 20.0f; // Padding for the texture

    float textureSize = 0;

    if (isHovered)
    {
        float ratio = player.button.rect.height / player.hoverTexture.height * 5 / 6; // Scale texture to button height
        float textureSizeX = player.hoverTexture.width * ratio;
        float textureSizeY = player.hoverTexture.height * ratio;
        Rectangle textureDest = {player.button.rect.x + texturePaddingX, player.button.rect.y + texturePaddingY, textureSizeX, textureSizeY};

        // Draw the player's texture
        DrawTexturePro(
            player.hoverTexture,   // Use hover texture if hovered
            {0, 0, static_cast<float>(player.hoverTexture.width), static_cast<float>(player.hoverTexture.height)}, // Source rectangle
            textureDest,                                        // Destination rectangle
            {0, 0},                                             // Origin point
            0.0f,                                               // No rotation
            WHITE);

        textureSize = textureSizeX;
    }
    else
    {
        float ratio = player.button.rect.height / player.texture.height * 5 / 6; // Scale texture to button height
        float textureSizeX = player.texture.width * ratio;
        float textureSizeY = player.texture.height * ratio;
        Rectangle textureDest = {player.button.rect.x + texturePaddingX, player.button.rect.y + texturePaddingY, textureSizeX, textureSizeY};

        // Draw the player's texture
        DrawTexturePro(
            player.texture,   // Use hover texture if hovered
            {0, 0, static_cast<float>(player.texture.width), static_cast<float>(player.texture.height)}, // Source rectangle
            textureDest,                                        // Destination rectangle
            {0, 0},                                             // Origin point
            0.0f,                                               // No rotation
            WHITE);

        textureSize = textureSizeX;
    }

    // --- Text on the Right ---
    float textX = player.button.rect.x + textureSize + texturePaddingX * 2; // Start text to the right of the texture
    float textY = player.button.rect.y + player.button.rect.height / 2 - 50; // Center vertically

    // Player name (large text)
    constexpr int nameFontSize = 40;
    Color textColor = player.name == "Mario" ? RED : GREEN; // Diiferent color for each player

    // Draw border for player name
    DrawTextEx(
        game.getFont(),
        player.name.c_str(),
        {textX + 3, textY},
        nameFontSize, 2, BLACK);
    DrawTextEx(
        game.getFont(),
        player.name.c_str(),
        {textX, textY + 3},
        nameFontSize, 2, BLACK);
    DrawTextEx(
        game.getFont(),
        player.name.c_str(),
        {textX, textY},
        nameFontSize, 2, textColor);

    // Description or stats (smaller text)
    constexpr int descFontSize = 18;
    std::string description;

    if (isHovered)
    {
        // Display player stats when hovered
        description = TextFormat("Speed: %d\nJump: %d\nBullet Speed: %d\nAttack Speed: %d",
                                 player.speed, player.jumpForce, player.bulletSpeed, player.bulletFreq);
    }
    else
    {
        // Fun description when not hovered
        description = (player.name == "Mario") ? "The heroic plumber!" : "The green brother!";
    }

    Color descColor = isHovered ? GOLD : WHITE;

    // Draw border for description
    DrawTextEx(
        game.getFont(),
        description.c_str(),
        {textX - 1, textY + nameFontSize + 19},
        descFontSize, 1, BLACK);
    DrawTextEx(
        game.getFont(),
        description.c_str(),
        {textX + 1, textY + nameFontSize + 19},
        descFontSize, 1, BLACK);
    DrawTextEx(
        game.getFont(),
        description.c_str(),
        {textX - 1, textY + nameFontSize + 21},
        descFontSize, 1, BLACK);
    DrawTextEx(
        game.getFont(),
        description.c_str(),
        {textX + 1, textY + nameFontSize + 21},
        descFontSize, 1, BLACK);

    // Draw the description/stats text
    DrawTextEx(
        game.getFont(),
        description.c_str(),
        {textX, textY + nameFontSize + 20}, // Position below the name
        descFontSize, 1, descColor);
}

void DrawDifficultyButton(const ImageButton& button, const std::string& difficulty)
{
    Font font = Game::getInstance()->getFont();
    // Define base colors for difficulty levels
    Color baseColor;
    Color hoverColor;
    if (difficulty == "Hard")
    {
        baseColor = (Color){198, 35, 0, 255};
        hoverColor = (Color){255, 69, 0, 255};
    }
    else if (difficulty == "Nightmare")
    {
        baseColor = (Color){80, 0, 115, 255};
        hoverColor = (Color){25, 25, 112, 255};
    }
    else if (difficulty == "Asian")
    {
        baseColor = (Color){42, 0, 78, 255};
        hoverColor = (Color){70, 70, 70, 255};
    }

    // Check hover state
    Vector2 mousePos = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePos, button.rect);

    // Adjust color based on hover state
    Color backgroundColor = isHovered ? hoverColor : baseColor;

    // Border color
    Color borderColor = isHovered ? WHITE : DARKBLUE;

    // --- Shadow Effect ---
    DrawRectangleRounded(
        {button.rect.x + 4, button.rect.y + 4, button.rect.width, button.rect.height},
        0.2f, 6, Fade(BLACK, 0.4f));

    // --- Background ---
    DrawRectangleRounded(button.rect, 0.2f, 6, backgroundColor);

    // --- Border ---
    DrawRectangleRoundedLines(button.rect, 0.2f, 6, 3, borderColor);

    float texturePadding = 10.0f; // Padding for the texture
    float textureSize = button.rect.height - texturePadding * 2; // Fit texture vertically

    if (isHovered) {
        // --- Texture/Icon on the Left ---
        float ratio = button.rect.height / button.hoverTexture.height * 4 / 5; // Scale texture to button height
        float textureSizeX = button.hoverTexture.width * ratio;
        float textureSizeY = button.hoverTexture.height * ratio;
        Rectangle textureDest = {button.rect.x + texturePadding, button.rect.y + texturePadding, textureSizeX, textureSizeY};

        DrawTexturePro(
            button.hoverTexture,   // Use hover texture if hovered
            {0, 0, static_cast<float>(button.hoverTexture.width), static_cast<float>(button.hoverTexture.height)}, // Source rectangle
            textureDest,                                        // Destination rectangle
            {0, 0},                                             // Origin point
            0.0f,                                               // No rotation
            WHITE);
    }
    else {
        // --- Texture/Icon on the Left ---
        float ratio = button.rect.height / button.texture.height * 4 / 5; // Scale texture to button height
        float textureSizeX = button.texture.width * ratio;
        float textureSizeY = button.texture.height * ratio;
        Rectangle textureDest = {button.rect.x + texturePadding, button.rect.y + texturePadding, textureSizeX, textureSizeY};

        DrawTexturePro(
            button.texture,   // Use hover texture if hovered
            {0, 0, static_cast<float>(button.texture.width), static_cast<float>(button.texture.height)}, // Source rectangle
            textureDest,                                        // Destination rectangle
            {0, 0},                                             // Origin point
            0.0f,                                               // No rotation
            WHITE);
    }

    // --- Difficulty Name Centered ---
    float textX = button.rect.x + textureSize + texturePadding * 3; // Start text to the right of the texture
    float textY = button.rect.y + button.rect.height / 2 - 40; // Center text vertically
    constexpr int fontSize = 36; // Font size for difficulty name

    // Draw border for text
    DrawTextEx(
        font, // Replace with your custom font if available
        difficulty.c_str(),
        {textX + 2, textY},
        fontSize, 2, BLACK);
    DrawTextEx(
        font, // Replace with your custom font if available
        difficulty.c_str(),
        {textX - 2, textY},
        fontSize, 2, BLACK);
    DrawTextEx(
        font, // Replace with your custom font if available
        difficulty.c_str(),
        {textX, textY + 2},
        fontSize, 2, BLACK);
    DrawTextEx(
        font, // Replace with your custom font if available
        difficulty.c_str(),
        {textX, textY - 2},
        fontSize, 2, BLACK);

    // Draw main text
    DrawTextEx(
        font, // Replace with your custom font if available
        difficulty.c_str(),
        {textX - 2, textY - 2},
        fontSize, 2, (Color){252, 199, 55, 255});

    constexpr int descFontSize = 16;
    std::string description;

    if (difficulty == "Hard")
    {
        description = "Every step is torment, survival demands\n\nrelentless precision and grit.";
    }
    else if (difficulty == "Nightmare")
    {
        description = "A descent into despair, where hope and\n\nvictory are distant dreams.";
    }
    else if (difficulty == "Asian")
    {
        description = "An unforgiving void, mocking every effort\n\nwith inevitable, crushing defeat.";
    }

    // Draw border for description
    DrawTextEx(
        font,
        description.c_str(),
        {textX + 2, textY + fontSize + 10},
        descFontSize, 1, BLACK);
    DrawTextEx(
        font,
        description.c_str(),
        {textX - 2, textY + fontSize + 10},
        descFontSize, 1, BLACK);
    DrawTextEx(
        font,
        description.c_str(),
        {textX, textY + fontSize + 8},
        descFontSize, 1, BLACK);
    DrawTextEx(
        font,
        description.c_str(),
        {textX, textY + fontSize + 12},
        descFontSize, 1, BLACK);

    // Draw main text
    DrawTextEx(
        font,
        description.c_str(),
        {textX, textY + fontSize + 10},
        descFontSize, 1, (Color){255, 233, 160, 255});
}

void DrawYesNoButton(const Button& button) {
    // Green for Yes, Red for No
    Color baseColor = button.text == "Yes" ? GREEN : RED;
    Color hoverColor = button.isHovered ? Fade(baseColor, 0.8f) : baseColor;
    Color borderColor = button.isHovered ? WHITE : DARKBLUE;

    // --- Shadow Effect ---
    DrawRectangleRounded(
        {button.rect.x + 4, button.rect.y + 4, button.rect.width, button.rect.height},
        0.3f, 6, Fade(BLACK, 0.4f));
    
    // --- Button Background ---
    DrawRectangleRounded(button.rect, 0.3f, 6, baseColor);
    DrawRectangleRoundedLines(button.rect, 0.3f, 6, 2, borderColor);

    // --- Text with Shadow ---
    Vector2 textSize = MeasureTextEx(Game::getInstance()->getFont(), button.text.c_str(), 30, 1);

    // Text shadow for depth
    DrawTextEx(
        Game::getInstance()->getFont(),
        button.text.c_str(),
        {button.rect.x + button.rect.width / 2 - textSize.x / 2 + 2.5f,
         button.rect.y + button.rect.height / 2 - textSize.y / 2},
        30, 1, Fade(BLACK, 0.6f));
    DrawTextEx(
        Game::getInstance()->getFont(),
        button.text.c_str(),
        {button.rect.x + button.rect.width / 2 - textSize.x / 2 - 2.5f,
         button.rect.y + button.rect.height / 2 - textSize.y / 2},
        30, 1, Fade(BLACK, 0.6f));
    DrawTextEx(
        Game::getInstance()->getFont(),
        button.text.c_str(),
        {button.rect.x + button.rect.width / 2 - textSize.x / 2,
         button.rect.y + button.rect.height / 2 - textSize.y / 2 + 2.5f},
        30, 1, Fade(BLACK, 0.6f));
    DrawTextEx(
        Game::getInstance()->getFont(),
        button.text.c_str(),
        {button.rect.x + button.rect.width / 2 - textSize.x / 2,
         button.rect.y + button.rect.height / 2 - textSize.y / 2 - 2.5f},
        30, 1, Fade(BLACK, 0.6f));
    
    // Text with vibrant color
    DrawTextEx(
        Game::getInstance()->getFont(),
        button.text.c_str(),
        {button.rect.x + button.rect.width / 2 - textSize.x / 2,
         button.rect.y + button.rect.height / 2 - textSize.y / 2},
        30, 1, WHITE);
}
