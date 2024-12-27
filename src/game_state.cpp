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

    buttons.push_back({{column1X, 340, buttonWidth, buttonHeight}, "1 Player", false});
    buttons.push_back({{column2X, 340, buttonWidth, buttonHeight}, "2 Players", false});
    buttons.push_back({{column1X, 440, buttonWidth, buttonHeight}, "Load Games", false});
    buttons.push_back({{column2X, 440, buttonWidth, buttonHeight}, "Tutorial", false});
    buttons.push_back({{column1X, 540, buttonWidth, buttonHeight}, "Settings", false});
    buttons.push_back({{column2X, 540, buttonWidth, buttonHeight}, "Exit", false});

    // Load background image
    backgroundTexture = LoadTexture("../resources/background/menuBackground2.png");

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
        Tilemap::getInstance()->~Tilemap();
        Tilemap::SetMapType(TILEMAP_2P);
        Tilemap::getInstance()->LoadMapFromJson("map-1-1.json", 1);
        Tilemap::getInstance()->setPlayer("mario");
        Tilemap::getInstance()->setPlayer2("luigi");
        Tilemap::getInstance()->GetPlayer()->setLives(5);
        Tilemap::getInstance()->GetPlayer2()->setLives(5);

        Tilemap::getInstance()->SetSaveSlotLoadedFrom(SaveSlot::NOT_LOADED);

        game->changeState(game->gameplay2PState.get());
    }
    if (IsButtonClicked(buttons[2])) {
        game->changeState(game->savedGameState.get());
    }
    if (IsButtonClicked(buttons[3])) {
        Tilemap* tilemap = Tilemap::getInstance();
        tilemap->~Tilemap();
        tilemap = Tilemap::getInstance();
        Tilemap::SetMapType(TILEMAP_1P);
        Tilemap::getInstance()->LoadMapFromJson("map-tutorial.json", 1);
        Tilemap::getInstance()->setPlayer("mario");
        game->changeState(game->tutorialState.get());
    }
    if (IsButtonClicked(buttons[4])) {
        game->changeState(game->settingsState.get());
    }
    if (IsButtonClicked(buttons[5])) {
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
    float logoRectWidth = 500.0f;
    float logoRectHeight = logoRectWidth * logoHeight / logoWidth;
    DrawTexturePro(
        logoTexture,
        {0, 0, static_cast<float>(logoTexture.width), static_cast<float>(logoTexture.height)},
        {game->getScreenWidth() / 2 - logoRectWidth / 2, 50, logoRectWidth, logoRectHeight},
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

    float centerX = (game->getScreenWidth() - 400) / 2;
    // Update sliders
    DrawMarioSlider({centerX, 500, 400, 50}, game->getSettings().volume, 0, 100, game->getFont(), "Volume");
    DrawMarioSlider({centerX, 600, 400, 50}, game->getSettings().brightness, 0, 100, game->getFont(), "Brightness");

    // Handle button clicks
    if (IsButtonClicked(buttons[0])) {
        game->getSettings().music = !game->getSettings().music;
    }
    else if (IsButtonClicked(buttons[1])) {
        game->getSettings().soundEffects = !game->getSettings().soundEffects;
        SoundEffectHandler::SwitchEnableSoundEffects();  
    }
    else if (IsButtonClicked(buttons[2])) {
        game->changeState(game->mainMenuState.get());
    }
    // Handle get back when clicking empty space
    else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), {centerX, 500, 400, 50}) || CheckCollisionPointRec(GetMousePosition(), {centerX, 600, 400, 50}))
            MouseClickToEmptySpaceHandler::SetIsTrue(false);
        else if (CheckCollisionPointRec(GetMousePosition(), buttons[0].rect) || CheckCollisionPointRec(GetMousePosition(), buttons[1].rect))
            MouseClickToEmptySpaceHandler::SetIsTrue(false);
        else {
            MouseClickToEmptySpaceHandler::SetIsTrue(true);
        }
    }
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && MouseClickToEmptySpaceHandler::GetIsTrue()) {
        game->changeState(game->mainMenuState.get());
    }
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
            ImportFileVisitor* visitor = ImportFileVisitor::getInstance();
            visitor->setFilePath(saveGamePath);
            visitor->openFile();
            MapType mapType = visitor->importMapType();
            cout << "Map type: " << mapType << endl;
            visitor->closeFile();
            if (mapType == MapType::ONE_PLAYER) {
                Tilemap::SetMapType(TILEMAP_1P);
            }
            else if (mapType == MapType::TWO_PLAYER) {
                Tilemap::SetMapType(TILEMAP_2P);
            }
            tilemap = Tilemap::getInstance();
            tilemap->LoadSaveGame(saveGamePath);

            tilemap->SetSaveSlotLoadedFrom(static_cast<SaveSlot>(i + 1));

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

    // Draw buttons
    for (const auto& button : buttons) {
        DrawButton(button, *game);
    }
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

    if (StageStateHandler::GetInstance().GetState() == StageState::PLAYER_DEAD){
        playSoundEffect(SoundEffect::PLAYER_DIE);
        game->changeState(game->deathState.get());
    }
    else if (StageStateHandler::GetInstance().GetState() == StageState::GAME_OVER){
        playSoundEffect(SoundEffect::GAME_OVER);
        StageStateHandler::GetInstance().SetState(StageState::NORMAL_STATE);
        game->changeState(game->gameOverState.get());
    }
    else if (StageStateHandler::GetInstance().GetState() == StageState::STAGE_CLEAR){
        StageStateHandler::GetInstance().SetState(StageState::NORMAL_STATE);
    }
    else if (StageStateHandler::GetInstance().GetState() == StageState::WORLD_CLEAR){
        StageStateHandler::GetInstance().SetState(StageState::NORMAL_STATE);
        game->changeState(game->changeStageState.get());
    }
}

