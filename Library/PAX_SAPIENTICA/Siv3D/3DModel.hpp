/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_3D_MODEL_HPP
#define PAX_SAPIENTICA_SIV3D_3D_MODEL_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/Siv3D/Init.hpp>

namespace paxs {

    class Graphics3DModel {
    private:
        int rot = 0; // 回転するための向き（度）

        s3d::ColorF backgroundColor; // 背景の色（透明度あり）

        // 3D モデルデータ
        s3d::Model sekishitsu_model;
        // 360 度写真のテクスチャ
        s3d::Texture sky;

        // 描画するテクスチャの設定
        s3d::MSRenderTexture renderTexture{
            s3d::Size{800, 500}/*s3d::Scene::Size()*/, // 表示領域
                s3d::TextureFormat::R8G8B8A8_Unorm_SRGB, // テクスチャのフォーマットを指定
                s3d::HasDepth::Yes }; // 深度バッファあり

        // カメラ位置を指定
        s3d::DebugCamera3D camera{
            //s3d::Size{500,300},
            s3d::Graphics3D::GetRenderTargetSize(),
                40.0 / 180.0 * 3.1416,
                s3d::Vec3{ 0, 3, -16 } };


    public:
        Graphics3DModel() {
            // 透明な背景を設定
            backgroundColor = s3d::ColorF{ 1,1,1,0 }.removeSRGBCurve();

            // 3D モデルデータをロード
            sekishitsu_model = s3d::Model{ U"./../../../../../Data/3DModel/KofunOBJ/Model/Sekishitsu/KamoKitaKofun/KamoKitaKofun.obj" };

            // 360 度写真をロード
            sky = s3d::Texture(s3d::Image{ U"./../../../../../Data/360DegreePhoto/stone2.jpg" }.mirror());

            // モデルに付随するテクスチャをアセット管理に登録
            s3d::Model::RegisterDiffuseTextures(sekishitsu_model, s3d::TextureDesc::MippedSRGB);

        }

        void updateRotation() {
            //camera.update(4.0);
            s3d::Graphics3D::SetCameraTransform(camera); // カメラ情報を設定

            // 3D シーンの描画
            const s3d::ScopedRenderTarget3D target{ renderTexture.clear(backgroundColor) };
            // モデルの描画
            ++rot;
            if (rot >= 360) rot = 0;
            //const s3d::ScopedRenderStates3D renderStates{ s3d::BlendState::OpaqueAlphaToCoverage, s3d::RasterizerState::SolidCullNone };
            //sekishitsu_model.draw(s3d::Vec3{ 0, 0, 0 }, s3d::Quaternion::RotateY(rot / 180.0 * 3.1416));

            {
                // 太陽光の影響を与えないようにする
                s3d::Graphics3D::SetGlobalAmbientColor(s3d::ColorF{ 1.0 });
                s3d::Graphics3D::SetSunColor(s3d::ColorF{ 0.0 });

                // カリングを無効にする
                const s3d::ScopedRenderStates3D rs{ s3d::RasterizerState::SolidCullNone };

                static double x = 0.0;
                static double y = 0.0;
                if (s3d::KeyLeft.pressed()) x += 1.0;
                if (s3d::KeyRight.pressed()) x -= 1.0;
                if (s3d::KeyDown.pressed()) y -= 1.0;
                if (s3d::KeyUp.pressed()) y += 1.0;
                if (x < 0.0) x += 360.0;
                if (y < 0.0) y += 360.0;
                if (x >= 360.0) x -= 360.0;
                if (y >= 360.0) y -= 360.0;
                // 球体に 360 度写真を貼り付ける
                s3d::Sphere{ 0, 2, /*-14.2*/-15.5, 2}.draw(
                    //s3d::Quaternion::RotateY(rot / 180.0 * 3.1416) *
                    //s3d::Quaternion::RotateX(rot / 180.0 * 3.1416)
                    s3d::Quaternion::RotateY(x / 180.0 * 3.1416) *
                    s3d::Quaternion::RotateX(y / 180.0 * 3.1416)
                    , sky);
            }

            // RenderTexture を 2D シーンに描画
            s3d::Graphics3D::Flush(); // 現在までの 3D 描画を実行
            renderTexture.resolve(); // テクスチャを描画可能にする
            renderTexture.draw(200, 200); // 指定した大きさで描画
        }

    };

}

#endif // !PAX_SAPIENTICA_SIV3D_3D_MODEL_HPP
