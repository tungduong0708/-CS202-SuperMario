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
    float buttonHeight = 75;
    float column1X = static_cast<float>(Game::getScreenWidth()) / 4 - buttonWidth / 2;
    float column2X = 3 * static_cast<float>(Game::getScreenWidth()) / 4 - buttonWidth / 2;
    float firstButtonWidth = column2X - column1X + buttonWidth;

    buttons.push_back({{column1X, 340, firstButtonWidth, buttonHeight}, "New Game", false});
    buttons.push_back({{column1X, 440, buttonWidth, buttonHeight}, "Continue", false});
    buttons.push_back({{column2X, 440, buttonWidth, buttonHeight}, "Tutorial", false});
    buttons.push_back({{column1X, 540, buttonWidth, buttonHeight}, "Settings", false});
    buttons.push_back({{column2X, 540, buttonWidth, buttonHeight}, "Saved Games", false});
    buttons.push_back({{column1X, 640, buttonWidth, buttonHeight}, "Map Builder", false});
    buttons.push_back({{column2X, 640, buttonWidth, buttonHeight}, "Exit", false});

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
        game->changeState(game->selectDifficultyState.get());
    }
    if (IsButtonClicked(buttons[1])) {
        game->changeState(game->gameplayState.get());
    }
    if (IsButtonClicked(buttons[2])) {
        game->changeState(game->tutorialState.get());
    }
    if (IsButtonClicked(buttons[3])) {
        game->changeState(game->settingsState.get());
    }
    if (IsButtonClicked(buttons[4])) {
        game->changeState(game->savedGameState.get());
    }
    if (IsButtonClicked(buttons[5])) {
        game->changeState(game->mapBuilderState.get());
    }
    if (IsButtonClicked(buttons[6])) {
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
        {game->getScreenWidth() / 2 - logoRectWidth / 2, 100, logoRectWidth, logoRectHeight},
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
    float buttonWidth = 400;
    float buttonHeight = 75;
    float centerX = (game->getScreenWidth() - buttonWidth) / 2;
    buttons.push_back({{centerX, 150, buttonWidth, buttonHeight}, "", false});
    buttons.push_back({{centerX, 250, buttonWidth, buttonHeight}, "", false});
    buttons.push_back({{centerX, 350, buttonWidth, buttonHeight}, "Back to Menu", false});
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

    float centerX = (game->getScreenWidth() - 400) / 2;
    // Update sliders
    DrawMarioSlider({centerX, 500, 400, 50}, game->getSettings().volume, 0, 100, game->getFont(), "Volume");
    DrawMarioSlider({centerX, 600, 400, 50}, game->getSettings().brightness, 0, 100, game->getFont(), "Brightness");
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

    float centerX = (game->getScreenWidth() - 400) / 2;
    // Draw sliders
    DrawMarioSlider({centerX, 500, 400, 50}, game->getSettings().volume, 0, 100, game->getFont(), "Volume");
    DrawMarioSlider({centerX, 600, 400, 50}, game->getSettings().brightness, 0, 100, game->getFont(), "Brightness");
}

TutorialState::TutorialState(Game *game) : GameState(game)
{
    // Tutorial here
}

void TutorialState::update()
{
    // Some update code here
}

void TutorialState::draw()
{
    // Some draw code here
}

TutorialState::~TutorialState()
{
    // Some cleanup code here
    for (const auto& texture : tutorialTextures)
    {
        UnloadTexture(texture);
    }
}

SavedGameState::SavedGameState(Game* game)
    : GameState(game)
{
    // Initialize buttons for saved game slots
    float buttonWidth = 300;
    float buttonHeight = 50;
    float centerX = (game->getScreenWidth() - buttonWidth) / 2;
    float spacing = 25;
    float firstY = (game->getScreenHeight() - 6 * buttonHeight - 5 * spacing) / 2;
    for (int i = 0; i < 5; ++i) {
        buttons.push_back(Button{{centerX, firstY + buttonHeight * i + spacing * i, buttonWidth, buttonHeight}, 
        "Slot " + std::to_string(i + 1), false});
    }
    // Initialize back to menu button
    buttons.push_back({{centerX, firstY + buttonHeight * 5 + spacing * 5, buttonWidth, buttonHeight}, "Back to Menu", false});

    // Load background image
    backgroundTexture = LoadTexture("../resources/images/icon/squareboard.png");
}

