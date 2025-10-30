/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_INIT_LOGO_HPP
#define PAX_MAHOROBA_INIT_LOGO_HPP

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Color/Background.hpp>
#include <PAX_MAHOROBA/Init.hpp>

#include <PAX_SAPIENTICA/Version.hpp>

namespace paxs {

    class PaxSapienticaInitSiv3D {
    public:
        // ソフトウェアを実行した最初のフレームの一番最初に実行
        static void firstInit() {
//#ifdef PAXS_USING_SIV3D
//            s3d::detail::Console_impl{}.open(); // コンソールを開く s3d::Console::Open()
//#endif

#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)

#else
            // ウィンドウのサイズを変える
            paxg::Window::setSize(1280, 720);
#endif

            // PAX SAPIENTICA 用の背景
            paxg::Window::setBackgroundColor(paxs::BackgroundColor::LightBlue);
            paxg::Window::setLetterbox(paxs::BackgroundColor::LightBlue);

            // タイトルを変更
            paxg::Window::setTitle(
                std::string("PAX SAPIENTICA v") + std::string(PAX_SAPIENTICA_LIBRARY_VERSION_NAME));

            // ウィンドウのサイズを変更可能にする
            paxg::Window::setResizable(true);
#ifdef PAXS_USING_DXLIB
            DxLib::DxLib_Init();
#endif // PAXS_USING_DXLIB

#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
            // DxLibのアンドロイド版の画面サイズを変更
            int w{ 1280 }, h{ 720 };
            DxLib::GetAndroidDisplayResolution(&w, &h);
            DxLib::SetGraphMode(w, h, 32);
#endif

#ifdef PAXS_USING_DXLIB
            DxLib::SetDrawScreen(DX_SCREEN_BACK);
#endif // PAXS_USING_DXLIB

#ifdef PAXS_USING_SIV3D
            // 一度 update を呼んでシーンサイズを反映させる
            paxg::Window::update();
            // 背景を描画してから update することで、初期化時に水色背景が表示される
            paxg::Rect{ 0, 0, static_cast<float>(paxg::Window::width()), static_cast<float>(paxg::Window::height()) }.draw(paxs::BackgroundColor::LightBlue);
            paxg::Window::update();
#endif

#ifdef PAXS_USING_SFML
            paxg::Window::setFPS(60);
            paxg::Window::clear();
            paxg::Window::display();
#endif
        }

    };

}

#endif // !PAX_MAHOROBA_INIT_LOGO_HPP
