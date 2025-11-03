/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_RENDERING_PHOTO_360_LAYER_HPP
#define PAX_MAHOROBA_RENDERING_PHOTO_360_LAYER_HPP

#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>
#include <PAX_GRAPHICA/Photo360.hpp>

namespace paxs {

    /// @brief 360度写真描画レイヤー
    /// @details UIレイヤーの次に描画される全画面没入型ビューア
    /// PAX_GRAPHICA/Photo360.hpp のラッパークラス
    class Photo360Layer : public IRenderable {
    private:
        mutable paxg::Photo360 photo360_;
        bool visible_ = true;

    public:
        /// @brief デフォルト設定でコンストラクタ
        Photo360Layer() = default;

        /// @brief カスタム設定でコンストラクタ
        explicit Photo360Layer(const paxg::Photo360Config& cfg) : photo360_(cfg) {}

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
        void setBackgroundConfig(const paxg::Photo360Config::Background& bg) {
            photo360_.setBackgroundConfig(bg);
        }

        /// @brief 現在の視点回転角度を取得（度）
        void getViewRotation(double& x, double& y, double& z) const {
            photo360_.getViewRotation(x, y, z);
        }

        /// @brief 視点回転角度を設定（度）
        void setViewRotation(double x, double y, double z) {
            photo360_.setViewRotation(x, y, z);
        }

        // IRenderable インターフェースの実装

        /// @brief レンダリングレイヤーを取得
        RenderLayer getLayer() const override {
            return RenderLayer::Map3D;
        }

        /// @brief 可視性を取得
        bool isVisible() const override {
            return visible_;
        }

        /// @brief 可視性を設定
        void setVisible(bool visible) override {
            visible_ = visible;
        }
    };

}

#endif // !PAX_MAHOROBA_RENDERING_PHOTO_360_LAYER_HPP
