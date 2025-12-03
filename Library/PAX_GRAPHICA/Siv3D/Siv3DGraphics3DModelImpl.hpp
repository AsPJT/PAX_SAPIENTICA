/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SIV3D_GRAPHICS_3D_MODEL_IMPL_HPP
#define PAX_GRAPHICA_SIV3D_GRAPHICS_3D_MODEL_IMPL_HPP

#include <PAX_GRAPHICA/Interface/Graphics3DModelImpl.hpp>

#ifdef PAXS_USING_SIV3D
#include <Siv3D.hpp>
#include <PAX_SAPIENTICA/Core/Math/Math.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#endif

namespace paxg {

    struct Graphics3DModelConfig;

#ifdef PAXS_USING_SIV3D

    /// @brief Siv3D implementation of Graphics3DModelImpl
    class Siv3DGraphics3DModelImpl : public Graphics3DModelImpl {
    private:
        const Graphics3DModelConfig& config_;
        int rotation_ = 0;
        s3d::ColorF backgroundColor_;
        s3d::Model model_;
        mutable s3d::MSRenderTexture renderTexture_;
        mutable s3d::DebugCamera3D camera_;

    public:
        /// @brief Constructor with configuration
        /// @param cfg Configuration for 3D model
        explicit Siv3DGraphics3DModelImpl(const Graphics3DModelConfig& cfg)
            : config_(cfg) {
            // カメラ設定
            camera_ = s3d::DebugCamera3D{
                s3d::Graphics3D::GetRenderTargetSize(),
                paxs::Math<double>::degToRad(config_.camera.verticalFOV),
                s3d::Vec3{ config_.camera.posX, config_.camera.posY, config_.camera.posZ }
            };

            renderTexture_ = s3d::MSRenderTexture{
                s3d::Size{s3d::Scene::Width(), s3d::Scene::Height()},
                s3d::TextureFormat::R8G8B8A8_Unorm_SRGB,
                s3d::HasDepth::Yes
            };

            // 背景色を設定
            backgroundColor_ = s3d::ColorF{
                config_.background.r,
                config_.background.g,
                config_.background.b,
                config_.background.a
            }.removeSRGBCurve();

            // 3D モデルデータをロード
            const auto rootPath = s3d::Unicode::FromUTF8(paxs::AppConfig::getInstance().getRootPath());
            model_ = s3d::Model{ rootPath + s3d::Unicode::FromUTF8(config_.paths.modelPath) };

            // モデルに付随するテクスチャをアセット管理に登録
            s3d::Model::RegisterDiffuseTextures(model_, s3d::TextureDesc::MippedSRGB);
        }

        /// @brief Update rotation angle
        void updateRotation() override {
            ++rotation_;
            if (rotation_ >= 360) rotation_ = 0;
        }

        /// @brief Render 3D model
        void render() const override {
            s3d::Graphics3D::SetCameraTransform(camera_);

            // 3D シーンの描画
            const s3d::ScopedRenderTarget3D target{ renderTexture_.clear(backgroundColor_) };

            // 3Dモデルの描画
            const s3d::ScopedRenderStates3D renderStates{
                s3d::BlendState::OpaqueAlphaToCoverage,
                s3d::RasterizerState::SolidCullNone
            };
            model_.draw(
                s3d::Vec3{ 0, 0, 0 },
                s3d::Quaternion::RotateY(paxs::Math<double>::degToRad(static_cast<double>(rotation_)))
            );

            // RenderTexture を 2D シーンに描画
            s3d::Graphics3D::Flush();

            // 画面サイズが変更されたら幅を変える
            static int resize_check_count = 0;
            if (resize_check_count >= 3) {
                resize_check_count = 0;
                if (renderTexture_.width() != s3d::Scene::Width()
                    || renderTexture_.height() != s3d::Scene::Height()) {
                    renderTexture_ = s3d::MSRenderTexture{
                        s3d::Size{s3d::Scene::Width(), s3d::Scene::Height()},
                        s3d::TextureFormat::R8G8B8A8_Unorm_SRGB,
                        s3d::HasDepth::Yes
                    };
                }
            }
            ++resize_check_count;

            renderTexture_.resolve();
            renderTexture_.draw(0, 0);
        }

        /// @brief Get current rotation angle (degrees)
        int getRotation() const override {
            return rotation_;
        }

        /// @brief Set rotation angle (degrees)
        /// @param angle Rotation angle in degrees
        void setRotation(int angle) override {
            rotation_ = angle % 360;
        }

        /// @brief Update camera configuration
        /// @param cam Camera configuration
        void setCameraConfig(const Graphics3DModelConfig::Camera& cam) override {
            camera_ = s3d::DebugCamera3D{
                s3d::Graphics3D::GetRenderTargetSize(),
                paxs::Math<double>::degToRad(cam.verticalFOV),
                s3d::Vec3{ cam.posX, cam.posY, cam.posZ }
            };
        }

        /// @brief Update background configuration
        /// @param bg Background configuration
        void setBackgroundConfig(const Graphics3DModelConfig::Background& bg) override {
            backgroundColor_ = s3d::ColorF{
                bg.r, bg.g, bg.b, bg.a
            }.removeSRGBCurve();
        }
    };

#endif // PAXS_USING_SIV3D

}

#endif // !PAX_GRAPHICA_SIV3D_GRAPHICS_3D_MODEL_IMPL_HPP
