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

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxs {

    class PaxSapienticaInitSiv3D {
    private:
    public:
        // ソフトウェアを実行した最初のフレームの一番最初に実行
        static void firstInit(const std::string& path8) {
            // ロゴ画像の読み込み
            const paxg::Texture texture_tl{
#ifdef __ANDROID__
                "Logo.png"
#else
                path8 + "Image/Logo/TitleBanner2.png"
#endif
            };
            // 画面サイズを変更
#ifdef PAXS_USING_DXLIB
            // paxg::Window::setSize(1280, 720);
#ifdef __ANDROID__
            int w{ 1280 }, h{ 720 };
            DxLib::GetAndroidDisplayResolution(&w, &h);
            DxLib::SetGraphMode(w, h, 32);
#else
            paxg::Window::setSize(1400, 800);
#endif
#else
            paxg::Window::setSize(
                (!texture_tl) ? 700 : texture_tl.width(), (!texture_tl) ? 180 : texture_tl.height());
#endif
#ifdef PAXS_USING_SIV3D
            // ウィンドウの上部のフレームを消す
            s3d::Window::SetStyle(s3d::WindowStyle::Frameless);
#endif
            // PAX SAPIENTICA 用の背景
#ifdef PAXS_USING_DXLIB
            const paxg::Color paxs_color = paxg::Color(140, 180, 250); // 青
#else
            const paxg::Color paxs_color = paxg::Color(181, 0, 0); // 濃い赤
#endif
            paxg::Window::setBackgroundColor(paxs_color);
            paxg::Window::setLetterbox(paxs_color);
#ifdef PAXS_USING_SIV3D
            // 画像の拡大縮小の方式を設定
            const s3d::ScopedRenderStates2D sampler{ s3d::SamplerState::ClampNearest };
#endif
#ifdef PAXS_USING_DXLIB
            DxLib::SetDrawScreen(DX_SCREEN_BACK);
#endif // PAXS_USING_DXLIB
            // タイトルを変更する前に 1 回更新
            paxg::Window::update();
            // タイトルを変更
            paxg::Window::setTitle(
                std::string("PAX SAPIENTICA v") + std::string(PAX_SAPIENTICA_LIBRARY_VERSION_NAME));
            // タイトルロゴを描画
            texture_tl.drawAt(paxg::Window::center());
            paxg::Window::update();
            // タイトルロゴを描画
            texture_tl.drawAt(paxg::Window::center());
        }

        // ソフトウェアを実行した最初のフレームの一番最後に実行
        static void secondInit() {
            static bool first_update = false;
            if (!first_update) {
                first_update = true;

                // 背景色
                const paxg::Color color =
                    paxg::Color(140, 180, 250); // 青
                // paxg::Color(255, 255, 255); // 白
                // paxg::Color(243, 243, 243); // 白
                // paxg::Color(181,0,0);
                // paxg::Color(180, 154, 100); // 茶色
                // paxg::Color(110, 146, 161); // 濁った青
                // paxg::Color(156, 192, 249); // 薄い青
                // paxg::Color();

            // 背景色を指定
                paxg::Window::setBackgroundColor(color);
                // ウィンドウの上下左右にできる背景の余白の色を設定
                paxg::Window::setLetterbox(color);
#ifdef PAXS_USING_SIV3D
                // ウィンドウのサイズを変更可能にする
                s3d::Window::SetStyle(s3d::WindowStyle::Sizable);
#endif
                // ウィンドウのサイズを変える
                paxg::Window::setSize(1280, 720);
#ifdef PAXS_USING_DXLIB
#ifdef __ANDROID__
                int w{ 1280 }, h{ 720 };
                DxLib::GetAndroidDisplayResolution(&w, &h);
                DxLib::SetGraphMode(w, h, 32);
#endif
                DxLib::SetDrawScreen(DX_SCREEN_BACK);
#endif // PAXS_USING_DXLIB


            }
        }

    };

}

#endif // !PAX_SAPIENTICA_SIV3D_INIT_LOGO_HPP
