/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_PHOTO_360_HPP
#define PAX_GRAPHICA_PHOTO_360_HPP

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#include <PAX_SAPIENTICA/AppConfig.hpp>

namespace paxg {

    /// @brief 360度写真描画の設定
    struct Photo360Config {
        /// @brief カメラ設定
        struct Camera {
            double verticalFOV = 40.0;  // 垂直視野角（度）
            double posX = 0.0;
            double posY = 3.0;
            double posZ = -16.0;
        } camera;

        /// @brief 背景色設定
        struct Background {
            float r = 1.0f;             // 赤成分
            float g = 1.0f;             // 緑成分
            float b = 1.0f;             // 青成分
            float a = 0.0f;             // 透明度
        } background;

        /// @brief 球体設定
        struct Sphere {
            double centerX = 0.0;       // 球体中心X座標
            double centerY = 2.0;       // 球体中心Y座標
            double initialZ = -15.5;    // 初期Z座標
            double radius = 2.0;        // 球体半径
            double moveSpeed = 1.2;     // 移動速度（度/フレーム）
            double zoomSpeed = 0.015;   // ズーム速度
            double zoomMin = -17.0;     // ズーム最小値
            double zoomMax = -14.2;     // ズーム最大値
        } sphere;

        /// @brief ファイルパス設定
        struct FilePaths {
            std::string photoPath = "Data/360DegreePhotos/sky.jpg";
        } paths;
    };

    /// @brief 360度写真描画クラス
    class Photo360 {
    private:
        Photo360Config config_;

#ifdef PAXS_USING_SIV3D
        s3d::ColorF backgroundColor_;
        s3d::Texture photo_texture_;
        s3d::MSRenderTexture renderTexture_;
        s3d::DebugCamera3D camera_;

        // 360度写真の表示状態
        double viewRotationX_ = 0.0;
        double viewRotationY_ = 0.0;
        double viewPositionZ_;
#endif

    public:
        /// @brief デフォルト設定でコンストラクタ
        Photo360() : Photo360(Photo360Config{}) {}

        /// @brief カスタム設定でコンストラクタ
        explicit Photo360(const Photo360Config& cfg) : config_(cfg) {
#ifdef PAXS_USING_SIV3D
            // 初期化
            viewPositionZ_ = config_.sphere.initialZ;

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

            // 360度写真をロード
            const auto rootPath = s3d::Unicode::FromUTF8(paxs::AppConfig::getInstance()->getRootPath());
            photo_texture_ = s3d::Texture(s3d::Image{ rootPath + s3d::Unicode::FromUTF8(config_.paths.photoPath) }.mirror());
#endif
        }

