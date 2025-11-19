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

#include <PAX_SAPIENTICA/Core/Type/Range.hpp>
#include <PAX_SAPIENTICA/Core/Type/Rect.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/WrappedScreenPositions.hpp>
#include <PAX_SAPIENTICA/Map/PersonLocationPoint.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

namespace paxs {

/// @brief 人物（肖像画+名前）を表す地物クラス
/// @brief Feature class representing a person (portrait + name)
class PersonFeature : public MapFeature {
public:
    /// @param data 人物の位置データ / Person location data
    /// @param group_data 人物グループデータ / Person group data
    PersonFeature(const PersonLocationPoint& data, const PersonLocationGroup& group_data)
        : data_(data)
        , group_data_(group_data) {
        visible_ = true;
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

    std::string getName(const std::string& language = "ja-JP") const override {
        // Localesシステムから取得
        const std::uint_least32_t key_hash = MurMur3::calcHash(data_.key.c_str());
        const std::string* name = paxs::FontSystem::getInstance().getLocalesText(person_names_domain_hash, key_hash);
        if (name != nullptr) {
            return *name;
        }
        PAXS_WARNING("[PersonFeature] getName FAILED: key=" + data_.key + ", language=" + language + " - returning nullptr from Locales");

        // TODO: Localesシステムへの完全移行後、以下のフォールバックコードは削除可能
        // フォールバック: 既存のnamesマップから取得
        const std::uint_least32_t lang_hash = MurMur3::calcHash(language.c_str());
        const auto iterator = data_.names.find(lang_hash);
        if (iterator != data_.names.end()) {
            return iterator->second;
        }
        // フォールバック: 最初の名前を返す
        if (!data_.names.empty()) {
            return data_.names.begin()->second;
        }
        return "";
    }

    std::uint_least32_t getFeatureTypeHash() const override {
        return data_.feature_type_hash;
    }

    void update(const RenderContext& context) override {
        // 地物種別の可視性チェック（最優先）
        if ((context.visibility_manager != nullptr) && !context.visibility_manager->isVisible(data_.feature_type_hash)) {
            cached_screen_positions_.clear();
            return;
        }

        // 時間補間座標の計算
        interpolated_pos_ = MapCoordinateConverter::interpolatePosition(
            data_.start_coordinate, data_.end_coordinate,
            context.jdn, data_.year_range.minimum, data_.year_range.maximum
        );

        // 空間フィルタリング：ビューの範囲外の場合はスキップ
        if (!context.isInViewBounds(interpolated_pos_)) {
            cached_screen_positions_.clear();
            return;
        }

        // スクリーン座標に変換（経度ラップ処理付き）
        MapCoordinateConverter::toScreenPositions(
            interpolated_pos_,
            context.map_view_size,
            context.map_view_center,
            cached_screen_positions_
        );

        // 表示サイズの計算
        const bool out_of_range = data_.zoom_range.excludes(context.map_view_size.x);
        cached_display_size_ = out_of_range
            ? 5  // 範囲外時の小さい表示サイズ
            : 60;  // 120 / 2

        // テクスチャサイズとテキストサイズを取得
        const std::uint_least32_t tex_key = (data_.texture_key == 0) ? group_data_.texture_key : data_.texture_key;
        if (context.texture_map != nullptr) {
            const auto iterator = context.texture_map->find(tex_key);
            if (iterator != context.texture_map->end()) {
                const auto& tex = iterator->second;
                cached_texture_size_ = Vector2<int>(tex.width(), tex.height());
            } else {
                cached_texture_size_ = Vector2<int>(cached_display_size_, cached_display_size_);
            }
        } else {
            cached_texture_size_ = Vector2<int>(cached_display_size_, cached_display_size_);
        }

        if (!out_of_range && context.font != nullptr) {
            const std::string name = getName();
            cached_text_size_ = Vector2<int>(context.font->width(name), context.font->height());
        } else {
            cached_text_size_ = Vector2<int>(0, 0);
        }

        cached_jdn_ = context.jdn;
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


    MercatorDeg getInterpolatedPosition() const {
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

    MercatorDeg interpolated_pos_;                    ///< 補間された座標 / Interpolated position
    WrappedScreenPositions cached_screen_positions_;  ///< 経度ラップされたスクリーン座標 / Wrapped screen positions
    int cached_display_size_ = 60;                    ///< 表示サイズ / Display size
    Vector2<int> cached_texture_size_{60, 60};        ///< キャッシュされたテクスチャサイズ / Cached texture size
    Vector2<int> cached_text_size_{0, 0};             ///< キャッシュされたテキストサイズ / Cached text size
    double cached_jdn_ = 0.0;                         ///< キャッシュされたJDN / Cached JDN
};

} // namespace paxs

#endif // !PAX_MAHOROBA_PERSON_FEATURE_HPP
