#include "SoundManager.h"

void SoundManager::Init() {
    InitAudioDevice();
}

void SoundManager::Cleanup() {
    CloseAudioDevice();
}
