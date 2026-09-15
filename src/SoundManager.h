#pragma once

#include "Constants.h"

class SoundManager {
public:
    // Khoi tao audio device va load tat ca sound & music
    void Init();

    // Giai phong tat ca sound & music, dong audio device
    void Cleanup();

private:
    // Am thanh hieu ung
    Sound sfxRotate{};
    Sound sfxDrop{};
    Sound sfxHold{};
    Sound sfxClear{};        // xoa 1-3 hang
    Sound sfxClear4{};       // xoa 4 hang (Tetris!)
    Sound sfxGameOver{};
};
