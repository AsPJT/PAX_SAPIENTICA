/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_DXLIB_WINDOW_IMPL_HPP
#define PAX_GRAPHICA_DXLIB_WINDOW_IMPL_HPP

#if defined(PAXS_USING_DXLIB)

#include <DxLib.h>

#if defined(_WIN32)
#include <Windows.h>
#endif

#include <PAX_GRAPHICA/WindowImpl.hpp>

#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/System/AppConst.hpp>

namespace paxg {
    namespace MinWindow {
        LRESULT CALLBACK MyWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg)
            {
            case WM_GETMINMAXINFO:
            {
                MINMAXINFO* pInfo = reinterpret_cast<MINMAXINFO*>(lParam);
                pInfo->ptMinTrackSize.x = paxs::AppConst::min_window_size.x;   // 最小幅
                pInfo->ptMinTrackSize.y = paxs::AppConst::min_window_size.y;  // 最小高さ
                pInfo->ptMaxTrackSize.x = paxs::AppConst::full_hd_window_size.x;   // 最大幅
                pInfo->ptMaxTrackSize.y = paxs::AppConst::full_hd_window_size.y;   // 最大高さ
                return 0; // 自分で処理したよ、という意味
            }
            }

            // それ以外はDxLibに任せる
            return 0;
        }
    } // namespace MinWindow

    class DxLibWindowImpl : public WindowImpl {
    private:
        mutable int cachedWidth = 1;
        mutable int cachedHeight = 1;
        mutable bool cacheDirty = true;

        void updateCache() const {
            if (!cacheDirty) return;

#if defined(__ANDROID__)
            DxLib::GetAndroidDisplayResolution(&cachedWidth, &cachedHeight);
#elif defined(__APPLE__)
            // iOS specific processing
            cachedWidth = 1;
            cachedHeight = 1;
#elif defined(__LINUX__)
            // Linux specific processing
            cachedWidth = 1;
            cachedHeight = 1;
#else
            DxLib::GetDrawScreenSize(&cachedWidth, &cachedHeight);
#endif
            cacheDirty = false;
        }

    public:
        /// @brief DxLib_Init() の前に呼び出される初期化処理
        /// @brief Pre-initialization called before DxLib_Init()
        void preInit() override {
#if !defined(__ANDROID__) && !defined(__APPLE__) && !defined(__LINUX__)
            // ウィンドウモードを通常ウィンドウに設定（DxLib_Init の前）
            DxLib::ChangeWindowMode(TRUE);

            // ウィンドウサイズ変更を許可（DxLib_Init の前に設定する必要がある）
            // Enable window resizing (must be set before DxLib_Init)
            DxLib::SetWindowSizeChangeEnableFlag(TRUE, TRUE);

            // 最大化ボックス（最大化ボタン）を有効化
            DxLib::SetWindowMaximizeButtonBehavior(1); // 1: 通常の最大化, 0: 無効

            // ウィンドウサイズの拡張率を設定（最大化やリサイズ時の拡大率）
            // Set window size extend rate (for maximization and resizing)
            // 1.0 = 拡大なし（デフォルト解像度のまま）
            DxLib::SetWindowSizeExtendRate(1.0);

            DxLib::SetHookWinProc(MinWindow::MyWndProc);
#endif
        }

        /// @brief メイン初期化処理
        /// @brief Main initialization
        void init(int width, int height, const std::string& title) override {
            DxLib::SetGraphMode(width, height, 32);
#if !defined(__ANDROID__) && !defined(__APPLE__) && !defined(__LINUX__)
            DxLib::SetMainWindowText(title.c_str());
#endif
            cachedWidth = width;
            cachedHeight = height;
            cacheDirty = false;
        }

        bool update() override {
            return (DxLib::ScreenFlip() != -1 &&
                    DxLib::ClearDrawScreen() != -1 &&
                    DxLib::ProcessMessage() != -1);
        }

        void setTitle(const std::string& title) override {
#if !defined(__ANDROID__) && !defined(__APPLE__) && !defined(__LINUX__)
            DxLib::SetMainWindowText(title.c_str());
#else
            (void)title; // Suppress unused parameter warning
#endif
        }

        void setSize(int width, int height) override {
            DxLib::SetGraphMode(width, height, 32);
            cachedWidth = width;
            cachedHeight = height;
            cacheDirty = false;
        }

        void setPosition(int, int) override {
            // DxLib does not support window position control
        }

        void setPosition(const Vec2i&) override {
            // DxLib does not support window position control
        }

        void setIcon(const std::string& path) override {
#if defined(_WIN32) && !defined(__ANDROID__)
            // Windows API を使ってアイコンを設定
            HWND hWnd = DxLib::GetMainWindowHandle();
            if (hWnd) {
                // アイコンファイルを読み込む（.icoファイルのみ対応）
                HICON hIcon = (HICON)LoadImageA(
                    NULL,
                    (paxs::AppConfig::getInstance()->getRootPath() + path).c_str(),
                    IMAGE_ICON,
                    0, 0,
                    LR_LOADFROMFILE | LR_DEFAULTSIZE
                );
                if (hIcon) {
                    // ウィンドウアイコンを設定
                    SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
                    SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
                }
            }
#else
            (void)path; // Suppress unused parameter warning
#endif
        }

        void setVisible(bool) override {
            // DxLib does not support visibility control
        }

        void setVSync(bool) override {
            // DxLib does not support VSync control
        }

        void setFPS(int) override {
            // DxLib does not support FPS limit control
        }

        void setMouseCursorVisible(bool) override {
            // DxLib does not support mouse cursor visibility control
        }

        void setMouseCursorGrabbed(bool) override {
            // DxLib does not support mouse cursor grabbing
        }

        void setMouseCursor(const std::string&) override {
            // DxLib does not support mouse cursor setting
        }

        void setMousePosition(int, int) override {
            // DxLib does not support mouse position setting
        }

        void setMousePosition(const Vec2i&) override {
            // DxLib does not support mouse position setting
        }

        void setKeyRepeat(bool) override {
            // DxLib does not support key repeat control
        }

        void setBackgroundColor(const Color color) override {
            DxLib::SetBackgroundColor(color.r, color.g, color.b);
        }

        void setLetterbox(const Color) override {
            // DxLib does not support letterbox
        }

        void setResizable(bool resizable) override {
            DxLib::SetWindowSizeChangeEnableFlag(resizable ? TRUE : FALSE);
        }

        void setDecorated(bool decorated) override {
            DxLib::SetWindowStyleMode(decorated ? 1 : 8); // 1: 通常ウィンドウ, 8: 枠なし
        }

        Vec2i center() const override {
            updateCache();
            return Vec2i{cachedWidth / 2, cachedHeight / 2};
        }

        int width() const override {
            updateCache();
            return cachedWidth;
        }

        int height() const override {
            updateCache();
            return cachedHeight;
        }

        Vec2i size() const override {
            updateCache();
            return Vec2i{cachedWidth, cachedHeight};
        }

        Vec2i getMousePosition() const override {
            // DxLib does not support mouse position query
            return Vec2i(0, 0);
        }

        bool hasFocus() const override {
            return DxLib::GetWindowActiveFlag() == TRUE;
        }

        void clear() override {
            // DxLib does not require explicit clear
        }

        void display() override {
            // DxLib does not require explicit display
        }

        void close() override {
            // DxLib does not support explicit close
        }
    };

} // namespace paxg

#endif // defined(PAXS_USING_DXLIB)

#endif // !PAX_GRAPHICA_DXLIB_WINDOW_IMPL_HPP
