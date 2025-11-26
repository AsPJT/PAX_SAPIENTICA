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
#include <utility>
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
/// @details 空間の更新が必要
class TerritoryFeature : public MapFeature, public ISpatiallyUpdatable {
public:
    /// @param data 領域の位置データ / Territory location data
    /// @param group_data 領域グループデータ / Territory group data
    TerritoryFeature(TerritoryLocationData  data, TerritoryLocationGroup  group_data)
        : data_(std::move(data))
        , group_data_(std::move(group_data))
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

        // スクリーン座標に変換
        cached_screen_points_.clear();
        cached_screen_points_.reserve(data_.coordinates.size());

        bool any_in_view = false;

        for (const auto& coord : data_.coordinates) {
            // ビューの範囲チェック（少なくとも1点が範囲内なら描画）
            if (context.isInViewBounds(coord)) {
                any_in_view = true;
            }

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

        visible_ = any_in_view && (cached_screen_points_.size() >= 2);
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
        // クリック時の処理（デバッグ出力）
        std::cout << "Territory clicked: " << getName() << std::endl;
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

        // スプライン曲線で描画（Settlementと同じ方法）
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
    bool visible_{};                                   ///< 可視性 / Visibility
};

} // namespace paxs

#endif // !PAX_MAHOROBA_TERRITORY_FEATURE_HPP
