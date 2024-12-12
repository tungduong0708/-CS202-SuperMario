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
