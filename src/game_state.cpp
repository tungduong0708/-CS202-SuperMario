#include "game_state.h"
#include "game.h"
#include "drawer.h"
#include "raylib.h"

GameState::GameState(Game *game)
{
    this->game = game;
}

MainMenuState::MainMenuState(Game* game): GameState(game)
{
    // Initialize buttons
    float buttonWidth = static_cast<float>(Game::getScreenWidth()) * 0.4;
    float buttonHeight = 50;
    float column1X = static_cast<float>(Game::getScreenWidth()) / 4 - buttonWidth / 2;
    float column2X = 3 * static_cast<float>(Game::getScreenWidth()) / 4 - buttonWidth / 2;
    float firstButtonWidth = column2X - column1X + buttonWidth;

    buttons.push_back({{column1X, 300, firstButtonWidth, buttonHeight}, "New Game", false});
    buttons.push_back({{column1X, 375, buttonWidth, buttonHeight}, "Settings", false});
    buttons.push_back({{column2X, 375, buttonWidth, buttonHeight}, "Saved Games", false});
    buttons.push_back({{column1X, 450, buttonWidth, buttonHeight}, "Map Builder", false});
    buttons.push_back({{column2X, 450, buttonWidth, buttonHeight}, "Exit", false});

    // Load background image
    backgroundTexture = LoadTexture("../resources/background/menuBackground.png");

    // Load logo image
    logoTexture = LoadTexture("../resources/images/logo/mario-logo.png");
}

void MainMenuState::update() {
    // Update button hover states
    for (auto& button : buttons) {
        button.isHovered = CheckCollisionPointRec(GetMousePosition(), button.rect);
    }

    // Handle button clicks
    if (IsButtonClicked(buttons[0])) {
        game->changeState(game->selectPlayerState.get());
    }
    if (IsButtonClicked(buttons[1])) {
        game->changeState(game->settingsState.get());
    }
    if (IsButtonClicked(buttons[2])) {
        game->changeState(game->savedGameState.get());
    }
    if (IsButtonClicked(buttons[3])) {
        game->changeState(game->mapBuilderState.get());
    }
    if (IsButtonClicked(buttons[4])) {
        game->exitGame(); // Exit the game
    }
}

void MainMenuState::draw() {
    // Draw the current background frame resized to fit the screen
    DrawTexturePro(
        backgroundTexture,
        {0, 0, static_cast<float>(backgroundTexture.width), static_cast<float>(backgroundTexture.height)},
        {0, 0, static_cast<float>(game->getScreenWidth()), static_cast<float>(game->getScreenHeight())},
        {0, 0},
        0.0f,
        WHITE
    );

    // Draw logo inside the rectangle of size 300x(logoHeight * 300 / logoWidth), remove the image's background
    float logoWidth = logoTexture.width;
    float logoHeight = logoTexture.height;
    float logoRectWidth = 400.0f;
    float logoRectHeight = logoRectWidth * logoHeight / logoWidth;
    DrawTexturePro(
        logoTexture,
        {0, 0, static_cast<float>(logoTexture.width), static_cast<float>(logoTexture.height)},
        {game->getScreenWidth() / 2 - logoRectWidth / 2, 70, logoRectWidth, logoRectHeight},
        {0, 0},
        0.0f,
        WHITE
    );

    // Draw buttons
    for (const auto& button : buttons) {
        DrawButton(button, *game);
    }
}

void MainMenuState::drawBackground()
{
    // Draw the current background frame resized to fit the screen
    DrawTexturePro(
        backgroundTexture,
        {0, 0, static_cast<float>(backgroundTexture.width), static_cast<float>(backgroundTexture.height)},
        {0, 0, static_cast<float>(game->getScreenWidth()), static_cast<float>(game->getScreenHeight())},
        {0, 0},
        0.0f,
        WHITE
    );
}

MainMenuState::~MainMenuState() {
    // Unload background texture
    UnloadTexture(backgroundTexture);
    // Unload logo texture
    UnloadTexture(logoTexture);
}

SettingsState::SettingsState(Game* game)
    : GameState(game)
{
    // Initialize buttons
    float buttonWidth = 250;
    float buttonHeight = 50;
    float centerX = (game->getScreenWidth() - buttonWidth) / 2;
    buttons.push_back({{centerX, 150, buttonWidth, buttonHeight}, "", false});
    buttons.push_back({{centerX, 225, buttonWidth, buttonHeight}, "", false});
    buttons.push_back({{centerX, 300, buttonWidth, buttonHeight}, "Back to Menu", false});
}

