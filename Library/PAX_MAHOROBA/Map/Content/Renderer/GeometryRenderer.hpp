/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_GEOMETRY_RENDERER_HPP
#define PAX_MAHOROBA_GEOMETRY_RENDERER_HPP

#include <cstdint>

#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxs {

/// @brief 地理的な図形の描画に共通する機能を提供する基底クラス
/// @brief Base class providing common functionality for rendering geographic shapes
class GeometryRenderer {
protected:
    GeometryRenderer() = default;
    ~GeometryRenderer() = default;

    /// @brief 座標が表示範囲内にあるかを判定
    /// @brief Check if coordinate is within view bounds
    /// @param coordinate EPSG:3857(Webメルカトル) 座標 / Web Mercator coordinate
    /// @param map_view_size マップビューのサイズ / Map view size
    /// @param map_view_center マップビューの中心座標 / Map view center
    /// @param margin_factor マージン係数（デフォルト: 1.6） / Margin factor (default: 1.6)
    /// @return 範囲内の場合true / true if within bounds
    static bool isInViewport(
        const Vector2<double>& coordinate,
        const Vector2<double>& map_view_size,
        const Vector2<double>& map_view_center,
        double margin_factor = 1.6
    ) {
        const double half_width = map_view_size.x / 2 * margin_factor;
        const double half_height = map_view_size.y / 2 * margin_factor;
        return (coordinate.x >= map_view_center.x - half_width &&
            coordinate.x <= map_view_center.x + half_width &&
            coordinate.y >= map_view_center.y - half_height &&
            coordinate.y <= map_view_center.y + half_height);
    }

    /// @brief テクスチャを描画（共通パターンの抽出）
    /// @brief Draw texture (common pattern extraction)
    /// @param texture_map テクスチャマップ / Texture map
    /// @param texture_key テクスチャキー / Texture key
    /// @param draw_pos 描画位置 / Draw position
    /// @param size 描画サイズ / Draw size
    /// @return 描画に成功した場合true / true if drawing succeeded
    static bool drawTexture(
        const UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_map,
        std::uint_least32_t texture_key,
        const paxg::Vec2<double>& draw_pos,
        int size
    ) {
        if (texture_key == 0) {
            return false;
        }
        const auto* const ptr = texture_map.try_get(texture_key);
        if (ptr == nullptr) {
            return false;
        }
        ptr->resizedDrawAt(size, draw_pos);
        return true;
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_GEOMETRY_RENDERER_HPP
