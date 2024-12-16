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
    TextHelper::loadFont("Mario256", "");
    TextHelper::loadTexture("coin", "smallmario");

    TilesetHandler::Init();
    Tilemap* tilemap = Tilemap::getInstance();
    tilemap->LoadMapFromJson("map-1-1.json");
    
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
        game->changeState(game->deathState.get());
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
    float buttonWidth = 250;
    float buttonHeight = 400;
    float column1X = static_cast<float>(Game::getScreenWidth()) / 4 - buttonWidth / 2;
    float column2X = 3 * static_cast<float>(Game::getScreenWidth()) / 4 - buttonWidth / 2;

    Texture2D player1Texture = LoadTexture("../resources/images/smallmario/idle.png");
    Texture2D player1HoverTexture = LoadTexture("../resources/images/smallmario/victory.png");
    player1Button = {{column1X, 100, buttonWidth, buttonHeight}, player1Texture, player1HoverTexture, "Mario", false};

    Texture2D player2Texture = LoadTexture("../resources/images/smallluigi/idle.png");
    Texture2D player2HoverTexture = LoadTexture("../resources/images/smallluigi/victory.png");
    player2Button = {{column2X, 100, buttonWidth, buttonHeight}, player2Texture, player2HoverTexture, "Luigi", false};
}

void SelectPlayerState::update() {
    // Update button hover states
    player1Button.isHovered = CheckCollisionPointRec(GetMousePosition(), player1Button.rect);
    player2Button.isHovered = CheckCollisionPointRec(GetMousePosition(), player2Button.rect);

    // Handle button clicks
    if (IsButtonClicked(player1Button)) {
        // Set player to Mario

        game->changeState(game->gameplayState.get());
    }
    if (IsButtonClicked(player2Button)) {
        // Set player to Luigi
        
        game->changeState(game->gameplayState.get());
    }
}

void SelectPlayerState::draw() {
    // Draw the underlying MainMenuState
    game->mainMenuState->drawBackground();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.5f));
    
    // Draw buttons
    DrawImageButton(player1Button, *game);
    DrawImageButton(player2Button, *game);
}

SelectPlayerState::~SelectPlayerState()
{
    UnloadTexture(player1Button.texture);
    UnloadTexture(player1Button.hoverTexture);
    UnloadTexture(player2Button.texture);
    UnloadTexture(player2Button.hoverTexture);
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
    }
}

void ChangeStageState::draw()
{
    // Draw the underlying GameplayState
    game->gameplayState->draw();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.5f));

    // Draw the small rectangle with Mario's image and life count
    float rectWidth = 200.0f;
    float rectHeight = 100.0f;
    float rectX = (game->getScreenWidth() - rectWidth) / 2;
    float rectY = (game->getScreenHeight() - rectHeight) / 2;
    DrawRectangle(rectX, rectY, rectWidth, rectHeight, Fade(BLACK, 0.7f));

    // Draw Mario's image
    DrawTexture(characterTexture, rectX + 10, rectY + 10, WHITE);

    // Draw life remaining text
    DrawTextEx(game->getFont(), TextFormat("x %d", lifeRemaining), {rectX + 60, rectY + 20}, 20, 2, WHITE);

    // Draw stage name
    DrawTextEx(game->getFont(), stageName.c_str(), {rectX + 60, rectY + 60}, 20, 2, WHITE);
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

void DeathState::draw()
{
    // Draw the underlying GameplayState
    game->gameplayState->draw();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.5f));

    // Draw the small rectangle with Mario's image and life count
    float rectWidth = 200.0f;
    float rectHeight = 100.0f;
    float rectX = (game->getScreenWidth() - rectWidth) / 2;
    float rectY = (game->getScreenHeight() - rectHeight) / 2;
    DrawRectangle(rectX, rectY, rectWidth, rectHeight, Fade(BLACK, 0.7f));

    // Draw Mario's image
    DrawTexture(characterTexture, rectX + 10, rectY + 10, WHITE);

    // Draw life remaining text
    DrawTextEx(game->getFont(), TextFormat("x %d", lifeRemaining), {rectX + 60, rectY + 20}, 20, 2, WHITE);

    if (showDeathImage) {
        // Draw death image and "life - 1" text
        DrawTexture(deathTexture, rectX + 10, rectY + 50, WHITE);
        DrawTextEx(game->getFont(), "life - 1", {rectX + 60, rectY + 60}, 10, 2, WHITE);
    }
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

GameOverState::GameOverState(Game* game) : GameState(game), buttons(), score(0), highScore(0), timeRemaining(0) {}

void GameOverState::update() {}

void GameOverState::draw() {}

VictoryState::VictoryState(Game* game) : GameOverState(game) {}

void VictoryState::update() {}

void VictoryState::draw() {}