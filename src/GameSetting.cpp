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
    // Gear vector 8 rang: form don gian, ro net o kich thuoc 27px cua nut Settings.
    constexpr int teeth = 8;
    float coreRadius = size * 0.285f;
    float toothLength = size * 0.205f;
    float toothWidth = size * 0.125f;

    for (int i = 0; i < teeth; ++i) {
        float angle = i * (360.0f / teeth);
        float rad = angle * DEG2RAD;
        float distance = coreRadius + toothLength * 0.36f;
        float tx = cx + cosf(rad) * distance;
        float ty = cy + sinf(rad) * distance;

        // Răng được xoay theo hướng xuyên tâm để hình gọn và đều.
        DrawRectanglePro(Rectangle{tx, ty, toothLength, toothWidth},
                         Vector2{toothLength * 0.5f, toothWidth * 0.5f}, angle, col);
    }

    // Vòng ngoài dày tạo silhouette rõ; hub lục giác cho cảm giác sci-fi.
    DrawCircleV(Vector2{cx, cy}, coreRadius, col);
    DrawCircleLines((int)cx, (int)cy, coreRadius * 0.72f, Color{210, 252, 255, 150});
    DrawPoly(Vector2{cx, cy}, 6, coreRadius * 0.48f, 30.0f, Color{10, 18, 44, 255});
    DrawPolyLines(Vector2{cx, cy}, 6, coreRadius * 0.48f, 30.0f, Color{151, 239, 255, 200});
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
    bool pressed = hover && IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    // Neon cyan/purple glow: tang do sang khi hover nhung van hop nen toi.
    Color glowOuter = hover ? Color{78, 104, 255, 46} : Color{48, 92, 205, 22};
    Color glowInner = hover ? Color{54, 238, 255, 68} : Color{54, 165, 255, 30};
    Rectangle outerGlow = { gearBtnBounds.x - 5.0f, gearBtnBounds.y - 5.0f,
                            gearBtnBounds.width + 10.0f, gearBtnBounds.height + 10.0f };
    Rectangle innerGlow = { gearBtnBounds.x - 2.0f, gearBtnBounds.y - 2.0f,
                            gearBtnBounds.width + 4.0f, gearBtnBounds.height + 4.0f };
    DrawRectangleRounded(outerGlow, 0.30f, 8, glowOuter);
    DrawRectangleRounded(innerGlow, 0.27f, 8, glowInner);

    // Bóng đổ và mặt nút beveled tạo chiều sâu.
    Rectangle shadow = { gearBtnBounds.x, gearBtnBounds.y + 3.0f,
                         gearBtnBounds.width, gearBtnBounds.height };
    DrawRectangleRounded(shadow, 0.22f, 8, Color{7, 10, 25, 200});

    float pressOffset = pressed ? 2.0f : 0.0f;
    Rectangle face = { gearBtnBounds.x, gearBtnBounds.y + pressOffset,
                       gearBtnBounds.width, gearBtnBounds.height - 2.0f };
    Color faceColor = hover ? Color{23, 34, 71, 248} : Color{18, 25, 52, 238};
    Color border = hover ? Color{76, 243, 255, 255} : Color{80, 125, 228, 230};
    DrawRectangleRounded(face, 0.22f, 8, faceColor);
    DrawRectangleRoundedLinesEx(face, 0.22f, 8, hover ? 2.2f : 1.6f, border);

    // Điểm sáng nhỏ ở mép trên, nhấn mạnh phong cách sci-fi.
    DrawLineEx({face.x + 10.0f, face.y + 4.0f},
               {face.x + face.width - 10.0f, face.y + 4.0f},
               1.2f, hover ? Color{175, 252, 255, 190} : Color{118, 168, 255, 110});

    float cx = face.x + face.width * 0.5f;
    float cy = face.y + face.height * 0.5f;
    Color iconShadow = hover ? Color{91, 55, 220, 170} : Color{91, 55, 220, 100};
    Color iconColor = hover ? Color{122, 248, 255, 255} : Color{101, 194, 255, 255};
    DrawGearIcon(cx + 1.0f, cy + 1.5f, 27.0f, iconShadow);
    DrawGearIcon(cx, cy, 27.0f, iconColor);
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

