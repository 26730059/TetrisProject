#pragma once

#include "GameLogic.h"
#include "Renderer.h"
#include "SoundManager.h"
#include "GameSetting.h"

class GameManager {
public:
    // Khoi tao window, load tai nguyen, setup game
    void Init();

    // Vong lap game chinh (chay den khi dong cua so)
    void Run();

    // Giai phong tat ca tai nguyen
    void Cleanup();

private:
    // Xu ly input (DAS/ARR, phim dieu khien)
    void HandleInput(float dt);

    // Update logic moi frame
    void Update(float dt);

    // Goi Renderer ve toan bo giao dien
    void Render();

    // Reset game choi lai tu dau
    void ResetGame();

    GameLogic    logic;       // Nguoi 1: Core Game Logic
    Renderer     renderer;    // Nguoi 2: UI/UX
    SoundManager sound;       // Nguoi 3: Sound Effect
    GameSetting  settings;    // Nguoi 4: Game Setting

    float dasTimer = 0.0f;
    float arrTimer = 0.0f;
    int   dasDir = 0;
};