void GameplayState::draw() {
    Tilemap* tilemap = Tilemap::getInstance();
    tilemap->Draw();

    DrawButton(pauseButton, *game);
}

GameplayState::~GameplayState() {
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
        SoundEffectHandler::SwitchEnableSoundEffects();
    }
    if (IsButtonClicked(buttons[2])) {
        game->changeState(game->gameplayState.get());
    }
    if (IsButtonClicked(buttons[3])) {
        game->setNextState(game->backToMenuState.get());
        game->changeState(game->wannaSaveState.get());
    }

    float centerX = (game->getScreenWidth() - 400) / 2;
    // Update sliders
    DrawMarioSlider({centerX, 525, 400, 50}, game->getSettings().volume, 0, 100, game->getFont(), "Volume");
    DrawMarioSlider({centerX, 625, 400, 50}, game->getSettings().brightness, 0, 100, game->getFont(), "Brightness");

    // Handle get back when clicking empty space
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), {centerX, 525, 400, 50}) || CheckCollisionPointRec(GetMousePosition(), {centerX, 625, 400, 50}))
            MouseClickToEmptySpaceHandler::SetIsTrue(false);
        else if (CheckCollisionPointRec(GetMousePosition(), buttons[0].rect) || CheckCollisionPointRec(GetMousePosition(), buttons[1].rect)
            || CheckCollisionPointRec(GetMousePosition(), buttons[2].rect) || CheckCollisionPointRec(GetMousePosition(), buttons[3].rect))
            MouseClickToEmptySpaceHandler::SetIsTrue(false);
        else {
            MouseClickToEmptySpaceHandler::SetIsTrue(true);
        }
    }
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && MouseClickToEmptySpaceHandler::GetIsTrue()) {
        game->changeState(game->gameplayState.get());
    }
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
        SoundEffectHandler::SwitchEnableSoundEffects();
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

    // Handle get back when clicking empty space
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), {centerX, 525, 400, 50}) || CheckCollisionPointRec(GetMousePosition(), {centerX, 625, 400, 50}))
            MouseClickToEmptySpaceHandler::SetIsTrue(false);
        else if (CheckCollisionPointRec(GetMousePosition(), buttons[0].rect) || CheckCollisionPointRec(GetMousePosition(), buttons[1].rect)
            || CheckCollisionPointRec(GetMousePosition(), buttons[2].rect) || CheckCollisionPointRec(GetMousePosition(), buttons[3].rect))
            MouseClickToEmptySpaceHandler::SetIsTrue(false);
        else {
            MouseClickToEmptySpaceHandler::SetIsTrue(true);
        }
    }
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && MouseClickToEmptySpaceHandler::GetIsTrue()) {
        game->changeState(game->mapBuilderState.get());
    }
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

    // Handle get back when clicking empty space
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), player1.button.rect) || CheckCollisionPointRec(GetMousePosition(), player2.button.rect))
            MouseClickToEmptySpaceHandler::SetIsTrue(false);
        else {
            MouseClickToEmptySpaceHandler::SetIsTrue(true);
        }
    }
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && MouseClickToEmptySpaceHandler::GetIsTrue()) {
        game->changeState(game->selectDifficultyState.get());
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
    Player* player = Tilemap::getInstance()->GetPlayer();
    lifeRemaining = player->getLives();

    if (elapsedTime > 3.0f) {
        game->changeState(game->gameplayState.get());
        reset();
    }
}

