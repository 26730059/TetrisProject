#include "GameLogic.h"

const std::array<std::array<std::array<Vec2i,4>,4>, PIECE_COUNT> SHAPES = {{
    // I
    {{
        {{{0,1},{1,1},{2,1},{3,1}}},
        {{{2,0},{2,1},{2,2},{2,3}}},
        {{{0,2},{1,2},{2,2},{3,2}}},
        {{{1,0},{1,1},{1,2},{1,3}}},
    }},
    // O
    {{
        {{{1,0},{2,0},{1,1},{2,1}}},
        {{{1,0},{2,0},{1,1},{2,1}}},
        {{{1,0},{2,0},{1,1},{2,1}}},
        {{{1,0},{2,0},{1,1},{2,1}}},
    }},
    // T
    {{
        {{{1,0},{0,1},{1,1},{2,1}}},
        {{{1,0},{1,1},{2,1},{1,2}}},
        {{{0,1},{1,1},{2,1},{1,2}}},
        {{{1,0},{0,1},{1,1},{1,2}}},
    }},
    // S
    {{
        {{{1,0},{2,0},{0,1},{1,1}}},
        {{{1,0},{1,1},{2,1},{2,2}}},
        {{{1,1},{2,1},{0,2},{1,2}}},
        {{{0,0},{0,1},{1,1},{1,2}}},
    }},
    // Z
    {{
        {{{0,0},{1,0},{1,1},{2,1}}},
        {{{2,0},{1,1},{2,1},{1,2}}},
        {{{0,1},{1,1},{1,2},{2,2}}},
        {{{1,0},{0,1},{1,1},{0,2}}},
    }},
    // J
    {{
        {{{0,0},{0,1},{1,1},{2,1}}},
        {{{1,0},{2,0},{1,1},{1,2}}},
        {{{0,1},{1,1},{2,1},{2,2}}},
        {{{1,0},{1,1},{0,2},{1,2}}},
    }},
    // L
    {{
        {{{2,0},{0,1},{1,1},{2,1}}},
        {{{1,0},{1,1},{1,2},{2,2}}},
        {{{0,1},{1,1},{2,1},{0,2}}},
        {{{0,0},{1,0},{1,1},{1,2}}},
    }},
}};

const char* PIECE_COLOR_NAME[PIECE_COUNT] = {
    "purple", "red", "blue", "yellow", "cyan", "pink", "green"
};

GameLogic::GameLogic() : rng(std::random_device{}()) {
    Reset(1);
}

void GameLogic::Reset(int startLevel) {
    InitBoard();
    ResetBag();

    nextQueue.clear();
    for (int i = 0; i < 3; i++) {
        nextQueue.push_back(NextFromBag());
    }

    current = SpawnNextPiece();
    holdType = -1;
    canHold = true;

    score = 0;
    lines = 0;
    level = std::max(1, startLevel);

    gravityTimer = 0.0f;
    lockTimer    = 0.0f;
    isLocking    = false;
    softDrop     = false;
    gameOver     = false;
    paused       = false;

    events.clear();
}

void GameLogic::InitBoard() {
    grid.assign(BOARD_H, std::vector<int>(BOARD_W, -1));
    clearingRows.clear();
    clearAnimTimer = 0;
}

bool GameLogic::Fits(const ActivePiece& p) const {
    Vec2i cells[4];
    GetCells(p, cells);
    for (auto& c : cells) {
        if (c.x < 0 || c.x >= BOARD_W || c.y >= BOARD_H) return false;
        if (c.y >= 0 && grid[c.y][c.x] != -1) return false;
    }
    return true;
}

bool GameLogic::LockPiece(const ActivePiece& p) {
    Vec2i cells[4];
    GetCells(p, cells);
    for (auto& c : cells) {
        if (c.y < 0) return false;  // Game over: khoi nam ngoai board
        grid[c.y][c.x] = (int)p.type;
    }
    return true;
}