#pragma once

#include "Constants.h"

// Forward declarations (tranh include vong tron)
class GameLogic;
class GameSetting;

class Renderer {
public:
    // Load tat ca 7 texture khoi tu thu muc assets (dung lam fallback)
    void LoadTextures();

    // Giai phong texture
    void UnloadTextures();

    // Ve nen va luoi board (duong ke mo)
    void DrawBoardBackground();

    // Ve cac khoi da khoa tren board (bao gom animation xoa hang)
    void DrawLockedBlocks(const GameLogic& logic);

    // Ve khoi dang roi (current piece)
    void DrawCurrentPiece(const ActivePiece& piece);

    // Ve ghost piece (bong mo xem truoc vi tri roi)
    void DrawGhostPiece(const ActivePiece& ghost, PieceType type);

    // Ve panel Hold (ben trai board)
    void DrawHoldPanel(int holdType, bool canHold);

    // Ve panel Next (ben phai board) — hien thi 3 khoi tiep theo
    void DrawNextPanel(const std::vector<PieceType>& nextQueue);

    // Ve bang diem (Score / Level / Lines)
    void DrawScorePanel(int score, int level, int lines);

    // Ve huong dan phim dieu khien
    void DrawControlsPanel();

    // Ve overlay tam dung
    void DrawPauseOverlay();

    // Ve overlay game over
    void DrawGameOverOverlay();

private:
    Texture2D blockTextures[PIECE_COUNT];   // 7 texture khoi (fallback)

    static Color GetPieceColor(PieceType type);
    static Color Lighten(Color c, int amount);
    static Color Darken(Color c, int amount);

    void DrawGlossyBlock(int px, int py, int size, Color baseColor, unsigned char alpha = 255);
    void DrawGlossyMiniBlock(int px, int py, float size, Color baseColor, unsigned char alpha = 255);

    void DrawMiniPiece(PieceType type, int px, int py, unsigned char alpha, float sizeMul);

    void DrawPanelFrame(int x, int y, int w, int h, const char* title);
};
