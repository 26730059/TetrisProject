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

// HELPER VE BUTTON 3D VA CAC ICON
void GameSetting::DrawBeveledButton(Rectangle rect, Color topColor, Color btmColor, float roundness) {
    // 1. Shadow / Bottom bevel
    DrawRectangleRounded(rect, roundness, 6, btmColor);
    // 2. Top face (nhích lên 3px de tao go noi 3D)
    Rectangle topRect = { rect.x, rect.y, rect.width, rect.height - 3.5f };
    DrawRectangleRounded(topRect, roundness, 6, topColor);
}

void GameSetting::DrawSpeakerIcon(float cx, float cy, float size, Color col, bool muted) {
    float s = size;
    float x = cx - s * 0.5f;
    float y = cy - s * 0.5f;

    // Phan than loa (hinh chu nhat nho phia sau)
    DrawRectangle((int)(x + s * 0.12f), (int)(y + s * 0.32f),
                  (int)(s * 0.22f), (int)(s * 0.36f), col);

    // Phan phao loa (hinh non mo rong)
    Vector2 p1 = { x + s * 0.30f, y + s * 0.32f };
    Vector2 p2 = { x + s * 0.56f, y + s * 0.16f };
    Vector2 p3 = { x + s * 0.56f, y + s * 0.84f };
    Vector2 p4 = { x + s * 0.30f, y + s * 0.68f };
    DrawTriangle(p1, p2, p3, col);
    DrawTriangle(p1, p3, p4, col);

    if (!muted) {
        // Song am thanh (Sound waves)
        Vector2 center = { x + s * 0.56f, y + s * 0.50f };
        DrawRing(center, s * 0.16f, s * 0.24f, -40, 40, 12, col);
        DrawRing(center, s * 0.30f, s * 0.38f, -45, 45, 12, col);
    } else {
        // Dau cheo Mute (X)
        float x1 = x + s * 0.65f, y1 = y + s * 0.34f;
        float x2 = x + s * 0.90f, y2 = y + s * 0.66f;
        DrawLineEx({x1, y1}, {x2, y2}, 3.5f, col);
        DrawLineEx({x2, y1}, {x1, y2}, 3.5f, col);
    }
}