// VE MODAL MENU THEO DESIGN THAM KHAO
void GameSetting::DrawModal() {
    if (!isModalOpen) return;

    Vector2 mouse = GetMousePosition();

    // 1. Dark overlay lam mo game phia sau
    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.65f));

    float mx = modalBounds.x;
    float my = modalBounds.y;
    float mw = modalBounds.width;
    float mh = modalBounds.height;

    // 2. Bong do modal
    DrawRectangleRounded(Rectangle{ mx + 6, my + 10, mw, mh }, 0.08f, 8, Fade(BLACK, 0.35f));

    // 3. Khung chinh mau kem/peach am ap (nhu trong anh)
    DrawRectangleRounded(modalBounds, 0.08f, 8, Color{255, 243, 230, 255});
    DrawRectangleRoundedLinesEx(modalBounds, 0.08f, 8, 2.5f, Color{236, 206, 182, 255});

    // 4. Header Bar
    float headH = 68.0f;
    DrawRectangleRounded(Rectangle{ mx, my, mw, headH + 12 }, 0.08f, 8, Color{255, 232, 215, 255});
    DrawRectangle((int)mx, (int)(my + headH - 12), (int)mw, 14, Color{255, 232, 215, 255});
    DrawLineEx({mx, my + headH}, {mx + mw, my + headH}, 2.0f, Color{235, 205, 180, 255});

    // --- Nut Info [i] o goc trai header ---
    Rectangle btnInfo = { mx + 16, my + 14, 40, 40 };
    bool infoHover = CheckCollisionPointRec(mouse, btnInfo);
    DrawBeveledButton(btnInfo,
                      infoHover ? Color{205, 145, 95, 255} : Color{188, 125, 80, 255},
                      Color{150, 95, 55, 255}, 0.22f);
    DrawInfoIcon(btnInfo.x + 20, btnInfo.y + 20, 24.0f, WHITE);

    // --- Tieu de "Settings" o giua ---
    const char* title = "Settings";
    int titleSize = 34;
    int tw = MeasureText(title, titleSize);
    DrawText(title, (int)(mx + (mw - tw) * 0.5f + 1), (int)(my + 19), titleSize, Color{255, 215, 190, 255});
    DrawText(title, (int)(mx + (mw - tw) * 0.5f), (int)(my + 18), titleSize, Color{175, 98, 48, 255});

    // --- Nut Close [X] o goc phai header ---
    Rectangle btnClose = { mx + mw - 56, my + 14, 40, 40 };
    bool closeHover = CheckCollisionPointRec(mouse, btnClose);
    DrawBeveledButton(btnClose,
                      closeHover ? Color{245, 75, 95, 255} : Color{232, 58, 78, 255},
                      Color{175, 30, 45, 255}, 0.22f);
    DrawCloseIcon(btnClose.x + 20, btnClose.y + 20, 20.0f, WHITE);

    // ROW 1: SFX TOGGLE
    float sfxY = my + 98;
    DrawText("SFX", (int)(mx + 50), (int)(sfxY + 12), 30, Color{175, 98, 48, 255});

    Rectangle sfxTrack = { mx + 165, sfxY, 245, 56 };
    // Nen track (mau kem nhat, viền am)
    DrawRectangleRounded(sfxTrack, 0.40f, 6, Color{248, 222, 204, 255});
    DrawRectangleRoundedLinesEx(sfxTrack, 0.40f, 6, 2.0f, Color{228, 192, 168, 255});

    float knobW = 95.0f, knobH = 50.0f;
    if (sfxEnabled) {
        // SFX BAT: Knob mau xanh ben trai (hien bieu tuong loa phat am)
        Rectangle knob = { sfxTrack.x + 3.0f, sfxTrack.y + 3.0f, knobW, knobH };
        DrawBeveledButton(knob, Color{88, 185, 58, 255}, Color{55, 140, 38, 255}, 0.35f);
        DrawSpeakerIcon(knob.x + knobW * 0.5f, knob.y + knobH * 0.5f, 32.0f, WHITE, false);

        // Bieu tuong mute ben phai track
        DrawSpeakerIcon(sfxTrack.x + sfxTrack.width - 48.0f, sfxTrack.y + sfxTrack.height * 0.5f,
                        28.0f, Color{195, 145, 110, 255}, true);
    } else {
        // SFX TAT: Knob mau xanh ben phai (hien bieu tuong mute X) giong trong anh
        Rectangle knob = { sfxTrack.x + sfxTrack.width - knobW - 3.0f, sfxTrack.y + 3.0f, knobW, knobH };
        DrawBeveledButton(knob, Color{88, 185, 58, 255}, Color{55, 140, 38, 255}, 0.35f);
        DrawSpeakerIcon(knob.x + knobW * 0.5f, knob.y + knobH * 0.5f, 32.0f, WHITE, true);

        // Bieu tuong loa ben trai track
        DrawSpeakerIcon(sfxTrack.x + 48.0f, sfxTrack.y + sfxTrack.height * 0.5f,
                        28.0f, Color{195, 145, 110, 255}, false);
    }

    // ROW 2: MUSIC TOGGLE
    float musY = my + 172;
    DrawText("Music", (int)(mx + 50), (int)(musY + 12), 30, Color{175, 98, 48, 255});

    Rectangle musTrack = { mx + 165, musY, 245, 56 };
    DrawRectangleRounded(musTrack, 0.40f, 6, Color{248, 222, 204, 255});
    DrawRectangleRoundedLinesEx(musTrack, 0.40f, 6, 2.0f, Color{228, 192, 168, 255});

    if (musicEnabled) {
        // MUSIC BAT: Knob mau xanh ben trai (hien am thanh phat) giong trong anh
        Rectangle knob = { musTrack.x + 3.0f, musTrack.y + 3.0f, knobW, knobH };
        DrawBeveledButton(knob, Color{88, 185, 58, 255}, Color{55, 140, 38, 255}, 0.35f);
        DrawSpeakerIcon(knob.x + knobW * 0.5f, knob.y + knobH * 0.5f, 32.0f, WHITE, false);

        // Bieu tuong mute mo ben phai
        DrawSpeakerIcon(musTrack.x + musTrack.width - 48.0f, musTrack.y + musTrack.height * 0.5f,
                        28.0f, Color{195, 145, 110, 255}, true);
    } else {
        // MUSIC TAT: Knob mau xanh ben phai (hien bieu tuong mute X)
        Rectangle knob = { musTrack.x + musTrack.width - knobW - 3.0f, musTrack.y + 3.0f, knobW, knobH };
        DrawBeveledButton(knob, Color{88, 185, 58, 255}, Color{55, 140, 38, 255}, 0.35f);
        DrawSpeakerIcon(knob.x + knobW * 0.5f, knob.y + knobH * 0.5f, 32.0f, WHITE, true);

        // Bieu tuong loa mo ben trai
        DrawSpeakerIcon(musTrack.x + 48.0f, musTrack.y + musTrack.height * 0.5f,
                        28.0f, Color{195, 145, 110, 255}, false);
    }

    // ROW 3: DO KHO (DIFFICULTY)
    float diffLabelY = my + 252;
    DrawText("Difficulty", (int)(mx + 50), (int)diffLabelY, 24, Color{175, 98, 48, 255});

    float diffY = my + 288;
    Rectangle optEasy   = { mx + 50, diffY, 114, 46 };
    Rectangle optNormal = { mx + 172, diffY, 114, 46 };
    Rectangle optHard   = { mx + 294, diffY, 116, 46 };

    // Easy tab
    if (difficulty == DIFF_EASY) {
        DrawBeveledButton(optEasy, Color{88, 185, 58, 255}, Color{55, 140, 38, 255}, 0.25f);
        int tw1 = MeasureText("Easy", 22);
        DrawText("Easy", (int)(optEasy.x + (optEasy.width - tw1) * 0.5f), (int)(optEasy.y + 11), 22, WHITE);
    } else {
        DrawRectangleRounded(optEasy, 0.25f, 4, Color{248, 224, 208, 255});
        DrawRectangleRoundedLinesEx(optEasy, 0.25f, 4, 1.5f, Color{225, 195, 172, 255});
        int tw1 = MeasureText("Easy", 20);
        DrawText("Easy", (int)(optEasy.x + (optEasy.width - tw1) * 0.5f), (int)(optEasy.y + 13), 20, Color{170, 110, 70, 255});
    }

    // Normal tab
    if (difficulty == DIFF_NORMAL) {
        DrawBeveledButton(optNormal, Color{88, 185, 58, 255}, Color{55, 140, 38, 255}, 0.25f);
        int tw2 = MeasureText("Normal", 22);
        DrawText("Normal", (int)(optNormal.x + (optNormal.width - tw2) * 0.5f), (int)(optNormal.y + 11), 22, WHITE);
    } else {
        DrawRectangleRounded(optNormal, 0.25f, 4, Color{248, 224, 208, 255});
        DrawRectangleRoundedLinesEx(optNormal, 0.25f, 4, 1.5f, Color{225, 195, 172, 255});
        int tw2 = MeasureText("Normal", 20);
        DrawText("Normal", (int)(optNormal.x + (optNormal.width - tw2) * 0.5f), (int)(optNormal.y + 13), 20, Color{170, 110, 70, 255});
    }

    // Hard tab
    if (difficulty == DIFF_HARD) {
        DrawBeveledButton(optHard, Color{88, 185, 58, 255}, Color{55, 140, 38, 255}, 0.25f);
        int tw3 = MeasureText("Hard", 22);
        DrawText("Hard", (int)(optHard.x + (optHard.width - tw3) * 0.5f), (int)(optHard.y + 11), 22, WHITE);
    } else {
        DrawRectangleRounded(optHard, 0.25f, 4, Color{248, 224, 208, 255});
        DrawRectangleRoundedLinesEx(optHard, 0.25f, 4, 1.5f, Color{225, 195, 172, 255});
        int tw3 = MeasureText("Hard", 20);
        DrawText("Hard", (int)(optHard.x + (optHard.width - tw3) * 0.5f), (int)(optHard.y + 13), 20, Color{170, 110, 70, 255});
    }

    // ROW 4: ACTION BUTTONS [ HOME ] & [ RESTART ]
    float actY = my + 418;

    // --- Nut HOME (Màu cam đất/đỏ cam) ---
    Rectangle btnHome = { mx + 50, actY, 155, 64 };
    bool homeHover = CheckCollisionPointRec(mouse, btnHome);
    DrawBeveledButton(btnHome,
                      homeHover ? Color{245, 105, 65, 255} : Color{232, 92, 54, 255},
                      Color{180, 55, 25, 255}, 0.22f);
    DrawHomeIcon(btnHome.x + btnHome.width * 0.5f, btnHome.y + btnHome.height * 0.48f, 36.0f, WHITE);

    // --- Nut RESTART (Màu vàng cam ấm) ---
    Rectangle btnRestart = { mx + 225, actY, 185, 64 };
    bool rstHover = CheckCollisionPointRec(mouse, btnRestart);
    DrawBeveledButton(btnRestart,
                      rstHover ? Color{255, 192, 50, 255} : Color{248, 178, 38, 255},
                      Color{198, 130, 18, 255}, 0.22f);
    const char* rstText = "Restart";
    int rstTw = MeasureText(rstText, 28);
    DrawText(rstText, (int)(btnRestart.x + (btnRestart.width - rstTw) * 0.5f),
             (int)(btnRestart.y + 17), 28, WHITE);

    // POPUP THONG TIN NHOM (khi an nut [i])
    if (showInfoModal) {
        Rectangle infoBox = { mx + 20, my + 80, mw - 40, mh - 100 };
        DrawRectangleRounded(infoBox, 0.08f, 8, Color{255, 250, 245, 255});
        DrawRectangleRoundedLinesEx(infoBox, 0.08f, 8, 2.5f, Color{210, 160, 120, 255});

        DrawText("TETRIS TEAM PROJECT", (int)(infoBox.x + 30), (int)(infoBox.y + 24), 22, Color{175, 98, 48, 255});
        DrawLineEx({infoBox.x + 30, infoBox.y + 54}, {infoBox.x + infoBox.width - 30, infoBox.y + 54}, 1.5f, Color{225, 190, 160, 255});

        int ly = (int)(infoBox.y + 68);
        int lstep = 30;
        DrawText("Member 1: GameLogic (Logic & Board)", (int)(infoBox.x + 30), ly + lstep * 0, 16, Color{60, 40, 30, 255});
        DrawText("Member 2: Renderer (UI/UX & Blocks)", (int)(infoBox.x + 30), ly + lstep * 1, 16, Color{60, 40, 30, 255});
        DrawText("Member 3: SoundManager (Music & SFX)", (int)(infoBox.x + 30), ly + lstep * 2, 16, Color{60, 40, 30, 255});
        DrawText("Member 4: GameSetting (Settings & Menu)", (int)(infoBox.x + 30), ly + lstep * 3, 16, Color{175, 98, 48, 255});
        DrawText("Member 5: GameManager (Game Control)", (int)(infoBox.x + 30), ly + lstep * 4, 16, Color{60, 40, 30, 255});

        Rectangle btnOk = { mx + mw * 0.5f - 60, my + 380, 120, 48 };
        bool okHover = CheckCollisionPointRec(mouse, btnOk);
        DrawBeveledButton(btnOk,
                          okHover ? Color{88, 185, 58, 255} : Color{75, 168, 50, 255},
                          Color{50, 125, 35, 255}, 0.25f);
        int okTw = MeasureText("OK", 22);
        DrawText("OK", (int)(btnOk.x + (btnOk.width - okTw) * 0.5f), (int)(btnOk.y + 12), 22, WHITE);
    }
}

