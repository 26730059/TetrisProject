#include "Renderer.h"
#include <algorithm>

namespace {
    const Color TEMPLE_BG_TOP    = {33, 16, 13, 255};   
    const Color TEMPLE_BG_BOT    = {22, 12, 10, 255};
    const Color BEAM_WOOD        = {74, 23, 18, 255};   
    const Color BEAM_SHADOW      = {40, 10, 8, 255};
    const Color COLUMN_RED       = {138, 45, 33, 255};  
    const Color ANTIQUE_GOLD     = {201, 155, 69, 255}; 
    const Color ANTIQUE_GOLD_DIM = {160, 120, 50, 255};
    const Color CREAM_TEXT       = {245, 222, 173, 255};
    const Color CREAM_MUTED      = {180, 160, 110, 255};
    const Color PANEL_BG         = {25, 23, 25, 240};   
    const Color BOARD_BG         = {20, 18, 19, 255};
    const Color GRID_LINE        = {48, 42, 40, 100};

    const int LOCAL_SHAPES[7][4][4][2] = {
        { {{0,1}, {1,1}, {2,1}, {3,1}}, {{2,0}, {2,1}, {2,2}, {2,3}}, {{0,2}, {1,2}, {2,2}, {3,2}}, {{1,0}, {1,1}, {1,2}, {1,3}} },
        { {{1,0}, {2,0}, {1,1}, {2,1}}, {{1,0}, {2,0}, {1,1}, {2,1}}, {{1,0}, {2,0}, {1,1}, {2,1}}, {{1,0}, {2,0}, {1,1}, {2,1}} },
        { {{1,0}, {0,1}, {1,1}, {2,1}}, {{1,0}, {1,1}, {2,1}, {1,2}}, {{0,1}, {1,1}, {2,1}, {1,2}}, {{1,0}, {0,1}, {1,1}, {1,2}} },
        { {{1,0}, {2,0}, {0,1}, {1,1}}, {{1,0}, {1,1}, {2,1}, {2,2}}, {{1,1}, {2,1}, {0,2}, {1,2}}, {{0,0}, {0,1}, {1,1}, {1,2}} },
        { {{0,0}, {1,0}, {1,1}, {2,1}}, {{2,0}, {1,1}, {2,1}, {1,2}}, {{0,1}, {1,1}, {1,2}, {2,2}}, {{1,0}, {0,1}, {1,1}, {0,2}} },
        { {{0,0}, {0,1}, {1,1}, {2,1}}, {{1,0}, {2,0}, {1,1}, {1,2}}, {{0,1}, {1,1}, {2,1}, {2,2}}, {{1,0}, {1,1}, {0,2}, {1,2}} },
        { {{2,0}, {0,1}, {1,1}, {2,1}}, {{1,0}, {1,1}, {1,2}, {2,2}}, {{0,1}, {1,1}, {2,1}, {0,2}}, {{0,0}, {1,0}, {1,1}, {1,2}} }
    };
}

void Renderer::LoadTextures() {}
void Renderer::UnloadTextures() {}

void Renderer::DrawBoardBackground() {
    DrawTempleBackground();
    DrawBoardTempleFrame();
    DrawRectangle(BOARD_OFFSET_X, BOARD_OFFSET_Y, BOARD_PIXEL_W, BOARD_PIXEL_H, BOARD_BG);

    for (int i = 0; i <= BOARD_W; ++i) {
        DrawLine(BOARD_OFFSET_X + i * TILE, BOARD_OFFSET_Y, 
                 BOARD_OFFSET_X + i * TILE, BOARD_OFFSET_Y + BOARD_PIXEL_H, GRID_LINE);
    }
    for (int i = 0; i <= BOARD_H; ++i) {
        DrawLine(BOARD_OFFSET_X, BOARD_OFFSET_Y + i * TILE, 
                 BOARD_OFFSET_X + BOARD_PIXEL_W, BOARD_OFFSET_Y + i * TILE, GRID_LINE);
    }

    const char* quoteText = "Every cleared line is a step closer to tranquility.";
    int qw = MeasureText(quoteText, 20);
    float quoteW = (float)qw + 60.0f;
    float quoteX = BOARD_OFFSET_X + (BOARD_PIXEL_W / 2.0f) - (quoteW / 2.0f);
    Rectangle quoteBounds = { quoteX, (float)(BOARD_OFFSET_Y + BOARD_PIXEL_H + 20), quoteW, 46.0f };
    DrawWoodPanel(quoteBounds, "");

    int txtX = (int)(quoteBounds.x + quoteBounds.width / 2 - qw / 2);
    int txtY = (int)(quoteBounds.y + 13);
    DrawText(quoteText, txtX + 1, txtY + 1, 20, Color{10, 5, 5, 220});
    DrawText(quoteText, txtX, txtY, 20, CREAM_MUTED);
}

