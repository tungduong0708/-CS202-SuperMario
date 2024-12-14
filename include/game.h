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
    Game();

    void run();
    void update();
    void draw();
    void exitGame();

    // Getters
    GameSettings& getSettings();
    [[nodiscard]] Font getFont() const;
    [[nodiscard]] static int getScreenWidth();
    [[nodiscard]] static int getScreenHeight();

    void changeState(GameState* state);

    // States
    std::unique_ptr<GameState> mainMenuState;
    std::unique_ptr<GameState> settingsState;
    std::unique_ptr<GameState> savedGameState;
    std::unique_ptr<GameState> mapBuilderState;
    std::unique_ptr<GameState> gameplayState;
    std::unique_ptr<GameState> pauseGameState;
    std::unique_ptr<GameState> selectPlayerState;

private:
    static constexpr int screenWidth = 800;
    static constexpr int screenHeight = 600;
    GameState* currentState;
    Font font{};
    Music music{};
    GameSettings settings{};
    bool shouldExit = false;
};
#endif // GAME_H