// KEY BINDING CHECKS
const char* GameSetting::GetKeyName(int key) const {
    switch (key) {
        case KEY_LEFT:        return "LEFT";
        case KEY_RIGHT:       return "RIGHT";
        case KEY_UP:          return "UP";
        case KEY_DOWN:        return "DOWN";
        case KEY_SPACE:       return "SPACE";
        case KEY_ENTER:       return "ENTER";
        case KEY_LEFT_SHIFT:  return "SHIFT";
        case KEY_A:           return "A";
        case KEY_B:           return "B";
        case KEY_C:           return "C";
        case KEY_D:           return "D";
        case KEY_E:           return "E";
        case KEY_P:           return "P";
        case KEY_Q:           return "Q";
        case KEY_S:           return "S";
        case KEY_W:           return "W";
        case KEY_X:           return "X";
        case KEY_Z:           return "Z";
        default:              return "?";
    }
}

bool GameSetting::IsKeyMoveLeft() const {
    return IsKeyPressed(keyMoveLeft) || IsKeyPressed(keyMoveLeft2);
}

bool GameSetting::IsKeyMoveRight() const {
    return IsKeyPressed(keyMoveRight) || IsKeyPressed(keyMoveRight2);
}

bool GameSetting::IsKeySoftDrop() const {
    return IsKeyPressed(keySoftDrop) || IsKeyPressed(keySoftDrop2);
}