void SettingsState::update() {
    // Update button texts based on settings
    buttons[0].text = game->getSettings().music ? "Music: On" : "Music: Off";
    buttons[1].text = game->getSettings().soundEffects ? "Sound Effects: On" : "Sound Effects: Off";

    // Update button hover states
    for (auto& button : buttons) {
        button.isHovered = CheckCollisionPointRec(GetMousePosition(), button.rect);
    }

    // Handle button clicks
    if (IsButtonClicked(buttons[0])) {
        game->getSettings().music = !game->getSettings().music;
    }
    if (IsButtonClicked(buttons[1])) {
        game->getSettings().soundEffects = !game->getSettings().soundEffects;
    }
    if (IsButtonClicked(buttons[2])) {
        game->changeState(game->mainMenuState.get());
    }

    float centerX = (game->getScreenWidth() - 250) / 2;
    // Update sliders
    DrawMarioSlider({centerX, 450, 250, 20}, game->getSettings().volume, 0, 100, game->getFont(), "Volume");
    DrawMarioSlider({centerX, 525, 250, 20}, game->getSettings().brightness, 0, 100, game->getFont(), "Brightness");
}

void SettingsState::draw() {
    // Draw the underlying GameplayState
    game->mainMenuState->drawBackground();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.5f));
    // Draw buttons
    for (const auto& button : buttons) {
        DrawButton(button, *game);
    }

    float centerX = (game->getScreenWidth() - 250) / 2;
    // Draw sliders
    DrawMarioSlider({centerX, 450, 250, 20}, game->getSettings().volume, 0, 100, game->getFont(), "Volume");
    DrawMarioSlider({centerX, 525, 250, 20}, game->getSettings().brightness, 0, 100, game->getFont(), "Brightness");
}

SavedGameState::SavedGameState(Game* game)
    : GameState(game)
{
    // Initialize buttons for saved game slots
    float buttonWidth = 250;
    float buttonHeight = 50;
    float centerX = (game->getScreenWidth() - buttonWidth) / 2;
    for (int i = 0; i < 5; ++i) {
        buttons.push_back(Button{{centerX, static_cast<float>(100 + i * 75), buttonWidth, buttonHeight}, "Slot " + std::to_string(i + 1), false});
    }
    // Initialize back to menu button
    buttons.push_back({{centerX, 475, buttonWidth, buttonHeight}, "Back to Menu", false});
}

void SavedGameState::update() {
    // Update button hover states
    for (auto& button : buttons) {
        button.isHovered = CheckCollisionPointRec(GetMousePosition(), button.rect);
    }

    // Handle button clicks
    for (int i = 0; i < 5; ++i) {
        if (IsButtonClicked(buttons[i])) {
            // Handle saved game slot click (currently does nothing)
        }
    }
    if (IsButtonClicked(buttons[5])) {
        game->changeState(game->mainMenuState.get());
    }
}

void SavedGameState::draw() {
    // Draw the underlying GameplayState
    game->mainMenuState->drawBackground();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.5f));
    // Draw buttons
    for (const auto& button : buttons) {
        DrawButton(button, *game);
    }
}

MapBuilderState::MapBuilderState(Game* game)
    : GameState(game)
{
    backgroundTexture = LoadTexture("../resources/background/menuBackground.png");
    float buttonWidth = 35;
    float buttonHeight = 35;
    pauseButton = {{755, 10, buttonWidth, buttonHeight}, "II", false};
}

void MapBuilderState::update() {
    // Update pause button hover state
    pauseButton.isHovered = CheckCollisionPointRec(GetMousePosition(), pauseButton.rect);

    // Handle pause button click
    if (IsButtonClicked(pauseButton)) {
        game->changeState(game->mapPauseState.get());
    }
}

void MapBuilderState::draw() {
    DrawTexturePro(
        backgroundTexture,
        {0, 0, static_cast<float>(backgroundTexture.width), static_cast<float>(backgroundTexture.height)},
        {0, 0, static_cast<float>(game->getScreenWidth()), static_cast<float>(game->getScreenHeight())},
        {0, 0},
        0.0f,
        WHITE
    );
    DrawButton(pauseButton, *game);
}

