#include "game.h"
#include "include.h"
#include "object.h"
#include "game_state.h"

Game* Game::instance = nullptr;

Game::Game()
    : mainMenuState(nullptr),
      settingsState(nullptr),
      savedGameState(nullptr),
      mapBuilderState(nullptr),
      gameplayState(nullptr),
      pauseGameState(nullptr),
      mapPauseState(nullptr),
      selectPlayerState(nullptr),
      deathState(nullptr),
      changeStageState(nullptr),
      gameOverState(nullptr),
      victoryState(nullptr),
      previousState(nullptr),
      currentState(nullptr),
      nextState(nullptr),
      shouldExit(false)
{
    InitWindow(screenWidth, screenHeight, "Game");
    InitAudioDevice();
    SetTargetFPS(60);
    font = LoadFont("../resources/fonts/SuperMario256.ttf");
    music = LoadMusicStream("../resources/audio/01. Ground Theme.mp3");
    PlayMusicStream(music);
    settings = { true, true, 100, 50 };
    std::cout << "Initializing Game" << std::endl;
    mainMenuState = std::make_unique<MainMenuState>(this);
    settingsState = std::make_unique<SettingsState>(this);
    savedGameState = std::make_unique<SavedGameState>(this);
    mapBuilderState = std::make_unique<MapBuilderState>(this);
    gameplayState = std::make_unique<GameplayState>(this);
    pauseGameState = std::make_unique<PauseGameState>(this);
    mapPauseState = std::make_unique<MapPauseState>(this);
    selectPlayerState = std::make_unique<SelectPlayerState>(this);
    deathState = std::make_unique<DeathState>(this);
    changeStageState = std::make_unique<ChangeStageState>(this);
    gameOverState = std::make_unique<GameOverState>(this);
    victoryState = std::make_unique<VictoryState>(this);
    currentState = mainMenuState.get();
    std::cout << "All states initialized" << std::endl;
}

Game::~Game() {
    UnloadFont(font);
    UnloadMusicStream(music);
}

Game* Game::getInstance() {
    if (instance == nullptr) {
        instance = new Game();
    }
    return instance;
}

void Game::run() {
    while (!WindowShouldClose() && !shouldExit) {
        update();
        draw();
    }
    gameplayState->cleanup();
    CloseAudioDevice();
    CloseWindow();
}

void Game::update() {
    if (settings.music)
    {
        UpdateMusicStream(music);
    }
    SetMusicVolume(music, settings.volume / 100.0f); // Update music volume
    if (currentState)
    {
        currentState->update();
    }
}

void Game::draw() {
    BeginDrawing();
    ClearBackground(Fade(RAYWHITE, settings.brightness / 100.0f)); // Update brightness
    if (currentState)
    {
        currentState->draw();
    }
    EndDrawing();
}

void Game::exitGame() {
    shouldExit = true;
}

GameSettings& Game::getSettings() {
    return settings;
}

Font Game::getFont() const {
    return font;
}

int Game::getScreenWidth() {
    return screenWidth;
}

int Game::getScreenHeight() {
    return screenHeight;
}

GameState* Game::getPreviousState() const {
    return previousState;
}

GameState* Game::getCurrentState() const {
    return currentState;
}

GameState* Game::getNextState() const {
    return nextState;
}

void Game::changeState(GameState *state) {
    previousState = currentState;
    currentState = state;
    nextState = nullptr;
}