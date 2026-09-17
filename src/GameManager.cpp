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
}

void GameManager::Cleanup() {
    renderer.UnloadTextures();
    CloseWindow();
}