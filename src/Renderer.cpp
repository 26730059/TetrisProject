#include "Renderer.h"
#include "GameLogic.h"
#include "GameSetting.h"
#include <cmath>
#include <algorithm>

Color Renderer::GetPieceColor(PieceType type) {
    switch (type) {
        case I_PIECE: return Color{   0, 168, 245, 255 };  // Cyan / Sky Blue
        case O_PIECE: return Color{ 255, 220,  15, 255 };  // Golden Yellow
        case T_PIECE: return Color{  18, 185,  85, 255 };  // Emerald Green (giong khoi T trong hinh)
        case S_PIECE: return Color{ 215,  35, 145, 255 };  // Magenta / Purple-Pink
        case Z_PIECE: return Color{ 235,  45,  45, 255 };  // Bright Red
        case J_PIECE: return Color{  45,  65, 185, 255 };  // Dark Blue / Cobalt
        case L_PIECE: return Color{ 250, 115,  25, 255 };  // Warm Orange
        default:      return Color{ 160, 160, 165, 255 };  // Gray
    }
}

Color Renderer::Lighten(Color c, int amount) {
    return Color{
        (unsigned char)std::min(255, c.r + amount),
        (unsigned char)std::min(255, c.g + amount),
        (unsigned char)std::min(255, c.b + amount),
        c.a
    };
}

Color Renderer::Darken(Color c, int amount) {
    return Color{
        (unsigned char)std::max(0, c.r - amount),
        (unsigned char)std::max(0, c.g - amount),
        (unsigned char)std::max(0, c.b - amount),
        c.a
    };
}

void Renderer::LoadTextures() {
    blockTextures[I_PIECE] = LoadTexture("assets/blocks/I.png");
    blockTextures[O_PIECE] = LoadTexture("assets/blocks/O.png");
    blockTextures[T_PIECE] = LoadTexture("assets/blocks/T.png");
    blockTextures[S_PIECE] = LoadTexture("assets/blocks/S.png");
    blockTextures[Z_PIECE] = LoadTexture("assets/blocks/Z.png");
    blockTextures[J_PIECE] = LoadTexture("assets/blocks/J.png");
    blockTextures[L_PIECE] = LoadTexture("assets/blocks/L.png");
    for (auto& tex : blockTextures)
        SetTextureFilter(tex, TEXTURE_FILTER_BILINEAR);
}

void Renderer::UnloadTextures() {
    for (auto& tex : blockTextures)
        UnloadTexture(tex);
}

void Renderer::DrawGlossyBlock(int px, int py, int size, Color baseColor, unsigned char alpha) {
    float a = alpha / 255.0f;
    float gap = 1.0f;  // Tao khe ho 1px tren moi canh de cac khoi cach nhau nhu trong anh mau

    float bx = (float)px + gap;
    float by = (float)py + gap;
    float bs = (float)size - gap * 2.0f;
    float roundness = 0.14f;  // Bo goc nhe, khong tron qua muc nhu candy

    // 1. Lop nen toi phia duoi/phai lam shadow goc
    Color darkBevel = Fade(Darken(baseColor, 65), a);
    DrawRectangleRounded(Rectangle{bx, by, bs, bs}, roundness, 4, darkBevel);

    // 2. Lop vat sang phia tren/trai (Light bevel)
    Color lightBevel = Fade(Lighten(baseColor, 55), a);
    DrawRectangleRounded(Rectangle{bx, by, bs - 1.2f, bs - 1.2f}, roundness, 4, lightBevel);

    // 3. Lop vat toi phia duoi/phai
    DrawRectangleRounded(Rectangle{bx + 1.2f, by + 1.2f, bs - 1.2f, bs - 1.2f}, roundness, 4, darkBevel);

    // 4. Mat chinh o giua (inset 2px)
    float inset = 2.2f;
    Rectangle innerRect = {bx + inset, by + inset, bs - inset * 2.0f, bs - inset * 2.0f};
    DrawRectangleRounded(innerRect, 0.12f, 4, Fade(baseColor, a));

    // 5. Gradient em tu tren xuong duoi tren mat chinh (tao hieu ung khoi vat 3D)
    Color topFace = Fade(Lighten(baseColor, 24), a);
    Color btmFace = Fade(Darken(baseColor, 20), a);
    DrawRectangleGradientV((int)innerRect.x, (int)innerRect.y,
                           (int)innerRect.width, (int)innerRect.height,
                           topFace, btmFace);

    // 6. Vien sang nhe o mep tren cung ben trong
    DrawRectangleRounded(
        Rectangle{innerRect.x + 1.5f, innerRect.y + 0.8f, innerRect.width - 3.0f, 1.8f},
        0.5f, 2, Fade(Lighten(baseColor, 75), 0.65f * a));

    // 7. Vien toi nhe o mep duoi cung ben trong
    DrawRectangleRounded(
        Rectangle{innerRect.x + 1.5f, innerRect.y + innerRect.height - 2.6f, innerRect.width - 3.0f, 1.8f},
        0.5f, 2, Fade(Darken(baseColor, 55), 0.55f * a));
}

