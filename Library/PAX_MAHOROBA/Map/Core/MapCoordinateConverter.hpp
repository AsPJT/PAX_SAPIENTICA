/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_COORDINATE_CONVERTER_HPP
#define PAX_MAHOROBA_MAP_COORDINATE_CONVERTER_HPP

#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/WrappedScreenPositions.hpp>

namespace paxs {

/// @brief 地図座標変換ヘルパークラス
/// @brief Map coordinate conversion helper class
struct MapCoordinateConverter {
    /// @brief メルカトル座標→スクリーン座標変換（経度ラップ処理付き）
    /// @brief Convert Mercator coordinates to screen positions (with longitude wrapping)
    /// @param mercator_pos メルカトル座標 / Mercator position
    /// @param map_view_size ビューポートサイズ / Viewport size
    /// @param map_view_center ビューポート中心 / Viewport center
    /// @param out_positions 出力先（経度ラップされた3つのスクリーン座標） / Output (three wrapped screen positions)
    static void toScreenPositions(
        const Vector2<double>& mercator_pos,
        const Vector2<double>& map_view_size,
        const Vector2<double>& map_view_center,
        WrappedScreenPositions& out_positions
    ) {
        // 3つの経度座標（-360°, 0°, +360°）を計算
        for (int offset_mult = -1, i = 0; offset_mult <= 1; ++offset_mult, ++i) {
            const double wrapped_x = mercator_pos.x + (offset_mult * 360.0);
            out_positions.positions[i] = toScreenPos(
                Vector2<double>(wrapped_x, mercator_pos.y),
                map_view_size, map_view_center
            );
        }
        out_positions.is_valid = true;  // Mark as valid after filling positions
    }

    /// @brief 時間補間座標の計算（PersonPortrait用）
    /// @brief Calculate time-interpolated position (for PersonPortrait)
    /// @param start 開始座標 / Start position
    /// @param end 終了座標 / End position
    /// @param jdn 現在のユリウス日 / Current Julian Day Number
    /// @param min_year 開始年 / Start year
    /// @param max_year 終了年 / End year
    /// @return 補間された座標 / Interpolated position
    static MercatorDeg interpolatePosition(
        const MercatorDeg& start,
        const MercatorDeg& end,
        double jdn,
        double min_year,
        double max_year
    ) {
        const double view_year_displacement = max_year - min_year;
        const double jdn_displacement = jdn - min_year;
        const double year_normalization = (view_year_displacement == 0.0)
            ? 0.0
            : (jdn_displacement / view_year_displacement);

        const Vector2<double> coordinate_displacement = end - start;

        return MercatorDeg(start + coordinate_displacement * year_normalization);
    }

    /// @brief 単一の座標をスクリーン座標に変換
    /// @brief Convert single coordinate to screen position
    /// @param mercator_pos メルカトル座標 / Mercator position
    /// @param map_view_size ビューポートサイズ / Viewport size
    /// @param map_view_center ビューポート中心 / Viewport center
    /// @return スクリーン座標 / Screen position
    static paxg::Vec2<double> toScreenPos(
        const Vector2<double>& mercator_pos,
        const Vector2<double>& map_view_size,
        const Vector2<double>& map_view_center
    ) {
        // メルカトル座標をスクリーン座標に変換
        return paxg::Vec2<double>(
            (mercator_pos.x - (map_view_center.x - map_view_size.x / 2))
            / map_view_size.x * double(paxg::Window::width()),
            double(paxg::Window::height())
            - ((mercator_pos.y - (map_view_center.y - map_view_size.y / 2))
            / map_view_size.y * double(paxg::Window::height()))
        );
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_COORDINATE_CONVERTER_HPP
