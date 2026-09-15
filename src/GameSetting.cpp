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

void GameSetting::DrawHomeIcon(float cx, float cy, float size, Color col) {
    float s = size;
    float x = cx - s * 0.5f;
    float y = cy - s * 0.5f;

    // Mai nha (Tam giac)
    Vector2 top = { x + s * 0.50f, y + s * 0.14f };
    Vector2 left = { x + s * 0.14f, y + s * 0.50f };
    Vector2 right = { x + s * 0.86f, y + s * 0.50f };
    DrawTriangle(top, left, right, col);

    // Ong khoi
    DrawRectangle((int)(x + s * 0.68f), (int)(y + s * 0.20f),
                  (int)(s * 0.10f), (int)(s * 0.18f), col);

    // Than nha
    DrawRectangle((int)(x + s * 0.22f), (int)(y + s * 0.48f),
                  (int)(s * 0.56f), (int)(s * 0.38f), col);

    // Cua ra vao
    DrawRectangleRounded(
        Rectangle{ x + s * 0.40f, y + s * 0.60f, s * 0.20f, s * 0.26f },
        0.35f, 4, Color{230, 92, 54, 255});
}

void GameSetting::DrawGearIcon(float cx, float cy, float size, Color col) {
    float r = size * 0.36f;
    int teeth = 8;
    for (int i = 0; i < teeth; i++) {
        float angle = i * (360.0f / teeth);
        float rad = angle * DEG2RAD;
        float tx = cx + cosf(rad) * (r + 3.0f);
        float ty = cy + sinf(rad) * (r + 3.0f);
        DrawRectanglePro(Rectangle{tx, ty, size * 0.22f, size * 0.16f},
                         Vector2{size * 0.11f, size * 0.08f}, angle, col);
    }
    DrawCircle((int)cx, (int)cy, r, col);
    DrawCircle((int)cx, (int)cy, r * 0.42f, Color{28, 28, 32, 255});
}

void GameSetting::DrawCloseIcon(float cx, float cy, float size, Color col) {
    float hs = size * 0.32f;
    DrawLineEx({cx - hs, cy - hs}, {cx + hs, cy + hs}, 3.8f, col);
    DrawLineEx({cx + hs, cy - hs}, {cx - hs, cy + hs}, 3.8f, col);
}

void GameSetting::DrawInfoIcon(float cx, float cy, float size, Color col) {
    // Cham tren dau
    DrawCircle((int)cx, (int)(cy - size * 0.22f), size * 0.10f, col);
    // Than chu i
    DrawRectangleRounded(
        Rectangle{ cx - size * 0.09f, cy - size * 0.06f, size * 0.18f, size * 0.34f },
        0.4f, 4, col);
}

// VE NUT GEAR (SETTINGS) O GOC TREN PHAI MAN HINH
void GameSetting::DrawSettingsButton() {
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, gearBtnBounds);

    Color bg = hover ? Color{45, 45, 52, 240} : Color{30, 30, 35, 210};
    Color border = hover ? Color{245, 178, 38, 255} : Color{60, 60, 68, 255};

    DrawRectangleRounded(gearBtnBounds, 0.22f, 6, bg);
    DrawRectangleRoundedLinesEx(gearBtnBounds, 0.22f, 6, 2.0f, border);

    float cx = gearBtnBounds.x + gearBtnBounds.width * 0.5f;
    float cy = gearBtnBounds.y + gearBtnBounds.height * 0.5f;
    DrawGearIcon(cx, cy, 26.0f, hover ? Color{245, 220, 130, 255} : Color{200, 200, 210, 255});
}

// UPDATE MODAL INPUT
void GameSetting::UpdateModal(SoundManager& sound, bool& outRestartGame) {
    if (!isModalOpen) return;

    Vector2 mouse = GetMousePosition();
    bool clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    float mx = modalBounds.x;
    float my = modalBounds.y;
    float mw = modalBounds.width;

    // 1. Nut Close [X] o header
    Rectangle btnClose = { mx + mw - 56, my + 14, 40, 40 };
    if (clicked && CheckCollisionPointRec(mouse, btnClose)) {
        CloseModal();
        return;
    }

    // 2. Nut Info [i] o header
    Rectangle btnInfo = { mx + 16, my + 14, 40, 40 };
    if (clicked && CheckCollisionPointRec(mouse, btnInfo)) {
        showInfoModal = !showInfoModal;
        return;
    }

    // Neu dang mo popup thong tin [i], chi xu ly click dong popup do
    if (showInfoModal) {
        Rectangle btnOk = { mx + mw * 0.5f - 60, my + 380, 120, 48 };
        if (clicked && CheckCollisionPointRec(mouse, btnOk)) {
            showInfoModal = false;
        }
        return;
    }

    // 3. SFX Toggle Track
    Rectangle sfxTrack = { mx + 165, my + 98, 245, 56 };
    if (clicked && CheckCollisionPointRec(mouse, sfxTrack)) {
        ToggleSfx(sound);
    }

    // 4. Music Toggle Track
    Rectangle musTrack = { mx + 165, my + 172, 245, 56 };
    if (clicked && CheckCollisionPointRec(mouse, musTrack)) {
        ToggleMusic(sound);
    }

    // 5. Do kho (Difficulty) Selector: [Easy] [Normal] [Hard]
    float diffY = my + 292;
    Rectangle optEasy   = { mx + 50, diffY, 114, 46 };
    Rectangle optNormal = { mx + 172, diffY, 114, 46 };
    Rectangle optHard   = { mx + 294, diffY, 116, 46 };

    if (clicked) {
        if (CheckCollisionPointRec(mouse, optEasy))   SetDifficulty(DIFF_EASY);
        if (CheckCollisionPointRec(mouse, optNormal)) SetDifficulty(DIFF_NORMAL);
        if (CheckCollisionPointRec(mouse, optHard))   SetDifficulty(DIFF_HARD);
    }

    // 6. Nut Home (Dong Modal / Quay lai choi)
    Rectangle btnHome = { mx + 50, my + 418, 155, 64 };
    if (clicked && CheckCollisionPointRec(mouse, btnHome)) {
        CloseModal();
        return;
    }

    // 7. Nut Restart (New Game)
    Rectangle btnRestart = { mx + 225, my + 418, 185, 64 };
    if (clicked && CheckCollisionPointRec(mouse, btnRestart)) {
        outRestartGame = true;
        CloseModal();
        return;
    }

    // Phim ESC de dong
    if (IsKeyPressed(KEY_ESCAPE)) {
        CloseModal();
    }
}