void Renderer::DrawLockedBlocks(const GameLogic& logic) {
    std::vector<int> fullRows = logic.FindFullRows();
    bool isAnim = logic.IsClearAnimRunning();
    float prog = isAnim ? logic.GetClearAnimProgress() : 0.0f;
    
    unsigned char alphaFull = 255;
    unsigned char alphaFade = (unsigned char)(255.0f * (1.0f - prog));

    for (int y = 0; y < BOARD_H; y++) {
        bool isFull = (std::find(fullRows.begin(), fullRows.end(), y) != fullRows.end());
        unsigned char alpha = isFull ? alphaFade : alphaFull;

        for (int x = 0; x < BOARD_W; x++) {
            int type = logic.GetCell(x, y);
            if (type != -1) {
                DrawGlossyBlock(BOARD_OFFSET_X + x * TILE, 
                                BOARD_OFFSET_Y + y * TILE, 
                                TILE, GetPieceColor((PieceType)type), alpha);
            }
        }
    }
}

void Renderer::DrawCurrentPiece(const ActivePiece& piece) {
    Color baseCol = GetPieceColor(piece.type);
    
    // Hieu ung nhip tho (Pulsing Glow): Sang len toi da 50 don vi theo thoi gian thuc
    float time = (float)GetTime();
    float pulse = (sinf(time * 8.0f) + 1.0f) * 0.5f; // Giao dong 0.0 -> 1.0
    Color glowCol = Lighten(baseCol, (int)(50 * pulse));

    for (int i = 0; i < 4; i++) {
        int px = piece.x + LOCAL_SHAPES[piece.type][piece.rot][i][0];
        int py = piece.y + LOCAL_SHAPES[piece.type][piece.rot][i][1];
        if (py >= 0) {
            DrawGlossyBlock(BOARD_OFFSET_X + px * TILE, 
                            BOARD_OFFSET_Y + py * TILE, TILE, glowCol, 255);
        }
    }
}

void Renderer::DrawGhostPiece(const ActivePiece& ghost, PieceType type) {
    Color col = GetPieceColor(type);
    float time = (float)GetTime();
    
    // Tinh toan do mo (alpha) dao dong de tao hieu ung Hologram chot tat
    unsigned char fillAlpha = (unsigned char)(35 + sinf(time * 12.0f) * 20);
    Color fillCol = {col.r, col.g, col.b, fillAlpha};
    
    // Vien ngoai cung dao dong do sang
    unsigned char lineAlpha = (unsigned char)(180 + sinf(time * 8.0f) * 75);
    Color lineCol = {col.r, col.g, col.b, lineAlpha};

    for (int i = 0; i < 4; i++) {
        int px = ghost.x + LOCAL_SHAPES[type][ghost.rot][i][0];
        int py = ghost.y + LOCAL_SHAPES[type][ghost.rot][i][1];
        if (py >= 0) {
            int screenX = BOARD_OFFSET_X + px * TILE;
            int screenY = BOARD_OFFSET_Y + py * TILE;
            
            // To nen hologram
            DrawRectangle(screenX + 1, screenY + 1, TILE - 2, TILE - 2, fillCol);
            DrawRectangleLines(screenX, screenY, TILE, TILE, lineCol);
            
            // Tia scanline (quet vach ngang) bieu dien du doan vi tri
            float scanY = fmodf(time * 40.0f + i * 8.0f, (float)TILE);
            DrawLine(screenX + 2, screenY + (int)scanY, screenX + TILE - 2, screenY + (int)scanY, Color{col.r, col.g, col.b, 100});
        }
    }
}