void SavedGameState::update() {
    // Update button hover states
    for (auto& button : buttons) {
        button.isHovered = CheckCollisionPointRec(GetMousePosition(), button.rect);
    }

    // Handle button clicks
    for (int i = 0; i < 5; ++i) {
        if (IsButtonClicked(buttons[i])) {
            // Handle saved game slot click 
            std::string saveGamePath = "../resources/savegames/slot" + std::to_string(i + 1) + ".txt";
            std::ifstream file(saveGamePath);

            Tilemap* tilemap = Tilemap::getInstance();
            tilemap->~Tilemap();
            tilemap = Tilemap::getInstance();
            tilemap->LoadSaveGame(saveGamePath);
            game->changeState(game->gameplayState.get());
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

    // Draw background image
    constexpr float backgroundScale = 4.0f;
    float textureWidth = backgroundTexture.width * backgroundScale;
    float textureHeight = backgroundTexture.height * backgroundScale;
    float posX = (game->getScreenWidth() - textureWidth) / 2;
    float posY = (game->getScreenHeight() - textureHeight) / 2;

    DrawTextureEx(backgroundTexture, {posX, posY}, 0.0f, backgroundScale, WHITE);

    // Draw buttons
    for (const auto& button : buttons) {
        DrawButton(button, *game);
    }
}

SavedGameState::~SavedGameState() {
    // Unload background texture
    UnloadTexture(backgroundTexture);
}

MapBuilderState::MapBuilderState(Game* game)
    : GameState(game)
{
    backgroundTexture = LoadTexture("../resources/background/menuBackground.png");
    float buttonWidth = 50;
    float buttonHeight = 50;
    pauseButton = {{game->getScreenWidth() - buttonWidth - 10, 10, buttonWidth, buttonHeight}, "II", false};
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
    CharacterCreator::InitCharacters();
    ObjectCreator::InitObjects();
    TextHelper::loadFont("PressStart2P-Regular", "");
    TilesetHandler::Init();
    
    float buttonWidth = 50;
    float buttonHeight = 50;
    pauseButton = {{game->getScreenWidth() - buttonWidth - 10, 10, buttonWidth, buttonHeight}, "II", false};
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
    float buttonWidth = 400;
    float buttonHeight = 75;
    float centerX = (game->getScreenWidth() - buttonWidth) / 2;
    float spacing = 25;
    float firstY = (game->getScreenHeight() - 6 * buttonHeight - 5 * spacing) / 2;

    buttons.push_back({{centerX, firstY, buttonWidth, buttonHeight}, "", false});
    buttons.push_back({{centerX, firstY + buttonHeight + spacing, buttonWidth, buttonHeight}, "", false});
    buttons.push_back({{centerX, firstY + buttonHeight * 2 + spacing * 2, buttonWidth, buttonHeight}, "Resume", false});
    buttons.push_back({{centerX, firstY + buttonHeight * 3 + spacing * 3, buttonWidth, buttonHeight}, "Back to Menu", false});
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
        game->changeState(game->gameSavingState.get());
    }

    float centerX = (game->getScreenWidth() - 400) / 2;
    // Update sliders
    DrawMarioSlider({centerX, 525, 400, 50}, game->getSettings().volume, 0, 100, game->getFont(), "Volume");
    DrawMarioSlider({centerX, 625, 400, 50}, game->getSettings().brightness, 0, 100, game->getFont(), "Brightness");
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

    float centerX = (game->getScreenWidth() - 400) / 2;
    // Draw sliders
    DrawMarioSlider({centerX, 525, 400, 50}, game->getSettings().volume, 0, 100, game->getFont(), "Volume");
    DrawMarioSlider({centerX, 625, 400, 50}, game->getSettings().brightness, 0, 100, game->getFont(), "Brightness");
}

MapPauseState::MapPauseState(Game *game) : PauseGameState(game)
{   
}

void MapPauseState::update()
{
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
        game->changeState(game->mapBuilderState.get());
    }
    if (IsButtonClicked(buttons[3])) {
        game->changeState(game->mainMenuState.get());
    }

    float centerX = (game->getScreenWidth() - 400) / 2;
    // Update sliders
    DrawMarioSlider({centerX, 525, 400, 50}, game->getSettings().volume, 0, 100, game->getFont(), "Volume");
    DrawMarioSlider({centerX, 625, 400, 50}, game->getSettings().brightness, 0, 100, game->getFont(), "Brightness");
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

    float centerX = (game->getScreenWidth() - 400) / 2;
    // Draw sliders
    DrawMarioSlider({centerX, 525, 400, 50}, game->getSettings().volume, 0, 100, game->getFont(), "Volume");
    DrawMarioSlider({centerX, 625, 400, 50}, game->getSettings().brightness, 0, 100, game->getFont(), "Brightness");
}

