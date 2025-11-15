/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_PLACE_NAME_FEATURE_HPP
#define PAX_MAHOROBA_PLACE_NAME_FEATURE_HPP

#include <string>
#include <vector>

#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Map/Location/ClickContext.hpp>
#include <PAX_MAHOROBA/Map/Location/FeatureType.hpp>
#include <PAX_MAHOROBA/Map/Location/MapContentHitTester.hpp>
#include <PAX_MAHOROBA/Map/Location/MapCoordinateConverter.hpp>
#include <PAX_MAHOROBA/Map/Location/MapFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/RenderContext.hpp>

#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/Map/LocationPoint.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

/// @brief 地名（テキストのみ）を表す地物クラス
/// @brief Feature class representing a place name (text only)
class PlaceNameFeature : public MapFeature {
public:
    /// @param data 地名の位置データ / Place location data
    PlaceNameFeature(const LocationPoint& data)
        : data_(data) {
        visible_ = true;
        min_year_ = data.min_year;
        max_year_ = data.max_year;
    }

    FeatureType getType() const override {
        return FeatureType::PlaceName;
    }

    std::string getId() const override {
        const std::uint_least32_t ja_jp = MurMur3::calcHash("ja-JP");
        if (data_.place_name.find(ja_jp) != data_.place_name.end()) {
            return data_.place_name.at(ja_jp);
        }
        if (!data_.place_name.empty()) {
            return data_.place_name.begin()->second;
        }
        return "unknown";
    }

    std::string getName(const std::string& language = "ja-JP") const override {
        const std::uint_least32_t lang_hash = MurMur3::calcHash(language.c_str());
        if (data_.place_name.find(lang_hash) != data_.place_name.end()) {
            return data_.place_name.at(lang_hash);
        }
        if (!data_.place_name.empty()) {
            return data_.place_name.begin()->second;
        }
        return "";
    }

    std::uint_least32_t getFeatureTypeHash() const override {
        return data_.feature_type_hash;
    }

    void update(const RenderContext& context) override {
        // 地物種別の可視性チェック（最優先）
        if (context.visibility_manager && !context.visibility_manager->isVisible(data_.feature_type_hash)) {
            cached_screen_positions_.clear();
            return;
        }
        // 空間フィルタリング：ビューの範囲外の場合はスキップ
        if (!context.isInViewBounds(data_.coordinate.x, data_.coordinate.y)) {
            cached_screen_positions_.clear();
            return;
        }

        // ズームレベルフィルタリング：範囲外の場合はスキップ
        if (data_.min_zoom_level > context.map_view_height || data_.max_zoom_level < context.map_view_height) {
            cached_screen_positions_.clear();
            return;
        }

        // スクリーン座標に変換（経度ラップ処理付き）
        cached_screen_positions_ = MapCoordinateConverter::toScreenPositions(
            data_.coordinate.x, data_.coordinate.y,
            context.map_view_width, context.map_view_height,
            context.map_view_center_x, context.map_view_center_y
        );

        // TODO: テキストの実際の境界ボックスサイズを計算
        cached_display_size_ = 50;
    }

    bool isVisible() const override {
        return visible_;
    }

    bool isInTimeRange(double jdn) const override {
        return jdn >= min_year_ && jdn <= max_year_;
    }

    std::vector<paxg::Vec2i> getScreenPositions() const override {
        return cached_screen_positions_;
    }

    int getDisplaySize() const override {
        return cached_display_size_;
    }

    bool isHit(const paxg::Vec2i& mouse_pos) const override {
        if (!visible_) return false;

        // TODO: テキストの境界ボックスで判定
        // 現在は円形判定を使用
        const int hit_radius = cached_display_size_;

        return MapContentHitTester::testMultiplePositions(
            mouse_pos.x(), mouse_pos.y(), cached_screen_positions_,
            [hit_radius](int mx, int my, const paxg::Vec2i& pos) {
                return MapContentHitTester::circleHitTest(mx, my, pos, hit_radius);
            }
        );
    }

    void onClick(const ClickContext& context) override {
        std::cout << "PlaceName clicked: " << getName() << std::endl;
        (void)context;
    }

    const LocationPoint& getData() const {
        return data_;
    }

private:
    LocationPoint data_;
    std::vector<paxg::Vec2i> cached_screen_positions_;
    int cached_display_size_ = 50;
};

} // namespace paxs

#endif // !PAX_MAHOROBA_PLACE_NAME_FEATURE_HPP
