/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_INIT_LOGO_HPP
#define PAX_SAPIENTICA_SIV3D_INIT_LOGO_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/Siv3D/Init.hpp>
#include <PAX_SAPIENTICA/Version.hpp>

namespace paxs {

    class PaxSapienticaInitSiv3D {
    private:
    public:
        // ソフトウェアを実行した最初のフレームの一番最初に実行
        static void firstInit() {
            // ロゴ画像の読み込み
            const s3d::Texture texture_tl{ U"./../../../../../Image/Logo/TitleBanner2.svg" };
            // 画面サイズを変更
            s3d::Window::Resize((!texture_tl) ? 700 : texture_tl.width(), (!texture_tl) ? 180 : texture_tl.height());
            // ウィンドウの上部のフレームを消す
            s3d::Window::SetStyle(s3d::WindowStyle::Frameless);
            // PAX SAPIENTICA 用の背景
            s3d::Scene::SetBackground(s3d::Color{ 181, 0, 0 });
            s3d::Scene::SetLetterbox(s3d::Color{ 181, 0, 0 });
            // 画像の拡大縮小の方式を設定
            const s3d::ScopedRenderStates2D sampler{ s3d::SamplerState::ClampNearest };
            // タイトルを変更する前に 1 回更新
            s3d::System::Update();
            // タイトルを変更
            s3d::Window::SetTitle(s3d::Unicode::FromUTF8("PAX SAPIENTICA v") + s3d::Unicode::FromUTF8(PAX_SAPIENTICA_LIBRARY_VERSION_NAME));
            // タイトルロゴを描画
            texture_tl.drawAt(s3d::Scene::Center());
            s3d::System::Update();
            // タイトルロゴを描画
            texture_tl.drawAt(s3d::Scene::Center());
        }

        // ソフトウェアを実行した最初のフレームの一番最後に実行
        static void secondInit() {
            static bool first_update = false;
            if (!first_update) {
                first_update = true;
                // ウィンドウの上下左右にできる背景の余白の色を設定
                s3d::Scene::SetLetterbox(s3d::Color{ 243, 243, 243 });
                //s3d::Scene::SetLetterbox(s3d::Color{ 181,0,0 });
                //s3d::Scene::SetBackground(s3d::Color{ 181, 0, 0 });
                        // 背景色を指定 (s3d::Color{ 180, 154, 100 }); // 茶色 (s3d::Color{ 110, 146, 161 }); // 濁った青
        //s3d::Scene::SetBackground(s3d::Color{ 255, 255, 255 }); // 白
        //s3d::Scene::SetBackground(s3d::Color{ 243, 243, 243 }); // 白
        //s3d::Scene::SetBackground(s3d::Color{ 156, 192, 249 }); // 薄い青
        //s3d::Scene::SetBackground(s3d::Color{ 140, 180, 250 }); // 青
                // 背景色を指定
                s3d::Scene::SetBackground(s3d::Color{ 243, 243, 243 }); // 白
                // ウィンドウのサイズを変更可能にする
                s3d::Window::SetStyle(s3d::WindowStyle::Sizable);
                // ウィンドウのサイズを変える
                s3d::Window::Resize(1280, 720);
            }
        }

    };

}

#endif // !PAX_SAPIENTICA_SIV3D_INIT_LOGO_HPP
