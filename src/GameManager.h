#pragma once

#include "GameLogic.h"
#include "Renderer.h"
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
    // Reset game choi lai tu dau
    void ResetGame();

    // Goi Renderer ve toan bo giao dien
    void Render();

    GameLogic logic;       // Nguoi 1: Core Game Logic
    Renderer     renderer;    // Nguoi 2: UI/UX
    GameSetting  settings;    // Nguoi 4: Game Setting
};