MapBuilderState::~MapBuilderState() {
    UnloadTexture(backgroundTexture);
}

GameplayState::GameplayState(Game* game)
    : GameState(game)
{
    SetTraceLogCallback([](int logLevel, const char* text, va_list args) {
        return;
    });

    Physics::Init(); 
    AnimationEffectCreator::InitEffects();
    EnemyCreator::InitEnemies();
    ObjectCreator::InitObjects();
    TextHelper::loadFont("PressStart2P-Regular", "");
    TilesetHandler::Init();
    
    float buttonWidth = 35;
    float buttonHeight = 35;
    pauseButton = {{755, 10, buttonWidth, buttonHeight}, "II", false};
}


void GameplayState::update() {
    float deltaTime = GetFrameTime();
    Physics::Update(deltaTime); 

    Tilemap* tilemap = Tilemap::getInstance();
    tilemap->Update(deltaTime);

    // Update pause button hover state
    pauseButton.isHovered = CheckCollisionPointRec(GetMousePosition(), pauseButton.rect);

    // Handle pause button click
    if (IsButtonClicked(pauseButton)) {
        game->changeState(game->pauseGameState.get());
    }
}

void GameplayState::draw() {
    Tilemap* tilemap = Tilemap::getInstance();
    tilemap->Draw();

    DrawButton(pauseButton, *game);
}

void GameplayState::cleanup() {
    TilesetHandler::Clear();
}

PauseGameState::PauseGameState(Game* game)
    : GameState(game)
{
    // Initialize buttons
    float buttonWidth = 250;
    float buttonHeight = 50;
    float centerX = (game->getScreenWidth() - buttonWidth) / 2;
    buttons.push_back({{centerX, 100, buttonWidth, buttonHeight}, "", false});
    buttons.push_back({{centerX, 175, buttonWidth, buttonHeight}, "", false});
    buttons.push_back({{centerX, 250, buttonWidth, buttonHeight}, "Resume", false});
    buttons.push_back({{centerX, 325, buttonWidth, buttonHeight}, "Back to Menu", false});
}

void PauseGameState::update() {
    // Update button texts based on settings
    buttons[0].text = game->getSettings().music ? "Music: On" : "Music: Off";
    buttons[1].text = game->getSettings().soundEffects ? "Sound Effects: On" : "Sound Effects: Off";

    // Update button hover states
    for (auto& button : buttons) {
        button.isHovered = CheckCollisionPointRec(GetMousePosition(), button.rect);
    }

    // Handle button clicks
    if (IsButtonClicked(buttons[0])) {
        game->getSettings().music = !game->getSettings().music;
    }
    if (IsButtonClicked(buttons[1])) {
        game->getSettings().soundEffects = !game->getSettings().soundEffects;
    }
    if (IsButtonClicked(buttons[2])) {
        game->changeState(game->gameplayState.get());
    }
    if (IsButtonClicked(buttons[3])) {
        game->changeState(game->mainMenuState.get());
    }

    float centerX = (game->getScreenWidth() - 250) / 2;
    // Update sliders
    DrawMarioSlider({centerX, 450, 250, 20}, game->getSettings().volume, 0, 100, game->getFont(), "Volume");
    DrawMarioSlider({centerX, 525, 250, 20}, game->getSettings().brightness, 0, 100, game->getFont(), "Brightness");
}

void PauseGameState::draw() {
    // Draw the underlying GameplayState
    game->gameplayState->draw();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.5f));

    // Draw buttons
    for (const auto& button : buttons) {
        DrawButton(button, *game);
    }

    float centerX = (game->getScreenWidth() - 250) / 2;
    // Draw sliders
    DrawMarioSlider({centerX, 450, 250, 20}, game->getSettings().volume, 0, 100, game->getFont(), "Volume");
    DrawMarioSlider({centerX, 525, 250, 20}, game->getSettings().brightness, 0, 100, game->getFont(), "Brightness");
}

MapPauseState::MapPauseState(Game *game) : PauseGameState(game)
{   
}