bool GameSetting::IsKeyHardDrop() const {
    return IsKeyPressed(keyHardDrop);
}

bool GameSetting::IsKeyRotateCW() const {
    return IsKeyPressed(keyRotateCW) || IsKeyPressed(keyRotateCW2) || IsKeyPressed(keyRotateCW3);
}

bool GameSetting::IsKeyRotateCCW() const {
    return IsKeyPressed(keyRotateCCW) || IsKeyPressed(keyRotateCCW2);
}

bool GameSetting::IsKeyHold() const {
    return IsKeyPressed(keyHold) || IsKeyPressed(keyHold2) || IsKeyPressed(keyHold3);
}

bool GameSetting::IsKeyPause() const {
    return IsKeyPressed(keyPause);
}

bool GameSetting::IsKeyRestart() const {
    return IsKeyPressed(keyRestart);
}

bool GameSetting::IsKeyMoveLeftDown() const {
    return IsKeyDown(keyMoveLeft) || IsKeyDown(keyMoveLeft2);
}

bool GameSetting::IsKeyMoveRightDown() const {
    return IsKeyDown(keyMoveRight) || IsKeyDown(keyMoveRight2);
}

bool GameSetting::IsKeySoftDropDown() const {
    return IsKeyDown(keySoftDrop) || IsKeyDown(keySoftDrop2);
}