void Renderer::DrawHoldPanel(int holdType, bool canHold) {
    Rectangle bounds = {152, 64, 220, 220};
    DrawWoodPanel(bounds, "HOLD");

    if (holdType != -1) {
        unsigned char alpha = canHold ? 255 : 80;
        DrawMiniPiece((PieceType)holdType, (int)(bounds.x + bounds.width / 2.0f), (int)(bounds.y + bounds.height / 2.0f + 10.0f), alpha, 1.2f);
    }
}

void Renderer::DrawNextPanel(const std::vector<PieceType>& nextQueue) {
    Rectangle bounds = {788, 64, 220, 300};
    DrawWoodPanel(bounds, "NEXT");

    for (size_t i = 0; i < nextQueue.size() && i < 3; ++i) {
        float py = bounds.y + 90.0f + i * 75.0f;
        DrawMiniPiece(nextQueue[i], (int)(bounds.x + bounds.width / 2.0f), (int)py, 255, 1.0f);
        if (i < 2) { 
            DrawLineEx({bounds.x + 60, py + 38}, {bounds.x + bounds.width - 60, py + 38}, 1.0f, {160, 120, 50, 50});
        }
    }
}

void Renderer::DrawScorePanel(int score, int level, int lines) {
    Rectangle bounds = {152, 308, 220, 160};
    DrawWoodPanel(bounds, "SCORE");

    const char* scoreStr = TextFormat("%d", score);
    int sw = MeasureText(scoreStr, 28);
    DrawText(scoreStr, (int)(bounds.x + bounds.width / 2 - sw / 2), (int)(bounds.y + 60), 28, CREAM_TEXT);

    const char* lvlStr = TextFormat("LEVEL %d", level);
    const char* linStr = TextFormat("LINES %d", lines);
    int lw = MeasureText(lvlStr, 20);
    int linw = MeasureText(linStr, 20);

    DrawText(lvlStr, (int)(bounds.x + bounds.width / 2 - lw / 2), (int)(bounds.y + 105), 20, CREAM_MUTED);
    DrawText(linStr, (int)(bounds.x + bounds.width / 2 - linw / 2), (int)(bounds.y + 130), 20, CREAM_MUTED);
}

void Renderer::DrawControlsPanel() {
    Rectangle bounds = {788, 388, 220, 235};
    DrawWoodPanel(bounds, "CONTROLS");

    const char* keys[] = {"A / D", "S", "Q / E", "SPACE", "W / C", "P", "ESC"};
    const char* desc[] = {"Move", "Soft Drop", "Rotate", "Hard Drop", "Hold", "Pause", "Settings"};

    float startY = bounds.y + 55.0f;
    for (int i = 0; i < 7; ++i) {
        DrawText(keys[i], (int)(bounds.x + 25), (int)(startY + i * 22), 16, ANTIQUE_GOLD);
        DrawText(desc[i], (int)(bounds.x + 105), (int)(startY + i * 22), 16, CREAM_MUTED);
    }
}

void Renderer::DrawPauseOverlay() {
    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Color{0, 0, 0, 180});
    Rectangle panel = { SCREEN_W / 2.0f - 150.0f, SCREEN_H / 2.0f - 80.0f, 300, 160 };
    DrawWoodPanel(panel, "");

    int w1 = MeasureText("PAUSED", 30);
    DrawText("PAUSED", SCREEN_W / 2 - w1 / 2, (int)(panel.y + 40), 30, ANTIQUE_GOLD);

    int w2 = MeasureText("Press P to continue", 20);
    DrawText("Press P to continue", SCREEN_W / 2 - w2 / 2, (int)(panel.y + 90), 20, CREAM_MUTED);

    DrawLotusMotif(SCREEN_W / 2.0f, panel.y + 135.0f, 1.2f, ANTIQUE_GOLD_DIM);
}