void ChangeStageState::draw() {
    std::string mapPath = Tilemap::getInstance()->GetNewMapPath();
    std::string mapName = "World " + mapPath.substr(4, 3); 
    setStageName(mapName);
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
    Color startColor = (Color){92, 179, 56, 255};  // Light-green
    Color endColor = (Color){54, 126, 24, 255};    // Dark-green

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

    // Draw text border
    DrawTextEx(game->getFont(), stageName.c_str(),
               {stageTextPos.x + 2, stageTextPos.y}, fontSize, 2, WHITE);
    DrawTextEx(game->getFont(), stageName.c_str(),
                {stageTextPos.x - 2, stageTextPos.y}, fontSize, 2, WHITE);
    DrawTextEx(game->getFont(), stageName.c_str(),
                {stageTextPos.x, stageTextPos.y + 2}, fontSize, 2, WHITE);
    DrawTextEx(game->getFont(), stageName.c_str(),
                {stageTextPos.x, stageTextPos.y - 2}, fontSize, 2, WHITE);
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

    //Handle get back when clicking outside noti rectangle
    float rectWidth = 400.0f;
    float rectHeight = 200.0f;
    float rectX = (game->getScreenWidth() - rectWidth) / 2;
    float rectY = (game->getScreenHeight() - rectHeight) / 2;

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && !CheckCollisionPointRec(GetMousePosition(), {rectX, rectY, rectWidth, rectHeight})){
        lifeRemaining = player->getLives();
        game->changeState(game->gameplayState.get());
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

    // Add a decorative border
    DrawRectangleRoundedLines({rectX, rectY, rectWidth, rectHeight}, 0.1f, 10, 5, WHITE);

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

GameOverState::GameOverState(Game* game) : GameState(game), score(0){
    // Initialize buttons
    float buttonWidth = 300;
    float buttonHeight = 75;
    float column1X = game->getScreenWidth() / 2 - buttonWidth - 20;
    float column2X = game->getScreenWidth() / 2 + 20;
    float buttonY = game->getScreenHeight() - buttonHeight - 100;

    buttons.push_back({{column1X, buttonY, buttonWidth, buttonHeight}, "Main Menu", false});
    buttons.push_back({{column2X, buttonY, buttonWidth, buttonHeight}, "Retry", false});
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
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.7f));

    // Draw very big "Game Over" text
    constexpr int fontSize = 96;
    constexpr int spacing = 2;
    string gameOverText = "Game Over";
    Vector2 gameOverTextSize = MeasureTextEx(game->getFont(), gameOverText.c_str(), fontSize, spacing);
    Vector2 gameOverTextPos = {
        (game->getScreenWidth() - gameOverTextSize.x) / 2, // Center horizontally
        (game->getScreenHeight() - gameOverTextSize.y) / 2 - 100 // Center vertically
    };

    Texture2D gameOverTexture = LoadTexture("../resources/images/logo/gameover.png");
    // Draw texture
    float TextureX = (game->getScreenWidth() - gameOverTexture.width) / 2;
    float TextureY = 125;

    DrawTexture(gameOverTexture, TextureX, TextureY, WHITE);

    score = Tilemap::getInstance()->GetPlayer()->getScore();

    // Draw score in the middle of the screen below "Game Over"
    constexpr int scoreFontSize = 32;
    string scoreText = "Score: " + std::to_string(score);
    Vector2 scoreTextSize = MeasureTextEx(game->getFont(), scoreText.c_str(), scoreFontSize, spacing);
    Vector2 scoreTextPos = {
        (game->getScreenWidth() - scoreTextSize.x) / 2, // Center horizontally
        TextureY + gameOverTexture.height + 20 // Below "Game Over" text
    };

    // Draw text shadow
    DrawTextEx(game->getFont(), scoreText.c_str(),
               {scoreTextPos.x + 2, scoreTextPos.y + 2}, scoreFontSize, 2, Fade(BLACK, 0.6f));
    // Draw main text
    DrawTextEx(game->getFont(), scoreText.c_str(),
               scoreTextPos, scoreFontSize, 2, GOLD);

    // Draw some funny message below the score
    constexpr int messageFontSize = 32;
    string messageText = "Better luck next time!";
    Vector2 messageTextSize = MeasureTextEx(game->getFont(), messageText.c_str(), messageFontSize, spacing);
    Vector2 messageTextPos = {
        (game->getScreenWidth() - messageTextSize.x) / 2, // Center horizontally
        scoreTextPos.y + scoreTextSize.y + 20 // Below the score
    };

    // Draw text shadow
    DrawTextEx(game->getFont(), messageText.c_str(),
               {messageTextPos.x + 2, messageTextPos.y + 2}, messageFontSize, 2, Fade(BLACK, 0.6f));
    // Draw main text
    DrawTextEx(game->getFont(), messageText.c_str(),
               messageTextPos, messageFontSize, 2, WHITE);

    // Draw buttons
    for (const auto& button : buttons) {
        DrawButton(button, *game);
    }
}

