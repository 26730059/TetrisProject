#include "SoundManager.h"

void SoundManager::Init() {
    InitAudioDevice();

    // Load am thanh hieu ung
    sfxRotate   = LoadSound("assets/sound/rotation.mp3");
    sfxDrop     = LoadSound("assets/sound/drop.mp3");
    sfxHold     = LoadSound("assets/sound/hold.mp3");
    sfxClear    = LoadSound("assets/sound/line_clear.mp3");
    sfxClear4   = LoadSound("assets/sound/tetris_clear.mp3");
    sfxGameOver = LoadSound("assets/sound/game_over.mp3");
}

void SoundManager::Cleanup() {
    UnloadSound(sfxRotate);
    UnloadSound(sfxDrop);
    UnloadSound(sfxHold);
    UnloadSound(sfxClear);
    UnloadSound(sfxClear4);
    UnloadSound(sfxGameOver);

    CloseAudioDevice();
}