SelectPlayerState::SelectPlayerState(Game* game) : GameState(game) {
    // Initialize buttons
    float buttonWidth = 600;
    float buttonHeight = 300;
    float column1X = 50;
    float column2X = static_cast<float>(game->getScreenWidth()) - buttonWidth - column1X;

    player1.name = "Mario";
    player1.texture = LoadTexture("../resources/images/smallmario/idle.png");
    player1.hoverTexture = LoadTexture("../resources/images/smallmario/victory.png");
    player1.button = {{column1X, 50, buttonWidth, buttonHeight}, player1.texture, player1.hoverTexture, false};
    player1.speed = 80;
    player1.jumpForce = 80;
    player1.bulletSpeed = 85;
    player1.bulletFreq = 60;

    player2.name = "Luigi";
    player2.texture = LoadTexture("../resources/images/smallluigi/idle.png");
    player2.hoverTexture = LoadTexture("../resources/images/smallluigi/victory.png");
    player2.button = {{column2X, game->getScreenHeight() - buttonHeight - 50, buttonWidth, buttonHeight}, 
                       player2.texture, player2.hoverTexture, false};
    player2.speed = 75;
    player2.jumpForce = 90;
    player2.bulletSpeed = 80;
    player2.bulletFreq = 74;
}

void SelectPlayerState::update() {
    // Update button hover states
    player1.button.isHovered = CheckCollisionPointRec(GetMousePosition(), player1.button.rect);
    player2.button.isHovered = CheckCollisionPointRec(GetMousePosition(), player2.button.rect);

    // Handle button clicks
    if (IsButtonClicked(player1.button)) {
        // Set player to Mario
        Tilemap* tilemap = Tilemap::getInstance();
        tilemap->setPlayer("mario");

        game->changeState(game->gameplayState.get());
    }
    if (IsButtonClicked(player2.button)) {
        // Set player to Luigi
        Tilemap* tilemap = Tilemap::getInstance();
        tilemap->setPlayer("luigi");
        
        game->changeState(game->gameplayState.get());
    }
}

void SelectPlayerState::draw() {
    // Draw the underlying MainMenuState
    game->mainMenuState->drawBackground();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.5f));
    
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