void GameOverState::setScore(int score) {
    this->score = score;
}

VictoryState::VictoryState(Game* game) : GameOverState(game) {
    float buttonWidth = 300;
    float buttonHeight = 75;
    float column1X = game->getScreenWidth() / 2 - buttonWidth - 20;
    float column2X = game->getScreenWidth() / 2 + 20;
    float buttonY = game->getScreenHeight() - buttonHeight - 150;

    buttons.clear();
    buttons.push_back({{column1X, buttonY, buttonWidth, buttonHeight}, "Main Menu", false});
    buttons.push_back({{column2X, buttonY, buttonWidth, buttonHeight}, "Play Again", false});
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
        // Retry the game
        game->changeState(game->selectDifficultyState.get());
    }
}

void VictoryState::draw() {
    // Draw the underlying GameplayState
    game->gameplayState->draw();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.7f));

    // Draw buttons
    for (const auto& button : buttons) {
        DrawButton(button, *game);
    }

    // Draw very big "Victory" text
    constexpr int fontSize = 96;
    constexpr int spacing = 2;
    string gameOverText = "Victory";
    Vector2 gameOverTextSize = MeasureTextEx(game->getFont(), gameOverText.c_str(), fontSize, spacing);
    Vector2 gameOverTextPos = {
        (game->getScreenWidth() - gameOverTextSize.x) / 2, // Center horizontally
        (game->getScreenHeight() - gameOverTextSize.y) / 2 - 100 // Center vertically
    };

    Texture2D victoryTexture = LoadTexture("../resources/images/logo/victory.png");
    // Draw texture
    float TextureX = (game->getScreenWidth() - victoryTexture.width) / 2;
    float TextureY = (game->getScreenHeight() - victoryTexture.height) / 2 - 150;
    DrawTexture(victoryTexture, TextureX, TextureY, WHITE);
    

    score = Tilemap::getInstance()->GetPlayer()->getScore();

    // Draw score in the middle of the screen below "Game Over"
    constexpr int scoreFontSize = 40;
    string scoreText = "Score: " + std::to_string(score);
    Vector2 scoreTextSize = MeasureTextEx(game->getFont(), scoreText.c_str(), scoreFontSize, spacing);
    Vector2 scoreTextPos = {
        (game->getScreenWidth() - scoreTextSize.x) / 2, // Center horizontally
        gameOverTextPos.y + gameOverTextSize.y + 30 // Below "Game Over" text
    };

    // Draw text shadow
    DrawTextEx(game->getFont(), scoreText.c_str(),
               {scoreTextPos.x + 2, scoreTextPos.y + 2}, scoreFontSize, 2, Fade(BLACK, 0.6f));
    // Draw main text
    DrawTextEx(game->getFont(), scoreText.c_str(),
               scoreTextPos, scoreFontSize, 2, GOLD);

    // Draw some funny message below the score
    constexpr int messageFontSize = 32;
    string messageText1 = "Congratulations!";
    Vector2 messageText1Size = MeasureTextEx(game->getFont(), messageText1.c_str(), messageFontSize, spacing);
    Vector2 messageText1Pos = {
        (game->getScreenWidth() - messageText1Size.x) / 2, // Center horizontally
        scoreTextPos.y + scoreTextSize.y + 20 // Below the score
    };

    DrawTextEx(game->getFont(), messageText1.c_str(),
               {messageText1Pos.x + 2, messageText1Pos.y + 2}, messageFontSize, 2, Fade(BLACK, 0.6f));
    // Draw main text
    DrawTextEx(game->getFont(), messageText1.c_str(),
               messageText1Pos, messageFontSize, 2, WHITE);
    
    string messageText2 = "You saved the princess!";
    Vector2 messageText2Size = MeasureTextEx(game->getFont(), messageText2.c_str(), messageFontSize, spacing);
    Vector2 messageText2Pos = {
        (game->getScreenWidth() - messageText2Size.x) / 2, // Center horizontally
        scoreTextPos.y + scoreTextSize.y + 60 // Below the score
    };

    // Draw text shadow
    DrawTextEx(game->getFont(), messageText2.c_str(),
               {messageText2Pos.x + 2, messageText2Pos.y + 2}, messageFontSize, 2, Fade(BLACK, 0.6f));
    // Draw main text
    DrawTextEx(game->getFont(), messageText2.c_str(),
               messageText2Pos, messageFontSize, 2, WHITE);

    // Draw buttons
    for (const auto& button : buttons) {
        DrawButton(button, *game);
    }
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
        if (game->getNextState() == game->backToMenuState.get()) {
            game->changeState(game->backToMenuState.get());
        }
        else {
            game->changeState(game->quitState.get());
        }
    }
}

