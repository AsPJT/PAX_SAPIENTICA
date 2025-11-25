/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_GEOGRAPHIC_FEATURE_HPP
#define PAX_MAHOROBA_GEOGRAPHIC_FEATURE_HPP

#include <string>

#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Map/Content/Feature/FeatureType.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/MapFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Interaction/ClickContext.hpp>
#include <PAX_MAHOROBA/Map/Content/Interaction/MapContentHitTester.hpp>
#include <PAX_MAHOROBA/Map/Content/Update/IUpdatable.hpp>
#include <PAX_MAHOROBA/Map/Content/Update/UpdateContext.hpp>
#include <PAX_MAHOROBA/Map/Core/MapCoordinateConverter.hpp>

#include <PAX_SAPIENTICA/Core/Type/Rect.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/WrappedScreenPositions.hpp>
#include <PAX_SAPIENTICA/Map/LocationPoint.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>
#include <PAX_SAPIENTICA/Utility/StringUtils.hpp>

namespace paxs {

/// @brief 地理的地物（アイコン）を表す地物クラス
/// @brief Feature class representing a geographic feature (icon)
/// @details 空間更新のみ必要（時間・ローカライゼーションは不要）
class GeographicFeature : public MapFeature, public ISpatiallyUpdatable {
public:
    /// @brief コンストラクタ
    /// @brief Constructor
    /// @param data 地物の位置データ / Geographic feature location data
    GeographicFeature(const LocationPoint& data)
        : data_(data) {
        visible_ = true;
    }

    // ========== 基本情報 / Basic Information ==========

    FeatureType getType() const override {
        return FeatureType::Geographic;
    }

    std::uint_least32_t getId() const override {
        // IDとして key の MurMur3 ハッシュを使用
        if (!data_.key.empty()) {
            return MurMur3::calcHash(data_.key.c_str());
        }
        return 0;
    }

    std::string getName() const override {
        // Geographic features do not have names - they are represented by icons only
        // Future: If geographic features need names, uncomment below and create GeographicFeatureNames domain
        /*
        const std::uint_least32_t geographic_feature_names_domain_hash = MurMur3::calcHash("GeographicFeatureNames");
        const std::uint_least32_t key_hash = MurMur3::calcHash(data_.key.c_str());
        const std::string* name = Fonts().getLocalesText(geographic_feature_names_domain_hash, key_hash);
        if (name != nullptr) {
            return *name;
        }
        */
        return data_.key;
    }

    std::uint_least32_t getFeatureTypeHash() const override {
        return data_.feature_type_hash;
    }

    // ========== 状態管理 / State Management ==========

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

        // 表示サイズの計算（zoom適用）
        cached_display_size_ = static_cast<int>(data_.overall_length / 2 * data_.zoom);

        // テクスチャサイズを描画時のサイズに合わせる（MapFeatureRenderer.hppと同じ）
        // 描画時は resizedDrawAt(display_size) を使用するため、当たり判定もdisplay_sizeに合わせる
        cached_texture_size_ = Vector2<int>(cached_display_size_, cached_display_size_);

        visible_ = true;
    }


    bool isVisible() const override {
        return visible_;
    }

    bool isInTimeRange(double jdn) const override {
        return data_.year_range.contains(jdn);
    }

    // ========== 座標・描画 / Coordinates & Rendering ==========

    const WrappedScreenPositions& getScreenPositions() const override {
        return cached_screen_positions_;
    }

    int getDisplaySize() const override {
        return cached_display_size_;
    }

    // ========== ヒット判定 / Hit Testing ==========

    bool isHit(const paxs::Vector2<int>& mouse_pos) const override {
        if (!visible_) {
            return false;
        }

        const Vector2<int> texture_size = cached_texture_size_;

        return MapContentHitTester::testMultiplePositions(
            mouse_pos.x, mouse_pos.y, cached_screen_positions_,
            [texture_size](int mouse_x, int mouse_y, const paxg::Vec2<double>& pos) {
                // テクスチャの矩形判定（中心から描画）
                const Rect<int> texture_rect = Rect<int>::fromCenter(
                    Vector2<int>(static_cast<int>(pos.x()), static_cast<int>(pos.y())),
                    texture_size
                );
                return texture_rect.contains(mouse_x, mouse_y);
            }
        );
    }

    // ========== イベント処理 / Event Handling ==========

    void onClick(const ClickContext& context) override {
        std::cout << "Geographic feature clicked: " << getName() << "\n";
        (void)context;
    }

    // ========== Geographic 固有のアクセサ / Geographic-specific Accessors ==========

    /// @brief 描画個数を取得（extra_dataから）
    /// @brief Get draw count from extra_data
    int getDrawCount() const {
        constexpr std::uint_least32_t draw_count_hash = MurMur3::calcHash("draw_count");
        if (data_.hasExtraData(draw_count_hash)) {
            const std::string& value = data_.getExtraData(draw_count_hash);
            auto count = StringUtils::toInt(value);
            return count.has_value() && *count > 0 ? *count : 1;
        }
        return 1;
    }

    /// @brief 元のデータを取得
    /// @brief Get original data
    const LocationPoint& getData() const {
        return data_;
    }

private:
    LocationPoint data_;                               ///< 地物位置データ / Geographic feature location data

    // キャッシュされた状態 / Cached state
    WrappedScreenPositions cached_screen_positions_; ///< 経度ラップされたスクリーン座標 / Wrapped screen positions
    int cached_display_size_ = 50;                     ///< 表示サイズ / Display size
    Vector2<int> cached_texture_size_{50, 50};         ///< キャッシュされたテクスチャサイズ / Cached texture size
    bool visible_ = true;                              ///< 可視性 / Visibility
};

} // namespace paxs

#endif // !PAX_MAHOROBA_GEOGRAPHIC_FEATURE_HPP