void Renderer::DrawGameOverOverlay() {
    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Color{0, 0, 0, 200});
    Rectangle panel = { SCREEN_W / 2.0f - 160.0f, SCREEN_H / 2.0f - 85.0f, 320, 170 };
    DrawWoodPanel(panel, "");

    int w1 = MeasureText("GAME OVER", 34);
    DrawText("GAME OVER", SCREEN_W / 2 - w1 / 2, (int)(panel.y + 40), 34, COLUMN_RED);

    int w2 = MeasureText("Press ENTER to restart", 20);
    DrawText("Press ENTER to restart", SCREEN_W / 2 - w2 / 2, (int)(panel.y + 100), 20, CREAM_MUTED);

    DrawLotusMotif(SCREEN_W / 2.0f, panel.y + 145.0f, 1.2f, ANTIQUE_GOLD_DIM);
}

Color Renderer::GetPieceColor(PieceType type) {
    switch (type) {
        case I_PIECE: return {0, 168, 245, 255};
        case O_PIECE: return {255, 220, 15, 255};
        case T_PIECE: return {18, 185, 85, 255};
        case S_PIECE: return {215, 35, 145, 255};
        case Z_PIECE: return {235, 45, 45, 255};
        case J_PIECE: return {45, 65, 185, 255};
        case L_PIECE: return {250, 115, 25, 255};
        default: return RAYWHITE;
    }
}

Color Renderer::Lighten(Color c, int amount) {
    return { (unsigned char)std::min(255, c.r + amount), 
             (unsigned char)std::min(255, c.g + amount), 
             (unsigned char)std::min(255, c.b + amount), c.a };
}

Color Renderer::Darken(Color c, int amount) {
    return { (unsigned char)std::max(0, c.r - amount), 
             (unsigned char)std::max(0, c.g - amount), 
             (unsigned char)std::max(0, c.b - amount), c.a };
}

void Renderer::DrawGlossyBlock(int px, int py, int size, Color baseColor, unsigned char alpha) {
    Color c = { baseColor.r, baseColor.g, baseColor.b, alpha };
    Color light = Lighten(baseColor, 60); light.a = alpha;
    Color dark = Darken(baseColor, 50); dark.a = alpha;

    DrawRectangle(px, py, size, size, dark);
    DrawRectangle(px + 2, py + 2, size - 4, size - 4, c);
    
    DrawLine(px + 2, py + 2, px + size - 3, py + 2, light);
    DrawLine(px + 2, py + 2, px + 2, py + size - 3, light);
    
    DrawLine(px + size - 2, py + 2, px + size - 2, py + size - 2, dark);
    DrawLine(px + 2, py + size - 2, px + size - 2, py + size - 2, dark);

    DrawRectangle(px + size / 2 - 2, py + size / 2 - 2, 4, 4, 
                  Color{201, 155, 69, (unsigned char)(alpha * 0.4f)});
}

void Renderer::DrawGlossyMiniBlock(int px, int py, float size, Color baseColor, unsigned char alpha) {
    DrawGlossyBlock(px, py, (int)size, baseColor, alpha);
}

void Renderer::DrawMiniPiece(PieceType type, int px, int py, unsigned char alpha, float sizeMul) {
    float bSize = (float)TILE * 0.6f * sizeMul;
    Color col = GetPieceColor(type);
    
    float minX = 4, maxX = 0, minY = 4, maxY = 0;
    for (int i = 0; i < 4; i++) {
        float bx = (float)LOCAL_SHAPES[type][0][i][0];
        float by = (float)LOCAL_SHAPES[type][0][i][1];
        if (bx < minX) minX = bx; if (bx > maxX) maxX = bx;
        if (by < minY) minY = by; if (by > maxY) maxY = by;
    }
    float w = (maxX - minX + 1) * bSize;
    float h = (maxY - minY + 1) * bSize;
    
    float startX = px - w / 2.0f - minX * bSize;
    float startY = py - h / 2.0f - minY * bSize;

    for (int i = 0; i < 4; i++) {
        float bx = startX + LOCAL_SHAPES[type][0][i][0] * bSize;
        float by = startY + LOCAL_SHAPES[type][0][i][1] * bSize;
        DrawGlossyMiniBlock((int)bx, (int)by, bSize, col, alpha);
    }
}

