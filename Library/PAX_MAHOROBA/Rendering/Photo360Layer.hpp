/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_RENDERING_PHOTO_360_LAYER_HPP
#define PAX_MAHOROBA_RENDERING_PHOTO_360_LAYER_HPP

#include <PAX_GRAPHICA/Photo360.hpp>

#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>

#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>

namespace paxs {

    /// @brief 360度写真描画レイヤー
    /// @details UIレイヤーの次に描画される全画面没入型ビューア
    class Photo360Layer : public IRenderable {
    private:
        mutable paxg::Photo360 photo360_;
        const FeatureVisibilityManager& visibility_manager_;

    public:
        Photo360Layer(const FeatureVisibilityManager& visibility_manager)
            : visibility_manager_(visibility_manager) {}

        /// @brief 360度写真を更新・描画
        void render() const override {
            photo360_.updateAndRender();
        }

        /// @brief 設定を取得
        const paxg::Photo360Config& getConfig() const { return photo360_.getConfig(); }

        /// @brief 設定を更新
        void setConfig(const paxg::Photo360Config& cfg) { photo360_.setConfig(cfg); }

        /// @brief カメラ設定を更新
        void setCameraConfig(const paxg::Photo360Config::Camera& cam) {
            photo360_.setCameraConfig(cam);
        }

        /// @brief 球体設定を更新
        void setSphereConfig(const paxg::Photo360Config::Sphere& sph) {
            photo360_.setSphereConfig(sph);
        }

        /// @brief 背景色設定を更新
        void setBackgroundConfig(const paxg::Photo360Config::Background& background) {
            photo360_.setBackgroundConfig(background);
        }

        /// @brief 現在の視点回転角度を取得（度）
        void getViewRotation(double& x, double& y, double& z) const {
            photo360_.getViewRotation(x, y, z);
        }

        /// @brief 視点回転角度を設定（度）
        void setViewRotation(double x, double y, double z) {
            photo360_.setViewRotation(x, y, z);
        }

        RenderLayer getLayer() const override { return RenderLayer::Photo360; }
        bool isVisible() const override { return visibility_manager_.isVisible(ViewMenu::view_3d); }
    };

}

#endif // !PAX_MAHOROBA_RENDERING_PHOTO_360_LAYER_HPP