void ChangeStageState::draw() {
    setStageName("Stage 1-2");

    // Draw the underlying GameplayState
    game->gameplayState->draw();

    // Draw a semi-transparent black overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(BLACK, 0.6f));

    // Update character texture if the player is Luigi
    if (Tilemap::getInstance()->GetPlayer()->getBulletSpeed() == 8.0f) {
        UnloadTexture(characterTexture);
        characterTexture = LoadTexture("../resources/images/smallluigi/idle.png");
    }

    // Draw the central rounded rectangle (panel)
    float rectWidth = 400.0f;
    float rectHeight = 200.0f;
    float rectX = (game->getScreenWidth() - rectWidth) / 2;
    float rectY = (game->getScreenHeight() - rectHeight) / 2;

    // Draw shadow for depth
    DrawRectangleRounded({rectX + 6, rectY + 6, rectWidth, rectHeight}, 0.1f, 10, Fade(BLACK, 0.4f));

    // --- Smooth Gradient ---
    int gradientSteps = 40;  // Increased steps for smoother gradient
    float stepHeight = rectHeight / static_cast<float>(gradientSteps);  // Height of each gradient step
    float overlap = 0.1f;  // Overlap amount to ensure smooth transitions

    // Start and end colors for the gradient
    Color startColor = (Color){255, 223, 0, 200};  // Yellow
    Color endColor = (Color){255, 140, 0, 230};    // Orange-red

    for (int i = 0; i < gradientSteps; i++) {
        float t = static_cast<float>(i) / (gradientSteps - 1);  // Interpolation factor (0 to 1)

        // Interpolating colors
        Color stepColor = {
            (unsigned char)((1 - t) * startColor.r + t * endColor.r),
            (unsigned char)((1 - t) * startColor.g + t * endColor.g),
            (unsigned char)((1 - t) * startColor.b + t * endColor.b),
            (unsigned char)((1 - t) * startColor.a + t * endColor.a)
        };

        // Determine the rounding for the step
        float rounding = 0.2f;  // Default rounding
        if (i < 10) {
            rounding = 1.0f;  // Increased rounding for the top rectangle
        } else if (i > gradientSteps - 10) {
            rounding = 1.0f;  // Increased rounding for the bottom rectangle
        }

        // Draw the gradient rectangle
        DrawRectangleRounded(
            {rectX, rectY + static_cast<float>(i) * stepHeight - overlap, rectWidth, stepHeight + overlap * 2},
            rounding, 10, stepColor);
    }

    // --- Draw Stage Name with Shadow ---
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

    // --- Draw Mario's Image (Scaled 2.5x) ---
    float imageScale = 2.5f;
    float imageX = rectX + 120;
    float imageY = rectY + (rectHeight - characterTexture.height * imageScale) / 2;
    DrawTextureEx(characterTexture, {imageX, imageY}, 0.0f, imageScale, WHITE);

    // --- Life Count with Shadow ---
    constexpr int lifeFontSize = 24;
    Vector2 lifeTextPos = {imageX + 90, imageY + 10};

    // Draw text shadow
    DrawTextEx(game->getFont(), TextFormat("x %d", lifeRemaining),
               {lifeTextPos.x + 2, lifeTextPos.y + 2}, lifeFontSize, 2, Fade(BLACK, 0.6f));
    // Draw main text
    DrawTextEx(game->getFont(), TextFormat("x %d", lifeRemaining),
               lifeTextPos, lifeFontSize, 2, WHITE);

    // Add a decorative border
    DrawRectangleRoundedLines({rectX, rectY, rectWidth, rectHeight}, 0.1f, 10, 5, WHITE);
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
    Player* player = Tilemap::getInstance()->GetPlayer();

    if (elapsedTime < 1.0f && !showDeathImage) {
        // Set the remaining lives correctly
        lifeRemaining = player->getLives() + 1;
    }

    if (elapsedTime > 1.0f && elapsedTime < 2.0f && !showDeathImage) {
        showDeathImage = true;
    }

    if (elapsedTime > 2.0f && showDeathImage) {
        showDeathImage = false;
        // set the remaining lives correctly
        lifeRemaining = player->getLives();
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

    if (Tilemap::getInstance()->GetPlayer()->getBulletSpeed() == 8.0f) {
        UnloadTexture(characterTexture);
        characterTexture = LoadTexture("../resources/images/smallluigi/idle.png");

        UnloadTexture(deathTexture);
        deathTexture = LoadTexture("../resources/images/smallluigi/dead.png");
    }

    // Draw the central rounded rectangle (panel)
    float rectWidth = 400.0f;
    float rectHeight = 200.0f;
    float rectX = (game->getScreenWidth() - rectWidth) / 2;
    float rectY = (game->getScreenHeight() - rectHeight) / 2;

    // Draw shadow for depth
    DrawRectangleRounded({rectX + 6, rectY + 6, rectWidth, rectHeight}, 0.1f, 10, Fade(BLACK, 0.4f));

    // --- Smooth Gradient ---
    int gradientSteps = 40;  // Increased steps for smoother gradient
    float stepHeight = rectHeight / static_cast<float>(gradientSteps);  // Height of each gradient step
    float overlap = 0.1f;  // Overlap amount to ensure smooth transitions

    // Start and end colors for the gradient (RGBA)
    Color startColor = (Color){255, 0, 0, 200};  // Bright red
    Color endColor = (Color){100, 0, 0, 230};    // Dark red (close to crimson)

    for (int i = 0; i < gradientSteps; i++) {
        float t = static_cast<float>(i) / (gradientSteps - 1);  // Interpolation factor (0 to 1)
        
        // Interpolating colors
        Color stepColor = {
            (unsigned char)((1 - t) * startColor.r + t * endColor.r),
            (unsigned char)((1 - t) * startColor.g + t * endColor.g),
            (unsigned char)((1 - t) * startColor.b + t * endColor.b),
            (unsigned char)((1 - t) * startColor.a + t * endColor.a)
        };

        // Determine the rounding for the step
        float rounding = 0.2f;  // Default rounding
        if (i < 10) {
            rounding = 1.0f;  // Increased rounding for the top rectangle
        } else if (i > gradientSteps - 10) {
            rounding = 1.0f;  // Increased rounding for the bottom rectangle
        }

        // Draw the gradient rectangle
        DrawRectangleRounded(
            {rectX, rectY + static_cast<float>(i) * stepHeight - overlap, rectWidth, stepHeight + overlap * 2},
            rounding, 10, stepColor);
    }

    // --- Draw "You Died" Text with Shadow ---
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

    // --- Mario's Image (Scaled 2x) ---
    float imageScale = 2.5f;
    float imageX = rectX + 120;
    float imageY = rectY + (rectHeight - characterTexture.height * imageScale) / 2;
    DrawTextureEx(characterTexture, {imageX, imageY}, 0.0f, imageScale, WHITE);

    // --- Life Count with Shadow ---
    constexpr int lifeFontSize = 24;
    Vector2 lifeTextPos = {imageX + 90, imageY + 10};

    // Draw text shadow
    DrawTextEx(game->getFont(), TextFormat("x %d", lifeRemaining),
               {lifeTextPos.x + 2, lifeTextPos.y + 2}, lifeFontSize, 2, Fade(BLACK, 0.6f));
    // Draw main text
    DrawTextEx(game->getFont(), TextFormat("x %d", lifeRemaining),
               lifeTextPos, lifeFontSize, 2, WHITE);

    // --- Death Image (Optional) ---
    if (showDeathImage) {
        float deathImageScale = 1.5f;
        float deathImageX = rectX + 100;
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
    DrawRectangleRoundedLines({rectX, rectY, rectWidth, rectHeight}, 0.1f, 10, 5, WHITE);
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
        game->changeState(game->selectDifficultyState.get());
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
    float buttonWidth = 300;
    float buttonHeight = 50;
    float centerX = (game->getScreenWidth() - buttonWidth) / 2;
    float spacing = 25;
    float firstY = (game->getScreenHeight() - 5 * buttonHeight - 4 * spacing) / 2;
    for (int i = 0; i < 5; ++i) {
        buttons.push_back(Button{{centerX, firstY + buttonHeight * i + spacing * i, buttonWidth, buttonHeight}, 
        "Slot " + std::to_string(i + 1), false});
    }

    // Initialize background texture
    backgroundTexture = LoadTexture("../resources/images/icon/squareboard.png");
}

void GameSavingState::update() {
    // Update button hover states
    for (auto& button : buttons) {
        button.isHovered = CheckCollisionPointRec(GetMousePosition(), button.rect);
    }

    // Handle button clicks
    bool isClicked = false;
    for (int i = 0; i < 5; ++i) {
        if (IsButtonClicked(buttons[i])) {
            // Save the game to the slot
            std::string saveGamePath = "../resources/savegames/slot" + std::to_string(i + 1) + ".txt";
            Tilemap* tilemap = Tilemap::getInstance();
            tilemap->SaveGame(saveGamePath);
            // Change state to MainMenuState
            isClicked = true;
            break;
        }
    }
    if (isClicked) {
        game->changeState(game->mainMenuState.get());
    }
}

void GameSavingState::draw() {
    // Draw the underlying GameplayState
    game->gameplayState->draw();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.5f));

    // Draw background texture
    constexpr float backgroundScale = 3.5f;
    float textureWidth = backgroundTexture.width * backgroundScale;
    float textureHeight = backgroundTexture.height * backgroundScale;
    float posX = (game->getScreenWidth() - textureWidth) / 2;
    float posY = (game->getScreenHeight() - textureHeight) / 2;

    DrawTextureEx(backgroundTexture, {posX, posY}, 0.0f, backgroundScale, WHITE);

    // Draw buttons
    for (const auto& button : buttons) {
        DrawButton(button, *game);
    }
}

