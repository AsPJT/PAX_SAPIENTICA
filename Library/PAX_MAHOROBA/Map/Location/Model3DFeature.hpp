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

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
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
    /// @param id 地物のID / Feature ID
    /// @param name 地物の名前 / Feature name
    /// @param coordinate 位置（メルカトル座標） / Position (Mercator coordinates)
    /// @param min_year 開始年（ユリウス日） / Start year (Julian Day Number)
    /// @param max_year 終了年（ユリウス日） / End year (Julian Day Number)
    /// @param model_config 3Dモデルの設定 / 3D model configuration
    /// @param feature_type_hash 地物種別（デフォルト: "model_3d"） / Feature type identifier (default: "model_3d")
    Model3DFeature(
        const std::string& id,
        const std::string& name,
        const MercatorDeg& coordinate,
        double min_year,
        double max_year,
        const paxg::Graphics3DModelConfig& model_config,
        std::uint_least32_t feature_type_hash = MurMur3::calcHash("model_3d")
    )
        : id_(id)
        , name_(name)
        , coordinate_(coordinate)
        , feature_type_hash_(feature_type_hash)
        , renderer_(model_config)
    {
        visible_ = true;
        min_year_ = min_year;
        max_year_ = max_year;
    }

    // ========== 基本情報 / Basic Information ==========

    FeatureType getType() const override {
        return FeatureType::Model3D;
    }

    std::string getId() const override {
        return id_;
    }

    std::string getName(const std::string& /*language*/ = "ja-JP") const override {
        return name_;
    }

    std::uint_least32_t getFeatureTypeHash() const override {
        return feature_type_hash_;
    }

    // ========== 状態管理 / State Management ==========

    void update(const RenderContext& context) override {
        // 地物種別の可視性チェック（最優先）
        if (context.visibility_manager && !context.visibility_manager->isVisible(feature_type_hash_)) {
            cached_screen_positions_.clear();
            return;
        }
        // 空間フィルタリング：ビューの範囲外の場合はスキップ
        if (!context.isInViewBounds(coordinate_.x, coordinate_.y)) {
            cached_screen_positions_.clear();
            return;
        }

        // スクリーン座標に変換（経度ラップ処理付き）
        cached_screen_positions_ = MapCoordinateConverter::toScreenPositions(
            coordinate_.x, coordinate_.y,
            context.map_view_size,
            context.map_view_center
        );

        // 3Dモデルの状態を更新（回転など）
        renderer_.update();
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
        return 100;  // 3Dモデルの表示サイズ（仮）
    }

    // ========== ヒット判定 / Hit Testing ==========

    bool isHit(const paxg::Vec2i& mouse_pos) const override {
        if (!visible_) return false;

        const int hit_radius = getDisplaySize() / 2;

        return MapContentHitTester::testMultiplePositions(
            mouse_pos.x(), mouse_pos.y(), cached_screen_positions_,
            [hit_radius](int mx, int my, const paxg::Vec2i& pos) {
                return MapContentHitTester::circleHitTest(mx, my, pos, hit_radius);
            }
        );
    }

    // ========== イベント処理 / Event Handling ==========

    void onClick(const ClickContext& context) override {
        std::cout << "3D Model clicked: " << name_ << std::endl;
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

    /// @brief 元の座標を取得
    /// @brief Get original coordinate
    const MercatorDeg& getCoordinate() const {
        return coordinate_;
    }

private:
    std::string id_;                                   ///< 地物ID / Feature ID
    std::string name_;                                 ///< 地物名 / Feature name
    MercatorDeg coordinate_;                           ///< 位置（メルカトル座標） / Position (Mercator)
    std::uint_least32_t feature_type_hash_;            ///< 地物の種別を識別するハッシュ値 / Feature type hash identifier
    Model3DRenderer renderer_;                         ///< 3Dモデルレンダラー / 3D model renderer

    // キャッシュされた状態 / Cached state
    std::vector<paxg::Vec2i> cached_screen_positions_; ///< スクリーン座標（3つ） / Screen positions (3)
};

} // namespace paxs

#endif // !PAX_MAHOROBA_MODEL_3D_FEATURE_HPP
