/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_LOCATION_MODEL_3D_RENDERER_HPP
#define PAX_MAHOROBA_MAP_LOCATION_MODEL_3D_RENDERER_HPP

#include <PAX_GRAPHICA/3DModel.hpp>

namespace paxs {

    /// @brief 3Dモデル描画クラス
    /// @brief 3D model renderer class
    /// @details 地図上に配置される3Dオブジェクト（古墳の石室など）を描画
    /// @details Renders 3D objects (burial chambers, etc.) placed on the map
    /// @note PAX_GRAPHICA/3DModel.hpp のラッパークラス
    /// @note Wrapper class for PAX_GRAPHICA/3DModel.hpp
    ///
    /// @section integration 統合方針 / Integration Plan
    /// このクラスは Model3DFeature クラス内で使用され、Featureシステムに統合されています。
    /// This class is used within Model3DFeature class and is integrated into the Feature system.
    ///
    /// @subsection usage 使用方法 / Usage
    /// Model3DFeature を通じて使用してください。直接インスタンス化することは推奨されません。
    /// Use through Model3DFeature. Direct instantiation is not recommended.
    ///
    /// @code
    /// // 使用例 / Example usage:
    /// auto feature = std::make_unique<Model3DFeature>(
    ///     "tomb_001",                    // ID
    ///     "石室",                         // 名前 / Name
    ///     MercatorDeg(135.0, 35.0),      // 座標 / Coordinate
    ///     2440000.0,                     // 開始年 / Start year (JDN)
    ///     2450000.0,                     // 終了年 / End year (JDN)
    ///     model_config                   // 3Dモデル設定 / 3D model config
    /// );
    /// @endcode
    ///
    /// @see Model3DFeature
    /// @see MapFeatureRenderer::drawModel3D()
    class Model3DRenderer {
    private:
        paxg::Graphics3DModel model3d_;

    public:
        /// @brief デフォルト設定でコンストラクタ
        Model3DRenderer() = default;

        /// @brief カスタム設定でコンストラクタ
        explicit Model3DRenderer(const paxg::Graphics3DModelConfig& cfg) : model3d_(cfg) {}

        /// @brief 3Dモデルの状態を更新
        /// @brief Update 3D model state
        void update() {
            model3d_.updateRotation();
        }

        /// @brief 3Dモデルを描画
        /// @brief Render 3D model
        void render() const {
            model3d_.render();
        }

        /// @brief 設定を取得
        const paxg::Graphics3DModelConfig& getConfig() const { return model3d_.getConfig(); }

        /// @brief 設定を更新
        void setConfig(const paxg::Graphics3DModelConfig& cfg) { model3d_.setConfig(cfg); }

        /// @brief カメラ設定を更新
        void setCameraConfig(const paxg::Graphics3DModelConfig::Camera& cam) {
            model3d_.setCameraConfig(cam);
        }

        /// @brief 背景色設定を更新
        void setBackgroundConfig(const paxg::Graphics3DModelConfig::Background& bg) {
            model3d_.setBackgroundConfig(bg);
        }

        /// @brief 現在の回転角度を取得（度）
        int getRotation() const {
            return model3d_.getRotation();
        }

        /// @brief 回転角度を設定（度）
        void setRotation(int angle) {
            model3d_.setRotation(angle);
        }
    };

}

#endif // !PAX_MAHOROBA_MAP_LOCATION_MODEL_3D_RENDERER_HPP
