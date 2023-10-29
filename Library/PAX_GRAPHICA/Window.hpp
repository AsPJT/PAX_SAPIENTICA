/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_WINDOW_HPP
#define PAX_GRAPHICA_WINDOW_HPP

/*##########################################################################################

##########################################################################################*/

#include <string>

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

namespace paxg {

    namespace Window {
#ifdef PAXS_USING_SFML
        sf::RenderWindow window(sf::VideoMode(1280, 720), "PAX SAPIENTICA Library");
        paxg::Color backgroundColor = paxg::Color(0, 0, 0);
#endif // PAXS_USING_SFML

#if defined(PAXS_USING_SIV3D)
        void Init(int width, int height, const std::string& title) {
            s3d::Window::Resize(width, height);
            s3d::Window::SetTitle(s3d::Unicode::FromUTF8(title));
        }
#elif defined(PAXS_USING_DXLIB)
        void Init(int width, int height, const std::string& title) {
            DxLib::SetGraphMode(width, height, 32);
#if defined(__ANDROID__)
            // Android 専用処理
#elif defined(__APPLE__)
            // iOS 専用処理
#elif defined(__LINUX__)
            // Linux 専用処理
#else
            // その他の処理 (Windows)
            DxLib::SetMainWindowText(title.c_str());
#endif
        }
#elif defined(PAXS_USING_SFML)
        void Init(int width, int height, const std::string& title) {
            window.create(sf::VideoMode(width, height), title);
        }
#else
        void Init(int, int, const std::string&) {}
#endif

        bool update() {
#if defined(PAXS_USING_SIV3D)
            return s3d::System::Update();

#elif defined(PAXS_USING_DXLIB)
            return (DxLib::ScreenFlip() != -1 &&
                DxLib::ClearDrawScreen() != -1 &&
                DxLib::ProcessMessage() != -1);

#elif defined(PAXS_USING_SFML)
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    return false;
            }

            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }

            return true;

#else
            return false;
#endif
        }

        // タイトルを設定
#if defined(PAXS_USING_SIV3D)
        void setTitle(const std::string& title) {
            s3d::Window::SetTitle(s3d::Unicode::FromUTF8(title));
        }

#elif defined(PAXS_USING_DXLIB)
        void setTitle(const std::string& title) {
#if defined(__ANDROID__)
            // Android 専用処理
#elif defined(__APPLE__)
            // iOS 専用処理
#elif defined(__LINUX__)
            // Linux 専用処理
#else
            // その他の処理 (Windows)
            DxLib::SetMainWindowText(title.c_str());
#endif
        }
#elif defined(PAXS_USING_SFML)
        void setTitle(const std::string& title) {
            window.setTitle(title);
        }
#else
        void setTitle(const std::string&) {}
