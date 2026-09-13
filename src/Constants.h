#pragma once

#include "raylib.h"
#include <array>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <cstdio>

static const int   BOARD_W        = 10;
static const int   BOARD_H        = 20;
static const int   TILE           = 32;          // kich thuoc 1 o (khop voi sprite 32x32)
static const int   BOARD_PIXEL_W  = BOARD_W * TILE;
static const int   BOARD_PIXEL_H  = BOARD_H * TILE;

static const int   BOARD_OFFSET_X = 420;          // vi tri ve board tren man hinh
static const int   BOARD_OFFSET_Y = 64;

static const int   SCREEN_W = 1360;
static const int   SCREEN_H = 800;

static const int   BLOCK_TEX_SIZE = 128;           // kich thuoc goc (px) cua tung anh khoi
static const float BLOCK_DRAW_SCALE = (float)TILE / BLOCK_TEX_SIZE;  // 128px -> 32px

static const Color COL_BACKGROUND   = { 24,  89, 117, 255 };
static const Color COL_PANEL_FILL   = { 40,  33,  38, 235 };
static const Color COL_PANEL_BORDER = { 12,  10,  12, 255 };
static const Color COL_ACCENT       = { 240, 165,  60, 255 };  // mau cam
static const Color COL_CREAM        = { 245, 222, 179, 255 };  // mau chu kem

// 7 loai khoi: I, O, T, S, Z, J, L
enum PieceType { I_PIECE = 0, O_PIECE, T_PIECE, S_PIECE, Z_PIECE, J_PIECE, L_PIECE, PIECE_COUNT };

struct Vec2i { int x, y; };

struct ActivePiece {
    PieceType type;
    int rot   = 0;
    int x     = 3;   // toa do goc (o luoi) cua bounding box 4x4
    int y     = -2;
};