void Renderer::DrawGlossyMiniBlock(int px, int py, float size, Color baseColor, unsigned char alpha) {
    float a = alpha / 255.0f;
    float gap = 1.0f;
    float bx = (float)px + gap;
    float by = (float)py + gap;
    float bs = size - gap * 2.0f;

    Color darkBevel = Fade(Darken(baseColor, 60), a);
    Color lightBevel = Fade(Lighten(baseColor, 50), a);

    DrawRectangleRounded(Rectangle{bx, by, bs, bs}, 0.16f, 4, darkBevel);
    DrawRectangleRounded(Rectangle{bx, by, bs - 1.0f, bs - 1.0f}, 0.16f, 4, lightBevel);
    DrawRectangleRounded(Rectangle{bx + 1.0f, by + 1.0f, bs - 1.0f, bs - 1.0f}, 0.16f, 4, darkBevel);

    float inset = 1.5f;
    Rectangle innerRect = {bx + inset, by + inset, bs - inset * 2.0f, bs - inset * 2.0f};
    DrawRectangleRounded(innerRect, 0.12f, 4, Fade(baseColor, a));
    DrawRectangleGradientV((int)innerRect.x, (int)innerRect.y,
                           (int)innerRect.width, (int)innerRect.height,
                           Fade(Lighten(baseColor, 20), a),
                           Fade(Darken(baseColor, 18), a));
}

void Renderer::DrawPanelFrame(int x, int y, int w, int h, const char* title) {
    // Nen panel — dark charcoal
    DrawRectangleRounded(
        {(float)x, (float)y, (float)w, (float)h},
        0.08f, 6, Color{26, 26, 30, 240});

    // Vien ngoai panel
    DrawRectangleRoundedLinesEx(
        {(float)x, (float)y, (float)w, (float)h},
        0.08f, 6, 2.0f, Color{52, 52, 58, 255});

    // Vien trong mo nhe
    DrawRectangleRoundedLinesEx(
        {(float)(x + 2), (float)(y + 2), (float)(w - 4), (float)(h - 4)},
        0.08f, 6, 1.0f, Color{40, 40, 46, 120});

    if (title && title[0] != '\0') {
        int fontSize = 22;
        int tw = MeasureText(title, fontSize);
        // Bong chu
        DrawText(title, x + (w - tw) / 2 + 1, y + 15, fontSize, Color{0, 0, 0, 160});
        // Chu chinh — mau vang/kem sang
        DrawText(title, x + (w - tw) / 2, y + 14, fontSize, Color{245, 220, 130, 255});
    }
}

void Renderer::DrawBoardBackground() {
    // 1. Ve nen Chanh Dien (Gradient Nau Do)
    DrawRectangleGradientV(0, 0, SCREEN_W, SCREEN_H, Color{33, 16, 13, 255}, Color{22, 12, 10, 255});

    // 2. Ve Xa ngang go tren cung (Chua co mai ngoi)
    DrawRectangle(0, 0, SCREEN_W, 55, Color{74, 23, 18, 255});
    DrawRectangle(0, 53, SCREEN_W, 2, Color{201, 155, 69, 255});
    DrawRectangle(0, 0, SCREEN_W, 6, Color{40, 10, 8, 255});

    // 3. Ve hai Cot do son mai o hai ben man hinh
    DrawRectangle(45, 55, 45, SCREEN_H - 55, Color{138, 45, 33, 255});
    DrawRectangle(50, 55, 5, SCREEN_H - 55, Color{40, 10, 8, 255});
    DrawRectangle(42, 55, 51, 8, Color{201, 155, 69, 255});
    DrawRectangle(1185, 55, 45, SCREEN_H - 55, Color{138, 45, 33, 255});
    DrawRectangle(1190, 55, 5, SCREEN_H - 55, Color{40, 10, 8, 255});
    DrawRectangle(1182, 55, 51, 8, Color{201, 155, 69, 255});

    // 4. Ve vien khung Board kieu Chanh dien co ban
    Rectangle outer = { BOARD_OFFSET_X - 14.0f, BOARD_OFFSET_Y - 14.0f, BOARD_PIXEL_W + 28.0f, BOARD_PIXEL_H + 28.0f };
    DrawRectangleRounded(outer, 0.02f, 4, Color{40, 10, 8, 255});
    Rectangle wood = { BOARD_OFFSET_X - 10.0f, BOARD_OFFSET_Y - 10.0f, BOARD_PIXEL_W + 20.0f, BOARD_PIXEL_H + 20.0f };
    DrawRectangleRounded(wood, 0.02f, 4, Color{138, 45, 33, 255});
    Rectangle innerGold = { BOARD_OFFSET_X - 2.0f, BOARD_OFFSET_Y - 2.0f, BOARD_PIXEL_W + 4.0f, BOARD_PIXEL_H + 4.0f };
    DrawRectangleLinesEx(innerGold, 2.0f, Color{201, 155, 69, 255});

    // 5. Nen toi cua bang Tetris
    DrawRectangle(BOARD_OFFSET_X, BOARD_OFFSET_Y, BOARD_PIXEL_W, BOARD_PIXEL_H, Color{20, 18, 19, 255});

    // Ve Grid luoi
    Color gridColor = {48, 42, 40, 100};
    for (int i = 0; i <= BOARD_W; ++i) {
        DrawLine(BOARD_OFFSET_X + i * TILE, BOARD_OFFSET_Y, BOARD_OFFSET_X + i * TILE, BOARD_OFFSET_Y + BOARD_PIXEL_H, gridColor);
    }
    for (int i = 0; i <= BOARD_H; ++i) {
        DrawLine(BOARD_OFFSET_X, BOARD_OFFSET_Y + i * TILE, BOARD_OFFSET_X + BOARD_PIXEL_W, BOARD_OFFSET_Y + i * TILE, gridColor);
    }
}

