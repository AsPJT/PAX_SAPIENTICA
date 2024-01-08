/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_INIT_LOGO_HPP
#define PAX_MAHOROBA_INIT_LOGO_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Version.hpp>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Init.hpp>

namespace paxs {

    class PaxSapienticaInitSiv3D {
    public:
        // ソフトウェアを実行した最初のフレームの一番最初に実行
        static void firstInit() {
//#ifdef PAXS_USING_SIV3D
//            s3d::detail::Console_impl{}.open(); // コンソールを開く s3d::Console::Open()
//#endif

            // 画面サイズを変更
#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)

#else
            // ウィンドウのサイズを変える
            paxg::Window::setSize(1280, 720);
#endif

            // PAX SAPIENTICA 用の背景
            const paxg::Color paxs_color = paxg::Color(140, 180, 250); // 青
            paxg::Window::setBackgroundColor(paxs_color);
            paxg::Window::setLetterbox(paxs_color);
            // paxg::Color(255, 255, 255); // 白
// paxg::Color(243, 243, 243); // 白
// paxg::Color(181,0,0);
// paxg::Color(180, 154, 100); // 茶色
// paxg::Color(110, 146, 161); // 濁った青
// paxg::Color(156, 192, 249); // 薄い青
// paxg::Color();

            // タイトルを変更
            paxg::Window::setTitle(
                std::string("PAX SAPIENTICA v") + std::string(PAX_SAPIENTICA_LIBRARY_VERSION_NAME));

#ifdef PAXS_USING_SIV3D
            // ウィンドウのサイズを変更可能にする
            s3d::Window::SetStyle(s3d::WindowStyle::Sizable);
            // 画像の拡大縮小の方式を設定
            const s3d::ScopedRenderStates2D sampler{ s3d::SamplerState::ClampNearest };
#endif
#ifdef PAXS_USING_DXLIB
            DxLib::DxLib_Init();
#endif // PAXS_USING_DXLIB
            // 画面サイズを変更
#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
            int w{ 1280 }, h{ 720 };
            DxLib::GetAndroidDisplayResolution(&w, &h);
            DxLib::SetGraphMode(w, h, 32);
#endif
#ifdef PAXS_USING_DXLIB
            DxLib::SetDrawScreen(DX_SCREEN_BACK);
#endif // PAXS_USING_DXLIB
#ifdef PAXS_USING_SIV3D
            paxg::Window::update();
#endif
#ifdef PAXS_USING_SFML
            paxg::Window::window.setFramerateLimit(60);
#endif
        }

    };

}

#endif // !PAX_MAHOROBA_INIT_LOGO_HPP
