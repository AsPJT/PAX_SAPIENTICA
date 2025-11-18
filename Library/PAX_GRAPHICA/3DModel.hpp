/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_3D_MODEL_HPP
#define PAX_MAHOROBA_3D_MODEL_HPP

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#include <PAX_SAPIENTICA/System/AppConfig.hpp>

namespace paxg {

    /// @brief 3D モデル描画の設定
    struct Graphics3DModelConfig {
        /// @brief カメラ設定
        struct Camera {
            /// @brief 垂直視野角（度）
            double verticalFOV = 40.0;
            double posX = 0.0;
            double posY = 3.0;
            double posZ = -16.0;
        } camera;

        /// @brief 背景色設定
        struct Background {
            float r = 1.0f;                 // 赤成分
            float g = 1.0f;                 // 緑成分
            float b = 1.0f;                 // 青成分
            float a = 0.0f;                 // 透明度
        } background;

        /// @brief ファイルパス設定
        struct FilePaths {
            std::string modelPath = "Data/3DModels/KofunOBJ/Model/Sekishitsu/KamoKitaKofun/KamoKitaKofun.obj";
        } paths;

        /// @brief モデルの回転速度（度/フレーム）
        double rotationSpeed = 1.0;
    };

    /// @brief 3D モデル描画クラス
    class Graphics3DModel {
    private:
        Graphics3DModelConfig config_;

#ifdef PAXS_USING_SIV3D
        // 回転角度
        int rotation_ = 0;

        s3d::ColorF backgroundColor_;

        // 3D モデルデータ
        s3d::Model model_;

        // 描画するテクスチャの設定
        mutable s3d::MSRenderTexture renderTexture_;

        // カメラ
        mutable s3d::DebugCamera3D camera_;
#endif

    public:
        /// @brief デフォルト設定でコンストラクタ
        Graphics3DModel() : Graphics3DModel(Graphics3DModelConfig{}) {}

        /// @brief カスタム設定でコンストラクタ
        explicit Graphics3DModel(const Graphics3DModelConfig& cfg) : config_(cfg) {
#ifdef PAXS_USING_SIV3D
            // カメラ設定
            camera_ = s3d::DebugCamera3D{
                s3d::Graphics3D::GetRenderTargetSize(),
                config_.camera.verticalFOV / 180.0 * s3d::Math::Pi,
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
#endif
        }

        /// @brief 回転角度を更新
        /// @brief Update rotation angle
        void updateRotation() {
#ifdef PAXS_USING_SIV3D
            // モデルの回転更新
            ++rotation_;
            if (rotation_ >= 360) rotation_ = 0;
#endif
        }

        /// @brief 3Dモデルを描画
        /// @brief Render 3D model
        void render() const {
#ifdef PAXS_USING_SIV3D
            s3d::Graphics3D::SetCameraTransform(camera_); // カメラ情報を設定

            // 3D シーンの描画
            const s3d::ScopedRenderTarget3D target{ renderTexture_.clear(backgroundColor_) };

            // 3Dモデルの描画
            const s3d::ScopedRenderStates3D renderStates{
                s3d::BlendState::OpaqueAlphaToCoverage,
                s3d::RasterizerState::SolidCullNone
            };
            model_.draw(
                s3d::Vec3{ 0, 0, 0 },
                s3d::Quaternion::RotateY(rotation_ / 180.0 * s3d::Math::Pi)
            );

            // RenderTexture を 2D シーンに描画
            s3d::Graphics3D::Flush(); // 現在までの 3D 描画を実行

            // 画面サイズが変更されたら幅を変える
            // 毎フレームチェックすると重いため、3フレームに1回チェック
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

            renderTexture_.resolve(); // テクスチャを描画可能にする
            renderTexture_.draw(0, 0); // 指定した大きさで描画
#endif
        }

        /// @brief 設定を取得
        const Graphics3DModelConfig& getConfig() const { return config_; }

        /// @brief 設定を更新
        void setConfig(const Graphics3DModelConfig& cfg) { config_ = cfg; }

        /// @brief カメラ設定を更新
        void setCameraConfig(const Graphics3DModelConfig::Camera& cam) {
            config_.camera = cam;
#ifdef PAXS_USING_SIV3D
            camera_ = s3d::DebugCamera3D{
                s3d::Graphics3D::GetRenderTargetSize(),
                config_.camera.verticalFOV / 180.0 * s3d::Math::Pi,
                s3d::Vec3{ config_.camera.posX, config_.camera.posY, config_.camera.posZ }
            };
#endif
        }

        /// @brief 背景色設定を更新
        void setBackgroundConfig(const Graphics3DModelConfig::Background& bg) {
            config_.background = bg;
#ifdef PAXS_USING_SIV3D
            backgroundColor_ = s3d::ColorF{
                config_.background.r,
                config_.background.g,
                config_.background.b,
                config_.background.a
            }.removeSRGBCurve();
#endif
        }

        /// @brief 現在の回転角度を取得（度）
        int getRotation() const {
#ifdef PAXS_USING_SIV3D
            return rotation_;
#else
            return 0;
#endif
        }

        /// @brief 回転角度を設定（度）
        void setRotation(int angle) {
#ifdef PAXS_USING_SIV3D
            rotation_ = angle % 360;
#else
            (void)angle;
#endif
        }
    };

}

#endif // !PAX_MAHOROBA_3D_MODEL_HPP
