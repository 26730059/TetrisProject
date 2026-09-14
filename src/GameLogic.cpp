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

int GameLogic::GetCell(int x, int y) const {
    if (x < 0 || x >= BOARD_W || y < 0 || y >= BOARD_H) return -1;
    return grid[y][x];
}

std::vector<int> GameLogic::FindFullRows() const {
    std::vector<int> fullRows;
    for (int y = 0; y < BOARD_H; y++) {
        bool full = true;
        for (int x = 0; x < BOARD_W; x++) {
            if (grid[y][x] == -1) { full = false; break; }
        }
        if (full) fullRows.push_back(y);
    }
    return fullRows;
}

int GameLogic::ClearRows(const std::vector<int>& rows) {
    int n = (int)rows.size();
    for (int row : rows) {
        grid.erase(grid.begin() + row);
        grid.insert(grid.begin(), std::vector<int>(BOARD_W, -1));
    }
    clearingRows.clear();
    return n;
}

void GameLogic::StartClearAnim(const std::vector<int>& rows) {
    clearingRows = rows;
    clearAnimTimer = 0.25f;
}

bool GameLogic::IsClearAnimRunning() const {
    return clearAnimTimer > 0;
}

void GameLogic::UpdateClearAnim(float dt) {
    if (clearAnimTimer > 0) {
        clearAnimTimer -= dt;
        if (clearAnimTimer < 0) clearAnimTimer = 0;
    }
}

float GameLogic::GetClearAnimProgress() const {
    if (clearAnimTimer <= 0) return 0.0f;
    return clearAnimTimer / 0.25f;
}

const std::vector<int>& GameLogic::GetClearRows() const {
    return clearingRows;
}

void GameLogic::GetCells(const ActivePiece& p, Vec2i out[4]) {
    const auto& shape = SHAPES[p.type][p.rot];
    for (int i = 0; i < 4; i++) {
        out[i].x = p.x + shape[i].x;
        out[i].y = p.y + shape[i].y;
    }
}

ActivePiece GameLogic::Spawn(PieceType type) {
    ActivePiece p;
    p.type = type;
    p.rot = 0;
    p.x = 3;
    p.y = -2;
    return p;
}

ActivePiece GameLogic::GetGhost(const ActivePiece& p) const {
    ActivePiece ghost = p;
    while (true) {
        ActivePiece t = ghost;
        t.y += 1;
        if (!Fits(t)) break;
        ghost = t;
    }
    return ghost;
}

bool GameLogic::TryRotate(ActivePiece& p, int dir) {
    ActivePiece rotated = p;
    rotated.rot = (rotated.rot + dir + 4) % 4;
    static const int kicks[5] = { 0, -1, 1, -2, 2 };
    for (int k : kicks) {
        ActivePiece test = rotated;
        test.x += k;
        if (Fits(test)) {
            p = test;
            return true;
        }
    }
    return false;
}

void GameLogic::ResetBag() {
    bagItems.clear();
    RefillBag();
}

void GameLogic::RefillBag() {
    bagItems = { I_PIECE, O_PIECE, T_PIECE, S_PIECE, Z_PIECE, J_PIECE, L_PIECE };
    std::shuffle(bagItems.begin(), bagItems.end(), rng);
}

PieceType GameLogic::NextFromBag() {
    if (bagItems.empty()) RefillBag();
    PieceType p = bagItems.back();
    bagItems.pop_back();
    return p;
}

// SPAWN & QUEUE HELPERS
ActivePiece GameLogic::SpawnNextPiece() {
    PieceType t = PopNextQueue();
    return Spawn(t);
}

PieceType GameLogic::PopNextQueue() {
    if (nextQueue.empty()) {
        nextQueue.push_back(NextFromBag());
    }
    PieceType t = nextQueue.front();
    nextQueue.erase(nextQueue.begin());
    nextQueue.push_back(NextFromBag());
    return t;
}

// CORE GAMEPLAY ACTIONS
bool GameLogic::Move(int dx, int dy, int softDropScore) {
    if (gameOver || paused || IsClearAnimRunning()) return false;
    ActivePiece p = current;
    p.x += dx;
    p.y += dy;
    if (Fits(p)) {
        current = p;
        if (dy > 0 && softDropScore > 0) score += softDropScore;
        isLocking = false;
        lockTimer = 0.0f;
        return true;
    } else if (dy > 0) {
        // Cham day / vat can -> bat dau dem thoi gian khoa
        isLocking = true;
        return false;
    }
    return false;
}