/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_WINDOW_HPP
#define PAX_GRAPHICA_WINDOW_HPP

#include <memory>
#include <string>

#include <PAX_SAPIENTICA/Type/Vector2.hpp>

#if defined(PAXS_USING_SIV3D)
#include <PAX_GRAPHICA/implementations/Siv3DWindowImpl.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <PAX_GRAPHICA/implementations/DxLibWindowImpl.hpp>
#elif defined(PAXS_USING_SFML)
#include <PAX_GRAPHICA/implementations/SFMLWindowImpl.hpp>
#else
#include <PAX_GRAPHICA/implementations/NullWindowImpl.hpp>
#endif

#include <PAX_GRAPHICA/WindowImpl.hpp>

namespace paxg {

    /// @brief Singleton window manager with strategy pattern for multi-library support
    class Window {
    private:
        static std::unique_ptr<WindowImpl> impl;

        Window() = delete;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        static WindowImpl& getImpl() {
            if (!impl) {
#if defined(PAXS_USING_SIV3D)
                impl = std::make_unique<Siv3DWindowImpl>();
#elif defined(PAXS_USING_DXLIB)
                impl = std::make_unique<DxLibWindowImpl>();
#elif defined(PAXS_USING_SFML)
                impl = std::make_unique<SFMLWindowImpl>();
#else
                impl = std::make_unique<NullWindowImpl>();
#endif
            }
            return *impl;
        }

    public:
        /// @brief ライブラリ初期化前の設定を行う（DxLib_Init前など）
        /// @brief Pre-initialization settings (before DxLib_Init, etc.)
        static void PreInit() {
            getImpl().preInit();
        }

        /// @brief メイン初期化
        /// @brief Main initialization
        static void Init(int width, int height, const std::string& title) {
            getImpl().init(width, height, title);
        }

        static bool update() {
            return getImpl().update();
        }

        static void setTitle(const std::string& title) {
            getImpl().setTitle(title);
        }

        static void setSize(const int width, const int height) {
            getImpl().setSize(width, height);
        }

        static void setPosition(int x, int y) {
            getImpl().setPosition(x, y);
        }

        static void setPosition(const paxg::Vec2i& pos) {
            getImpl().setPosition(pos);
        }

        /// @brief Set the window icon
        /// @param path Path to the icon file (relative to root path)
        static void setIcon(const std::string& path) {
            getImpl().setIcon(path);
        }

        static void setVisible(bool visible) {
            getImpl().setVisible(visible);
        }

        static void setVSync(bool vsync) {
            getImpl().setVSync(vsync);
        }

        static void setFPS(int fps) {
            getImpl().setFPS(fps);
        }

        static void setMouseCursorVisible(bool visible) {
            getImpl().setMouseCursorVisible(visible);
        }

        static void setMouseCursorGrabbed(bool grabbed) {
            getImpl().setMouseCursorGrabbed(grabbed);
        }

        static void setMouseCursor(const std::string& path) {
            getImpl().setMouseCursor(path);
        }

        static void setMousePosition(int x, int y) {
            getImpl().setMousePosition(x, y);
        }

        static void setMousePosition(const paxg::Vec2i& pos) {
            getImpl().setMousePosition(pos);
        }

        static void setKeyRepeat(bool repeat) {
            getImpl().setKeyRepeat(repeat);
        }

        static void setBackgroundColor(const paxg::Color color) {
            getImpl().setBackgroundColor(color);
        }

        static void setLetterbox(const paxg::Color color) {
            getImpl().setLetterbox(color);
        }

        static void setResizable(bool resizable) {
            getImpl().setResizable(resizable);
        }

        /// @brief ウィンドウの装飾（タイトルバー等）を設定
        /// @param decorated true: 表示, false: 非表示
        static void setDecorated(bool decorated) {
            getImpl().setDecorated(decorated);
        }

        static paxg::Vec2i center() {
            return getImpl().center();
        }

        static int width() {
            return getImpl().width();
        }

        static int height() {
            return getImpl().height();
        }

        static paxg::Vec2i size() {
            return getImpl().size();
        }

        /// @brief Get window size (alias for size() to match Siv3D Scene::Size())
        static paxs::Vector2<int> Size() {
            auto s = getImpl().size();
            return paxs::Vector2<int>(s.x(), s.y());
        }

        static paxg::Vec2i getMousePosition() {
            return getImpl().getMousePosition();
        }

        static bool hasFocus() {
            // Return true if window is not initialized yet (to avoid crashes during startup)
            if (!impl) return true;
            return getImpl().hasFocus();
        }

        static void clear() {
            getImpl().clear();
        }

        static void display() {
            getImpl().display();
        }

        static void close() {
            getImpl().close();
        }

#if defined(PAXS_USING_SFML)
        // For backward compatibility with existing drawing code
        static sf::RenderWindow& window() {
            return static_cast<SFMLWindowImpl&>(getImpl()).getWindow();
        }
#endif
    };

    // Static member definition
    std::unique_ptr<WindowImpl> Window::impl = nullptr;

} // namespace paxg

#endif // !PAX_GRAPHICA_WINDOW_HPP