void MapPauseState::draw()
{
    // Draw the underlying MapBuiderState
    game->mapBuilderState->draw();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.5f));

    // Draw buttons
    for (const auto& button : buttons) {
        DrawButton(button, *game);
    }

    float centerX = (game->getScreenWidth() - 250) / 2;
    // Draw sliders
    DrawMarioSlider({centerX, 450, 250, 20}, game->getSettings().volume, 0, 100, game->getFont(), "Volume");
    DrawMarioSlider({centerX, 525, 250, 20}, game->getSettings().brightness, 0, 100, game->getFont(), "Brightness");
}

SelectPlayerState::SelectPlayerState(Game* game) : GameState(game) {
    // Initialize buttons
    float buttonWidth = 500;
    float buttonHeight = 250;
    float column1X = 30;
    float column2X = static_cast<float>(game->getScreenWidth()) - buttonWidth - 30;

    player1.name = "Mario";
    player1.texture = LoadTexture("../resources/images/smallmario/idle.png");
    player1.hoverTexture = LoadTexture("../resources/images/smallmario/victory.png");
    player1.button = {{column1X, 30, buttonWidth, buttonHeight}, player1.texture, player1.hoverTexture, false};
    player1.speed = 85;
    player1.jumpForce = 90;
    player1.bulletSpeed = 72;
    player1.bulletFreq = 60;

    player2.name = "Luigi";
    player2.texture = LoadTexture("../resources/images/smallluigi/idle.png");
    player2.hoverTexture = LoadTexture("../resources/images/smallluigi/victory.png");
    player2.button = {{column2X, 330, buttonWidth, buttonHeight}, player2.texture, player2.hoverTexture, false};
    player2.speed = 75;
    player2.jumpForce = 84;
    player2.bulletSpeed = 88;
    player2.bulletFreq = 80;
}

void SelectPlayerState::update() {
    // Update button hover states
    player1.button.isHovered = CheckCollisionPointRec(GetMousePosition(), player1.button.rect);
    player2.button.isHovered = CheckCollisionPointRec(GetMousePosition(), player2.button.rect);

    // Handle button clicks
    if (IsButtonClicked(player1.button)) {
        // Set player to Mario
        Tilemap* tilemap = Tilemap::getInstance();
        tilemap->LoadMapFromJson("map-1-1.json");
        tilemap->setPlayer("mario");

        game->changeState(game->gameplayState.get());
    }
    if (IsButtonClicked(player2.button)) {
        // Set player to Luigi
        Tilemap* tilemap = Tilemap::getInstance();
        tilemap->LoadMapFromJson("map-1-1.json");
        tilemap->setPlayer("luigi");
        
        game->changeState(game->gameplayState.get());
    }
}

void SelectPlayerState::draw() {
    // Draw the underlying MainMenuState
    game->mainMenuState->drawBackground();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.6f));
    
    // Draw buttons
    DrawImageButton(*game, player1);
    DrawImageButton(*game, player2);
}

SelectPlayerState::~SelectPlayerState()
{
    UnloadTexture(player1.button.texture);
    UnloadTexture(player1.button.hoverTexture);
    UnloadTexture(player2.button.texture);
    UnloadTexture(player2.button.hoverTexture);
}

ChangeStageState::ChangeStageState(Game* game) : GameState(game), elapsedTime(0.0f), lifeRemaining(3)
{
    characterTexture = LoadTexture("../resources/images/smallmario/idle.png");
}

void ChangeStageState::update()
{
    elapsedTime += GetFrameTime();

    if (elapsedTime > 3.0f) {
        game->changeState(game->gameplayState.get());
        reset();
    }
}

