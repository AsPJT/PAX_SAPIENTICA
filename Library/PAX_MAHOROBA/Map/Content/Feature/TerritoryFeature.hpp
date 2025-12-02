/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_TERRITORY_FEATURE_HPP
#define PAX_MAHOROBA_TERRITORY_FEATURE_HPP

#include <string>
#include <vector>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Spline2D.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Map/Content/Feature/FeatureType.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/MapFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Interaction/ClickContext.hpp>
#include <PAX_MAHOROBA/Map/Content/Update/IUpdatable.hpp>
#include <PAX_MAHOROBA/Map/Content/Update/UpdateContext.hpp>
#include <PAX_MAHOROBA/Map/Core/MapCoordinateConverter.hpp>

#include <PAX_SAPIENTICA/Core/Type/Range.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/Map/TerritoryLocationPoint.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

/// @brief 領域（国や文化圏などのスプライン曲線）を表す地物クラス
/// @brief Feature class representing a territory (spline curves for countries, cultural areas, etc.)
class TerritoryFeature : public MapFeature, public ISpatiallyUpdatable {
public:
    /// @param data 領域の位置データ / Territory location data
    /// @param group_data 領域グループデータ / Territory group data
    TerritoryFeature(const TerritoryLocationData& data, const TerritoryLocationGroup& group_data)
        : data_(data)
        , group_data_(group_data)
        , cached_screen_points_()
        , color_(0, 0, 0, 255)  // デフォルト色（黒） / Default color (black)
    {
        // 色情報からRGBAを取得（将来的にはAppConfigから色マッピングを読み込む）
        // TODO: 色情報の管理システムを実装
    }

    FeatureType getType() const override {
        return FeatureType::Territory;
    }

    std::uint_least32_t getId() const override {
        if (!data_.key.empty()) {
            return MurMur3::calcHash(data_.key.c_str());
        }
        return 0;
    }

    std::string getName() const override {
        return data_.key;
    }

    std::uint_least32_t getFeatureTypeHash() const override {
        return data_.feature_type_hash;
    }

    /// @brief 空間的更新（ISpatiallyUpdatableの実装）
    /// @brief Spatial update (ISpatiallyUpdatable implementation)
    void updateSpatial(const SpatialContext& context) override {
        // 地物種別の可視性チェック（最優先）
        if ((context.visibility_manager != nullptr) && !context.visibility_manager->isVisible(data_.feature_type_hash)) {
            cached_screen_points_.clear();
            visible_ = false;
            return;
        }

        // データが空の場合は早期リターン
        if (data_.coordinates.empty()) {
            cached_screen_points_.clear();
            visible_ = false;
            return;
        }

        // 領域のバウンディングボックスを計算（最小・最大経度/緯度）
        // Calculate bounding box of the territory (min/max longitude/latitude)
        double min_lon = data_.coordinates[0].x;
        double max_lon = data_.coordinates[0].x;
        double min_lat = data_.coordinates[0].y;
        double max_lat = data_.coordinates[0].y;

        for (const auto& coord : data_.coordinates) {
            min_lon = (std::min)(min_lon, coord.x);
            max_lon = (std::max)(max_lon, coord.x);
            min_lat = (std::min)(min_lat, coord.y);
            max_lat = (std::max)(max_lat, coord.y);
        }

        // 日付変更線をまたぐかチェック（経度差が180°以上）
        // Check if territory crosses date line (longitude difference >= 180°)
        const bool crosses_dateline = (max_lon - min_lon) > 180.0;

        // ±3°のマージンを追加
        // Add ±3° margin
        constexpr double bbox_margin = 3.0;
        min_lon -= bbox_margin;
        max_lon += bbox_margin;
        min_lat -= bbox_margin;
        max_lat += bbox_margin;

        // 画面の範囲を計算
        // Calculate screen bounds
        const double view_half_width = context.map_view_size.x / 2.0;
        const double view_half_height = context.map_view_size.y / 2.0;
        const double view_min_lon = context.map_view_center.x - view_half_width;
        const double view_max_lon = context.map_view_center.x + view_half_width;
        const double view_min_lat = context.map_view_center.y - view_half_height;
        const double view_max_lat = context.map_view_center.y + view_half_height;

        // 経度ラップ処理：3つのオフセット候補（-360°, 0°, +360°）で試す
        // Longitude wrapping: try 3 offset candidates (-360°, 0°, +360°)
        int best_offset_mult = 0;
        bool found_overlap = false;

        for (int offset_mult = -1; offset_mult <= 1; ++offset_mult) {
            const double offset_min_lon = min_lon + (offset_mult * 360.0);
            const double offset_max_lon = max_lon + (offset_mult * 360.0);

            // バウンディングボックスが画面範囲と重なるかチェック
            // Check if bounding box overlaps with screen bounds
            const bool lon_overlap = (offset_min_lon <= view_max_lon) && (offset_max_lon >= view_min_lon);
            const bool lat_overlap = (min_lat <= view_max_lat) && (max_lat >= view_min_lat);

            if (lon_overlap && lat_overlap) {
                best_offset_mult = offset_mult;
                found_overlap = true;
                break;  // 最初に見つかったオフセットを使用
            }
        }

        // 画面範囲と重ならない場合は非表示
        // Hide if no overlap with screen bounds
        if (!found_overlap) {
            cached_screen_points_.clear();
            visible_ = false;
            return;
        }

        // 最適なオフセットを適用した後、日付変更線をまたぐ場合の経度正規化
        // Apply best offset, then normalize longitude for date line wrapping
        std::vector<paxs::WebMercatorDeg> normalized_coords;
        normalized_coords.reserve(data_.coordinates.size());

        // 最初の点にオフセットを適用
        const double first_x = data_.coordinates[0].x + (best_offset_mult * 360.0);
        normalized_coords.push_back(paxs::WebMercatorDeg(paxs::Vector2<double>(first_x, data_.coordinates[0].y)));

        // 2点目以降は、日付変更線をまたぐ場合のみ正規化
        // For subsequent points, normalize only if crossing date line
        if (crosses_dateline) {
            // 日付変更線をまたぐ場合：前の点との経度差が180°を超えないように正規化
            // Date line crossing: normalize so longitude difference from previous point doesn't exceed 180°
            for (size_t i = 1; i < data_.coordinates.size(); ++i) {
                const double prev_lon = normalized_coords[i - 1].x;
                double current_lon = data_.coordinates[i].x + (best_offset_mult * 360.0);

                // 経度差を計算
                double lon_diff = current_lon - prev_lon;

                // 差が180°を超える場合、短い方の経路を選択
                // If difference exceeds 180°, choose shorter path
                if (lon_diff > 180.0) {
                    current_lon -= 360.0;
                } else if (lon_diff < -180.0) {
                    current_lon += 360.0;
                }

                normalized_coords.push_back(paxs::WebMercatorDeg(
                    paxs::Vector2<double>(current_lon, data_.coordinates[i].y)
                ));
            }
        } else {
            // 日付変更線をまたがない場合：そのままオフセットを適用
            // Non-crossing: simply apply offset
            for (size_t i = 1; i < data_.coordinates.size(); ++i) {
                const double current_lon = data_.coordinates[i].x + (best_offset_mult * 360.0);
                normalized_coords.push_back(paxs::WebMercatorDeg(
                    paxs::Vector2<double>(current_lon, data_.coordinates[i].y)
                ));
            }
        }

        // 正規化された座標をスクリーン座標に変換
        // Convert normalized coordinates to screen coordinates
        cached_screen_points_.clear();
        cached_screen_points_.reserve(normalized_coords.size());

        for (const auto& coord : normalized_coords) {
            // スクリーン座標に変換
            const paxg::Vec2<double> screen_pos = MapCoordinateConverter::toScreenPos(
                Vector2<double>(coord.x, coord.y),
                context.map_view_size,
                context.map_view_center
            );

            cached_screen_points_.emplace_back(
                static_cast<float>(screen_pos.x()),
                static_cast<float>(screen_pos.y())
            );
        }

        visible_ = (cached_screen_points_.size() >= 2);
    }