void Renderer::DrawTempleBackground() {
    DrawRectangleGradientV(0, 0, SCREEN_W, SCREEN_H, TEMPLE_BG_TOP, TEMPLE_BG_BOT);

    // --- Hieu ung Dom Dom (Ambient Fireflies) bay lo lung ---
    float time = (float)GetTime();
    for (int i = 0; i < 35; ++i) {
        // Random vi tri dua vao index (khong dung rand() de giu nguyen trang thai khao khao)
        float baseX = (float)((i * i * 37) % SCREEN_W);
        float baseY = (float)((i * 53) % SCREEN_H);
        
        // Di chuyen cham rai bang sin/cos va troi dan len tren
        float px = baseX + sinf(time * 0.4f + i) * 35.0f;
        float py = baseY + cosf(time * 0.3f + i) * 25.0f - (time * 15.0f);
        
        // Wrap man hinh de hat bay mat thi moc lai tu duoi (Fix bug mất đom đóm)
        float wrappedY = fmodf(py, (float)SCREEN_H);
        if (wrappedY < 0) wrappedY += SCREEN_H;
        float wrappedX = fmodf(px, (float)SCREEN_W);
        if (wrappedX < 0) wrappedX += SCREEN_W;

        // Hieu ung chot tat (Flickering)
        unsigned char alpha = (unsigned char)(60 + (sinf(time * 1.5f + i) + 1.0f) * 60);
        
        float size = (i % 3 == 0) ? 2.5f : 1.5f;
        DrawCircle((int)wrappedX, (int)wrappedY, size, Color{255, 220, 100, alpha}); // Loi sang
        DrawCircle((int)wrappedX, (int)wrappedY, size * 2.5f, Color{255, 180, 50, (unsigned char)(alpha / 4)}); // Halo
    }

    // Mai ngoi am duong
    DrawRectangle(0, 0, SCREEN_W, 35, Color{38, 18, 15, 255}); 
    for (int x = 12; x < SCREEN_W + 24; x += 24) {
        DrawRectangle(x - 12, 0, 24, 35, Color{48, 22, 18, 255});
        DrawLine(x - 12, 0, x - 12, 35, Color{25, 10, 8, 255});
        DrawRectangle(x - 5, 0, 10, 38, Color{68, 28, 22, 255});
        DrawCircle(x, 37, 10, Color{85, 32, 25, 255});
        DrawCircle(x, 37, 6, Color{48, 22, 18, 255});
        DrawCircle(x, 37, 2, ANTIQUE_GOLD_DIM);
    }

    // Xa ngang
    DrawRectangle(0, 35, SCREEN_W, 20, BEAM_WOOD);
    DrawRectangle(0, 53, SCREEN_W, 2, ANTIQUE_GOLD);
    DrawRectangle(0, 35, SCREEN_W, 3, BEAM_SHADOW);

    // Cot do trai
    DrawRectangle(45, 55, 45, SCREEN_H - 55, COLUMN_RED);
    DrawRectangle(50, 55, 5, SCREEN_H - 55, BEAM_SHADOW);
    DrawRectangle(42, 55, 51, 8, ANTIQUE_GOLD);
    DrawTriangle(Vector2{90, 55}, Vector2{130, 55}, Vector2{90, 85}, BEAM_WOOD);
    DrawTriangleLines(Vector2{90, 55}, Vector2{130, 55}, Vector2{90, 85}, ANTIQUE_GOLD_DIM);

    // Cot do phai
    DrawRectangle(1185, 55, 45, SCREEN_H - 55, COLUMN_RED);
    DrawRectangle(1190, 55, 5, SCREEN_H - 55, BEAM_SHADOW);
    DrawRectangle(1182, 55, 51, 8, ANTIQUE_GOLD);
    DrawTriangle(Vector2{1185, 55}, Vector2{1185, 85}, Vector2{1145, 55}, BEAM_WOOD);
    DrawTriangleLines(Vector2{1185, 55}, Vector2{1185, 85}, Vector2{1145, 55}, ANTIQUE_GOLD_DIM);
}