void GameSavingState::draw() {
    // Draw the underlying GameplayState
    game->gameplayState->draw();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.5f));

    // Draw buttons
    for (const auto& button : buttons) {
        DrawButton(button, *game);
    }
}

SelectDifficultyState::SelectDifficultyState(Game* game) : GameState(game)
{
    // Initialize buttons
    float buttonWidth = 650;
    float buttonHeight = 150;
    float centerX = (game->getScreenWidth() - buttonWidth) / 2;

    difficultyTextures.push_back(LoadTexture("../resources/images/beetle/walk.png"));
    difficultyTextures.push_back(LoadTexture("../resources/images/beetle/idle.png"));

    difficultyTextures.push_back(LoadTexture("../resources/images/goomba/walk.png"));
    difficultyTextures.push_back(LoadTexture("../resources/images/goomba/dead.png"));
    
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
            Tilemap::SetMapType(TilemapType::TILEMAP_1P);
            tilemap = Tilemap::getInstance();
            tilemap->LoadMapFromJson("map-1-1.json", i + 1);

            tilemap->SetSaveSlotLoadedFrom(SaveSlot::NOT_LOADED);

            game->changeState(game->selectPlayerState.get());
        }
    }

    // Handle get back when clicking empty space
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), buttons[0].rect) || CheckCollisionPointRec(GetMousePosition(), buttons[1].rect) ||  CheckCollisionPointRec(GetMousePosition(), buttons[2].rect))
            MouseClickToEmptySpaceHandler::SetIsTrue(false);
        else {
            MouseClickToEmptySpaceHandler::SetIsTrue(true);
        }
    }
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && MouseClickToEmptySpaceHandler::GetIsTrue()) {
        game->changeState(game->mainMenuState.get());
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

WannaSaveState::WannaSaveState(Game* game) : GameState(game) {
    // Initialize buttons
    float buttonWidth = 150;
    float buttonHeight = 75;
    float buttonY = 400;
    float column1X = game->getScreenWidth() / 2 - buttonWidth - 50;
    float column2X = game->getScreenWidth() / 2 + 50;

    buttons.push_back({{column1X, buttonY, buttonWidth, buttonHeight}, "Yes", false});
    buttons.push_back({{column2X, buttonY, buttonWidth, buttonHeight}, "No", false});
}

void WannaSaveState::update() {
    // Update button hover states
    for (auto& button : buttons) {
        button.isHovered = CheckCollisionPointRec(GetMousePosition(), button.rect);
    }

    // Handle button clicks
    if (IsButtonClicked(buttons[0])) {
        SaveSlot saveSlot = Tilemap::getInstance()->GetSaveSlotLoadedFrom();
        if (saveSlot == SaveSlot::NOT_LOADED) {
            game->changeState(game->gameSavingState.get());
        }
        else {
            std::string saveGamePath = "../resources/savegames/slot" + std::to_string(static_cast<int>(saveSlot) + 1) + ".txt";
            Tilemap* tilemap = Tilemap::getInstance();
            tilemap->SaveGame(saveGamePath);
            if (game->getNextState() == game->backToMenuState.get()) {
            game->changeState(game->backToMenuState.get());
            }
            else {
                game->changeState(game->quitState.get());
            }
        }
    }
    if (IsButtonClicked(buttons[1])) {
        if (game->getNextState() == game->backToMenuState.get()) {
            game->changeState(game->mainMenuState.get());
        }
        else if (game->getNextState() == game->quitState.get()) {
            game->exitGame();
        }
    }
}

void WannaSaveState::draw() {
    // Draw the underlying GamePlayState
    game->gameplayState->draw();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.5f));

    // Draw the central rounded rectangle (panel)
    float rectWidth = 450.0f;
    float rectHeight = 250.0f;
    float rectX = (game->getScreenWidth() - rectWidth) / 2;
    float rectY = (game->getScreenHeight() - rectHeight) / 2;

    // Draw shadow for depth
    DrawRectangleRounded({rectX + 6, rectY + 6, rectWidth, rectHeight}, 0.1f, 10, Fade(BLACK, 0.4f));

    // --- Smooth Gradient ---
    int gradientSteps = 40;  // Increased steps for smoother gradient
    float stepHeight = rectHeight / static_cast<float>(gradientSteps);  // Height of each gradient step
    float overlap = 0.1f;  // Overlap amount to ensure smooth transitions

    // Start and end colors for the gradient (RGBA)
    Color startColor = (Color){255, 245, 116, 255};  
    Color endColor = (Color){252, 199, 55, 255};

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

    // Add a decorative border
    DrawRectangleRoundedLines({rectX, rectY, rectWidth, rectHeight}, 0.1f, 10, 5, WHITE);

    // Draw text
    string message = "";
    if (game->getNextState() == game->backToMenuState.get()) {
        message = "Do you want to save\n\nthe game before going back\n\nto the main menu?";
    }
    else if (game->getNextState() == game->quitState.get()) {
        message = "Do you want to save\n\nthe game before quitting?";
    }

    constexpr int fontSize = 20;
    constexpr int spacing = 1;
    // Draw text at the center of the rectangle's width and on top of the rectangle
    Vector2 textSize = MeasureTextEx(game->getFont(), message.c_str(), fontSize, spacing);
    Vector2 textPos = {
        rectX + (rectWidth - textSize.x) / 2, // Center horizontally
        rectY + 40                             // Margin from the top
    };

    // Draw border for text
    DrawTextEx(game->getFont(), 
        message.c_str(), 
        {textPos.x + 2, textPos.y}, 
        fontSize, 2, BLACK);
    DrawTextEx(game->getFont(), 
        message.c_str(), 
        {textPos.x - 2, textPos.y }, 
        fontSize, 2, BLACK);
    DrawTextEx(game->getFont(),
        message.c_str(),
        {textPos.x, textPos.y + 2},
        fontSize, 2, BLACK);
    DrawTextEx(game->getFont(),
        message.c_str(),
        {textPos.x, textPos.y - 2},
        fontSize, 2, BLACK);


    DrawTextEx(game->getFont(), message.c_str(), textPos, fontSize, 2, WHITE);

    // Draw buttons
    for (const auto& button : buttons) {
        DrawYesNoButton(button);
    }
}