void ChangeStageState::draw()
{
    setStageName("Stage 1-2");
    // Draw the underlying GameplayState
    game->gameplayState->draw();

    // Draw a semi-transparent black overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(BLACK, 0.6f));

    // Draw the central rounded rectangle (panel)
    float rectWidth = 300.0f;
    float rectHeight = 150.0f;
    float rectX = (game->getScreenWidth() - rectWidth) / 2;
    float rectY = (game->getScreenHeight() - rectHeight) / 2;

    // Draw shadow for depth
    DrawRectangleRounded({rectX + 6, rectY + 6, rectWidth, rectHeight}, 0.2f, 10, Fade(BLACK, 0.4f));

    int gradientSteps = 10;  // Number of steps for the gradient
    float stepHeight = rectHeight / static_cast<float>(gradientSteps); // Height of each step

    for (int i = 0; i < gradientSteps; i++)
    {
        float alpha = 0.8f - (static_cast<float>(i) * 0.1f); // Gradually decrease alpha for a fade effect
        Color stepColor = Fade(YELLOW, alpha); // Base color BLUE, fading gradually

        DrawRectangleRounded(
            {rectX, rectY + static_cast<float>(i) * stepHeight, rectWidth, stepHeight + 1}, // Adjust height for smooth overlap
            0.2f, 10, stepColor);
    }

    // Draw Stage Name with Shadow
    constexpr int fontSize = 30;
    constexpr int spacing = 1;

    Vector2 stageTextSize = MeasureTextEx(game->getFont(), stageName.c_str(), fontSize, spacing);
    Vector2 stageTextPos = {
        rectX + (rectWidth - stageTextSize.x) / 2, // Center horizontally
        rectY + 20                                // Margin from the top
    };

    // Draw text shadow
    DrawTextEx(game->getFont(), stageName.c_str(),
               {stageTextPos.x + 2, stageTextPos.y + 2}, fontSize, 2, Fade(BLACK, 0.6f));
    // Draw main text
    DrawTextEx(game->getFont(), stageName.c_str(),
               stageTextPos, fontSize, 2, BLUE);

    // Draw Mario's Image (Scaled 2x)
    float imageScale = 2.0f;
    float imageX = rectX + 40;
    float imageY = rectY + (rectHeight - characterTexture.height * imageScale) / 3 * 2;
    DrawTextureEx(characterTexture, {imageX, imageY}, 0.0f, imageScale, WHITE);

    // Draw Life Count with Shadow
    constexpr int lifeFontSize = 24;
    Vector2 lifeTextPos = {imageX + 90, imageY + 10};

    // Draw text shadow
    DrawTextEx(game->getFont(), TextFormat("x %d", lifeRemaining),
               {lifeTextPos.x + 2, lifeTextPos.y + 2}, lifeFontSize, 2, Fade(BLACK, 0.6f));
    // Draw main text
    DrawTextEx(game->getFont(), TextFormat("x %d", lifeRemaining),
               lifeTextPos, lifeFontSize, 2, WHITE);

    // Add a decorative border
    DrawRectangleRoundedLines({rectX, rectY, rectWidth, rectHeight}, 0.2f, 10, 5, WHITE);
}

void ChangeStageState::setLifeRemaining(int life)
{
    lifeRemaining = life;
}

void ChangeStageState::reset()
{
    elapsedTime = 0.0f; // Reset elapsed time
}

void ChangeStageState::setStageName(const std::string& name)
{
    stageName = name;
}

ChangeStageState::~ChangeStageState()
{
    UnloadTexture(characterTexture);
}

DeathState::DeathState(Game* game) : ChangeStageState(game), showDeathImage(false)
{
    deathTexture = LoadTexture("../resources/images/smallmario/dead.png");
}

void DeathState::update()
{
    elapsedTime += GetFrameTime();

    if (elapsedTime > 1.0f && elapsedTime < 2.0f && !showDeathImage) {
        showDeathImage = true;
    }

    if (elapsedTime > 2.0f && showDeathImage) {
        showDeathImage = false;
        lifeRemaining--;
    }

    if (elapsedTime > 4.0f) {
        game->changeState(game->gameplayState.get());
        reset();
    }
}

