/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MODEL_3D_FEATURE_HPP
#define PAX_MAHOROBA_MODEL_3D_FEATURE_HPP

#include <string>
#include <vector>

#include <PAX_GRAPHICA/3DModel.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Map/Location/ClickContext.hpp>
#include <PAX_MAHOROBA/Map/Location/FeatureType.hpp>
#include <PAX_MAHOROBA/Map/Location/MapContentHitTester.hpp>
#include <PAX_MAHOROBA/Map/Location/MapCoordinateConverter.hpp>
#include <PAX_MAHOROBA/Map/Location/MapFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/Model3DRenderer.hpp>
#include <PAX_MAHOROBA/Map/Location/RenderContext.hpp>

#include <PAX_SAPIENTICA/Core/Type/Range.hpp>
#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/WrappedScreenPositions.hpp>
#include <PAX_SAPIENTICA/Map/LocationPoint.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

/// @brief 3Dモデル地物を表すクラス
/// @brief Feature class representing a 3D model (burial chambers, buildings, etc.)
/// @note 将来の実装: 古墳の石室、建物などの3Dオブジェクトを地図上に配置
/// @note Future implementation: Place 3D objects like burial chambers, buildings on the map
class Model3DFeature : public MapFeature {
public:
    /// @brief コンストラクタ
    /// @brief Constructor
    /// @param data 地物の位置データ / Feature location data
    /// @param model_config 3Dモデルの設定 / 3D model configuration
    Model3DFeature(const LocationPoint& data, const paxg::Graphics3DModelConfig& model_config)
        : data_(data)
        , renderer_(model_config)
    {
        visible_ = true;
    }

    // ========== 基本情報 / Basic Information ==========

    FeatureType getType() const override {
        return FeatureType::Model3D;
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
        const auto iterator = data_.names.find(lang_hash);
        if (iterator != data_.names.end()) {
            return iterator->second;
        }
        if (!data_.names.empty()) {
            return data_.names.begin()->second;
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
        if (!context.isInViewBounds(data_.coordinate)) {
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

        // 3Dモデルの状態を更新（回転など）
        renderer_.update();
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
        return 100;  // 3Dモデルの表示サイズ（仮）
    }

    // ========== ヒット判定 / Hit Testing ==========

    bool isHit(const paxs::Vector2<int>& mouse_pos) const override {
        if (!visible_) return false;

        const int hit_radius = getDisplaySize() / 2;

        return MapContentHitTester::testMultiplePositions(
            mouse_pos.x, mouse_pos.y, cached_screen_positions_,
            [hit_radius](int mx, int my, const paxg::Vec2<double>& pos) {
                return MapContentHitTester::circleHitTest(mx, my, pos, hit_radius);
            }
        );
    }

    // ========== イベント処理 / Event Handling ==========

    void onClick(const ClickContext& context) override {
        std::cout << "3D Model clicked: " << getName() << std::endl;
        (void)context;
    }

    // ========== Model3D 固有のアクセサ / Model3D-specific Accessors ==========

    /// @brief 3Dモデルレンダラーを取得
    /// @brief Get 3D model renderer
    Model3DRenderer& getRenderer() {
        return renderer_;
    }

    /// @brief 3Dモデルレンダラーを取得（const版）
    /// @brief Get 3D model renderer (const version)
    const Model3DRenderer& getRenderer() const {
        return renderer_;
    }

    /// @brief 元のデータを取得
    /// @brief Get original data
    const LocationPoint& getData() const {
        return data_;
    }

private:
    LocationPoint data_;                               ///< 地物位置データ / Feature location data
    Model3DRenderer renderer_;                         ///< 3Dモデルレンダラー / 3D model renderer

    // キャッシュされた状態 / Cached state
    WrappedScreenPositions cached_screen_positions_;  ///< 経度ラップされたスクリーン座標 / Wrapped screen positions
};

} // namespace paxs

#endif // !PAX_MAHOROBA_MODEL_3D_FEATURE_HPP
