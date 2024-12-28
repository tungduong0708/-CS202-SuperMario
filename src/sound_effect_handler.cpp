#include "sound_effect_handler.h"

bool SoundEffectHandler::isEnabled = true;

SoundEffectHandler& SoundEffectHandler::GetInstance() {
    static SoundEffectHandler instance;
    return instance;
}

SoundEffectHandler::SoundEffectHandler() {
    InitializeSoundEffects();
    cout << "Sound effect handler initialized" << endl;
}

void SoundEffectHandler::LoadSoundEffect(SoundEffect effect, const std::string& filePath) {
    if (!ifFileExists(filePath)) {
        cerr << "Failed to load sound effect: " << filePath << endl;
        return;
    }
    Sound sound = LoadSound(filePath.c_str());
    soundEffects[effect] = sound;
}

void SoundEffectHandler::PlaySoundEffect(SoundEffect effect) {
    if (!isEnabled) {
        return;
    }
    if (soundEffects.find(effect) != soundEffects.end()) {
        PlaySound(soundEffects[effect]);
    }
}

SoundEffectHandler::~SoundEffectHandler() {
    for (auto& sound : soundEffects) {
        UnloadSound(sound.second);
    }
}

void SoundEffectHandler::InitializeSoundEffects() {
    LoadSoundEffect(SoundEffect::PLAYER_DIE, "resources/audio/player_die_sound.wav");
    LoadSoundEffect(SoundEffect::GAME_OVER, "resources/audio/game_over_sound.wav");
    LoadSoundEffect(SoundEffect::STAGE_CLEAR, "resources/audio/stage_clear_sound.wav");
    LoadSoundEffect(SoundEffect::WORLD_CLEAR, "resources/audio/world_clear_sound.wav");
    LoadSoundEffect(SoundEffect::JUMP, "resources/audio/jump_sound.wav");
    LoadSoundEffect(SoundEffect::JUMP_SUPER, "resources/audio/jump_super_sound.wav");
    LoadSoundEffect(SoundEffect::COIN_GRAB, "resources/audio/coin_grab_sound.wav");
    LoadSoundEffect(SoundEffect::BRICK_BREAK, "resources/audio/brick_break_sound.wav");
    LoadSoundEffect(SoundEffect::POWER_UP, "resources/audio/power_up_sound.wav");
    LoadSoundEffect(SoundEffect::POWER_DOWN, "resources/audio/power_down_sound.wav");
    LoadSoundEffect(SoundEffect::POWER_UP_APPEAR, "resources/audio/power_up_appear_sound.wav");
    LoadSoundEffect(SoundEffect::HIT_ENEMY, "resources/audio/hit_enemy_sound.wav");
    LoadSoundEffect(SoundEffect::SHOOT_FIREBALL, "resources/audio/shoot_fireball_sound.wav");
    LoadSoundEffect(SoundEffect::PIPE_DOWN, "resources/audio/pipe_down_sound.wav");
    LoadSoundEffect(SoundEffect::FLAG_POLE_DOWN, "resources/audio/flag_pole_down_sound.wav");
    LoadSoundEffect(SoundEffect::OUT_OF_TIME_WARNING, "resources/audio/out_of_time_warning_sound.wav");
}

void SoundEffectHandler::SwitchEnableSoundEffects() {
    isEnabled = !isEnabled;
}

void playSoundEffect(SoundEffect effect) {
    SoundEffectHandler::GetInstance().PlaySoundEffect(effect);
}
