#pragma once

#include "included_library.h"
#include <unordered_map>
#include <string>
#include <vector>

enum class SoundEffect {
    PLAYER_DIE,
    GAME_OVER,
    STAGE_CLEAR,
    WORLD_CLEAR,
    JUMP,
    JUMP_SUPER,
    COIN_GRAB,
    BRICK_BREAK,
    POWER_UP,
    POWER_DOWN,
    POWER_UP_APPEAR,
    HIT_ENEMY,
    SHOOT_FIREBALL,
    PIPE_DOWN,
    FLAG_POLE_DOWN,
    OUT_OF_TIME_WARNING
};

class SoundEffectHandler {
public:
    static SoundEffectHandler& GetInstance();

    void LoadSoundEffect(SoundEffect effect, const std::string& filePath);
    void PlaySoundEffect(SoundEffect effect);

    static void SwitchEnableSoundEffects();
private:
    SoundEffectHandler();
    ~SoundEffectHandler();

    void InitializeSoundEffects();

    SoundEffectHandler(const SoundEffectHandler&) = delete;
    SoundEffectHandler& operator=(const SoundEffectHandler&) = delete;

    std::unordered_map<SoundEffect, Sound> soundEffects;

    static bool isEnabled;
};

void playSoundEffect(SoundEffect effect);
