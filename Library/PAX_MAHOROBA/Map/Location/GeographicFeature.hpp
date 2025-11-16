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

/// @brief 地理的地物（アイコン）を表す地物クラス
/// @brief Feature class representing a geographic feature (icon)
class GeographicFeature : public MapFeature {
public:
    /// @brief コンストラクタ
    /// @brief Constructor
    /// @param data 地物の位置データ / Geographic feature location data
    GeographicFeature(const LocationPoint& data)
        : data_(data) {
        visible_ = true;
        min_year_ = data.min_year;
        max_year_ = data.max_year;
    }

    // ========== 基本情報 / Basic Information ==========

    FeatureType getType() const override {
        return FeatureType::Geographic;
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

    // ========== 状態管理 / State Management ==========

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
    }

    bool isVisible() const override {
        return visible_;
    }

    bool isInTimeRange(double jdn) const override {
        return jdn >= min_year_ && jdn <= max_year_;
    }

    // ========== 座標・描画 / Coordinates & Rendering ==========

    std::vector<paxg::Vec2i> getScreenPositions() const override {
        return cached_screen_positions_;
    }

    int getDisplaySize() const override {
        return cached_display_size_;
    }

    // ========== ヒット判定 / Hit Testing ==========

    bool isHit(const paxg::Vec2i& mouse_pos) const override {
        if (!visible_) {
            return false;
        }

        const Vector2<int> texture_size = cached_texture_size_;

        return MapContentHitTester::testMultiplePositions(
            mouse_pos.x(), mouse_pos.y(), cached_screen_positions_,
            [texture_size](int mouse_x, int mouse_y, const paxg::Vec2i& pos) {
                // テクスチャの矩形判定（中心から描画）
                const Rect<int> texture_rect = Rect<int>::fromCenter(
                    Vector2<int>(pos.x(), pos.y()),
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

    /// @brief 複数タイルにまたがるかチェック
    /// @brief Check if feature spans multiple tiles
    bool hasMultipleTiles() const {
        return data_.x_size > 1 || data_.y_size > 1;
    }

    /// @brief タイルサイズを取得
    /// @brief Get tile size
    Vector2<std::uint_least16_t> getTileSize() const {
        return Vector2<std::uint_least16_t>(data_.x_size, data_.y_size);
    }

    /// @brief 元のデータを取得
    /// @brief Get original data
    const LocationPoint& getData() const {
        return data_;
    }

private:
    LocationPoint data_;                               ///< 地物位置データ / Geographic feature location data

    // キャッシュされた状態 / Cached state
    std::vector<paxg::Vec2i> cached_screen_positions_; ///< スクリーン座標（3つ） / Screen positions (3)
    int cached_display_size_ = 50;                     ///< 表示サイズ / Display size
    Vector2<int> cached_texture_size_{50, 50};         ///< キャッシュされたテクスチャサイズ / Cached texture size
};

} // namespace paxs

#endif // !PAX_MAHOROBA_GEOGRAPHIC_FEATURE_HPP
