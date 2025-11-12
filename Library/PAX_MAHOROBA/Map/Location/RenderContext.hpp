/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_RENDER_CONTEXT_HPP
#define PAX_MAHOROBA_RENDER_CONTEXT_HPP

#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>

namespace paxs {

/// @brief 描画コンテキスト - 描画に必要なパラメータをまとめた構造体
/// @brief Render context - Structure containing parameters needed for rendering
struct RenderContext {
    double jdn = 0.0;              ///< ユリウス日 / Julian Day Number
    double map_view_width = 0.0;   ///< 地図ビューの幅 / Map view width
    double map_view_height = 0.0;  ///< 地図ビューの高さ / Map view height
    double map_view_center_x = 0.0; ///< 地図ビューの中心X座標 / Map view center X
    double map_view_center_y = 0.0; ///< 地図ビューの中心Y座標 / Map view center Y
    const FeatureVisibilityManager* visibility_manager = nullptr; ///< 地物種別ごとの可視性管理 / Per-feature-type visibility manager

    /// @brief メルカトル座標からスクリーン座標に変換
    /// @brief Convert Mercator coordinates to screen coordinates
    /// @param mercator_x メルカトル座標X / Mercator X coordinate
    /// @param mercator_y メルカトル座標Y / Mercator Y coordinate
    /// @return スクリーン座標 / Screen coordinates
    inline paxg::Vec2i toScreenPos(double mercator_x, double mercator_y) const {
        return paxg::Vec2i(
            static_cast<int>(
                (mercator_x - (map_view_center_x - map_view_width / 2))
                / map_view_width * double(paxg::Window::width())
            ),
            static_cast<int>(
                double(paxg::Window::height())
                - ((mercator_y - (map_view_center_y - map_view_height / 2))
                / map_view_height * double(paxg::Window::height()))
            )
        );
    }

    /// @brief 座標がビューの範囲内にあるかチェック
    /// @brief Check if coordinates are within view bounds
    /// @param mercator_x メルカトル座標X / Mercator X coordinate
    /// @param mercator_y メルカトル座標Y / Mercator Y coordinate
    /// @param margin マージン倍率（デフォルト1.6） / Margin multiplier (default 1.6)
    /// @return 範囲内ならtrue / True if within bounds
    inline bool isInViewBounds(double mercator_x, double mercator_y, double margin = 1.6) const {
        const double half_width = map_view_width / 2 * margin;
        const double half_height = map_view_height / 2 * margin;
        return (mercator_x >= map_view_center_x - half_width &&
                mercator_x <= map_view_center_x + half_width &&
                mercator_y >= map_view_center_y - half_height &&
                mercator_y <= map_view_center_y + half_height);
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_RENDER_CONTEXT_HPP