QuitState::QuitState(Game *game) : GameState(game), elapsedTime(0.0f) {
}

void QuitState::update()
{
    elapsedTime += GetFrameTime();

    if (elapsedTime > 2.0f) {
        game->exitGame();
    }
}

void QuitState::draw()
{
    // Draw the underlying GamePlayState
    game->gameplayState->draw();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.5f));

    // Draw a very big text in the middle of the screen: "Saving and Quitting..."
    string message = "Saving and Quitting...";
    constexpr int fontSize = 50;
    constexpr int spacing = 1;

    Vector2 textSize = MeasureTextEx(game->getFont(), message.c_str(), fontSize, spacing);
    Vector2 textPos = {
        (game->getScreenWidth() - textSize.x) / 2, // Center horizontally
        static_cast<float>(game->getScreenHeight()) / 2 // Center vertically
    };

    // Draw border for text
    DrawTextEx(game->getFont(), 
        message.c_str(), 
        {textPos.x + 2, textPos.y}, 
        fontSize, 2, BLACK);
    DrawTextEx(game->getFont(),
        message.c_str(),
        {textPos.x, textPos.y + 2},
        fontSize, 2, BLACK);
    DrawTextEx(game->getFont(),
        message.c_str(),
        {textPos.x - 2, textPos.y},
        fontSize, 2, BLACK);
    DrawTextEx(game->getFont(),
        message.c_str(),
        {textPos.x, textPos.y - 2},
        fontSize, 2, BLACK);

    DrawTextEx(game->getFont(), message.c_str(), textPos, fontSize, 2, YELLOW);
    
    // Draw a loading bar
    float barWidth = 400.0f;
    float barHeight = 50.0f;
    float barX = (game->getScreenWidth() - barWidth) / 2;
    float barY = textPos.y + 100;
    float progress = elapsedTime / 2.0f;
    DrawRectangle(barX, barY, barWidth * progress, barHeight, YELLOW);
    DrawRectangleLines(barX, barY, barWidth, barHeight, WHITE);
}


