#pragma once

#include <vector>
#include <string>
#include "object.h"
#include "raylib.h"
#include "mouse_click_to_empty_space_handler.h"

class Game;

struct Button
{
    Rectangle rect;
    std::string text;
    bool isHovered;
};

struct ImageButton
{
    Rectangle rect;
    Texture2D texture;
    Texture2D hoverTexture;
    bool isHovered;
};

struct PlayerInfo
{
    std::string name;
    Texture2D texture;
    Texture2D hoverTexture;
    ImageButton button;
    int speed;
    int jumpForce;
    int bulletSpeed;
    int bulletFreq;
};

class GameState
{
public:
    explicit GameState(Game* game);
    virtual ~GameState() = default;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void cleanup() {};
    virtual void drawBackground() {};
protected:
    Game* game;
};

// Concrete States
class MainMenuState : public GameState
{
public:
    explicit MainMenuState(Game* game);
    ~MainMenuState() override;
    void update() override;
    void draw() override;
    void drawBackground() override;

private:
    std::vector<Button> buttons;
    Texture2D backgroundTexture{};
    Texture2D logoTexture{};
};

class SettingsState : public GameState
{
public:
    explicit SettingsState(Game* game);
    void update() override;
    void draw() override;

private:
    std::vector<Button> buttons;
};

class TutorialState : public GameState
{
public:
    explicit TutorialState(Game* game);
    void update() override;
    void draw() override;
    ~TutorialState() override;
private:
    std::vector<Button> buttons;
    std::vector<Texture2D> tutorialTextures;
};

class SavedGameState : public GameState
{
public:
    explicit SavedGameState(Game* game);
    void update() override;
    void draw() override;
    ~SavedGameState() override;
private:
    std::vector<Button> buttons;
    Texture2D backgroundTexture;
};

class MapBuilderState : public GameState
{
public:
    explicit MapBuilderState(Game* game);
    void update() override;
    void draw() override;
    ~MapBuilderState() override;

private:
    Texture2D backgroundTexture{};
    Button pauseButton;
};

class GameplayState : public GameState
{
public:
    explicit GameplayState(Game* game);
    void update() override;
    void draw() override;
    void cleanup() override;

private:
    std::vector<MovingObject*> movingObjects;
    Button pauseButton;
};

class PauseGameState : public GameState
{
public:
    explicit PauseGameState(Game* game);
    void update() override;
    void draw() override;

protected:
    std::vector<Button> buttons;
};

class MapPauseState : public PauseGameState
{
public:
    explicit MapPauseState(Game* game);
    void update() override;
    void draw() override;
};

class SelectPlayerState : public GameState
{
public:
    explicit SelectPlayerState(Game* game);
    void update() override;
    void draw() override;
    ~SelectPlayerState() override;
private:
    PlayerInfo player1;
    PlayerInfo player2;
};

class ChangeStageState : public GameState
{
public:
    explicit ChangeStageState(Game* game);
    void update() override;
    void draw() override;
    void setLifeRemaining(int life);
    virtual void reset();

    void setStageName(const std::string& name);

    ~ChangeStageState() override;

protected:
    float elapsedTime;
    int lifeRemaining;
    Texture2D characterTexture;
    std::string stageName;
};

class DeathState : public ChangeStageState
{
public:
    explicit DeathState(Game* game);
    void update() override;
    void draw() override;
    void reset() override;
    ~DeathState() override;

private:
    bool showDeathImage;
    Texture2D deathTexture;
};

class GameOverState : public GameState
{
public:
    explicit GameOverState(Game* game);
    void update() override;
    void draw() override;

    // Setters
    void setScore(int score);
    void setHighScore(int highScore);
    void setTimeRemaining(int timeRemaining);
protected:
    std::vector<Button> buttons;
    int score;
    int highScore;
    int timeRemaining;
};

class VictoryState : public GameOverState
{
public:
    explicit VictoryState(Game* game);
    void update() override;
    void draw() override;
};


class GameSavingState : public GameState
{
public:
    explicit GameSavingState(Game* game);
    void update() override;
    void draw() override;
    ~GameSavingState() override;
private:
    std::vector<Button> buttons;
    Texture2D backgroundTexture;
};


class SelectDifficultyState : public GameState
{
public:
    explicit SelectDifficultyState(Game* game);
    void update() override;
    void draw() override;
    ~SelectDifficultyState() override;
private:
    std::vector<ImageButton> buttons;
    std::vector<Texture2D> difficultyTextures;
};

class AreYouSureState : public GameState
{
public:
    explicit AreYouSureState(Game* game);
    void update() override;
    void draw() override;
    ~AreYouSureState() override;
protected:
    std::vector<Button> buttons;
    Texture2D backgroundTexture;
};

class WannaSaveState : public AreYouSureState
{
public:
    explicit WannaSaveState(Game* game);
    void update() override;
    void draw() override;
    ~WannaSaveState() override;
};

class QuitState : public GameState {
public:
    explicit QuitState(Game* game);
    void update() override;
    void draw() override;
    ~QuitState() override;
protected:
    std::vector<Button> buttons;
};

class BackToMenuState : public QuitState {
public:
    explicit BackToMenuState(Game* game);
    void update() override;
    void draw() override;
    ~BackToMenuState() override;
};