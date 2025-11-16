/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_GENOME_FEATURE_HPP
#define PAX_MAHOROBA_GENOME_FEATURE_HPP

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

#include <PAX_SAPIENTICA/Core/Type/Rect.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/Map/LocationPoint.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

/// @brief ゲノム情報（人骨、mtDNA、Y-DNA等）を表す地物クラス
/// @brief Feature class representing genome information (human bones, mtDNA, Y-DNA, etc.)
class GenomeFeature : public MapFeature {
public:
    /// @brief コンストラクタ
    /// @brief Constructor
    /// @param data ゲノムの位置データ / Genome location data
    GenomeFeature(const LocationPoint& data)
        : data_(data) {
        visible_ = true;
        min_year_ = data.min_year;
        max_year_ = data.max_year;
    }

    FeatureType getType() const override {
        return FeatureType::Genome;
    }

    std::string getId() const override {
        const std::uint_least32_t ja_jp = MurMur3::calcHash("ja-JP");
        if (data_.place_name.find(ja_jp) != data_.place_name.end()) {
            return data_.place_name.at(ja_jp);
        }
        if (!data_.place_name.empty()) {
            return data_.place_name.begin()->second;
        }
        return "unknown_genome";
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
        if ((context.visibility_manager != nullptr) && !context.visibility_manager->isVisible(data_.feature_type_hash)) {
            cached_screen_positions_.clear();
            return;
        }

        // 空間フィルタリング：ビューの範囲外の場合はスキップ
        if (!context.isInViewBounds(data_.coordinate.x, data_.coordinate.y)) {
            cached_screen_positions_.clear();
            return;
        }

        // ズームレベルフィルタリング：範囲外の場合のみ描画（アイコンのみ）
        if (data_.min_zoom_level <= context.map_view_size.y && data_.max_zoom_level >= context.map_view_size.y) {
            cached_screen_positions_.clear();
            return;
        }

        // 時代フィルタリング：ユリウス日の範囲をチェック
        if (data_.min_year > context.jdn || context.jdn > data_.max_year) {
            cached_screen_positions_.clear();
            return;
        }

        // スクリーン座標に変換（経度ラップ処理付き）
        cached_screen_positions_ = MapCoordinateConverter::toScreenPositions(
            data_.coordinate.x, data_.coordinate.y,
            context.map_view_size,
            context.map_view_center
        );

        // 表示サイズの計算（zoom適用）
        cached_display_size_ = static_cast<int>(data_.overall_length / 2 * data_.zoom);

        // テクスチャサイズを取得してキャッシュ
        if (context.texture_map != nullptr && context.texture_map->find(data_.texture_key) != context.texture_map->end()) {
            const auto& tex = context.texture_map->at(data_.texture_key);
            cached_texture_size_ = Vector2<int>(tex.width(), tex.height());
        } else {
            cached_texture_size_ = Vector2<int>(cached_display_size_, cached_display_size_);
        }

        // テキストサイズを計算（表示サイズが十分な場合のみ）
        const bool should_show_name = (cached_display_size_ >= 15);
        if (should_show_name && !data_.place_name.empty() && context.font != nullptr) {
            const std::string name = getName();
            cached_text_size_ = Vector2<int>(context.font->width(name), context.font->height());
        } else {
            cached_text_size_ = Vector2<int>(0, 0);
        }
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
        if (!visible_) {
            return false;
        }

        const Vector2<int> texture_size = cached_texture_size_;
        const Vector2<int> text_size = cached_text_size_;

        return MapContentHitTester::testMultiplePositions(
            mouse_pos.x(), mouse_pos.y(), cached_screen_positions_,
            [texture_size, text_size](int mouse_x, int mouse_y, const paxg::Vec2i& pos) {
                // テクスチャの矩形判定（中心から描画）
                const Rect<int> texture_rect = Rect<int>::fromCenter(
                    Vector2<int>(pos.x(), pos.y()),
                    texture_size
                );
                if (texture_rect.contains(mouse_x, mouse_y)) {
                    return true;
                }

                // テキストの矩形判定（drawBottomCenterは横中央、縦は下から描画）
                // text_pos.y = pos.y() - display_size / 2 - 5（アイコンの上部から少し離す）
                // drawBottomCenterなので、そこから上にtext_heightの範囲
                if (text_size.x > 0 && text_size.y > 0) {
                    const int text_bottom_y = pos.y() - (texture_size.y / 2) - 5;  // テキスト下端
                    const Rect<int> text_rect = Rect<int>::fromCenter(
                        Vector2<int>(pos.x(), text_bottom_y - (text_size.y / 2)),
                        text_size
                    );
                    if (text_rect.contains(mouse_x, mouse_y)) {
                        return true;
                    }
                }

                return false;
            }
        );
    }

    void onClick(const ClickContext& context) override {
        std::cout << "Genome feature clicked: " << getName() << "\n";
        (void)context;
    }

    const LocationPoint& getData() const {
        return data_;
    }

private:
    LocationPoint data_;  ///< ゲノムデータ / Genome data
    std::vector<paxg::Vec2i> cached_screen_positions_;  ///< キャッシュされた画面座標 / Cached screen positions
    int cached_display_size_ = 0;  ///< キャッシュされた表示サイズ / Cached display size
    Vector2<int> cached_texture_size_{50, 50};  ///< キャッシュされたテクスチャサイズ / Cached texture size
    Vector2<int> cached_text_size_{0, 0};       ///< キャッシュされたテキストサイズ / Cached text size
};

} // namespace paxs

#endif // !PAX_MAHOROBA_GENOME_FEATURE_HPP
