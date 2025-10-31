/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_INIT_LOGO_HPP
#define PAX_MAHOROBA_INIT_LOGO_HPP

#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Rendering/BackgroundColor.hpp>
#include <PAX_MAHOROBA/Core/Init.hpp>

#include <PAX_SAPIENTICA/Version.hpp>

namespace paxs {

    class PaxSapienticaInit {
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

            // アプリケーションアイコンを設定
#ifdef PAXS_USING_SFML
            paxg::Window::setIcon("Images/Logo/LogoRed.png");
#endif
#ifdef PAXS_USING_DXLIB
            DxLib::DxLib_Init();
            // DxLib は初期化後にアイコンを設定
            paxg::Window::setIcon("Images/Logo/LogoRed.ico");
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
#endif

#ifdef PAXS_USING_SFML
            paxg::Window::setFPS(60);

#endif
            // ローディング画面を表示
            displayLoadingScreen();
        }

        // ローディング画面を表示する
        static void displayLoadingScreen() {
            // ウィンドウの装飾を非表示にする（フルスクリーン風）
            // 注意: SFMLでは実行時の変更が不安定なため無効化
#if defined(PAXS_USING_SIV3D)
            paxg::Window::setDecorated(false);
#endif

            // ローディング画面の画像を読み込む
            paxg::Texture loading_texture("Data/LoadingScreen/LoadingScreen.png");

            // ウィンドウをクリア
            paxg::Window::clear();

            // 画面サイズを取得
            const int window_width = paxg::Window::width();
            const int window_height = paxg::Window::height();

            // 画像のサイズを取得
            const int texture_width = loading_texture.width();
            const int texture_height = loading_texture.height();

            // 画像をウィンドウサイズに合わせてスケーリング
            // アスペクト比を維持しながらウィンドウに収まるようにする
            const float scale_x = static_cast<float>(window_width) / static_cast<float>(texture_width);
            const float scale_y = static_cast<float>(window_height) / static_cast<float>(texture_height);
            const float scale = (scale_x < scale_y) ? scale_x : scale_y;

            const int scaled_width = static_cast<int>(texture_width * scale);
            const int scaled_height = static_cast<int>(texture_height * scale);

            // 中央配置の座標を計算
            const int x = (window_width - scaled_width) / 2;
            const int y = (window_height - scaled_height) / 2;

            // 画像をリサイズして描画
            loading_texture.resizedDraw(paxg::Vec2i{scaled_width, scaled_height}, paxg::Vec2i{x, y});

            // 画面を更新
#ifdef PAXS_USING_SFML
            paxg::Window::display();
#endif
#if defined(PAXS_USING_SIV3D) || defined(PAXS_USING_DXLIB)
            paxg::Window::update();
#endif
        }

        // ローディング画面を終了し、通常のウィンドウに戻す
        static void endLoadingScreen() {
            // ウィンドウの装飾を再表示
            // 注意: SFMLでは実行時の変更が不安定なため無効化
#if defined(PAXS_USING_SIV3D)
            paxg::Window::setDecorated(true);
#endif
        }
    };

}

#endif // !PAX_MAHOROBA_INIT_LOGO_HPP