void GameSetting::SaveToFile(const char* path) const {
    std::ofstream file(path);
    if (!file.is_open()) return;

    file << "# Tetris Game Settings\n";
    file << "musicEnabled=" << (musicEnabled ? 1 : 0) << "\n";
    file << "sfxEnabled=" << (sfxEnabled ? 1 : 0) << "\n";
    file << "difficulty=" << (int)difficulty << "\n";

    file << "\n# Audio Volumes\n";
    file << "musicVolume=" << musicVolume << "\n";
    file << "sfxVolume=" << sfxVolume << "\n";

    file << "\n# Key bindings\n";
    file << "keyMoveLeft=" << keyMoveLeft << "\n";
    file << "keyMoveLeft2=" << keyMoveLeft2 << "\n";
    file << "keyMoveRight=" << keyMoveRight << "\n";
    file << "keyMoveRight2=" << keyMoveRight2 << "\n";
    file << "keySoftDrop=" << keySoftDrop << "\n";
    file << "keySoftDrop2=" << keySoftDrop2 << "\n";
    file << "keyHardDrop=" << keyHardDrop << "\n";
    file << "keyRotateCW=" << keyRotateCW << "\n";
    file << "keyRotateCW2=" << keyRotateCW2 << "\n";
    file << "keyRotateCW3=" << keyRotateCW3 << "\n";
    file << "keyRotateCCW=" << keyRotateCCW << "\n";
    file << "keyRotateCCW2=" << keyRotateCCW2 << "\n";
    file << "keyHold=" << keyHold << "\n";
    file << "keyHold2=" << keyHold2 << "\n";
    file << "keyHold3=" << keyHold3 << "\n";
    file << "keyPause=" << keyPause << "\n";
    file << "keyRestart=" << keyRestart << "\n";

    file.close();
}

