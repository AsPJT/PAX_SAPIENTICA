/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_RENDER_CONTEXT_HPP
#define PAX_MAHOROBA_RENDER_CONTEXT_HPP

#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_SAPIENTICA/Core/Type/Rect.hpp>
#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>

namespace paxs {

/// @brief 描画コンテキスト - 描画に必要なパラメータをまとめた構造体
/// @brief Render context - Structure containing parameters needed for rendering
struct RenderContext {
    double jdn = 0.0;  ///< ユリウス日 / Julian Day Number
    Vector2<double> map_view_size{0.0, 0.0};      ///< 地図ビューのサイズ（幅、高さ） / Map view size (width, height)
    Vector2<double> map_view_center{0.0, 0.0};    ///< 地図ビューの中心座標 / Map view center
    const FeatureVisibilityManager* visibility_manager = nullptr; ///< 地物種別ごとの可視性管理 / Per-feature-type visibility manager
    paxg::Font* font = nullptr; ///< 地名・人名描画用フォント / Font for place/person names
    const UnorderedMap<std::uint_least32_t, paxg::Texture>* texture_map = nullptr; ///< テクスチャマップ / Texture map

    /// @brief 座標がビューの範囲内にあるかチェック
    /// @brief Check if coordinates are within view bounds
    /// @param mercator_x メルカトル座標X / Mercator X coordinate
    /// @param mercator_y メルカトル座標Y / Mercator Y coordinate
    /// @param margin マージン倍率（デフォルト1.6） / Margin multiplier (default 1.6)
    /// @return 範囲内ならtrue / True if within bounds
    [[nodiscard]] bool isInViewBounds(Vector2<double> mercator_pos, double margin = 1.6) const {
        const Rect<double> view_rect = Rect<double>::fromCenter(
            map_view_center,
            map_view_size * margin
        );
        return view_rect.contains(mercator_pos);
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_RENDER_CONTEXT_HPP
