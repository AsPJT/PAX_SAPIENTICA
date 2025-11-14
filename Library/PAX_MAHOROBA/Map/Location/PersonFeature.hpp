/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_PERSON_FEATURE_HPP
#define PAX_MAHOROBA_PERSON_FEATURE_HPP

#include <string>
#include <vector>

#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Map/Location/ClickContext.hpp>
#include <PAX_MAHOROBA/Map/Location/FeatureType.hpp>
#include <PAX_MAHOROBA/Map/Location/MapContentHitTester.hpp>
#include <PAX_MAHOROBA/Map/Location/MapCoordinateConverter.hpp>
#include <PAX_MAHOROBA/Map/Location/MapFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/RenderContext.hpp>

#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/Map/Repository/PersonNameRepository.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

/// @brief 人物（肖像画+名前）を表す地物クラス
/// @brief Feature class representing a person (portrait + name)
class PersonFeature : public MapFeature {
public:
    /// @param data 人物の位置データ / Person location data
    /// @param list_data 人物リストデータ / Person list data
    PersonFeature(const PersonLocationPoint& data, const PersonLocationList& list_data)
        : data_(data), list_data_(list_data) {
        visible_ = true;
        min_year_ = data.min_year;
        max_year_ = data.max_year;
    }

    FeatureType getType() const override {
        return FeatureType::Person;
    }

    std::string getId() const override {
        // IDとして人物名のハッシュを使用
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
        // フォールバック: 最初の名前を返す
        if (!data_.place_name.empty()) {
            return data_.place_name.begin()->second;
        }
        return "";
    }

    std::uint_least32_t getLpe() const override {
        return data_.lpe;
    }

    void update(const RenderContext& context) override {
        // 地物種別の可視性チェック（最優先）
        if (context.visibility_manager && !context.visibility_manager->isVisible(data_.lpe)) {
            cached_screen_positions_.clear();
            return;
        }

        // 時間補間座標の計算
        interpolated_pos_ = MapCoordinateConverter::interpolatePosition(
            data_.start_coordinate, data_.end_coordinate,
            context.jdn, data_.min_year, data_.max_year
        );

        // 空間フィルタリング：ビューの範囲外の場合はスキップ
        if (!context.isInViewBounds(interpolated_pos_.x, interpolated_pos_.y)) {
            cached_screen_positions_.clear();
            return;
        }

        // スクリーン座標に変換（経度ラップ処理付き）
        cached_screen_positions_ = MapCoordinateConverter::toScreenPositions(
            interpolated_pos_.x, interpolated_pos_.y,
            context.map_view_width, context.map_view_height,
            context.map_view_center_x, context.map_view_center_y
        );

        // 表示サイズの計算
        const bool out_of_range = (data_.min_view > context.map_view_width ||
                                   data_.max_view < context.map_view_width);
        cached_display_size_ = out_of_range
            ? static_cast<int>(data_.overall_length / 2)
            : 60;  // 120 / 2

        cached_jdn_ = context.jdn;
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

        const int hit_radius = cached_display_size_;

        // 3つのスクリーン座標でヒット判定
        return MapContentHitTester::testMultiplePositions(
            mouse_pos.x(), mouse_pos.y(), cached_screen_positions_,
            [hit_radius](int mx, int my, const paxg::Vec2i& pos) {
                return MapContentHitTester::circleHitTest(mx, my, pos, hit_radius);
            }
        );
    }

    void onClick(const ClickContext& context) override {
        // クリック時の処理（デバッグ出力）
        std::cout << "Person clicked: " << getName() << std::endl;
        (void)context;  // 未使用警告を抑制
    }


    MercatorDeg getInterpolatedPosition() const {
        return interpolated_pos_;
    }

    const PersonLocationPoint& getData() const {
        return data_;
    }

    const PersonLocationList& getListData() const {
        return list_data_;
    }

private:
    PersonLocationPoint data_;           ///< 人物位置データ / Person location data
    PersonLocationList list_data_;       ///< 人物リストデータ / Person list data

    MercatorDeg interpolated_pos_;                    ///< 補間された座標 / Interpolated position
    std::vector<paxg::Vec2i> cached_screen_positions_; ///< スクリーン座標（3つ） / Screen positions (3)
    int cached_display_size_ = 60;                    ///< 表示サイズ / Display size
    double cached_jdn_ = 0.0;                         ///< キャッシュされたJDN / Cached JDN
};

} // namespace paxs

#endif // !PAX_MAHOROBA_PERSON_FEATURE_HPP
