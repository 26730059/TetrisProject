#pragma once

#include "Constants.h"

// Forward declaration
class SoundManager;

// 3 muc do kho cua game
enum Difficulty {
    DIFF_EASY = 0,
    DIFF_NORMAL,
    DIFF_HARD,
    DIFF_COUNT
};

class GameSetting {
public:
    GameSetting();                             // Gan gia tri mac dinh

    void LoadFromFile(const char* path);       // Doc config tu file
    void SaveToFile(const char* path) const;   // Luu config ra file

    void OpenModal();                          // Mo modal cai dat
    void CloseModal();                         // Dong modal cai dat
    void ToggleModal();                        // Bat / tat modal cai dat
    bool IsModalOpen() const { return isModalOpen; }

    // Kiem tra click nut Gear ⚙️ o goc tren man hinh
    bool CheckSettingsButtonClicked();

    // Xu ly input cua modal (hover, click toggles, buttons)
    void UpdateModal(SoundManager& sound, bool& outRestartGame);

    // Ve modal menu Settings theo dung design tham khao
    void DrawModal();

    // Ve nut Gear ⚙️ o goc tren man hinh de nguoi choi click mo Settings
    void DrawSettingsButton();

    bool IsMusicEnabled() const { return musicEnabled; }
    bool IsSfxEnabled()   const { return sfxEnabled; }
    void ToggleMusic(SoundManager& sound);
    void ToggleSfx(SoundManager& sound);

    Difficulty GetDifficulty() const { return difficulty; }
    void SetDifficulty(Difficulty d);
    const char* GetDifficultyName() const;

    // Phim di chuyen
    int keyMoveLeft    = KEY_LEFT;
    int keyMoveLeft2   = KEY_A;               // phim phu
    int keyMoveRight   = KEY_RIGHT;
    int keyMoveRight2  = KEY_D;               // phim phu
    int keySoftDrop    = KEY_DOWN;
    int keySoftDrop2   = KEY_S;               // phim phu

    // Phim hanh dong
    int keyHardDrop    = KEY_SPACE;
    int keyRotateCW    = KEY_UP;              // xoay theo chieu kim dong ho
    int keyRotateCW2   = KEY_X;
    int keyRotateCW3   = KEY_E;
    int keyRotateCCW   = KEY_Z;              // xoay nguoc chieu kim dong ho
    int keyRotateCCW2  = KEY_Q;
    int keyHold        = KEY_C;
    int keyHold2       = KEY_LEFT_SHIFT;
    int keyHold3       = KEY_W;
    int keyPause       = KEY_P;
    int keyRestart     = KEY_ENTER;

    float dasDelay      = 0.16f;              // Delayed Auto Shift (giay)
    float arrSpeed      = 0.04f;              // Auto Repeat Rate (giay)
    float lockDelay     = 0.5f;               // Thoi gian khoa khoi (giay)
    float softDropSpeed = 0.04f;              // Toc do soft drop (giay)
    float baseGravity   = 0.8f;               // Gravity co ban level 1
    float gravityStep   = 0.06f;              // Giam gravity moi level
    float minGravity    = 0.08f;              // Gravity toi thieu

    float musicVolume   = 0.5f;               // Am luong nhac nen (0.0 - 1.0)
    float sfxVolume     = 1.0f;               // Am luong hieu ung (0.0 - 1.0)

    int scoreTable[5] = { 0, 100, 300, 500, 800 }; // Used by Game Manager
    int softDropPoints  = 1;                  // Diem moi o soft drop
    int hardDropPoints  = 2;                  // Diem moi o hard drop
    int linesPerLevel   = 10;                 // So hang de len level

    float GetGravityInterval(int level) const;
    const char* GetKeyName(int key) const;

    bool IsKeyMoveLeft() const;
    bool IsKeyMoveRight() const;
    bool IsKeySoftDrop() const;
    bool IsKeyHardDrop() const;
    bool IsKeyRotateCW() const;
    bool IsKeyRotateCCW() const;
    bool IsKeyHold() const;
    bool IsKeyPause() const;
    bool IsKeyRestart() const;

    bool IsKeyMoveLeftDown() const;
    bool IsKeyMoveRightDown() const;
    bool IsKeySoftDropDown() const;

private:
    bool isModalOpen   = false;               // Modal menu dang mo hay khong
    bool showInfoModal = false;               // Modal thong tin nhom [i] dang mo
    bool musicEnabled  = true;                // Nhac nen bat/tat
    bool sfxEnabled    = true;                // Am thanh hieu ung bat/tat
    Difficulty difficulty = DIFF_NORMAL;      // Do kho mac dinh la Normal

    // Toa do va kich thuoc modal
    Rectangle modalBounds   = { 0, 0, 450, 480 };
    Rectangle gearBtnBounds = { 0, 0, 48, 48 };

    // Ve cac icon procedurally (khong can anh roi)
    static void DrawSpeakerIcon(float cx, float cy, float size, Color col, bool muted);
    static void DrawHomeIcon(float cx, float cy, float size, Color col);
    static void DrawGearIcon(float cx, float cy, float size, Color col);
    static void DrawCloseIcon(float cx, float cy, float size, Color col);
    static void DrawInfoIcon(float cx, float cy, float size, Color col);

    // Helper ve button 3D noi dep mat
    static void DrawBeveledButton(Rectangle rect, Color topColor, Color btmColor, float roundness = 0.2f);
};