BackToMenuState::BackToMenuState(Game *game) : QuitState(game) {
}

void BackToMenuState::update()
{
    elapsedTime += GetFrameTime();

    if (elapsedTime > 1.5f) {
        game->changeState(game->mainMenuState.get());
        reset();
    }
}

void BackToMenuState::draw()
{
    // Draw the underlying GamePlayState
    game->mainMenuState->drawBackground();

    // Draw a semi-transparent gray overlay
    DrawRectangle(0, 0, game->getScreenWidth(), game->getScreenHeight(), Fade(GRAY, 0.5f));

    // Draw a very big text in the middle of the screen: "Saving and Going Back to Main Menu..."
    string message1 = "Saving and Going Back";
    string message2 = "to Main Menu...";
    constexpr int fontSize = 50;
    constexpr int spacing = 1;
    
    Vector2 textSize1 = MeasureTextEx(game->getFont(), message1.c_str(), fontSize, spacing);
    Vector2 textPos1 = {
        (game->getScreenWidth() - textSize1.x) / 2, // Center horizontally
        static_cast<float>(game->getScreenHeight()) / 2 - 50                             // Margin from the top
    };

    // Draw border for text
    DrawTextEx(game->getFont(), 
        message1.c_str(), 
        {textPos1.x + 2, textPos1.y}, 
        fontSize, 2, BLACK);
    DrawTextEx(game->getFont(),
        message1.c_str(),
        {textPos1.x, textPos1.y + 2},
        fontSize, 2, BLACK);
    DrawTextEx(game->getFont(),
        message1.c_str(),
        {textPos1.x - 2, textPos1.y},
        fontSize, 2, BLACK);
    DrawTextEx(game->getFont(),
        message1.c_str(),
        {textPos1.x, textPos1.y - 2},
        fontSize, 2, BLACK);

    DrawTextEx(game->getFont(), message1.c_str(), textPos1, fontSize, 2, YELLOW);

    Vector2 textSize2 = MeasureTextEx(game->getFont(), message2.c_str(), fontSize, spacing);
    Vector2 textPos2 = {
        (game->getScreenWidth() - textSize2.x) / 2, // Center horizontally
        static_cast<float>(game->getScreenHeight()) / 2 + 50                             // Margin from the top
    };

    // Draw border for text
    DrawTextEx(game->getFont(), 
        message2.c_str(), 
        {textPos2.x + 2, textPos2.y}, 
        fontSize, 2, BLACK);
    DrawTextEx(game->getFont(),
        message2.c_str(),
        {textPos2.x, textPos2.y + 2},
        fontSize, 2, BLACK);
    DrawTextEx(game->getFont(),
        message2.c_str(),
        {textPos2.x - 2, textPos2.y},
        fontSize, 2, BLACK);
    DrawTextEx(game->getFont(),
        message2.c_str(),
        {textPos2.x, textPos2.y - 2},
        fontSize, 2, BLACK);

    DrawTextEx(game->getFont(), message2.c_str(), textPos2, fontSize, 2, YELLOW);

    // Draw a loading bar
    float barWidth = 400.0f;
    float barHeight = 50.0f;
    float barX = (game->getScreenWidth() - barWidth) / 2;
    float barY = textPos2.y + 100;
    float progress = elapsedTime / 1.5f;
    DrawRectangle(barX, barY, barWidth * progress, barHeight, YELLOW);
    DrawRectangleLines(barX, barY, barWidth, barHeight, WHITE);
}

void BackToMenuState::reset()
{
    elapsedTime = 0.0f;
}

TutorialState::TutorialState(Game* game) : GameplayState(game) {
}

