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

#include <PAX_GRAPHICA/3DModel.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Map/Content/Feature/FeatureType.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/MapFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Interaction/ClickContext.hpp>
#include <PAX_MAHOROBA/Map/Content/Interaction/MapContentHitTester.hpp>
#include <PAX_MAHOROBA/Map/Content/Renderer/Model3DRenderer.hpp>
#include <PAX_MAHOROBA/Map/Core/MapCoordinateConverter.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

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
private:
    static constexpr std::uint_least32_t place_info_domain_hash = MurMur3::calcHash("Model3DNames");

public:
    /// @brief コンストラクタ
    /// @brief Constructor
    /// @param data 地物の位置データ / Feature location data
    /// @param model_config 3Dモデルの設定 / 3D model configuration
    Model3DFeature(const LocationPoint& data, const paxg::Graphics3DModelConfig& model_config)
        : data_(data)
        , renderer_(model_config)
    {
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

    std::string getName() const override {
        // data_.keyをハッシュ化してLocalesシステムから名前を取得
        // const std::uint_least32_t key_hash = MurMur3::calcHash(data_.key.c_str());
        // const std::string* name = Fonts().getLocalesText(place_info_domain_hash, key_hash);
        // if (name != nullptr) {
        //     return *name;
        // }
        // フォールバック: keyをそのまま返す
        return data_.key;
    }

    std::uint_least32_t getFeatureTypeHash() const override {
        return data_.feature_type_hash;
    }

    // ========== 状態管理 / State Management ==========

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
    bool visible_ = true;                              ///< 可視性 / Visibility
};

} // namespace paxs

#endif // !PAX_MAHOROBA_MODEL_3D_FEATURE_HPP
