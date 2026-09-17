#pragma once

#include "Constants.h"
#include "GameLogic.h"
#include <vector>

/**
 * @class Renderer
 * @brief Quản lý toàn bộ giao diện hình ảnh của trò chơi Tetris.
 * 
 * Module này độc lập hoàn toàn với logic game, chuyên xử lý thao tác vẽ 
 * khối gạch, lưới (board), các bảng thông tin (Hold, Next, Score) và màn hình chờ.
 * 
 * @theme Vietnamese Temple Architecture (Sử dụng 100% Procedural Graphics raylib)
 * @author UI/UX Module
 */
class Renderer {
public:
    /** @brief Khởi tạo và nạp các tài nguyên hình ảnh (nếu có). */
    void LoadTextures();

    /** @brief Giải phóng tài nguyên hình ảnh khỏi bộ nhớ GPU. */
    void UnloadTextures();

    /** @brief Vẽ bối cảnh chánh điện và khung lưới Tetris. */
    void DrawBoardBackground();

    /** 
     * @brief Vẽ các khối gạch đã bị khóa chặt dưới đáy lưới.
     * @param logic Tham chiếu (chỉ đọc) đến trạng thái GameLogic.
     */
    void DrawLockedBlocks(const GameLogic& logic);

    /** 
     * @brief Vẽ khối Tetromino đang rơi xuống.
     * @param piece Trạng thái khối đang rơi (loại, góc xoay, tọa độ).
     */
    void DrawCurrentPiece(const ActivePiece& piece);

    /** 
     * @brief Vẽ bóng ảo (Hologram) dự báo vị trí khối sẽ đáp xuống.
     * @param ghost Tọa độ của bóng ảo.
     * @param type Loại khối để xác định màu sắc.
     */
    void DrawGhostPiece(const ActivePiece& ghost, PieceType type);

    /** @brief Vẽ bảng hiển thị khối đang được Giữ (Hold). */
    void DrawHoldPanel(int holdType, bool canHold);

    /** @brief Vẽ bảng hiển thị 3 khối tiếp theo sắp ra (Next Queue). */
    void DrawNextPanel(const std::vector<PieceType>& nextQueue);

    /** @brief Vẽ bảng Điểm số, Cấp độ và Số hàng đã xóa. */
    void DrawScorePanel(int score, int level, int lines);

    /** @brief Vẽ bảng hướng dẫn phím bấm điều khiển. */
    void DrawControlsPanel();

    /** @brief Phủ màn hình tối và hiện thông báo Tạm dừng (Pause). */
    void DrawPauseOverlay();

    /** @brief Phủ màn hình tối và hiện thông báo Thua cuộc (Game Over). */
    void DrawGameOverOverlay();

private:
    Texture2D blockTextures[PIECE_COUNT]; ///< Mảng chứa texture dự phòng

    // --- Tiện ích xử lý Màu sắc & Đổ bóng ---
    static Color GetPieceColor(PieceType type);
    static Color Lighten(Color c, int amount);
    static Color Darken(Color c, int amount);

    // --- Tiện ích vẽ Khối gạch 3D ---
    void DrawGlossyBlock(int px, int py, int size, Color baseColor, unsigned char alpha = 255);
    void DrawGlossyMiniBlock(int px, int py, float size, Color baseColor, unsigned char alpha = 255);
    void DrawMiniPiece(PieceType type, int px, int py, unsigned char alpha, float sizeMul);

    // --- Tiện ích vẽ Kiến trúc Chánh Điện (Procedural) ---
    /** @brief Vẽ cảnh nền kiến trúc (cột gỗ, xà ngang, đom đóm). */
    void DrawTempleBackground();

    /** @brief Vẽ khung gỗ viền vàng bọc xung quanh Board. */
    void DrawBoardTempleFrame();

    /** @brief Helper đa năng: Vẽ bảng gỗ đồng bộ cho Hold/Next/Score/Controls. */
    void DrawWoodPanel(Rectangle bounds, const char* title);

    /** @brief Vẽ họa tiết góc hoa văn chạm trổ truyền thống. */
    void DrawCornerOrnament(float x, float y, bool flipX, bool flipY);

    /** @brief Vẽ họa tiết hoa sen cách điệu cho các Overlay. */
    void DrawLotusMotif(float cx, float cy, float scale, Color color);
};