void GameSetting::LoadFromFile(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;

        std::string key = line.substr(0, eq);
        std::string val = line.substr(eq + 1);

        if (key == "musicEnabled")      musicEnabled = (std::stoi(val) != 0);
        else if (key == "sfxEnabled")   sfxEnabled   = (std::stoi(val) != 0);
        else if (key == "difficulty")   SetDifficulty((Difficulty)std::stoi(val));
        else if (key == "musicVolume")  musicVolume  = std::stof(val);
        else if (key == "sfxVolume")    sfxVolume    = std::stof(val);
        else if (key == "keyMoveLeft")  keyMoveLeft  = std::stoi(val);
        else if (key == "keyMoveLeft2") keyMoveLeft2 = std::stoi(val);
        else if (key == "keyMoveRight") keyMoveRight = std::stoi(val);
        else if (key == "keyMoveRight2")keyMoveRight2= std::stoi(val);
        else if (key == "keySoftDrop")  keySoftDrop  = std::stoi(val);
        else if (key == "keySoftDrop2") keySoftDrop2 = std::stoi(val);
        else if (key == "keyHardDrop")  keyHardDrop  = std::stoi(val);
        else if (key == "keyRotateCW")  keyRotateCW  = std::stoi(val);
        else if (key == "keyRotateCW2") keyRotateCW2 = std::stoi(val);
        else if (key == "keyRotateCW3") keyRotateCW3 = std::stoi(val);
        else if (key == "keyRotateCCW") keyRotateCCW = std::stoi(val);
        else if (key == "keyRotateCCW2")keyRotateCCW2= std::stoi(val);
        else if (key == "keyHold")      keyHold      = std::stoi(val);
        else if (key == "keyHold2")     keyHold2     = std::stoi(val);
        else if (key == "keyHold3")     keyHold3     = std::stoi(val);
        else if (key == "keyPause")     keyPause     = std::stoi(val);
        else if (key == "keyRestart")   keyRestart   = std::stoi(val);
    }
    file.close();
}