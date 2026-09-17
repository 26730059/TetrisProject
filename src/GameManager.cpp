#include "GameManager.h"

void GameManager::Init() {
    InitWindow(SCREEN_W, SCREEN_H, "Tetris - raylib");
    SetTargetFPS(60);

    // Load cai dat tu file (neu co)
    settings.LoadFromFile("settings.cfg");

    renderer.LoadTextures();

    ResetGame();
}

void GameManager::ResetGame() {
    logic.Reset();
}

void GameManager::Run() {
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Update(dt);

        BeginDrawing();
        Render();
        EndDrawing();
    }
}

void GameManager::Update(float dt) {
    HandleInput(dt);
}

void GameManager::HandleInput(float dt) {
    // Xoay
    if (settings.IsKeyRotateCW()) {
        logic.RotateCW();
    }
    if (settings.IsKeyRotateCCW()) {
        logic.RotateCCW();
    }

    // Hold
    if (settings.IsKeyHold()) {
        logic.Hold();
    }

    // Hard drop
    if (settings.IsKeyHardDrop()) {
        logic.HardDrop(settings.hardDropPoints);
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
    // Luu cai dat truoc khi thoat
    settings.SaveToFile("settings.cfg");

    renderer.UnloadTextures();
    CloseWindow();
}