void Renderer::DrawBoardTempleFrame() {
    Rectangle outer = { BOARD_OFFSET_X - 14.0f, BOARD_OFFSET_Y - 14.0f, 
                        BOARD_PIXEL_W + 28.0f, BOARD_PIXEL_H + 28.0f };
    DrawRectangleRounded(outer, 0.02f, 4, BEAM_SHADOW);

    Rectangle wood = { BOARD_OFFSET_X - 10.0f, BOARD_OFFSET_Y - 10.0f, 
                       BOARD_PIXEL_W + 20.0f, BOARD_PIXEL_H + 20.0f };
    DrawRectangleRounded(wood, 0.02f, 4, COLUMN_RED);
    
    Rectangle innerGold = { BOARD_OFFSET_X - 2.0f, BOARD_OFFSET_Y - 2.0f, 
                            BOARD_PIXEL_W + 4.0f, BOARD_PIXEL_H + 4.0f };
    DrawRectangleLinesEx(innerGold, 2.0f, ANTIQUE_GOLD);
}

void Renderer::DrawWoodPanel(Rectangle bounds, const char* title) {
    DrawRectangleRounded(Rectangle{bounds.x + 5, bounds.y + 5, bounds.width, bounds.height}, 
                         0.04f, 4, Color{10, 5, 5, 200});
    DrawRectangleRounded(bounds, 0.04f, 4, BEAM_WOOD);
    DrawRectangleRoundedLinesEx(bounds, 0.04f, 4, 2.0f, ANTIQUE_GOLD);
    
    Rectangle inner = {bounds.x + 6, bounds.y + 6, bounds.width - 12, bounds.height - 12};
    DrawRectangleRounded(inner, 0.03f, 4, PANEL_BG);
    DrawRectangleRoundedLinesEx(inner, 0.03f, 4, 1.0f, ANTIQUE_GOLD_DIM);

    DrawCornerOrnament(inner.x + 3, inner.y + 3, false, false);
    DrawCornerOrnament(inner.x + inner.width - 3, inner.y + 3, true, false);
    DrawCornerOrnament(inner.x + 3, inner.y + inner.height - 3, false, true);
    DrawCornerOrnament(inner.x + inner.width - 3, inner.y + inner.height - 3, true, true);

    if (title && title[0] != '\0') {
        int tw = MeasureText(title, 20);
        DrawText(title, (int)(bounds.x + bounds.width / 2 - tw / 2), (int)(bounds.y + 16), 20, CREAM_TEXT);
        DrawLine((int)(bounds.x + 25), (int)(bounds.y + 44), 
                 (int)(bounds.x + bounds.width - 25), (int)(bounds.y + 44), ANTIQUE_GOLD_DIM);
    }
}

void Renderer::DrawCornerOrnament(float x, float y, bool flipX, bool flipY) {
    float dx = flipX ? -1.0f : 1.0f;
    float dy = flipY ? -1.0f : 1.0f;
    float size = 10.0f;

    DrawLineEx(Vector2{x, y}, Vector2{x + size * dx, y}, 2.0f, ANTIQUE_GOLD);
    DrawLineEx(Vector2{x, y}, Vector2{x, y + size * dy}, 2.0f, ANTIQUE_GOLD);
    
    DrawLineEx(Vector2{x + size * dx, y + 4.0f * dy}, Vector2{x + size * dx, y + size * dy}, 2.0f, ANTIQUE_GOLD);
    DrawLineEx(Vector2{x + 4.0f * dx, y + size * dy}, Vector2{x + size * dx, y + size * dy}, 2.0f, ANTIQUE_GOLD);
}

void Renderer::DrawLotusMotif(float cx, float cy, float scale, Color color) {
    DrawEllipse((int)cx, (int)cy, 4.0f * scale, 8.0f * scale, color);
    DrawEllipse((int)(cx - 5.0f * scale), (int)(cy + 2.0f * scale), 3.0f * scale, 6.0f * scale, color);
    DrawEllipse((int)(cx + 5.0f * scale), (int)(cy + 2.0f * scale), 3.0f * scale, 6.0f * scale, color);
    DrawLineEx(Vector2{cx - 6.0f * scale, cy + 7.0f * scale}, 
               Vector2{cx + 6.0f * scale, cy + 7.0f * scale}, 2.0f, color);
}