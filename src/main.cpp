#include "Constants.h"
#include "raylib.h"

// =====================================================================
//  main.cpp — Diem vao chuong trinh (Skeleton Version)
//  
//  Khi Nguoi 5 hoan thanh GameManager, cap nhat main.cpp nhu sau:
//
//  #include "GameManager.h"
//  int main() {
//      GameManager game;
//      game.Init();
//      game.Run();
//      game.Cleanup();
//      return 0;
//  }
// =====================================================================

int main() {
    InitWindow(SCREEN_W, SCREEN_H, "Tetris Game - Skeleton Version");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(COL_BACKGROUND);

        // Tieu de
        DrawText("TETRIS GAME - SKELETON VERSION", 350, 180, 38, COL_ACCENT);
        DrawText("Khung du an san sang cho 5 thanh vien tu tao class & push code:", 350, 240, 20, COL_CREAM);

        // Danh sach 5 thanh vien & nhiem vu
        DrawText("1. Nguoi 1: GameLogic     (src/GameLogic.h, .cpp)    - Core Rules, Board, Piece", 370, 300, 19, RAYWHITE);
        DrawText("2. Nguoi 2: Renderer      (src/Renderer.h, .cpp)     - 3D Blocks, Board, UI Panels", 370, 340, 19, RAYWHITE);
        DrawText("3. Nguoi 3: SoundManager  (src/SoundManager.h, .cpp) - Sound Effects, BGM", 370, 380, 19, RAYWHITE);
        DrawText("4. Nguoi 4: GameSetting   (src/GameSetting.h, .cpp)  - Modal Settings, Keys, Audio", 370, 420, 19, RAYWHITE);
        DrawText("5. Nguoi 5: GameManager   (src/GameManager.h, .cpp)  - Game Loop & Integration", 370, 460, 19, RAYWHITE);

        // Huong dan
        DrawText("Chi tiet cach tao class & phuong thuc xem tai: FEATURE_ASSIGNMENT.md", 350, 540, 18, LIGHTGRAY);
        DrawText("Nhan ESC hoac tat cua so de thoat.", 350, 580, 16, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
