#include "game.h"
#include "include.h"
#include "object.h"
#include "game_state.h"


// const int screenWidth = 800;
// const int screenHeight = 600;

// Player player("Player", 0, 0, true, false, 0, 0, 0, 0, Vector2{0, 0}, 0, 0, {}, "mario");
// MyCamera camera;

// Game::Game(){
//     Init();
// }

// void Game::Init() {
//     SetTraceLogCallback([](int logLevel, const char* text, va_list args) {
//         return;
//     });
//     InitWindow(screenWidth, screenHeight, "Super Mario Bros");
//     SetTargetFPS(60);

//     Physics::Init(); 
//     AnimationEffectCreator::InitEffects();
//     TextHelper::loadFont("highway_gothic", "");
//     TextHelper::loadTexture("coin");


//     ImageSet idleImageSet = IDLE;
//     movingObjects.push_back(player.copy());

//     movingObjects[0]->Init(b2Vec2(10.0f, 12.0f), idleImageSet);

//     TilesetHandler::Init();
//     Tilemap* tilemap = Tilemap::getInstance();
//     tilemap->LoadMapFromJson("resources/tilemaps/map-1-1.json");
//     camera = MyCamera(38.0f, Vector2{ (float)tilemap->GetWidth(), (float)tilemap->GetHeight() }, screenWidth, screenHeight);
// }

// void Game::Update(float deltaTime) {
//     Physics::Update(deltaTime); 
//     if (GetMouseWheelMove() > 0) camera.SetZoom(camera.GetZoom() * 1.1f);
//     if (GetMouseWheelMove() < 0) camera.SetZoom(camera.GetZoom() / 1.1f);


//     for (int i = 0; i < movingObjects.size(); i++) {
//         Vector2 pos = movingObjects[i]->getPosition();
//         b2Vec2 vel = movingObjects[i]->getVelocity();
//         movingObjects[i]->HandleInput();
//         movingObjects[i]->Update(Vector2{vel.x, vel.y}, deltaTime);
//     }

//     b2Vec2 velocity = movingObjects[0]->getVelocity();
//     Vector2 position = movingObjects[0]->getPosition();
//     //cout << "player position: " << position.x << " " << position.y << endl;
//     camera.Update(movingObjects[0]->getPosition());  
//     Tilemap* tilemap = Tilemap::getInstance();
//     tilemap->Update(Vector2{velocity.x, velocity.y}, deltaTime);
// }

// void Game::Draw() {
//     BeginDrawing();
//     ClearBackground(RAYWHITE);
//     BeginMode2D(camera.GetCamera());

//     Tilemap* tilemap = Tilemap::getInstance();
//     tilemap->Draw();
//     for (int i = 0; i < movingObjects.size(); i++) {
//         movingObjects[i]->Draw();
//     }

//     Vector2 cameraTarget = camera.GetCameraTarget();
//     movingObjects[0]->Draw(Vector2{cameraTarget.x - 9.0f, cameraTarget.y - 7.0f}, 0.0f);
    
//     Physics::world.DebugDraw(); 

//     EndMode2D();
//     EndDrawing();
// }

// void Game::Cleanup() {
//     // Physics::world.DestroyBody(groundBody); 
//     TilesetHandler::Clear();
//     CloseWindow();

//     for (int i = 0; i < movingObjects.size(); i++) {
//         delete movingObjects[i];
//     } 
// }

// void Game::Run() {
//     while (!WindowShouldClose()) {
//         float deltaTime = GetFrameTime();
//         Update(deltaTime); 
//         Draw();         
//     }
//     Cleanup(); 
// }


Game::Game()
    : mainMenuState(nullptr),
      settingsState(nullptr),
      savedGameState(nullptr),
      mapBuilderState(nullptr),
      gameplayState(nullptr),
      pauseGameState(nullptr),
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