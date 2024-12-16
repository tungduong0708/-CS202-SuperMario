#pragma once

#include <vector>
#include <string>
#include "object.h"
#include "raylib.h"

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
    std::string text;
    bool isHovered;
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

class SavedGameState : public GameState
{
public:
    explicit SavedGameState(Game* game);
    void update() override;
    void draw() override;

private:
    std::vector<Button> buttons;
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
    ImageButton player1Button;
    ImageButton player2Button;
};

class DeathState : public GameState
{
public:
    explicit DeathState(Game* game);
    void update() override;
    void draw() override;
protected:
    int lifeRemaining;
    std::vector<Button> buttons;
};

class ChangeStageState : public DeathState
{
public:
    explicit ChangeStageState(Game* game);
    void update() override;
    void draw() override;
};

class GameOverState : public GameState
{
public:
    explicit GameOverState(Game* game);
    void update() override;
    void draw() override;
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