void DeathState::draw() {
    // Draw the underlying GameplayState
    game->gameplayState->draw();

    // Draw a semi-transparent black overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(BLACK, 0.6f));

    // Draw the central rounded rectangle (panel)
    float rectWidth = 300.0f;
    float rectHeight = 150.0f;
    float rectX = (game->getScreenWidth() - rectWidth) / 2;
    float rectY = (game->getScreenHeight() - rectHeight) / 2;

    // Draw shadow for depth
    DrawRectangleRounded({rectX + 6, rectY + 6, rectWidth, rectHeight}, 0.2f, 10, Fade(BLACK, 0.4f));

    int gradientSteps = 10;  // Number of steps for the gradient
    float stepHeight = rectHeight / static_cast<float>(gradientSteps); // Height of each step

    for (int i = 0; i < gradientSteps; i++) {
        float alpha = 0.8f - (static_cast<float>(i) * 0.1f); // Gradually decrease alpha for a fade effect
        Color stepColor = Fade(RED, alpha); // Base color RED, fading gradually

        DrawRectangleRounded(
            {rectX, rectY + static_cast<float>(i) * stepHeight, rectWidth, stepHeight + 1.5f}, // Adjust height for smooth overlap
            0.2f, 10, stepColor);
    }

    // Draw "You Died" text with Shadow
    constexpr int fontSize = 30;
    constexpr int spacing = 1;

    const char* deathText = "You Died";
    Vector2 deathTextSize = MeasureTextEx(game->getFont(), deathText, fontSize, spacing);
    Vector2 deathTextPos = {
        rectX + (rectWidth - deathTextSize.x) / 2, // Center horizontally
        rectY + 20                                // Margin from the top
    };

    // Draw text shadow
    DrawTextEx(game->getFont(), deathText,
               {deathTextPos.x + 2, deathTextPos.y + 2}, fontSize, 2, Fade(BLACK, 0.6f));
    // Draw main text
    DrawTextEx(game->getFont(), deathText,
               deathTextPos, fontSize, 2, ORANGE);

    // Draw Mario's Image (Scaled 2x)
    float imageScale = 2.0f;
    float imageX = rectX + 70;
    float imageY = rectY + (rectHeight - characterTexture.height * imageScale) / 2;
    DrawTextureEx(characterTexture, {imageX, imageY}, 0.0f, imageScale, WHITE);

    // Draw Life Count with Shadow
    constexpr int lifeFontSize = 24;
    Vector2 lifeTextPos = {imageX + 90, imageY + 10};

    // Draw text shadow
    DrawTextEx(game->getFont(), TextFormat("x %d", lifeRemaining),
               {lifeTextPos.x + 2, lifeTextPos.y + 2}, lifeFontSize, 2, Fade(BLACK, 0.6f));
    // Draw main text
    DrawTextEx(game->getFont(), TextFormat("x %d", lifeRemaining),
               lifeTextPos, lifeFontSize, 2, WHITE);

    // Draw Death Image
    if (showDeathImage) {
        float deathImageScale = 1.5f;
        float deathImageX = rectX + 70;
        float deathImageY = rectY + (rectHeight - deathTexture.height * deathImageScale) / 6 * 5;
        DrawTextureEx(deathTexture, {deathImageX, deathImageY}, 0.0f, deathImageScale, WHITE);

        // Write life - 1
        constexpr int deathFontSize = 18;
        Vector2 deathTextPos = {deathImageX + 72, deathImageY + 10};

        // Draw text shadow
        DrawTextEx(game->getFont(), TextFormat("LIFE - %d", 1),
                   {deathTextPos.x + 2, deathTextPos.y + 2}, deathFontSize, 2, Fade(BLACK, 0.6f));
        // Draw main text
        DrawTextEx(game->getFont(), TextFormat("LIFE - %d", 1),
                   deathTextPos, deathFontSize, 2, WHITE);
    }

    // Add a decorative border
    DrawRectangleRoundedLines({rectX, rectY, rectWidth, rectHeight}, 0.2f, 10, 5, WHITE);
}

void DeathState::reset()
{
    elapsedTime = 0.0f; // Reset elapsed time
    showDeathImage = false; // Reset showDeathImage flag
}

DeathState::~DeathState()
{
    UnloadTexture(deathTexture);
}

GameOverState::GameOverState(Game* game) : GameState(game), score(0), highScore(0), timeRemaining(0) {
    // Initialize buttons
    float buttonWidth = 250;
    float buttonHeight = 50;
    float centerX = (game->getScreenWidth() - buttonWidth) / 2;
    buttons.push_back({{centerX, 300, buttonWidth, buttonHeight}, "Main Menu", false});
    buttons.push_back({{centerX, 375, buttonWidth, buttonHeight}, "Retry", false});
}

void GameOverState::update() {
    // Update button hover states
    for (auto& button : buttons) {
        button.isHovered = CheckCollisionPointRec(GetMousePosition(), button.rect);
    }

    // Handle button clicks
    if (IsButtonClicked(buttons[0])) {
        game->changeState(game->mainMenuState.get());
    }
    if (IsButtonClicked(buttons[1])) {
        // Retry the game
    }
}