#endif

        // ウィンドウの中心を取得
        Vec2i center() {
#if defined(PAXS_USING_SIV3D)
            return Vec2i(s3d::Scene::Center().x, s3d::Scene::Center().y);

#elif defined(PAXS_USING_DXLIB)
            int width = 0, height = 0;
#if defined(__ANDROID__)
            // Android 専用処理
            DxLib::GetAndroidDisplayResolution(&width, &height);
#elif defined(__APPLE__)
            // iOS 専用処理
#elif defined(__LINUX__)
            // Linux 専用処理
#else
            // その他の処理 (Windows)
            DxLib::GetWindowSize(&width, &height);
            // DxLib::GetWindowSize(&width, &height);
#endif
            return Vec2i{ static_cast<int>(width / 2) ,static_cast<int>(height / 2) };

#elif defined(PAXS_USING_SFML)
            return Vec2i{ static_cast<int>(window.getSize().x / 2) ,static_cast<int>(window.getSize().y / 2) };

#else
            return Vec2i{};
#endif
        }

        // ウィンドウの幅を取得
        int width() {
#if defined(PAXS_USING_SIV3D)
            return s3d::Scene::Width();

#elif defined(PAXS_USING_DXLIB)
            int width = 1, height = 1; // 0 除算を防ぐために 1 を指定

#if defined(__ANDROID__)
            // Android 専用処理
            DxLib::GetAndroidDisplayResolution(&width, &height);
#elif defined(__APPLE__)
            // iOS 専用処理
#elif defined(__LINUX__)
            // Linux 専用処理
#else
            // その他の処理 (Windows)
            DxLib::GetWindowSize(&width, &height);
            // DxLib::GetWindowSize(&width, &height);
#endif
            return static_cast<int>(width);

#elif defined(PAXS_USING_SFML)
            return static_cast<int>(window.getSize().x);

#else
            return 1;
#endif
        }

        // ウィンドウの高さを取得
        int height() {
#if defined(PAXS_USING_SIV3D)
            return s3d::Scene::Height();

#elif defined(PAXS_USING_DXLIB)
            int width = 1, height = 1; // 0 除算を防ぐために 1 を指定
#if defined(__ANDROID__)
            // Android 専用処理
            DxLib::GetAndroidDisplayResolution(&width, &height);
#elif defined(__APPLE__)
            // iOS 専用処理
#elif defined(__LINUX__)
            // Linux 専用処理
#else
            // その他の処理 (Windows)
            DxLib::GetWindowSize(&width, &height);
            // DxLib::GetWindowSize(&width, &height);
#endif
            return static_cast<int>(height);

#elif defined(PAXS_USING_SFML)
            return static_cast<int>(window.getSize().y);

#else
            return 1;
#endif
        }

        // ウィンドウの大きさを取得
        paxg::Vec2i size() {
#if defined(PAXS_USING_SIV3D)
            return paxg::Vec2i(s3d::Scene::Width(), s3d::Scene::Height());

#elif defined(PAXS_USING_DXLIB)
            int width = 0, height = 0;
#if defined(__ANDROID__)
            // Android 専用処理
            DxLib::GetAndroidDisplayResolution(&width, &height);
#elif defined(__APPLE__)
            // iOS 専用処理
#elif defined(__LINUX__)
            // Linux 専用処理
#else
            // その他の処理 (Windows)
            DxLib::GetWindowSize(&width, &height);
            // DxLib::GetWindowSize(&width, &height);
#endif
            return Vec2i{ static_cast<int>(width) ,static_cast<int>(height) };

#elif defined(PAXS_USING_SFML)
            return paxg::Vec2i{ static_cast<int>(window.getSize().x) ,static_cast<int>(window.getSize().y) };

#else
            return paxg::Vec2i{};
#endif
        }

        // 画面サイズを変更
#if defined(PAXS_USING_SIV3D)
        void setSize(const int width, const int height) {
            s3d::Window::Resize(width, height);
        }

#elif defined(PAXS_USING_DXLIB)
        void setSize(const int width, const int height) {
            DxLib::SetGraphMode(width, height, 32);
        }

#elif defined(PAXS_USING_SFML)
        void setSize(const int width, const int height) {
            window.setSize(sf::Vector2u(width, height));
        }
#else
        void setSize(const int, const int) {}
#endif

#if defined(PAXS_USING_SIV3D)
        void setPosition(int x, int y) {
            s3d::Window::SetPos(x, y);
        }
#elif defined(PAXS_USING_SFML)
        void setPosition(int x, int y) {
            window.setPosition(sf::Vector2i(x, y));
        }
#else
        void setPosition(int, int) {}
#endif

#if defined(PAXS_USING_SIV3D)
        void setPosition(const paxg::Vec2i& pos) {
            s3d::Window::SetPos(static_cast<s3d::int32>(pos.x()), static_cast<s3d::int32>(pos.y()));
        }
#elif defined(PAXS_USING_SFML)
        void setPosition(const paxg::Vec2i& pos) {
            window.setPosition(pos);
        }
#else
        void setPosition(const paxg::Vec2i&) {}
#endif

#if defined(PAXS_USING_SIV3D)
        void setIcon(const std::string& /*path*/) {
            // s3d::Window::SetIcon(s3d::Unicode::FromUTF8(path));
        }
#elif defined(PAXS_USING_SFML)
        void setIcon(const std::string& path) {
            sf::Image icon;
            if (!icon.loadFromFile(path))
                return;
            window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        }
#else
        void setIcon(const std::string&) {}
#endif

#if defined(PAXS_USING_SFML)
        void setVisible(bool visible) {
            window.setVisible(visible);
        }
#else
        void setVisible(bool) {}
#endif

#if defined(PAXS_USING_SFML)
        void setVSync(bool vsync) {
            window.setVerticalSyncEnabled(vsync);
        }
#else
        void setVSync(bool) {}
#endif

#if defined(PAXS_USING_SFML)
        void setFPS(int fps) {
            window.setFramerateLimit(fps);
        }
