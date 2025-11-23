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

#include <PAX_SAPIENTICA/Core/Type/Rect.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/WrappedScreenPositions.hpp>
#include <PAX_SAPIENTICA/Map/LocationPoint.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>
namespace paxs {

/// @brief ゲノム情報（人骨、mtDNA、Y-DNA等）を表す地物クラス
/// @brief Feature class representing genome information (human bones, mtDNA, Y-DNA, etc.)
/// @details 空間更新とローカライゼーション更新が必要（時間更新は不要）
class GenomeFeature : public MapFeature, public ISpatiallyUpdatable, public ILocalizable {
private:
    static constexpr std::uint_least32_t place_info_domain_hash = MurMur3::calcHash("GenomeNames");

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

    std::string getName() const override {
        // data_.keyをハッシュ化してLocalesシステムから名前を取得
         const std::uint_least32_t key_hash = MurMur3::calcHash(data_.key.c_str());
         const std::string* name = Fonts().getLocalesText(place_info_domain_hash, key_hash, true);
         if (name != nullptr) {
             return *name;
         }
        return "";
    }

    std::uint_least32_t getFeatureTypeHash() const override {
        return data_.feature_type_hash;
    }

    /// @brief 空間的更新（ISpatiallyUpdatableの実装）
    /// @brief Spatial update (ISpatiallyUpdatable implementation)
    void updateSpatial(const SpatialContext& context) override {
        // 地物種別の可視性チェック（最優先）
        if ((context.visibility_manager != nullptr) && !context.visibility_manager->isVisible(data_.feature_type_hash)) {
            cached_screen_positions_.clear();
            visible_ = false;
            return;
        }

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

        // 表示サイズの計算（zoom適用）
        cached_display_size_ = static_cast<int>(data_.overall_length / 2 * data_.zoom);

        // テクスチャサイズを描画時のサイズに合わせる（MapFeatureRenderer.hppと同じ）
        // 描画時は resizedDrawAt(display_size) を使用するため、当たり判定もdisplay_sizeに合わせる
        cached_texture_size_ = Vector2<int>(cached_display_size_, cached_display_size_);

        visible_ = true;
    }

    /// @brief ローカライゼーション更新（ILocalizableの実装）
    /// @brief Localization update (ILocalizable implementation)
    void updateLocalization(const LocalizationContext& context) override {
        // 名前を取得してキャッシュ
        cached_name_ = getName();

        // テキストサイズを計算（表示サイズが十分な場合のみ）
        const bool should_show_name = (cached_display_size_ >= 15);
        if (should_show_name && context.font != nullptr && !cached_name_.empty()) {
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
    std::string cached_name_;  ///< キャッシュされた名前 / Cached name
    bool visible_ = true;  ///< 可視性 / Visibility
};

} // namespace paxs

#endif // !PAX_MAHOROBA_GENOME_FEATURE_HPP
