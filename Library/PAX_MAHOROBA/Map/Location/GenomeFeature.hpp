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
#include <PAX_SAPIENTICA/Geography/Coordinate/WrappedScreenPositions.hpp>
#include <PAX_SAPIENTICA/Map/LocationPoint.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>
#include <utility>

namespace paxs {

/// @brief ゲノム情報（人骨、mtDNA、Y-DNA等）を表す地物クラス
/// @brief Feature class representing genome information (human bones, mtDNA, Y-DNA, etc.)
class GenomeFeature : public MapFeature {
public:
    /// @brief コンストラクタ
    /// @brief Constructor
    /// @param data ゲノムの位置データ / Genome location data
    GenomeFeature(LocationPoint data) : data_(std::move(data)) {}

    FeatureType getType() const override {
        return FeatureType::Genome;
    }

    std::uint_least32_t getId() const override {
        // IDとして key の MurMur3 ハッシュを使用
        if (!data_.key.empty()) {
            return MurMur3::calcHash(data_.key.c_str());
        }
        return 0;
    }

    std::string getName(const std::string& language = "ja-JP") const override {
        const std::uint_least32_t lang_hash = MurMur3::calcHash(language.c_str());
        if (data_.names.find(lang_hash) != data_.names.end()) {
            return data_.names.at(lang_hash);
        }
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

        // 空間フィルタリング：ビューの範囲外の場合はスキップ
        if (!context.isInViewBounds(data_.coordinate)) {
            cached_screen_positions_.clear();
            return;
        }

        // ズームレベルフィルタリング：範囲外の場合のみ描画（アイコンのみ）
        if (data_.zoom_range.contains(context.map_view_size.y)) {
            cached_screen_positions_.clear();
            return;
        }

        // 時代フィルタリング：ユリウス日の範囲をチェック
        if (data_.year_range.excludes(context.jdn)) {
            cached_screen_positions_.clear();
            return;
        }

        // スクリーン座標に変換（経度ラップ処理付き）
        MapCoordinateConverter::toScreenPositions(
            data_.coordinate,
            context.map_view_size,
            context.map_view_center,
            cached_screen_positions_
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
        if (should_show_name && !data_.names.empty() && context.font != nullptr) {
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

        const Vector2<int> texture_size = cached_texture_size_;
        const Vector2<int> text_size = cached_text_size_;

        return MapContentHitTester::testMultiplePositions(
            mouse_pos.x, mouse_pos.y, cached_screen_positions_,
            [texture_size, text_size](int mouse_x, int mouse_y, const paxg::Vec2<double>& pos) {
                // テクスチャの矩形判定（中心から描画）
                const Rect<int> texture_rect = Rect<int>::fromCenter(
                    Vector2<int>(static_cast<int>(pos.x()), static_cast<int>(pos.y())),
                    texture_size
                );
                if (texture_rect.contains(mouse_x, mouse_y)) {
                    return true;
                }

                // テキストの矩形判定（drawBottomCenterは横中央、縦は下から描画）
                // text_pos.y = pos.y() - display_size / 2 - 5（アイコンの上部から少し離す）
                // drawBottomCenterなので、そこから上にtext_heightの範囲
                if (text_size.x > 0 && text_size.y > 0) {
                    const int text_bottom_y = static_cast<int>(pos.y()) - (texture_size.y / 2) - 5;  // テキスト下端
                    const Rect<int> text_rect = Rect<int>::fromCenter(
                        Vector2<int>(static_cast<int>(pos.x()), text_bottom_y - (text_size.y / 2)),
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
    WrappedScreenPositions cached_screen_positions_;  ///< 経度ラップされたスクリーン座標 / Wrapped screen positions
    int cached_display_size_ = 0;  ///< キャッシュされた表示サイズ / Cached display size
    Vector2<int> cached_texture_size_{50, 50};  ///< キャッシュされたテクスチャサイズ / Cached texture size
    Vector2<int> cached_text_size_{0, 0};       ///< キャッシュされたテキストサイズ / Cached text size
};

} // namespace paxs

#endif // !PAX_MAHOROBA_GENOME_FEATURE_HPP