#else
        void setFPS(int) {}
#endif

#if defined(PAXS_USING_SFML)
        void setMouseCursorVisible(bool visible) {
            window.setMouseCursorVisible(visible);
        }
#else
        void setMouseCursorVisible(bool) {}
#endif

#if defined(PAXS_USING_SFML)
        void setMouseCursorGrabbed(bool grabbed) {
            window.setMouseCursorGrabbed(grabbed);
        }
#else
        void setMouseCursorGrabbed(bool) {}
#endif


#if defined(PAXS_USING_SIV3D)
        void setMouseCursor(const std::string& path) {
            s3d::Cursor::RequestStyle(s3d::Unicode::FromUTF8(path));
        }
#elif defined(PAXS_USING_SFML)
        void setMouseCursor(const std::string& path) {
            sf::Image image;
            if (!image.loadFromFile(path))
                return;

            sf::Cursor cursor;
            cursor.loadFromPixels(image.getPixelsPtr(), image.getSize(), sf::Vector2u(0, 0));
            window.setMouseCursor(cursor);
        }
#else
        void setMouseCursor(const std::string&) {}
#endif

        // マウスの位置を設定
#if defined(PAXS_USING_SIV3D)
        void setMousePosition(int x, int y) {
            s3d::Cursor::SetPos(x, y);
        }
#elif defined(PAXS_USING_SFML)
        void setMousePosition(int x, int y) {
            sf::Mouse::setPosition(sf::Vector2i(x, y), window);
        }
#else
        void setMousePosition(int, int) {}
#endif

        // マウスの位置を設定
#if defined(PAXS_USING_SIV3D)
        void setMousePosition(const paxg::Vec2i& pos) {
            s3d::Cursor::SetPos(static_cast<s3d::int32>(pos.x()), static_cast<s3d::int32>(pos.y()));
        }
#elif defined(PAXS_USING_SFML)
        void setMousePosition(const paxg::Vec2i& pos) {
            sf::Mouse::setPosition(pos, window);
        }
#else
        void setMousePosition(const paxg::Vec2i&) {}
#endif

        // マウスの位置を取得
        paxg::Vec2i getMousePosition() {
#if defined(PAXS_USING_SIV3D)
            return paxg::Vec2i(static_cast<std::int_least32_t>(s3d::Cursor::PosF().x), static_cast<std::int_least32_t>(s3d::Cursor::PosF().y));

#elif defined(PAXS_USING_SFML)
            return sf::Mouse::getPosition(window);
#else
            return paxg::Vec2i(0, 0);
#endif
        }

#if defined(PAXS_USING_SFML)
        void setKeyRepeat(bool repeat) {
            window.setKeyRepeatEnabled(repeat);
        }
#else
        void setKeyRepeat(bool) {}
#endif

        // 背景色を指定
#if defined(PAXS_USING_SIV3D)
        void setBackgroundColor(const paxg::Color color) {
            s3d::Scene::SetBackground(color.color);
        }
#elif defined(PAXS_USING_DXLIB)
        void setBackgroundColor(const paxg::Color color) {
            DxLib::SetBackgroundColor(color.r, color.g, color.b);
        }
#elif defined(PAXS_USING_SFML)
        void setBackgroundColor(const paxg::Color color) {
            backgroundColor = color;
        }
#else
        void setBackgroundColor(const paxg::Color) {}
#endif

        // ウィンドウの上下左右にできる背景の余白の色を設定
#if defined(PAXS_USING_SIV3D)
        void setLetterbox(const paxg::Color color) {
            s3d::Scene::SetLetterbox(color.color);
        }
#else
        void setLetterbox(const paxg::Color) {}
#endif

        void clear() {
#if defined(PAXS_USING_SIV3D)
            // s3d::Graphics::Clear();

#elif defined(PAXS_USING_SFML)
            window.clear(backgroundColor.color);
#endif
        }

        void display() {
#if defined(PAXS_USING_SIV3D)
            // s3d::Graphics::Flush();

#elif defined(PAXS_USING_SFML)
            window.display();
#endif
        }

        void close() {
#if defined(PAXS_USING_SIV3D)
            s3d::System::Exit();

#elif defined(PAXS_USING_SFML)
            window.close();

#endif
        }
    };
}

#endif // !PAX_GRAPHICA_WINDOW_HPP
