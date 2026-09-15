#pragma once

#include "Constants.h"

class SoundManager {
public:
    // Khoi tao audio device va load tat ca sound & music
    void Init();

    // Giai phong tat ca sound & music, dong audio device
    void Cleanup();

    // Phat am thanh theo su kien game
    void PlayRotate();                        // Khi xoay khoi
    void PlayDrop();                          // Khi hard drop

private:
    // Am thanh hieu ung
    Sound sfxRotate{};
    Sound sfxDrop{};
    Sound sfxHold{};
    Sound sfxClear{};        // xoa 1-3 hang
    Sound sfxClear4{};       // xoa 4 hang (Tetris!)
    Sound sfxGameOver{};

    // Nhac nen
    Music bgm{};
    bool bgmLoaded = false;

    // Phat sound an toan (chi phat neu sound da load thanh cong)
    void PlaySoundSafe(Sound s);
};
