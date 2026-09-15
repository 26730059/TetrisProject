#include "GameSetting.h"
#include "SoundManager.h"
#include <fstream>
#include <sstream>
#include <cmath>

GameSetting::GameSetting() {
    // Thiet lap vi tri modal va gear button
    float mw = 460.0f;
    float mh = 520.0f;
    modalBounds   = { (SCREEN_W - mw) * 0.5f, (SCREEN_H - mh) * 0.5f, mw, mh };
    gearBtnBounds = { (float)(SCREEN_W - 74), 18.0f, 48.0f, 48.0f };
}

// MODAL STATE MANAGEMENT
void GameSetting::OpenModal() {
    isModalOpen = true;
    showInfoModal = false;
}

void GameSetting::CloseModal() {
    isModalOpen = false;
    showInfoModal = false;
}

void GameSetting::ToggleModal() {
    isModalOpen = !isModalOpen;
    showInfoModal = false;
}

bool GameSetting::CheckSettingsButtonClicked() {
    Vector2 mouse = GetMousePosition();
    return CheckCollisionPointRec(mouse, gearBtnBounds) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

// TOGGLE AUDIO
void GameSetting::ToggleMusic(SoundManager& sound) {
    musicEnabled = !musicEnabled;
    sound.SetMusicVolume(musicEnabled ? musicVolume : 0.0f);
}

void GameSetting::ToggleSfx(SoundManager& sound) {
    sfxEnabled = !sfxEnabled;
    sound.SetSfxVolume(sfxEnabled ? sfxVolume : 0.0f);
}

// DO KHO (DIFFICULTY)
void GameSetting::SetDifficulty(Difficulty d) {
    difficulty = d;
    switch (difficulty) {
        case DIFF_EASY:
            baseGravity   = 1.15f;
            gravityStep   = 0.035f;
            minGravity    = 0.16f;
            lockDelay     = 0.65f;
            break;
        case DIFF_NORMAL:
            baseGravity   = 0.80f;
            gravityStep   = 0.060f;
            minGravity    = 0.08f;
            lockDelay     = 0.50f;
            break;
        case DIFF_HARD:
            baseGravity   = 0.45f;
            gravityStep   = 0.075f;
            minGravity    = 0.03f;
            lockDelay     = 0.35f;
            break;
        default:
            break;
    }
}

const char* GameSetting::GetDifficultyName() const {
    switch (difficulty) {
        case DIFF_EASY:   return "Easy";
        case DIFF_NORMAL: return "Normal";
        case DIFF_HARD:   return "Hard";
        default:          return "Normal";
    }
}

float GameSetting::GetGravityInterval(int level) const {
    float interval = baseGravity - (level - 1) * gravityStep;
    return std::max(minGravity, interval);
}

// DO KHO (DIFFICULTY)
void GameSetting::SetDifficulty(Difficulty d) {
    difficulty = d;
    switch (difficulty) {
        case DIFF_EASY:
            baseGravity   = 1.15f;
            gravityStep   = 0.035f;
            minGravity    = 0.16f;
            lockDelay     = 0.65f;
            break;
        case DIFF_NORMAL:
            baseGravity   = 0.80f;
            gravityStep   = 0.060f;
            minGravity    = 0.08f;
            lockDelay     = 0.50f;
            break;
        case DIFF_HARD:
            baseGravity   = 0.45f;
            gravityStep   = 0.075f;
            minGravity    = 0.03f;
            lockDelay     = 0.35f;
            break;
        default:
            break;
    }
}

const char* GameSetting::GetDifficultyName() const {
    switch (difficulty) {
        case DIFF_EASY:   return "Easy";
        case DIFF_NORMAL: return "Normal";
        case DIFF_HARD:   return "Hard";
        default:          return "Normal";
    }
}

float GameSetting::GetGravityInterval(int level) const {
    float interval = baseGravity - (level - 1) * gravityStep;
    return std::max(minGravity, interval);
}