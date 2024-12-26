#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <memory>

#include "game_state.h"

struct GameSettings
{
    bool soundEffects;
    bool music;
    int volume;
    int brightness;
};

class Game {
public:
    static Game* getInstance();

    void run();
    void update();
    void draw();
    void exitGame();

    ~Game();

    // Getters
    GameSettings& getSettings();
    [[nodiscard]] Font getFont() const;
    [[nodiscard]] static int getScreenWidth();
    [[nodiscard]] static int getScreenHeight();
    [[nodiscard]] GameState* getPreviousState() const;
    [[nodiscard]] GameState* getCurrentState() const;
    [[nodiscard]] GameState* getNextState() const;

    void changeState(GameState* state);

    // States
    std::unique_ptr<GameState> mainMenuState;
    std::unique_ptr<GameState> settingsState;
    std::unique_ptr<GameState> tutorialState;
    std::unique_ptr<GameState> savedGameState;
    std::unique_ptr<GameState> mapBuilderState;
    std::unique_ptr<GameState> gameplayState;
    std::unique_ptr<GameState> pauseGameState;
    std::unique_ptr<GameState> mapPauseState;
    std::unique_ptr<GameState> selectDifficultyState;
    std::unique_ptr<GameState> selectPlayerState;
    std::unique_ptr<GameState> gameSavingState;
    std::unique_ptr<GameState> deathState;
    std::unique_ptr<GameState> changeStageState;
    std::unique_ptr<GameState> gameOverState;
    std::unique_ptr<GameState> victoryState;
    std::unique_ptr<GameState> pauseTutorialState;
    std::unique_ptr<GameState> gameplay2PState;

private:
    static Game* instance;

    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    static constexpr int screenWidth = 1000;
    static constexpr int screenHeight = 750;
    GameState* currentState;
    GameState* previousState;
    GameState* nextState;
    Font font{};
    Music music{};
    GameSettings settings{};
    bool shouldExit = false;
};
#endif // GAME_H
