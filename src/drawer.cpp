#include "drawer.h"
#include <game.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


void DrawButton(const Button& button, const Game& game)
{
    // Define Mario-themed colors
    Color baseColor = button.isHovered ? Fade(RED, 0.9f) : Fade(GOLD, 0.9f); // Red when hovered, gold otherwise
    Color borderColor = button.isHovered ? Fade(WHITE, 1.0f) : Fade(DARKBLUE, 0.9f); // Border changes color
    Color shadowColor = Fade(BLACK, 0.4f);       // Shadow color
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
    DrawRectangleRoundedLines(button.rect, 0.3f, 6, 4, borderColor);

    // --- Text with Shadow ---
    Vector2 textSize = MeasureTextEx(game.getFont(), button.text.c_str(), 20, 1);

    // Text shadow for depth
    DrawTextEx(
        game.getFont(),
        button.text.c_str(),
        {button.rect.x + button.rect.width / 2 - textSize.x / 2 + 2,
         button.rect.y + button.rect.height / 2 - textSize.y / 2 + 2},
        20, 1, shadowColor);

    // Text with vibrant color
    DrawTextEx(
        game.getFont(),
        button.text.c_str(),
        {button.rect.x + button.rect.width / 2 - textSize.x / 2,
         button.rect.y + button.rect.height / 2 - textSize.y / 2},
        20, 1, WHITE);
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

// Customization function
void DrawMarioSlider(Rectangle rect, int& value, const float& minValue, const float& maxValue, const Font& font, const char* label)
{
    // Colors for slider
    Color baseColor = Fade(LIGHTGRAY, 0.9f);
    Color borderColor = DARKGRAY;
    Color shadowColor = Fade(BLACK, 0.2f);
    Color knobColor = YELLOW;
    Color knobShadowColor = Fade(BLACK, 0.3f);
    Color hoverColor = GOLD;

    // --- Background with shadow ---
    // Draw shadow
    DrawRectangleRounded(
        {rect.x + 4, rect.y + 4, rect.width, rect.height}, 0.3f, 6, shadowColor);

    // Draw slider background with border
    DrawRectangleRounded(rect, 0.3f, 6, baseColor);
    DrawRectangleRoundedLines(rect, 0.3f, 6, 2, borderColor);

    // --- Slider bar ---
    Rectangle bar = {
        rect.x + 10, rect.y + rect.height / 2 - 5, rect.width - 20, 10};
    DrawRectangleRounded(bar, 0.5f, 6, BLUE);

    // --- Knob ---
    // Calculate knob position
    float knobX = bar.x + ((value - minValue) / (maxValue - minValue)) * bar.width;

    // Check for hover
    Vector2 mousePos = GetMousePosition();
    bool isHovered = CheckCollisionPointCircle(mousePos, {knobX, rect.y + rect.height / 2}, 15);

    // Draw knob shadow
    DrawCircle(static_cast<int>(knobX) + 3, static_cast<int>(rect.y + rect.height / 2) + 3, 15, knobShadowColor);

    // Draw knob
    DrawCircle(static_cast<int>(knobX), static_cast<int>(rect.y + rect.height / 2), 15, isHovered ? hoverColor : knobColor);

    // Draw star inside the knob
    DrawText("★", static_cast<int>(knobX) - 6, static_cast<int>(rect.y + rect.height / 2) - 8, 20, WHITE);

    // --- Slider Label and Value Text ---
    const char* sliderText = TextFormat("%s: %d", label, value);
    Vector2 textSize = MeasureTextEx(font, sliderText, 20, 1);

    // Draw shadow for text
    DrawTextEx(font, sliderText, {rect.x + (rect.width - textSize.x) / 2 + 2, rect.y - 30 + 2}, 20, 1, shadowColor);

    // Draw text
    DrawTextEx(font, sliderText, {rect.x + (rect.width - textSize.x) / 2, rect.y - 30}, 20, 1, WHITE);

    // --- Mouse Interaction ---
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
    float ratio = player.button.rect.height / player.texture.height * 5 / 6; // Scale texture to button height
    float textureSizeX = player.texture.width * ratio;
    float textureSizeY = player.texture.height * ratio;
    Rectangle textureDest = {player.button.rect.x + texturePaddingX, player.button.rect.y + texturePaddingY, textureSizeX, textureSizeY};

    // Draw the player's texture
    DrawTexturePro(
        isHovered ? player.hoverTexture : player.texture,   // Use hover texture if hovered
        {0, 0, static_cast<float>(player.texture.width), static_cast<float>(player.texture.height)}, // Source rectangle
        textureDest,                                        // Destination rectangle
        {0, 0},                                             // Origin point
        0.0f,                                               // No rotation
        WHITE);

    // --- Text on the Right ---
    float textX = player.button.rect.x + textureSizeX + texturePaddingX * 2; // Start text to the right of the texture
    float textY = player.button.rect.y + player.button.rect.height / 2 - 50; // Center vertically

    // Player name (large text)
    constexpr int nameFontSize = 35;
    Color textColor = player.name == "Mario" ? RED : GREEN; // Diiferent color for each player
    // Draw shadow for player name
    DrawTextEx(
        game.getFont(),
        player.name.c_str(),
        {textX + 2, textY + 2},
        nameFontSize, 2, shadowColor);

    // Draw border for player name
    DrawTextEx(
        game.getFont(),
        player.name.c_str(),
        {textX - 1, textY - 1},
        nameFontSize, 2, BLACK);
    DrawTextEx(
        game.getFont(),
        player.name.c_str(),
        {textX + 1, textY - 1},
        nameFontSize, 2, BLACK);
    DrawTextEx(
        game.getFont(),
        player.name.c_str(),
        {textX - 1, textY + 1},
        nameFontSize, 2, BLACK);
    DrawTextEx(
        game.getFont(),
        player.name.c_str(),
        {textX + 1, textY + 1},
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

    // Draw the description/stats text with border
    // Draw shadow for description
    DrawTextEx(
        game.getFont(),
        description.c_str(),
        {textX + 2, textY + nameFontSize + 22},
        descFontSize, 1, shadowColor);

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
    if (difficulty == "Easy")
    {
        baseColor = GREEN;
        hoverColor = DARKGREEN;
    }
    else if (difficulty == "Medium")
    {
        baseColor = YELLOW;
        hoverColor = ORANGE;
    }
    else if (difficulty == "Hard")
    {
        baseColor = RED;
        hoverColor = MAROON;
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

    // --- Texture/Icon on the Left ---
    float texturePadding = 10.0f; // Padding for the texture
    float textureSize = button.rect.height - texturePadding * 2; // Fit texture vertically
    Rectangle textureDest = {button.rect.x + texturePadding, button.rect.y + texturePadding, textureSize, textureSize};

    DrawTexturePro(
        isHovered ? button.hoverTexture : button.texture,   // Use hover texture if hovered
        {0, 0, static_cast<float>(button.texture.width), static_cast<float>(button.texture.height)}, // Source rectangle
        textureDest,                                        // Destination rectangle
        {0, 0},                                             // Origin point
        0.0f,                                               // No rotation
        WHITE);

    // --- Difficulty Name Centered ---
    float textX = button.rect.x + textureSize + texturePadding * 3; // Start text to the right of the texture
    float textY = button.rect.y + button.rect.height / 2 - 40; // Center text vertically
    constexpr int fontSize = 36; // Font size for difficulty name

    // Draw shadow for text
    DrawTextEx(
        font, // Replace with your custom font if available
        difficulty.c_str(),
        {textX + 2, textY + 2},
        fontSize, 2, Fade(BLACK, 0.6f));

    // Draw main text
    DrawTextEx(
        font, // Replace with your custom font if available
        difficulty.c_str(),
        {textX, textY},
        fontSize, 2, BROWN);

    constexpr int descFontSize = 20;
    std::string description;

    if (difficulty == "Easy")
    {
        description = "Relaxed pace, perfect for \nbeginners to enjoy.";
    }
    else if (difficulty == "Medium")
    {
        description = "Balanced challenge for \nseasoned and casual players alike.";
    }
    else if (difficulty == "Hard")
    {
        description = "Intense gameplay, only for \nthe bravest of heroes!";
    }

    // Draw shadow for description
    DrawTextEx(
        font,
        description.c_str(),
        {textX + 2, textY + fontSize + 10},
        descFontSize, 1, Fade(BLACK, 0.6f));

    // Draw main text
    DrawTextEx(
        font,
        description.c_str(),
        {textX, textY + fontSize + 8},
        descFontSize, 1, BLUE);
}