void TutorialState::update() {
    float deltaTime = GetFrameTime();
    Physics::Update(deltaTime); 

    Tilemap* tilemap = Tilemap::getInstance();
    tilemap->Update(deltaTime);

    // Update pause button hover state
    pauseButton.isHovered = CheckCollisionPointRec(GetMousePosition(), pauseButton.rect);

    if (IsButtonClicked(pauseButton)) {
        game->changeState(game->pauseTutorialState.get());
    }    

    if (StageStateHandler::GetInstance().GetState() == StageState::PLAYER_DEAD) {
        playSoundEffect(SoundEffect::PLAYER_DIE);
        Tilemap::getInstance()->GetPlayer()->setLives(Tilemap::getInstance()->GetPlayer()->getLives() + 1);
    }
    else if (StageStateHandler::GetInstance().GetState() == StageState::GAME_OVER) {
        StageStateHandler::GetInstance().SetState(StageState::NORMAL_STATE);
        //No need to do anything
    }
    else if (StageStateHandler::GetInstance().GetState() == StageState::WORLD_CLEAR || StageStateHandler::GetInstance().GetState() == StageState::STAGE_CLEAR) {
        StageStateHandler::GetInstance().SetState(StageState::NORMAL_STATE);
        game->changeState(game->mainMenuState.get());
    }
}

void TutorialState::draw() {
    GameplayState::draw();
}

TutorialState::~TutorialState() {
}

PauseTutorialState::PauseTutorialState(Game* game) : PauseGameState(game)
{
}

void PauseTutorialState::update() {
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
        SoundEffectHandler::SwitchEnableSoundEffects();
    }
    if (IsButtonClicked(buttons[2])) {
        game->changeState(game->gameplayState.get());
    }
    if (IsButtonClicked(buttons[3])) {
        game->changeState(game->mainMenuState.get());
    }

    float centerX = (game->getScreenWidth() - 400) / 2;
    // Update sliders
    DrawMarioSlider({centerX, 525, 400, 50}, game->getSettings().volume, 0, 100, game->getFont(), "Volume");
    DrawMarioSlider({centerX, 625, 400, 50}, game->getSettings().brightness, 0, 100, game->getFont(), "Brightness");

    // Handle get back when clicking empty space
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), {centerX, 525, 400, 50}) || CheckCollisionPointRec(GetMousePosition(), {centerX, 625, 400, 50}))
            MouseClickToEmptySpaceHandler::SetIsTrue(false);
        else if (CheckCollisionPointRec(GetMousePosition(), buttons[0].rect) || CheckCollisionPointRec(GetMousePosition(), buttons[1].rect)
            || CheckCollisionPointRec(GetMousePosition(), buttons[2].rect) || CheckCollisionPointRec(GetMousePosition(), buttons[3].rect))
            MouseClickToEmptySpaceHandler::SetIsTrue(false);
        else {
            MouseClickToEmptySpaceHandler::SetIsTrue(true);
        }
    }
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && MouseClickToEmptySpaceHandler::GetIsTrue()) {
        game->changeState(game->tutorialState.get());
    }
}

void PauseTutorialState::draw()
{
    PauseGameState::draw();
}

PauseTutorialState::~PauseTutorialState()
{
}

Gameplay2PState::Gameplay2PState(Game* game) : GameplayState(game) {
}

Gameplay2PState::~Gameplay2PState() {
}

void Gameplay2PState::update() {
    float deltaTime = GetFrameTime();
    Physics::Update(deltaTime); 

    Tilemap* tilemap = Tilemap::getInstance();
    tilemap->Update(deltaTime);

    // Update pause button hover state
    pauseButton.isHovered = CheckCollisionPointRec(GetMousePosition(), pauseButton.rect);

    if (IsButtonClicked(pauseButton)) {
        game->changeState(game->pauseGameState.get());
    }    

    if (StageStateHandler::GetInstance().GetState() == StageState::PLAYER_DEAD) {
        //Do nothing
    }
    else if (StageStateHandler::GetInstance().GetState() == StageState::GAME_OVER) {
        playSoundEffect(SoundEffect::GAME_OVER);
        StageStateHandler::GetInstance().SetState(StageState::NORMAL_STATE);
        game->changeState(game->gameOverState.get());
    }
    else if (StageStateHandler::GetInstance().GetState() == StageState::STAGE_CLEAR) {
        StageStateHandler::GetInstance().SetState(StageState::NORMAL_STATE);
    }
    else if (StageStateHandler::GetInstance().GetState() == StageState::WORLD_CLEAR) {
        StageStateHandler::GetInstance().SetState(StageState::NORMAL_STATE);
        game->changeState(game->changeStageState.get());
    }
}

void Gameplay2PState::draw() {
    GameplayState::draw();
}