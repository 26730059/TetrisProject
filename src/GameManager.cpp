#include "GameManager.h"

void GameManager::Init() {
    InitWindow(SCREEN_W, SCREEN_H, "Tetris - raylib");
    SetTargetFPS(60);

    // Load cai dat tu file (neu co)
    settings.LoadFromFile("settings.cfg");

    // Khoi tao cac thanh phan con
    sound.Init();
    sound.SetMusicVolume(settings.IsMusicEnabled() ? settings.musicVolume : 0.0f);
    sound.SetSfxVolume(settings.IsSfxEnabled() ? settings.sfxVolume : 0.0f);

    renderer.LoadTextures();

    // Khoi tao game logic
    ResetGame();
}

void GameManager::ResetGame() {
    logic.Reset();
    dasTimer = 0.0f;
    arrTimer = 0.0f;
    dasDir = 0;
}

// GAME LOOP CHINH
void GameManager::Run() {
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        sound.UpdateMusic();
        Update(dt);

        BeginDrawing();
        Render();
        EndDrawing();
    }
}

void GameManager::Update(float dt) {
    // 1. Kiem tra mo/dong Settings Modal tu nut Gear ⚙️ hoac phim ESC / O
    if (settings.CheckSettingsButtonClicked() || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_O)) {
        settings.ToggleModal();
    }

    // Neu Modal Settings dang mo: xu ly input modal va tam dung gameplay
    if (settings.IsModalOpen()) {
        bool restartReq = false;
        settings.UpdateModal(sound, restartReq);
        if (restartReq) {
            ResetGame();
        }
        return;
    }

    // Game over: cho phim restart
    if (logic.IsGameOver()) {
        if (settings.IsKeyRestart()) {
            ResetGame();
        }
        return;
    }

    // Pause
    if (settings.IsKeyPause()) {
        logic.TogglePause();
    }
    if (logic.IsPaused()) return;

    // Xu ly input tu nguoi choi
    HandleInput(dt);

    // Tinh gravity theo do kho / level / soft drop
    float interval = logic.IsSoftDrop()
        ? settings.softDropSpeed
        : settings.GetGravityInterval(logic.GetLevel());

    // Update toan bo Core Game Logic
    logic.Update(dt,
        interval,
        settings.lockDelay,
        settings.linesPerLevel,
        settings.scoreTable,
        settings.softDropPoints);

    // Xu ly cac su kien game phat ra tu GameLogic de phat am thanh
    for (const auto& ev : logic.GetEvents()) {
        switch (ev.type) {
            case LogicEvent::Rotate:
                sound.PlayRotate();
                break;
            case LogicEvent::HardDrop:
                sound.PlayDrop();
                break;
            case LogicEvent::Hold:
                sound.PlayHold();
                break;
            case LogicEvent::LineClear:
                sound.PlayLineClear(ev.data);

                if (ev.data >= 4) {
                    sound.PlayDrop();
                if (ev.data > 0) {
                    sound.PlayLineClear(ev.data);
                }
                break;

            case LogicEvent::GameOver:
                sound.PlayGameOver();
                break;
            default:
                break;
        }
    }
    logic.ClearEvents();
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

    // Di chuyen ngang: DAS/ARR
    int dir = 0;
    if (settings.IsKeyMoveLeftDown()) dir = -1;
    else if (settings.IsKeyMoveRightDown()) dir = 1;

    if (dir != dasDir) {
        dasDir = dir;
        dasTimer = 0.0f;
        if (dir != 0) logic.Move(dir, 0);
    }
    else if (dir != 0) {
        dasTimer += dt;
        if (dasTimer >= settings.dasDelay) {
            arrTimer += dt;
            if (arrTimer >= settings.arrSpeed) {
                arrTimer = 0.0f;
                logic.Move(dir, 0);
            }
        }
    }

    // Soft drop
    logic.SetSoftDrop(settings.IsKeySoftDropDown());
}



void GameManager::Render() {
    // Ve nen + luoi
    renderer.DrawBoardBackground();

    // Ve cac khoi da khoa
    renderer.DrawLockedBlocks(logic);

    // Ve ghost + khoi dang roi (chi khi chua game over va khong dang clear)
    if (!logic.IsGameOver() && !logic.IsClearAnimRunning()) {
        ActivePiece ghost = logic.GetGhostPiece();
        renderer.DrawGhostPiece(ghost, logic.GetCurrentPiece().type);
        renderer.DrawCurrentPiece(logic.GetCurrentPiece());
    }

    // Ve cac panel
    renderer.DrawHoldPanel(logic.GetHoldType(), logic.CanHold());
    renderer.DrawScorePanel(logic.GetScore(), logic.GetLevel(), logic.GetLines());
    renderer.DrawNextPanel(logic.GetNextQueue());
    renderer.DrawControlsPanel();

    // Ve nut Gear (Settings) o goc tren phai
    settings.DrawSettingsButton();

    // Overlay pause / game over (chi hien khi khong mo modal)
    if (logic.IsPaused() && !settings.IsModalOpen()) renderer.DrawPauseOverlay();
    if (logic.IsGameOver() && !settings.IsModalOpen()) renderer.DrawGameOverOverlay();

    // Neu Modal Settings dang mo: ve modal de len tren toan bo
    if (settings.IsModalOpen()) {
        settings.DrawModal();
    }
}

void GameManager::Cleanup() {
    // Luu cai dat truoc khi thoat
    settings.SaveToFile("settings.cfg");

    renderer.UnloadTextures();
    sound.Cleanup();
    CloseWindow();
}