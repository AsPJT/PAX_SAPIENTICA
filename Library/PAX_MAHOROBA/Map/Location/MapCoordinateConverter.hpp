/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_COORDINATE_CONVERTER_HPP
#define PAX_MAHOROBA_MAP_COORDINATE_CONVERTER_HPP

#include <vector>

#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>
#include <PAX_SAPIENTICA/MapProjection.hpp>

namespace paxs {

/// @brief 地図座標変換ヘルパークラス
/// @brief Map coordinate conversion helper class
class MapCoordinateConverter {
public:
    /// @brief メルカトル座標→スクリーン座標変換（経度ラップ処理付き）
    /// @brief Convert Mercator coordinates to screen positions (with longitude wrapping)
    /// @param mercator_x メルカトル座標X（経度）/ Mercator X (longitude)
    /// @param mercator_y メルカトル座標Y（緯度）/ Mercator Y (latitude)
    /// @param map_view_width ビューポート幅 / Viewport width
    /// @param map_view_height ビューポート高さ / Viewport height
    /// @param map_view_center_x ビューポート中心X / Viewport center X
    /// @param map_view_center_y ビューポート中心Y / Viewport center Y
    /// @return 3つのスクリーン座標（-360°, 0°, +360°）/ Three screen positions
    static std::vector<paxg::Vec2i> toScreenPositions(
        double mercator_x,
        double mercator_y,
        double map_view_width,
        double map_view_height,
        double map_view_center_x,
        double map_view_center_y
    );

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
    );

    /// @brief 単一の座標をスクリーン座標に変換
    /// @brief Convert single coordinate to screen position
    /// @param mercator_x メルカトル座標X / Mercator X coordinate
    /// @param mercator_y メルカトル座標Y / Mercator Y coordinate
    /// @param map_view_width ビューポート幅 / Viewport width
    /// @param map_view_height ビューポート高さ / Viewport height
    /// @param map_view_center_x ビューポート中心X / Viewport center X
    /// @param map_view_center_y ビューポート中心Y / Viewport center Y
    /// @return スクリーン座標 / Screen position
    static paxg::Vec2i toScreenPos(
        double mercator_x,
        double mercator_y,
        double map_view_width,
        double map_view_height,
        double map_view_center_x,
        double map_view_center_y
    );
};

// ========================================
// インライン実装
// ========================================

inline std::vector<paxg::Vec2i> MapCoordinateConverter::toScreenPositions(
    double mercator_x,
    double mercator_y,
    double map_view_width,
    double map_view_height,
    double map_view_center_x,
    double map_view_center_y
) {
    std::vector<paxg::Vec2i> positions;
    positions.reserve(3);

    // 3つの経度座標（-360°, 0°, +360°）を計算
    for (int offset_mult = -1; offset_mult <= 1; ++offset_mult) {
        const double wrapped_x = mercator_x + (offset_mult * 360.0);
        positions.emplace_back(toScreenPos(
            wrapped_x, mercator_y,
            map_view_width, map_view_height,
            map_view_center_x, map_view_center_y
        ));
    }

    return positions;
}

inline MercatorDeg MapCoordinateConverter::interpolatePosition(
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

    const double coordinate_displacement_x = end.x - start.x;
    const double coordinate_displacement_y = end.y - start.y;

    return MercatorDeg(paxs::Vector2<double>(
        start.x + coordinate_displacement_x * year_normalization,
        start.y + coordinate_displacement_y * year_normalization
    ));
}

inline paxg::Vec2i MapCoordinateConverter::toScreenPos(
    double mercator_x,
    double mercator_y,
    double map_view_width,
    double map_view_height,
    double map_view_center_x,
    double map_view_center_y
) {
    // メルカトル座標をスクリーン座標に変換
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

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_COORDINATE_CONVERTER_HPP
