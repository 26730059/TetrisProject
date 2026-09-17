#include "GameManager.h"

void GameManager::Init() {
    InitWindow(SCREEN_W, SCREEN_H, "Tetris - raylib");
    SetTargetFPS(60);
}

void GameManager::ResetGame() {}

void GameManager::Run() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        EndDrawing();
    }
}

void GameManager::Cleanup() {
    CloseWindow();
}