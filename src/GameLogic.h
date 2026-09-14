#pragma once

#include "Constants.h"


extern const std::array<std::array<std::array<Vec2i,4>,4>, PIECE_COUNT> SHAPES;


extern const char* PIECE_COLOR_NAME[PIECE_COUNT];


enum class LogicEvent {
    None = 0,
    Rotate,
    HardDrop,
    Hold,
    Lock,
    LineClear,
    GameOver
};

struct GameEvent {
    LogicEvent type = LogicEvent::None;
    int data = 0;
};

class GameLogic {
public:
    GameLogic();

    void InitBoard();
    void Reset(int startLevel = 1);

    bool Fits(const ActivePiece& p) const;
    bool LockPiece(const ActivePiece& p);
    int GetCell(int x, int y) const;

    std::vector<int> FindFullRows() const;
    int ClearRows(const std::vector<int>& rows);


    void StartClearAnim(const std::vector<int>& rows);
    bool IsClearAnimRunning() const;
    void UpdateClearAnim(float dt);
    float GetClearAnimProgress() const;
    const std::vector<int>& GetClearRows() const;

    static void GetCells(const ActivePiece& p, Vec2i out[4]);
    ActivePiece Spawn(PieceType type);
    ActivePiece GetGhost(const ActivePiece& p) const;
    ActivePiece GetGhostPiece() const { return GetGhost(current); }
    bool TryRotate(ActivePiece& p, int dir);


    PieceType NextFromBag();
    void ResetBag();

    bool Move(int dx, int dy, int softDropScore = 0);
    bool RotateCW();
    bool RotateCCW();
    int HardDrop(int dropScorePerTile = 2);
    bool Hold();

    void Update(float dt,
                float gravityInterval,
                float lockDelay = 0.5f,
                int linesPerLevel = 10,
                const int scoreTable[5] = nullptr,
                int softDropPoints = 1);

    const ActivePiece& GetCurrentPiece() const { return current; }
    const std::vector<PieceType>& GetNextQueue() const { return nextQueue; }
    int GetHoldType() const { return holdType; }
    bool CanHold() const { return canHold; }

    int GetScore() const { return score; }
    int GetLevel() const { return level; }
    int GetLines() const { return lines; }

    bool IsGameOver() const { return gameOver; }
    bool IsPaused() const { return paused; }
    void SetPaused(bool p) { paused = p; }
    void TogglePause() { paused = !paused; }

    bool IsLocking() const { return isLocking; }
    bool IsSoftDrop() const { return softDrop; }
    void SetSoftDrop(bool sd) { softDrop = sd; }

    const std::vector<GameEvent>& GetEvents() const { return events; }
    void ClearEvents() { events.clear(); }
    void PushEvent(LogicEvent type, int data = 0) { events.push_back({type, data}); }

private:
    ActivePiece SpawnNextPiece();
    PieceType PopNextQueue();
    void LockAndSpawn();
    void ResolveLineClear(int linesPerLevel, const int scoreTable[5]);
    void RefillBag();


    std::vector<std::vector<int>> grid;
    std::vector<int> clearingRows;
    float clearAnimTimer = 0.0f;

    ActivePiece current{};
    std::vector<PieceType> nextQueue;
    int holdType = -1;
    bool canHold = true;
};
