#include "game.h"
#include "include.h"
#include "object.h"
#include "game_state.h"

Game::Game()
    : mainMenuState(nullptr),
      settingsState(nullptr),
      savedGameState(nullptr),
      mapBuilderState(nullptr),
      gameplayState(nullptr),
      pauseGameState(nullptr),
      selectPlayerState(nullptr),
      currentState(nullptr)
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
    std::cout << "MainMenuState initialized" << std::endl;
    settingsState = std::make_unique<SettingsState>(this);
    savedGameState = std::make_unique<SavedGameState>(this);
    mapBuilderState = std::make_unique<MapBuilderState>(this);
    gameplayState = std::make_unique<GameplayState>(this);
    pauseGameState = std::make_unique<PauseGameState>(this);
    selectPlayerState = std::make_unique<SelectPlayerState>(this);
    currentState = mainMenuState.get();
    std::cout << "All states initialized" << std::endl;
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

void Game::changeState(GameState *state) {
    currentState = state;
}