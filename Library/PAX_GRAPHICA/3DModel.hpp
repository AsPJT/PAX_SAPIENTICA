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

#include <PAX_SAPIENTICA/AppConfig.hpp>

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

        /// @brief 360度写真の球体設定
        struct Sphere {
            double centerX = 0.0;           // 球体中心X座標
            double centerY = 2.0;           // 球体中心Y座標
            double initialZ = -15.5;        // 初期Z座標
            double radius = 2.0;            // 球体半径
            double moveSpeed = 1.2;         // 移動速度（度/フレーム）
            double zoomSpeed = 0.015;       // ズーム速度
            double zoomMin = -17.0;         // ズーム最小値
            double zoomMax = -14.2;         // ズーム最大値
        } sphere;

        /// @brief ファイルパス設定
        struct FilePaths {
            std::string modelPath = "Data/3DModels/KofunOBJ/Model/Sekishitsu/KamoKitaKofun/KamoKitaKofun.obj";
            std::string skyTexturePath = "Data/360DegreePhotos/sky.jpg";
        } paths;
    };

    /// @brief 3D モデル描画クラス
    class Graphics3DModel {
    private:
        Graphics3DModelConfig config;

#ifdef PAXS_USING_SIV3D
        // 回転角度
        int rot = 0;

        s3d::ColorF backgroundColor;

        // 3D モデルデータ
        s3d::Model sekishitsu_model;

        // 360 度写真のテクスチャ
        s3d::Texture sky;

        // 描画するテクスチャの設定
        s3d::MSRenderTexture renderTexture;

        // カメラ
        s3d::DebugCamera3D camera;

        // 360度写真の表示状態
        double viewRotationX = 0.0;
        double viewRotationY = 0.0;
        double viewPositionZ;
#endif

    public:
        /// @brief デフォルト設定でコンストラクタ
        Graphics3DModel() : Graphics3DModel(Graphics3DModelConfig{}) {}

        /// @brief カスタム設定でコンストラクタ
        explicit Graphics3DModel(const Graphics3DModelConfig& cfg) : config(cfg) {
#ifdef PAXS_USING_SIV3D
            // 初期化
            viewPositionZ = config.sphere.initialZ;

            // カメラ設定
            camera = s3d::DebugCamera3D{
                s3d::Graphics3D::GetRenderTargetSize(),
                config.camera.verticalFOV / 180.0 * s3d::Math::Pi,
                s3d::Vec3{ config.camera.posX, config.camera.posY, config.camera.posZ }
            };

            renderTexture = s3d::MSRenderTexture{
                s3d::Size{s3d::Scene::Width(), s3d::Scene::Height()},
                s3d::TextureFormat::R8G8B8A8_Unorm_SRGB,
                s3d::HasDepth::Yes
            };

            // 背景色を設定
            backgroundColor = s3d::ColorF{
                config.background.r,
                config.background.g,
                config.background.b,
                config.background.a
            }.removeSRGBCurve();

            // 3D モデルデータをロード
            const auto rootPath = s3d::Unicode::FromUTF8(paxs::AppConfig::getInstance()->getRootPath());
            sekishitsu_model = s3d::Model{ rootPath + s3d::Unicode::FromUTF8(config.paths.modelPath) };

            // 360 度写真をロード
            sky = s3d::Texture(s3d::Image{ rootPath + s3d::Unicode::FromUTF8(config.paths.skyTexturePath) }.mirror());

            // モデルに付随するテクスチャをアセット管理に登録
            s3d::Model::RegisterDiffuseTextures(sekishitsu_model, s3d::TextureDesc::MippedSRGB);
#endif
        }

        void updateRotation() {
#ifdef PAXS_USING_SIV3D
            s3d::Graphics3D::SetCameraTransform(camera); // カメラ情報を設定

            // 3D シーンの描画
            const s3d::ScopedRenderTarget3D target{ renderTexture.clear(backgroundColor) };
            // モデルの描画
            ++rot;
            if (rot >= 360) rot = 0;

            // TODO: 石室モデルの描画（現在は360度写真のみを表示）
            // 将来的に石室モデルと360度写真を同時に表示する場合は以下のコメントを解除
            // const s3d::ScopedRenderStates3D renderStates{ s3d::BlendState::OpaqueAlphaToCoverage, s3d::RasterizerState::SolidCullNone };
            // sekishitsu_model.draw(s3d::Vec3{ 0, 0, 0 }, s3d::Quaternion::RotateY(rot / 180.0 * s3d::Math::Pi));

            {
                // 太陽光の影響を与えないようにする
                s3d::Graphics3D::SetGlobalAmbientColor(s3d::ColorF{ 1.0 });
                s3d::Graphics3D::SetSunColor(s3d::ColorF{ 0.0 });

                // カリングを無効にする
                const s3d::ScopedRenderStates3D rs{ s3d::RasterizerState::SolidCullNone };

                // キー入力による視点操作
                if (s3d::KeyLeft.pressed() || s3d::KeyA.pressed())
                    viewRotationX += config.sphere.moveSpeed;
                if (s3d::KeyRight.pressed() || s3d::KeyD.pressed())
                    viewRotationX -= config.sphere.moveSpeed;
                if (s3d::KeyDown.pressed() || s3d::KeyS.pressed())
                    viewRotationY -= config.sphere.moveSpeed;
                if (s3d::KeyUp.pressed() || s3d::KeyW.pressed())
                    viewRotationY += config.sphere.moveSpeed;

                // ズーム操作
                if (s3d::KeyQ.pressed()) {
                    viewPositionZ -= config.sphere.zoomSpeed;
                    if (viewPositionZ < config.sphere.zoomMin)
                        viewPositionZ = config.sphere.zoomMin;
                }
                if (s3d::KeyE.pressed()) {
                    viewPositionZ += config.sphere.zoomSpeed;
                    if (viewPositionZ > config.sphere.zoomMax)
                        viewPositionZ = config.sphere.zoomMax;
                }

                // 回転角度を 0-360 度の範囲に正規化
                if (viewRotationX < 0.0) viewRotationX += 360.0;
                if (viewRotationY < 0.0) viewRotationY += 360.0;
                if (viewRotationX >= 360.0) viewRotationX -= 360.0;
                if (viewRotationY >= 360.0) viewRotationY -= 360.0;

                // 球体に 360 度写真を貼り付ける
                s3d::Sphere{
                    config.sphere.centerX,
                    config.sphere.centerY,
                    viewPositionZ,
                    config.sphere.radius
                }.draw(
                    s3d::Quaternion::RotateY(viewRotationX / 180.0 * s3d::Math::Pi) *
                    s3d::Quaternion::RotateX(viewRotationY / 180.0 * s3d::Math::Pi),
                    sky
                );
            }

            // RenderTexture を 2D シーンに描画
            s3d::Graphics3D::Flush(); // 現在までの 3D 描画を実行

            static int count = 0;
            // 画面サイズが変更されたら幅を変える
            if (count >= 3) {
                count = 0;
                if (renderTexture.width() != s3d::Scene::Width()
                    || renderTexture.height() != s3d::Scene::Height()) {
                    renderTexture = s3d::MSRenderTexture{
    s3d::Size{s3d::Scene::Width(), s3d::Scene::Height()}/*s3d::Scene::Size()*/, // 表示領域
        s3d::TextureFormat::R8G8B8A8_Unorm_SRGB, // テクスチャのフォーマットを指定
        s3d::HasDepth::Yes }; // 深度バッファあり
                }
            }
            ++count;

            renderTexture.resolve(); // テクスチャを描画可能にする
            renderTexture.draw(0, 0); // 指定した大きさで描画
#endif
        }

        /// @brief 設定を取得
        const Graphics3DModelConfig& getConfig() const { return config; }

        /// @brief 設定を更新
        void setConfig(const Graphics3DModelConfig& cfg) { config = cfg; }

        /// @brief カメラ設定を更新
        void setCameraConfig(const Graphics3DModelConfig::Camera& cam) {
            config.camera = cam;
#ifdef PAXS_USING_SIV3D
            camera = s3d::DebugCamera3D{
                s3d::Graphics3D::GetRenderTargetSize(),
                config.camera.verticalFOV / 180.0 * s3d::Math::Pi,
                s3d::Vec3{ config.camera.posX, config.camera.posY, config.camera.posZ }
            };
#endif
        }

        /// @brief 球体設定を更新
        void setSphereConfig(const Graphics3DModelConfig::Sphere& sph) {
            config.sphere = sph;
        }

        /// @brief 背景色設定を更新
        void setBackgroundConfig(const Graphics3DModelConfig::Background& bg) {
            config.background = bg;
#ifdef PAXS_USING_SIV3D
            backgroundColor = s3d::ColorF{
                config.background.r,
                config.background.g,
                config.background.b,
                config.background.a
            }.removeSRGBCurve();
#endif
        }

        /// @brief 現在の視点回転角度を取得（度）
        void getViewRotation(double& x, double& y, double& z) const {
#ifdef PAXS_USING_SIV3D
            x = viewRotationX;
            y = viewRotationY;
            z = viewPositionZ;
#else
            x = y = z = 0.0;
#endif
        }

        /// @brief 視点回転角度を設定（度）
        void setViewRotation(double x, double y, double z) {
#ifdef PAXS_USING_SIV3D
            viewRotationX = x;
            viewRotationY = y;
            viewPositionZ = z;
#else
            (void)x; // 未使用パラメータの警告を抑制
            (void)y; // 未使用パラメータの警告を抑制
            (void)z; // 未使用パラメータの警告を抑制
#endif
        }
    };

}

#endif // !PAX_MAHOROBA_3D_MODEL_HPP