    bool isVisible() const override {
        return visible_;
    }

    bool isInTimeRange(double jdn) const override {
        return data_.year_range.contains(jdn);
    }

    const WrappedScreenPositions& getScreenPositions() const override {
        // Territoryは複数点を持つため、WrappedScreenPositionsは使用しない
        // Use empty positions since territories use multiple points
        static const WrappedScreenPositions empty_positions;
        return empty_positions;
    }

    int getDisplaySize() const override {
        return 0; // Territoryは点ではなく線なのでサイズは0
    }

    bool isHit(const paxs::Vector2<int>& mouse_pos) const override {
        // 領域のクリック判定は実装しない（将来的には線の近傍判定を実装可能）
        // Click detection for territories is not implemented (can be added in the future)
        (void)mouse_pos;
        return false;
    }

    void onClick(const ClickContext& context) override {
        // std::cout << "Territory clicked: " << getName() << std::endl;
        (void)context;
    }

    /// @brief スプライン曲線を描画
    /// @brief Draw spline curve
    void renderSpline() const {
        if (!visible_ || cached_screen_points_.size() < 2) {
            return;
        }

        // グループデータから線幅を取得
        const float line_width = group_data_.line_width;

        // スプライン曲線で描画
        // Spline2Dは最初と最後の点が同じ場合、自動的に閉じたループとして綺麗に描画する
        // Spline2D automatically detects closed loops when first and last points are the same
        paxg::Spline2D(cached_screen_points_).draw(line_width, color_);
    }

    const TerritoryLocationData& getData() const {
        return data_;
    }

    const TerritoryLocationGroup& getGroupData() const {
        return group_data_;
    }

    const std::vector<paxg::Vec2f>& getScreenPoints() const {
        return cached_screen_points_;
    }

private:
    TerritoryLocationData data_;           ///< 領域位置データ / Territory location data
    TerritoryLocationGroup group_data_;    ///< 領域グループデータ / Territory group data

    std::vector<paxg::Vec2f> cached_screen_points_;  ///< キャッシュされたスクリーン座標列 / Cached screen points
    paxg::Color color_;                              ///< 描画色 / Drawing color
    bool visible_{true};                             ///< 可視性 / Visibility
};

} // namespace paxs

#endif // !PAX_MAHOROBA_TERRITORY_FEATURE_HPP
