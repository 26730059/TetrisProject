#pragma once

#include "GameLogic.h"

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

    GameLogic logic;       // Nguoi 1: Core Game Logic
};
