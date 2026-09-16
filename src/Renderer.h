#pragma once

#include "Constants.h"
#include "GameLogic.h"
#include <vector>

class Renderer {
public:
    void LoadTextures();
    void UnloadTextures();

    void DrawBoardBackground();
    void DrawLockedBlocks(const GameLogic& logic);
    void DrawCurrentPiece(const ActivePiece& piece);
    void DrawGhostPiece(const ActivePiece& ghost, PieceType type);

    void DrawHoldPanel(int holdType, bool canHold);
    void DrawNextPanel(const std::vector<PieceType>& nextQueue);
    void DrawScorePanel(int score, int level, int lines);
    void DrawControlsPanel();

    void DrawPauseOverlay();
    void DrawGameOverOverlay();

private:
    Texture2D blockTextures[PIECE_COUNT];

    static Color GetPieceColor(PieceType type);
    static Color Lighten(Color c, int amount);
    static Color Darken(Color c, int amount);

    void DrawGlossyBlock(int px, int py, int size, Color baseColor, unsigned char alpha = 255);
    void DrawGlossyMiniBlock(int px, int py, float size, Color baseColor, unsigned char alpha = 255);
    void DrawMiniPiece(PieceType type, int px, int py, unsigned char alpha, float sizeMul);

    void DrawTempleBackground();
    void DrawBoardTempleFrame();
    void DrawWoodPanel(Rectangle bounds, const char* title);
    void DrawCornerOrnament(float x, float y, bool flipX, bool flipY);
    void DrawLotusMotif(float cx, float cy, float scale, Color color);
};