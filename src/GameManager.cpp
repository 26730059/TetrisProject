#include "GameManager.h"

void GameManager::Init() {
    InitWindow(SCREEN_W, SCREEN_H, "Tetris - raylib");
    SetTargetFPS(60);

    renderer.LoadTextures();

    ResetGame();
}

void GameManager::ResetGame() {
    logic.Reset();
}

void GameManager::Run() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        Render();
        EndDrawing();
    }
}

void GameManager::Render() {
    renderer.DrawBoardBackground();
    renderer.DrawLockedBlocks(logic);

    if (!logic.IsGameOver() && !logic.IsClearAnimRunning()) {
        ActivePiece ghost = logic.GetGhostPiece();
        renderer.DrawGhostPiece(ghost, logic.GetCurrentPiece().type);
        renderer.DrawCurrentPiece(logic.GetCurrentPiece());
    }

    renderer.DrawHoldPanel(logic.GetHoldType(), logic.CanHold());
    renderer.DrawScorePanel(logic.GetScore(), logic.GetLevel(), logic.GetLines());
    renderer.DrawNextPanel(logic.GetNextQueue());
    renderer.DrawControlsPanel();
}

void GameManager::Cleanup() {
    renderer.UnloadTextures();
    CloseWindow();
}