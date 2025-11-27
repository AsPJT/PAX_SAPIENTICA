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
#include <utility>

#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Map/Content/Feature/FeatureType.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/MapFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Interaction/ClickContext.hpp>
#include <PAX_MAHOROBA/Map/Content/Interaction/MapContentHitTester.hpp>
#include <PAX_MAHOROBA/Map/Content/Update/IUpdatable.hpp>
#include <PAX_MAHOROBA/Map/Content/Update/UpdateContext.hpp>
#include <PAX_MAHOROBA/Map/Core/MapCoordinateConverter.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

#include <PAX_SAPIENTICA/Core/Type/Range.hpp>
#include <PAX_SAPIENTICA/Core/Type/Rect.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/WrappedScreenPositions.hpp>
#include <PAX_SAPIENTICA/Map/PersonLocationPoint.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

/// @brief 人物（肖像画+名前）を表す地物クラス
/// @brief Feature class representing a person (portrait + name)
class PersonFeature : public MapFeature,
                      public ISpatiallyUpdatable,
                      public ITemporallyUpdatable,
                      public ILocalizable {
public:
    /// @param data 人物の位置データ / Person location data
    /// @param group_data 人物グループデータ / Person group data
    PersonFeature(PersonLocationPoint  data, PersonLocationGroup  group_data)
        : data_(std::move(data))
        , group_data_(std::move(group_data)) {

    }

    FeatureType getType() const override {
        return FeatureType::Person;
    }

    std::uint_least32_t getId() const override {
        // IDとして key の MurMur3 ハッシュを使用
        if (!data_.key.empty()) {
            return MurMur3::calcHash(data_.key.c_str());
        }
        return 0;
    }

    std::string getName() const override {
        // Localesシステムから取得
        const std::uint_least32_t key_hash = MurMur3::calcHash(data_.key.c_str());
        const std::string* name = paxs::FontSystem::getInstance().getLocalesText(person_names_domain_hash, key_hash);
        if (name != nullptr) {
            return *name;
        }
        PAXS_WARNING("[PersonFeature] getName FAILED: key=" + data_.key + " - returning nullptr from Locales");
        return "";
    }

    std::uint_least32_t getFeatureTypeHash() const override {
        return data_.feature_type_hash;
    }

    /// @brief 時間的更新（ITemporallyUpdatableの実装）
    /// @brief Temporal update (ITemporallyUpdatable implementation)
    void updateTemporal(const TemporalContext& context) override {
        if (!isInTimeRange(context.jdn)) {
            return;
        }
        // 時間補間座標の計算
        interpolated_pos_ = MapCoordinateConverter::interpolatePosition(
            data_.start_coordinate, data_.end_coordinate,
            context.jdn, data_.year_range.minimum, data_.year_range.maximum
        );
        cached_jdn_ = context.jdn;

        updateSpatial(context.toSpatial());
    }

    /// @brief 空間的更新（ISpatiallyUpdatableの実装）
    /// @brief Spatial update (ISpatiallyUpdatable implementation)
    void updateSpatial(const SpatialContext& context) override {
        // 空間フィルタリング：ビューの範囲外の場合はスキップ（補間済み座標を使用）
        if (!context.isInViewBounds(interpolated_pos_)) {
            cached_screen_positions_.clear();
            visible_ = false;
            return;
        }

        // スクリーン座標に変換（経度ラップ処理付き）
        MapCoordinateConverter::toScreenPositions(
            interpolated_pos_,
            context.map_view_size,
            context.map_view_center,
            cached_screen_positions_
        );

        // 表示サイズの計算（人物は常に通常サイズで表示）
        cached_display_size_ = 60;

        // テクスチャサイズを描画時のサイズに合わせる（MapFeatureRenderer.hppと同じ）
        // 描画時は resizedDrawAt(120) を使用するため、当たり判定は70×110
        cached_texture_size_ = Vector2<int>(70, 110);

        visible_ = true;
    }

    /// @brief ローカライゼーション更新（ILocalizableの実装）
    /// @brief Localization update (ILocalizable implementation)
    void updateLocalization(const LocalizationContext& context) override {
        // 名前を取得（言語に応じて変わる）
        cached_name_ = getName();

        // フォント幅を計算（ズーム範囲内の場合のみ）
        const bool out_of_range = data_.zoom_range.excludes(cached_display_size_);
        if (!out_of_range && context.font != nullptr && !cached_name_.empty()) {
            cached_text_size_ = Vector2<int>(context.font->width(cached_name_), context.font->height());
        } else {
            cached_text_size_ = Vector2<int>(0, 0);
        }
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
        if (!visible_) return false;

        const Vector2<int> texture_size = cached_texture_size_;
        const Vector2<int> text_size = cached_text_size_;

        // 3つのスクリーン座標でヒット判定（テクスチャ + テキスト）
        return MapContentHitTester::testMultiplePositions(
            mouse_pos.x, mouse_pos.y, cached_screen_positions_,
            [texture_size, text_size](int mx, int my, const paxg::Vec2<double>& pos) {
                // テクスチャの矩形判定（中心から描画）
                const Rect<int> texture_rect = Rect<int>::fromCenter(
                    Vector2<int>(static_cast<int>(pos.x()), static_cast<int>(pos.y())),
                    texture_size
                );
                if (texture_rect.contains(mx, my)) {
                    return true;
                }
                // テキストの矩形判定（drawTopCenterは横中央、縦は上から描画）
                // draw_font_pos = {x, y - 60}から描画されるため、
                // 実際のテキスト位置は pos.y() - 60 から text_height の範囲
                if (text_size.x > 0 && text_size.y > 0) {
                    const int text_y = static_cast<int>(pos.y()) - 60;  // テクスチャの上部（MapFeatureRenderer参照）
                    const Rect<int> text_rect(
                        static_cast<int>(pos.x()) - (text_size.x / 2),
                        text_y,
                        text_size.x,
                        text_size.y
                    );
                    if (text_rect.contains(mx, my)) {
                        return true;
                    }
                }
                return false;
            }
        );
    }

    void onClick(const ClickContext& context) override {
        // クリック時の処理（デバッグ出力）
        std::cout << "Person clicked: " << getName() << std::endl;
        (void)context;  // 未使用警告を抑制
    }


    WebMercatorDeg getInterpolatedPosition() const {
        return interpolated_pos_;
    }

    const PersonLocationPoint& getData() const {
        return data_;
    }

    const PersonLocationGroup& getGroupData() const {
        return group_data_;
    }

private:
    static constexpr std::uint_least32_t person_names_domain_hash = MurMur3::calcHash("PersonNames");

    PersonLocationPoint data_;           ///< 人物位置データ / Person location data
    PersonLocationGroup group_data_;     ///< 人物グループデータ / Person group data

    WebMercatorDeg interpolated_pos_;                    ///< 補間された座標 / Interpolated position
    WrappedScreenPositions cached_screen_positions_;  ///< 経度ラップされたスクリーン座標 / Wrapped screen positions
    int cached_display_size_ = 60;                    ///< 表示サイズ / Display size
    Vector2<int> cached_texture_size_{60, 60};        ///< キャッシュされたテクスチャサイズ / Cached texture size
    Vector2<int> cached_text_size_{0, 0};             ///< キャッシュされたテキストサイズ / Cached text size
    std::string cached_name_;                         ///< キャッシュされた名前 / Cached name
    double cached_jdn_ = 0.0;                         ///< キャッシュされたJDN / Cached JDN
    bool visible_ = true;                             ///< 可視性 / Visibility
};

} // namespace paxs

#endif // !PAX_MAHOROBA_PERSON_FEATURE_HPP
