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
#include <utility>

#include <PAX_MAHOROBA/Map/Content/Feature/FeatureType.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/MapFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Interaction/ClickContext.hpp>
#include <PAX_MAHOROBA/Map/Content/Interaction/MapContentHitTester.hpp>
#include <PAX_MAHOROBA/Map/Content/Update/IUpdatable.hpp>
#include <PAX_MAHOROBA/Map/Content/Update/UpdateContext.hpp>
#include <PAX_MAHOROBA/Map/Core/MapCoordinateConverter.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

#include <PAX_SAPIENTICA/Core/Type/Rect.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/WrappedScreenPositions.hpp>
#include <PAX_SAPIENTICA/Map/LocationPoint.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

/// @brief 地名（テキストのみ）を表す地物クラス
/// @brief Feature class representing a place name (text only)
class PlaceNameFeature : public MapFeature,
                         public ISpatiallyUpdatable,
                         public ITemporallyUpdatable,
                         public ILocalizable {
public:
    /// @param data 地名の位置データ / Place location data
    PlaceNameFeature(LocationPoint  data)
        : data_(std::move(data)) {
    }

    FeatureType getType() const override {
        return FeatureType::PlaceName;
    }

    std::uint_least32_t getId() const override {
        // IDとして key の MurMur3 ハッシュを使用
        if (!data_.key.empty()) {
            return MurMur3::calcHash(data_.key.c_str());
        }
        return 0;
    }

    std::string getName() const override {
        const std::uint_least32_t key_hash = MurMur3::calcHash(data_.key.c_str());
        const std::string* name = Fonts().getLocalesText(place_names_domain_hash, key_hash);
        if (name != nullptr) {
            return *name;
        }
        // フォールバック: keyをそのまま返す
        PAXS_WARNING("[PlaceNameFeature] Missing localized name for key: " + data_.key + " - using key as fallback");
        return data_.key;
    }

    std::uint_least32_t getFeatureTypeHash() const override {
        return data_.feature_type_hash;
    }

    /// @brief 時間的更新（ITemporallyUpdatableの実装）
    /// @brief Temporal update (ITemporallyUpdatable implementation)
    void updateTemporal(const TemporalContext& context) override {
        const bool previous_in_time_range = in_time_range_;
        in_time_range_ = isInTimeRange(context.jdn);
        if (!in_time_range_) {
            // 時間範囲外なら可視化しない
            return;
        }
        if (previous_in_time_range == in_time_range_) {
            // 可視性が変化した場合のみ更新
            return;
        }

        updateSpatial(context.toSpatial());
    }

    /// @brief 空間的更新（ISpatiallyUpdatableの実装）
    /// @brief Spatial update (ISpatiallyUpdatable implementation)
    void updateSpatial(const SpatialContext& context) override {
        // 空間フィルタリング：ビューの範囲外の場合はスキップ
        if (!context.isInViewBounds(data_.coordinate)) {
            cached_screen_positions_.clear();
            visible_ = false;
            return;
        }

        // ズームレベルフィルタリング：範囲外の場合はスキップ
        if (data_.zoom_range.excludes(context.map_view_size.y)) {
            cached_screen_positions_.clear();
            visible_ = false;
            return;
        }

        // スクリーン座標に変換（経度ラップ処理付き）
        MapCoordinateConverter::toScreenPositions(
            data_.coordinate,
            context.map_view_size,
            context.map_view_center,
            cached_screen_positions_
        );
        visible_ = true;
    }

    /// @brief ローカライゼーション更新（ILocalizableの実装）
    /// @brief Localization update (ILocalizable implementation)
    void updateLocalization(const LocalizationContext& context) override {
        // 名前を取得してキャッシュ
        cached_name_ = getName();

        // フォントからテキストサイズを計算
        if (context.font != nullptr && !cached_name_.empty()) {
            cached_text_size_ = Vector2<int>(context.font->width(cached_name_), context.font->height());
        } else {
            // フォントが利用できない場合はデフォルト値を使用
            cached_text_size_ = Vector2<int>(100, 20);
        }
        cached_display_size_ = cached_text_size_.y;
    }


    bool isVisible() const override {
        return visible_;
    }

    bool isInTimeRange(double jdn) const override {
        return data_.year_range.contains(jdn);
    }

    const WrappedScreenPositions& getScreenPositions() const override {
        return cached_screen_positions_;
    }

    int getDisplaySize() const override {
        return cached_display_size_;
    }

    bool isHit(const paxs::Vector2<int>& mouse_pos) const override {
        if (!visible_) {
            return false;
        }

        const Vector2<int> text_size = cached_text_size_;

        return MapContentHitTester::testMultiplePositions(
            mouse_pos.x, mouse_pos.y, cached_screen_positions_,
            [text_size](int mouse_x, int mouse_y, const paxs::Vector2<double>& pos) {
                // テキストの矩形判定: drawAtは横方向中央、縦方向上を基準に描画
                const Rect<int> text_rect(
                    static_cast<int>(pos.x) - (text_size.x / 2),
                    static_cast<int>(pos.y),
                    text_size.x,
                    text_size.y
                );
                return text_rect.contains(mouse_x, mouse_y);
            }
        );
    }

    void onClick(const ClickContext& context) override {
        // std::cout << "PlaceName clicked: " << getName() << "\n";
        (void)context;
    }

    const LocationPoint& getData() const {
        return data_;
    }

private:
    LocationPoint data_;
    WrappedScreenPositions cached_screen_positions_;  ///< 経度ラップされたスクリーン座標 / Wrapped screen positions
    int cached_display_size_ = 50;
    Vector2<int> cached_text_size_{100, 20};  ///< キャッシュされたテキストサイズ / Cached text size
    std::string cached_name_;  ///< キャッシュされた名前 / Cached name
    bool visible_ = true;  ///< 可視性 / Visibility
    bool in_time_range_ = true;  ///< 時間範囲内か / Whether within time range

    static constexpr std::uint_least32_t place_names_domain_hash = MurMur3::calcHash("PlaceNames");
};

} // namespace paxs

#endif // !PAX_MAHOROBA_PLACE_NAME_FEATURE_HPP