GameSavingState::~GameSavingState() {
    // Unload textures
    UnloadTexture(backgroundTexture);
}

SelectDifficultyState::SelectDifficultyState(Game* game) : GameState(game)
{
    // Initialize buttons
    float buttonWidth = 650;
    float buttonHeight = 150;
    float centerX = (game->getScreenWidth() - buttonWidth) / 2;

    difficultyTextures.push_back(LoadTexture("../resources/images/goomba/walk.png"));
    difficultyTextures.push_back(LoadTexture("../resources/images/goomba/dead.png"));
    difficultyTextures.push_back(LoadTexture("../resources/images/koopa/idle.png"));
    difficultyTextures.push_back(LoadTexture("../resources/images/koopa/fly1.png"));
    difficultyTextures.push_back(LoadTexture("../resources/images/boss/walk2.png"));
    difficultyTextures.push_back(LoadTexture("../resources/images/boss/blow3.png"));

    buttons.push_back({{centerX, 100, buttonWidth, buttonHeight}, difficultyTextures[0],
        difficultyTextures[1], false});
    buttons.push_back({{centerX, 300, buttonWidth, buttonHeight}, difficultyTextures[2],
        difficultyTextures[3], false});
    buttons.push_back({{centerX, 500, buttonWidth, buttonHeight}, difficultyTextures[4],
        difficultyTextures[5], false});
}

void SelectDifficultyState::update() {
    // Update button hover states
    for (auto& button : buttons) {
        button.isHovered = CheckCollisionPointRec(GetMousePosition(), button.rect);
    }

    for (int i = 0; i < 3; ++i)
    {
        if (IsButtonClicked(buttons[i])) {
            Tilemap* tilemap = Tilemap::getInstance();
            tilemap->~Tilemap();
            tilemap = Tilemap::getInstance();
            tilemap->LoadMapFromJson("map-1-1.json", i + 1);
            game->changeState(game->selectPlayerState.get());
        }
    }
}

void SelectDifficultyState::draw() {
    // Draw the underlying SelectDifficultState
    game->mainMenuState->drawBackground();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.6f));

    // Draw buttons
    DrawDifficultyButton(buttons[0], "Easy");
    DrawDifficultyButton(buttons[1], "Medium");
    DrawDifficultyButton(buttons[2], "Hard");
}

SelectDifficultyState::~SelectDifficultyState() {
    // Unload textures
    for (const auto& texture : difficultyTextures) {
        UnloadTexture(texture);
    }
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
