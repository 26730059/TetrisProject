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

    // Load nhac nen
    bgm = LoadMusicStream("assets/music/the_mountain-joy-444134.mp3");
    bgmLoaded = true;
    PlayMusicStream(bgm);
    ::SetMusicVolume(bgm, currentMusicVol);
}

void SoundManager::Cleanup() {
    UnloadSound(sfxRotate);
    UnloadSound(sfxDrop);
    UnloadSound(sfxHold);
    UnloadSound(sfxClear);
    UnloadSound(sfxClear4);
    UnloadSound(sfxGameOver);

    if (bgmLoaded) {
        UnloadMusicStream(bgm);
        bgmLoaded = false;
    }

    CloseAudioDevice();
}

void SoundManager::PlaySoundSafe(Sound s) {
    if (s.frameCount > 0) PlaySound(s);
}

void SoundManager::PlayRotate() {
    PlaySoundSafe(sfxRotate);
}

void SoundManager::PlayDrop() {
    PlaySoundSafe(sfxDrop);
}

void SoundManager::PlayHold() {
    PlaySoundSafe(sfxHold);
}

void SoundManager::PlayLineClear(int lineCount) {
    if (lineCount >= 4) {
        PlaySoundSafe(sfxClear4);
    } else {
        PlaySoundSafe(sfxClear);
    }
}

void SoundManager::PlayGameOver() {
    PlaySoundSafe(sfxGameOver);
}

void SoundManager::UpdateMusic() {
    if (bgmLoaded) UpdateMusicStream(bgm);
}

void SoundManager::SetMusicVolume(float vol) {
    currentMusicVol = vol;
    if (bgmLoaded) ::SetMusicVolume(bgm, vol);
}

void SoundManager::SetSfxVolume(float vol) {
    currentSfxVol = vol;
    // Raylib khong co SetSoundVolume global, nhung co the set cho tung sound
    SetSoundVolume(sfxRotate, vol);
    SetSoundVolume(sfxDrop, vol);
    SetSoundVolume(sfxHold, vol);
    SetSoundVolume(sfxClear, vol);
    SetSoundVolume(sfxClear4, vol);
    SetSoundVolume(sfxGameOver, vol);
}

void SoundManager::PauseMusic() {
    if (bgmLoaded) PauseMusicStream(bgm);
}

void SoundManager::ResumeMusic() {
    if (bgmLoaded) ResumeMusicStream(bgm);
}
