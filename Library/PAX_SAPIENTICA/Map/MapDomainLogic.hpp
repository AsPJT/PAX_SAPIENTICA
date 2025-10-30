/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_MAP_DOMAIN_LOGIC_HPP
#define PAX_SAPIENTICA_MAP_MAP_DOMAIN_LOGIC_HPP

#include <cmath>
#include <cstdint>

namespace paxs {
namespace map {

/// @brief 地図の可視領域を表す構造体
/// @brief Structure representing the visible area of the map
struct VisibleArea {
    double min_lon;  // 最小経度
    double max_lon;  // 最大経度
    double min_lat;  // 最小緯度
    double max_lat;  // 最大緯度

    /// @brief 指定座標が可視領域内にあるかチェック
    /// @brief Check if the specified coordinates are within the visible area
    /// @param lon 経度
    /// @param lat 緯度
    /// @return 可視領域内ならtrue
    bool contains(double lon, double lat) const {
        return (lon >= min_lon && lon <= max_lon &&
                lat >= min_lat && lat <= max_lat);
    }
};

/// @brief 地図のドメインモデル（UI非依存のロジック）
/// @brief Map domain model (UI-independent logic)
class MapDomainLogic {
public:
    MapDomainLogic() = default;

    /// @brief MapViewから可視領域を計算する
    /// @brief Calculate visible area from MapView
    /// @param center_x 中心X座標（メルカトル座標）
    /// @param center_y 中心Y座標（メルカトル座標）
    /// @param width 幅
    /// @param height 高さ
    /// @return 可視領域
    VisibleArea calculateVisibleArea(
        double center_x,
        double center_y,
        double width,
        double height
    ) const {
        // メルカトル座標から可視領域を計算
        const double half_width = width / 2.0;
        const double half_height = height / 2.0;

        VisibleArea area;
        area.min_lon = center_x - half_width;
        area.max_lon = center_x + half_width;
        area.min_lat = center_y - half_height;
        area.max_lat = center_y + half_height;

        return area;
    }

    /// @brief 指定座標が可視領域内にあるかチェック
    /// @brief Check if the specified coordinates are within the visible area
    /// @param lon 経度
    /// @param lat 緯度
    /// @param area 可視領域
    /// @return 可視領域内ならtrue
    bool isInVisibleArea(
        double lon,
        double lat,
        const VisibleArea& area
    ) const {
        return area.contains(lon, lat);
    }

    /// @brief 2点間の距離を計算（簡易版）
    /// @brief Calculate distance between two points (simplified)
    /// @param x1 点1のX座標
    /// @param y1 点1のY座標
    /// @param x2 点2のX座標
    /// @param y2 点2のY座標
    /// @return 距離
    double calculateDistance(
        double x1,
        double y1,
        double x2,
        double y2
    ) const {
        const double dx = x2 - x1;
        const double dy = y2 - y1;
        return std::sqrt(dx * dx + dy * dy);
    }

    /// @brief ズームレベルに応じた表示フィルタリングを判定
    /// @brief Determine display filtering based on zoom level
    /// @param current_zoom 現在のズームレベル
    /// @param required_zoom 必要なズームレベル
    /// @return 表示すべきならtrue
    bool shouldDisplay(
        double current_zoom,
        double required_zoom
    ) const {
        return current_zoom >= required_zoom;
    }

private:
    // UI非依存の純粋なロジックのみ
    // 状態は持たない（Stateless）
};

} // namespace map
} // namespace paxs

#endif // !PAX_SAPIENTICA_MAP_MAP_MODEL_HPP
