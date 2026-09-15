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