void Renderer::DrawCurrentPiece(const ActivePiece& piece) {
    Vec2i cells[4];
    GameLogic::GetCells(piece, cells);
    Color color = GetPieceColor(piece.type);

    for (auto& c : cells) {
        if (c.y >= 0) {
            DrawGlossyBlock(BOARD_OFFSET_X + c.x * TILE,
                            BOARD_OFFSET_Y + c.y * TILE,
                            TILE, color, 255);
        }
    }
}

void Renderer::DrawGhostPiece(const ActivePiece& ghost, PieceType type) {
    Vec2i gcells[4];
    GameLogic::GetCells(ghost, gcells);
    Color color = GetPieceColor(type);

    for (auto& c : gcells) {
        if (c.y >= 0) {
            int px = BOARD_OFFSET_X + c.x * TILE;
            int py = BOARD_OFFSET_Y + c.y * TILE;
            int gap = 1;
            // Vien mo
            DrawRectangleRoundedLinesEx(
                {(float)(px + gap), (float)(py + gap),
                 (float)(TILE - gap * 2), (float)(TILE - gap * 2)},
                0.14f, 4, 1.8f, Fade(color, 0.45f));
            // Fill nhe ben trong
            DrawRectangleRounded(
                {(float)(px + gap), (float)(py + gap),
                 (float)(TILE - gap * 2), (float)(TILE - gap * 2)},
                0.14f, 4, Fade(color, 0.12f));
        }
    }
}

void Renderer::DrawMiniPiece(PieceType type, int px, int py, unsigned char alpha, float sizeMul) {
    const auto& shape = SHAPES[type][0];
    int minX = 99, minY = 99;
    for (int i = 0; i < 4; i++) {
        minX = std::min(minX, shape[i].x);
        minY = std::min(minY, shape[i].y);
    }
    float miniSize = 22.0f * sizeMul;
    Color color = GetPieceColor(type);

    for (int i = 0; i < 4; i++) {
        int lx = shape[i].x - minX;
        int ly = shape[i].y - minY;
        DrawGlossyMiniBlock(
            px + (int)(lx * miniSize),
            py + (int)(ly * miniSize),
            miniSize, color, alpha);
    }
}

void Renderer::DrawHoldPanel(int holdType, bool canHold) {
    int holdW = 220, holdH = 220;
    int holdX = BOARD_OFFSET_X - holdW - 48, holdY = BOARD_OFFSET_Y;
    DrawPanelFrame(holdX, holdY, holdW, holdH, "HOLD");
    if (holdType != -1)
        DrawMiniPiece((PieceType)holdType, holdX + holdW / 2 - 40, holdY + 90,
                      canHold ? 255 : 80, 1.0f);
}

void Renderer::DrawNextPanel(const std::vector<PieceType>& nextQueue) {
    int nextW = 220, nextH = 300;
    int nextX = BOARD_OFFSET_X + BOARD_PIXEL_W + 48, nextY = BOARD_OFFSET_Y;
    DrawPanelFrame(nextX, nextY, nextW, nextH, "NEXT");
    for (int i = 0; i < 3 && i < (int)nextQueue.size(); i++)
        DrawMiniPiece(nextQueue[i], nextX + nextW / 2 - 40, nextY + 60 + i * 80, 255, 1.0f);
}

