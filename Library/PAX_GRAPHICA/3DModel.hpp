/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_3D_MODEL_HPP
#define PAX_MAHOROBA_3D_MODEL_HPP

#include <memory>

#include <PAX_GRAPHICA/Interface/Graphics3DModelImpl.hpp>

#if defined(PAXS_USING_SIV3D)
#include <PAX_GRAPHICA/Siv3D/Siv3DGraphics3DModelImpl.hpp>
#else
#include <PAX_GRAPHICA/Null/NullGraphics3DModelImpl.hpp>
#endif

namespace paxg {

    /// @brief 3D モデル描画クラス with Pimpl idiom for graphics library abstraction
    /// @note Uses unique_ptr for implementation hiding and automatic resource management
    class Graphics3DModel {
    private:
        Graphics3DModelConfig config_;
        std::unique_ptr<Graphics3DModelImpl> impl_;

        /// @brief Create implementation based on the active graphics library
        /// @param cfg Configuration for 3D model
        /// @return Unique pointer to the implementation
        static std::unique_ptr<Graphics3DModelImpl> createImpl(const Graphics3DModelConfig& cfg) {
#if defined(PAXS_USING_SIV3D)
            return std::make_unique<Siv3DGraphics3DModelImpl>(cfg);
#else
            return std::make_unique<NullGraphics3DModelImpl>(cfg);
#endif
        }

    public:
        /// @brief デフォルト設定でコンストラクタ
        Graphics3DModel() : Graphics3DModel(Graphics3DModelConfig{}) {}

        /// @brief カスタム設定でコンストラクタ
        explicit Graphics3DModel(const Graphics3DModelConfig& cfg)
            : config_(cfg), impl_(createImpl(cfg)) {}

        /// @brief 回転角度を更新
        void updateRotation() {
            if (impl_) {
                impl_->updateRotation();
            }
        }

        /// @brief 3Dモデルを描画
        void render() const {
            if (impl_) {
                impl_->render();
            }
        }

        /// @brief 設定を取得
        const Graphics3DModelConfig& getConfig() const { return config_; }

        /// @brief 設定を更新
        void setConfig(const Graphics3DModelConfig& cfg) {
            config_ = cfg;
            impl_ = createImpl(cfg);
        }

        /// @brief カメラ設定を更新
        void setCameraConfig(const Graphics3DModelConfig::Camera& cam) {
            config_.camera = cam;
            if (impl_) {
                impl_->setCameraConfig(cam);
            }
        }

        /// @brief 背景色設定を更新
        void setBackgroundConfig(const Graphics3DModelConfig::Background& bg) {
            config_.background = bg;
            if (impl_) {
                impl_->setBackgroundConfig(bg);
            }
        }

        /// @brief 現在の回転角度を取得（度）
        int getRotation() const {
            return impl_ ? impl_->getRotation() : 0;
        }

        /// @brief 回転角度を設定（度）
        void setRotation(int angle) {
            if (impl_) {
                impl_->setRotation(angle);
            }
        }
    };

}

#endif // !PAX_MAHOROBA_3D_MODEL_HPP