void GameOverState::draw() {
    // Draw the underlying GameplayState
    game->gameplayState->draw();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.5f));

    // Draw buttons
    for (const auto& button : buttons) {
        DrawButton(button, *game);
    }

    // Draw score, high score, and time remaining
    DrawTextEx(game->getFont(), TextFormat("Score: %d", score), {10, 10}, 20, 2, WHITE);
    DrawTextEx(game->getFont(), TextFormat("High Score: %d", highScore), {10, 40}, 20, 2, WHITE);
    DrawTextEx(game->getFont(), TextFormat("Time Remaining: %d", timeRemaining), {10, 70}, 20, 2, WHITE);
}

void GameOverState::setScore(int score) {
    this->score = score;
}

void GameOverState::setHighScore(int highScore) {
    this->highScore = highScore;
}

void GameOverState::setTimeRemaining(int timeRemaining) {
    this->timeRemaining = timeRemaining;
}

VictoryState::VictoryState(Game* game) : GameOverState(game) {
    float buttonWidth = 250;
    float buttonHeight = 50;
    float centerX = (game->getScreenWidth() - buttonWidth) / 2;
    buttons.push_back({{centerX, 300, buttonWidth, buttonHeight}, "Main Menu", false});
    buttons.push_back({{centerX, 375, buttonWidth, buttonHeight}, "Next Stage", false});
    buttons.push_back({{centerX, 450, buttonWidth, buttonHeight}, "Play Again", false});
}

void VictoryState::update() {
    // Update button hover states
    for (auto& button : buttons) {
        button.isHovered = CheckCollisionPointRec(GetMousePosition(), button.rect);
    }

    // Handle button clicks
    if (IsButtonClicked(buttons[0])) {
        game->changeState(game->mainMenuState.get());
    }
    if (IsButtonClicked(buttons[1])) {
        // Next stage
    }
    if (IsButtonClicked(buttons[2])) {
        // Retry the game
    }
}

void VictoryState::draw() {
    // Draw the underlying GameplayState
    game->gameplayState->draw();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.5f));

    // Draw buttons
    for (const auto& button : buttons) {
        DrawButton(button, *game);
    }

    // Draw score, high score, and time remaining
    DrawTextEx(game->getFont(), TextFormat("Score: %d", score), {10, 10}, 20, 2, WHITE);
    DrawTextEx(game->getFont(), TextFormat("High Score: %d", highScore), {10, 40}, 20, 2, WHITE);
    DrawTextEx(game->getFont(), TextFormat("Time Remaining: %d", timeRemaining), {10, 70}, 20, 2, WHITE);
}

GameSavingState::GameSavingState(Game* game) : GameState(game) {
    // Initialize buttons
}

void GameSavingState::update() {
    // Update button hover states
}

void GameSavingState::draw() {
    // Draw the underlying GameplayState
}

GameSavingState::~GameSavingState() {
    // Unload textures
}

SelectDifficultyState::SelectDifficultyState(Game* game) : GameState(game) {
    // Initialize buttons
}

void SelectDifficultyState::update() {
    // Update button hover states
}

void SelectDifficultyState::draw() {
    // Draw the underlying MainMenuState
}

SelectDifficultyState::~SelectDifficultyState() {
    // Unload textures
}

AreYouSureState::AreYouSureState(Game* game) : GameState(game) {
    // Initialize buttons
}

void AreYouSureState::update() {
    // Update button hover states
}

void AreYouSureState::draw() {
    // Draw the underlying MainMenuState
}

AreYouSureState::~AreYouSureState() {
    // Unload textures
}

WannaSaveState::WannaSaveState(Game* game) : AreYouSureState(game) {
    // Initialize buttons
}

void WannaSaveState::update() {
    // Update button hover states
}

void WannaSaveState::draw() {
    // Draw the underlying MainMenuState
}

WannaSaveState::~WannaSaveState() {
    // Unload textures
}

QuitState::QuitState(Game *game) : GameState(game) {
    // Initialize buttons
}

void QuitState::update()
{
    // Update button hover states
}

void QuitState::draw()
{
    // Draw the underlying MainMenuState
}

QuitState::~QuitState()
{
    // Unload textures
}

BackToMenuState::BackToMenuState(Game *game) : QuitState(game) {
    // Initialize buttons
}

void BackToMenuState::update()
{
    // Update button hover states
}

void BackToMenuState::draw()
{
    // Draw the underlying MainMenuState
}

BackToMenuState::~BackToMenuState()
{
    // Unload textures
}