void Renderer::DrawScorePanel(int score, int level, int lines) {
    int holdW = 220, holdH = 220;
    int holdX = BOARD_OFFSET_X - holdW - 48, holdY = BOARD_OFFSET_Y;
    int infoY = holdY + holdH + 24;
    int infoH = 160;
    DrawPanelFrame(holdX, infoY, holdW, infoH, nullptr);

    int ix = holdX + 24, iy = infoY + 18;

    // SCORE
    DrawText("SCORE", ix, iy, 20, Color{245, 220, 130, 255});
    DrawText(TextFormat("%d", score), ix, iy + 28, 36, WHITE);

    // Level va Lines
    DrawText(TextFormat("LEVEL  %d", level), ix, iy + 82, 20, Color{190, 190, 200, 255});
    DrawText(TextFormat("LINES  %d", lines), ix, iy + 112, 20, Color{190, 190, 200, 255});
}

// VE HUONG DAN PHIM DIEU KHIEN
void Renderer::DrawControlsPanel() {
    int nextW = 220, nextH = 300;
    int nextX = BOARD_OFFSET_X + BOARD_PIXEL_W + 48, nextY = BOARD_OFFSET_Y;
    int ctrlY = nextY + nextH + 24;
    int ctrlW = nextW, ctrlH = 235;
    DrawPanelFrame(nextX, ctrlY, ctrlW, ctrlH, "CONTROLS");

    int cx = nextX + 20, cy = ctrlY + 44;
    int fs = 15, gap = 24;
    Color keyCol  = Color{245, 220, 130, 255};
    Color descCol = Color{190, 190, 200, 220};

    DrawText("A / D",   cx, cy + gap * 0, fs, keyCol);
    DrawText(": Move",        cx + 75, cy + gap * 0, fs, descCol);
    DrawText("S",       cx, cy + gap * 1, fs, keyCol);
    DrawText(": Soft Drop",   cx + 75, cy + gap * 1, fs, descCol);
    DrawText("Q / E",   cx, cy + gap * 2, fs, keyCol);
    DrawText(": Rotate",      cx + 75, cy + gap * 2, fs, descCol);
    DrawText("SPACE",   cx, cy + gap * 3, fs, keyCol);
    DrawText(": Hard Drop",   cx + 75, cy + gap * 3, fs, descCol);
    DrawText("W / C",   cx, cy + gap * 4, fs, keyCol);
    DrawText(": Hold",        cx + 75, cy + gap * 4, fs, descCol);
    DrawText("P",       cx, cy + gap * 5, fs, keyCol);
    DrawText(": Pause",       cx + 75, cy + gap * 5, fs, descCol);
    DrawText("ESC",     cx, cy + gap * 6, fs, keyCol);
    DrawText(": Settings",    cx + 75, cy + gap * 6, fs, descCol);
}

// VE OVERLAY TAM DUNG
void Renderer::DrawPauseOverlay() {
    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.70f));

    const char* text1 = "PAUSED";
    int tw1 = MeasureText(text1, 48);
    DrawText(text1, SCREEN_W / 2 - tw1 / 2 + 2, SCREEN_H / 2 - 30 + 2, 48, Color{0, 0, 0, 180});
    DrawText(text1, SCREEN_W / 2 - tw1 / 2, SCREEN_H / 2 - 30, 48, Color{245, 220, 130, 255});

    const char* text2 = "Press P to continue";
    int tw2 = MeasureText(text2, 22);
    DrawText(text2, SCREEN_W / 2 - tw2 / 2, SCREEN_H / 2 + 30, 22, Color{200, 200, 210, 220});
}

// VE OVERLAY GAME OVER
void Renderer::DrawGameOverOverlay() {
    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.75f));

    const char* text1 = "GAME OVER";
    int tw1 = MeasureText(text1, 52);
    DrawText(text1, SCREEN_W / 2 - tw1 / 2 + 2, SCREEN_H / 2 - 40 + 2, 52, Color{0, 0, 0, 200});
    DrawText(text1, SCREEN_W / 2 - tw1 / 2, SCREEN_H / 2 - 40, 52, Color{235, 45, 45, 255});

    const char* text2 = "Press ENTER to restart";
    int tw2 = MeasureText(text2, 22);
    DrawText(text2, SCREEN_W / 2 - tw2 / 2, SCREEN_H / 2 + 30, 22, Color{200, 200, 210, 220});
}