        /// @brief 360度写真を更新・描画
        void updateAndRender() {
#ifdef PAXS_USING_SIV3D
            s3d::Graphics3D::SetCameraTransform(camera_); // カメラ情報を設定

            // 3D シーンの描画
            const s3d::ScopedRenderTarget3D target{ renderTexture_.clear(backgroundColor_) };

            {
                // 太陽光の影響を与えないようにする
                s3d::Graphics3D::SetGlobalAmbientColor(s3d::ColorF{ 1.0 });
                s3d::Graphics3D::SetSunColor(s3d::ColorF{ 0.0 });

                // カリングを無効にする
                const s3d::ScopedRenderStates3D rs{ s3d::RasterizerState::SolidCullNone };

                // キー入力による視点操作
                if (s3d::KeyLeft.pressed() || s3d::KeyA.pressed())
                    viewRotationX_ += config_.sphere.moveSpeed;
                if (s3d::KeyRight.pressed() || s3d::KeyD.pressed())
                    viewRotationX_ -= config_.sphere.moveSpeed;
                if (s3d::KeyDown.pressed() || s3d::KeyS.pressed())
                    viewRotationY_ -= config_.sphere.moveSpeed;
                if (s3d::KeyUp.pressed() || s3d::KeyW.pressed())
                    viewRotationY_ += config_.sphere.moveSpeed;

                // ズーム操作
                if (s3d::KeyQ.pressed()) {
                    viewPositionZ_ -= config_.sphere.zoomSpeed;
                    if (viewPositionZ_ < config_.sphere.zoomMin)
                        viewPositionZ_ = config_.sphere.zoomMin;
                }
                if (s3d::KeyE.pressed()) {
                    viewPositionZ_ += config_.sphere.zoomSpeed;
                    if (viewPositionZ_ > config_.sphere.zoomMax)
                        viewPositionZ_ = config_.sphere.zoomMax;
                }

                // 回転角度を 0-360 度の範囲に正規化
                if (viewRotationX_ < 0.0) viewRotationX_ += 360.0;
                if (viewRotationY_ < 0.0) viewRotationY_ += 360.0;
                if (viewRotationX_ >= 360.0) viewRotationX_ -= 360.0;
                if (viewRotationY_ >= 360.0) viewRotationY_ -= 360.0;

                // 球体に 360 度写真を貼り付ける
                s3d::Sphere{
                    config_.sphere.centerX,
                    config_.sphere.centerY,
                    viewPositionZ_,
                    config_.sphere.radius
                }.draw(
                    s3d::Quaternion::RotateY(viewRotationX_ / 180.0 * s3d::Math::Pi) *
                    s3d::Quaternion::RotateX(viewRotationY_ / 180.0 * s3d::Math::Pi),
                    photo_texture_
                );
            }

            // RenderTexture を 2D シーンに描画
            s3d::Graphics3D::Flush(); // 現在までの 3D 描画を実行

            static int count = 0;
            // 画面サイズが変更されたら幅を変える
            if (count >= 3) {
                count = 0;
                if (renderTexture_.width() != s3d::Scene::Width()
                    || renderTexture_.height() != s3d::Scene::Height()) {
                    renderTexture_ = s3d::MSRenderTexture{
                        s3d::Size{s3d::Scene::Width(), s3d::Scene::Height()},
                        s3d::TextureFormat::R8G8B8A8_Unorm_SRGB,
                        s3d::HasDepth::Yes
                    };
                }
            }
            ++count;

            renderTexture_.resolve(); // テクスチャを描画可能にする
            renderTexture_.draw(0, 0); // 指定した大きさで描画
#endif
        }

        /// @brief 設定を取得
        const Photo360Config& getConfig() const { return config_; }

        /// @brief 設定を更新
        void setConfig(const Photo360Config& cfg) { config_ = cfg; }

        /// @brief カメラ設定を更新
        void setCameraConfig(const Photo360Config::Camera& cam) {
            config_.camera = cam;
#ifdef PAXS_USING_SIV3D
            camera_ = s3d::DebugCamera3D{
                s3d::Graphics3D::GetRenderTargetSize(),
                config_.camera.verticalFOV / 180.0 * s3d::Math::Pi,
                s3d::Vec3{ config_.camera.posX, config_.camera.posY, config_.camera.posZ }
            };
#endif
        }

        /// @brief 球体設定を更新
        void setSphereConfig(const Photo360Config::Sphere& sph) {
            config_.sphere = sph;
        }

        /// @brief 背景色設定を更新
        void setBackgroundConfig(const Photo360Config::Background& bg) {
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

        /// @brief 現在の視点回転角度を取得（度）
        void getViewRotation(double& x, double& y, double& z) const {
#ifdef PAXS_USING_SIV3D
            x = viewRotationX_;
            y = viewRotationY_;
            z = viewPositionZ_;
#else
            x = y = z = 0.0;
#endif
        }

        /// @brief 視点回転角度を設定（度）
        void setViewRotation(double x, double y, double z) {
#ifdef PAXS_USING_SIV3D
            viewRotationX_ = x;
            viewRotationY_ = y;
            viewPositionZ_ = z;
#else
            (void)x;
            (void)y;
            (void)z;
#endif
        }
    };

}

#endif // !PAX_GRAPHICA_PHOTO_360_HPP
