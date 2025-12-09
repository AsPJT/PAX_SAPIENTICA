/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SFML_WINDOW_IMPL_HPP
#define PAX_GRAPHICA_SFML_WINDOW_IMPL_HPP

#ifdef PAXS_USING_SFML
#include <SFML/Graphics.hpp>

#include <PAX_GRAPHICA/Interface/WindowImpl.hpp>
#include <PAX_GRAPHICA/SFML_Event.hpp>

#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/System/AppConst.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxg {

    class SFMLWindowImpl : public WindowImpl {
    private:
        sf::RenderWindow m_window;
        paxg::Color backgroundColor{145, 190, 240};
        std::string m_title = "PAX SAPIENTICA Library";
        int m_width = paxs::AppConst::default_window_size.x;
        int m_height = paxs::AppConst::default_window_size.y;

    public:
        SFMLWindowImpl() {
            sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
            sf::ContextSettings settings;
            settings.antiAliasingLevel = 16;
            m_window.create(
                sf::VideoMode({ paxs::AppConst::default_window_size.x, paxs::AppConst::default_window_size.y }, desktop.bitsPerPixel)
                , m_title
                , sf::Style::Default
                , sf::State::Windowed
                , settings);
        }

        // For backward compatibility with existing drawing code
        sf::RenderWindow& getWindow() { return m_window; }

        void init(int width, int height, const std::string& title) override {
            sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
            sf::ContextSettings settings;
            settings.antiAliasingLevel = 16;
            m_window.create(
                sf::VideoMode({static_cast<unsigned int>(width), static_cast<unsigned int>(height)}, desktop.bitsPerPixel)
                , title
                , sf::Style::Default
                , sf::State::Windowed
                , settings);
        }

        bool update() override {
            // 1. 前フレームの描画内容を表示
            // 2. イベント処理
            // 3. 次フレーム用にバッファをクリア
            m_window.display();
            bool upd = paxg::SFML_Event::getInstance()->update(m_window);
            if (upd) {
                // ウィンドウが開いている場合のみクリア
                m_window.clear(backgroundColor);
            }
            return upd;
        }

        void setTitle(const std::string& title) override {
            m_title = title;
            m_window.setTitle(title);
        }

        void setSize(int width, int height) override {
            m_width = width;
            m_height = height;
            m_window.setSize(sf::Vector2u(width, height));
        }

        void setPosition(const paxs::Vector2<int>& pos) override {
            m_window.setPosition(sf::Vector2i(pos.x, pos.y));
        }

        void setIcon(const std::string& path) override {
            sf::Image icon;
            if (!icon.loadFromFile(paxs::AppConfig::getInstance().getRootPath() + path)){
                PAXS_WARNING("Failed to load icon from: " + path);
                return;
            }
            m_window.setIcon({icon.getSize().x, icon.getSize().y}, icon.getPixelsPtr());
        }

        void setVisible(bool visible) override {
            m_window.setVisible(visible);
        }

        void setVSync(bool vsync) override {
            m_window.setVerticalSyncEnabled(vsync);
        }

        void setFPS(int fps) override {
            m_window.setFramerateLimit(fps);
        }

        void setMouseCursorVisible(bool visible) override {
            m_window.setMouseCursorVisible(visible);
        }

        void setMouseCursor(const std::string& path) override {
            sf::Image image;
            if (!image.loadFromFile(path))
                return;

            sf::Cursor cursor(image.getPixelsPtr(), image.getSize(), sf::Vector2u(0, 0));
            m_window.setMouseCursor(cursor);
        }

        void setMousePosition(const paxs::Vector2<int>& pos) override {
            sf::Mouse::setPosition(sf::Vector2i(pos.x, pos.y), m_window);
        }

        void setBackgroundColor(const Color color) override {
            backgroundColor = color;
        }

        void setLetterbox(const Color) override {
            // SFML does not support letterbox
        }

        void setResizable(bool resizable) override {
            // SFML window style cannot be changed after creation
            // This would require recreating the window which causes OpenGL context issues
            // The window is created with Default style (resizable) in init()
            // Note: Changing this at runtime is not safe in SFML 3.0
            (void)resizable; // Suppress unused parameter warning
        }

        void setDecorated(bool decorated) override {
            // SFML does not support changing window decoration after creation
            // Recreating the window causes event handling issues and crashes
            // This feature is not supported in SFML at runtime
            (void)decorated; // Suppress unused parameter warning

            // Note: To change window style in SFML, you must create the window with
            // the desired style from the beginning (in constructor or init())
        }

        paxs::Vector2<int> center() const override {
            return paxs::Vector2<int>{static_cast<int>(m_window.getSize().x / 2), static_cast<int>(m_window.getSize().y / 2)};
        }

        int width() const override {
            return static_cast<int>(m_window.getSize().x);
        }

        int height() const override {
            return static_cast<int>(m_window.getSize().y);
        }

        paxs::Vector2<int> size() const override {
            return paxs::Vector2<int>{static_cast<int>(m_window.getSize().x), static_cast<int>(m_window.getSize().y)};
        }

        paxs::Vector2<int> getMousePosition() const override {
            auto pos = sf::Mouse::getPosition(m_window);
            return paxs::Vector2<int>{pos.x, pos.y};
        }

        bool hasFocus() const override {
            // SFMLのhasFocus()とイベントベースのフォーカス状態の両方をチェック
            // イベントベースの状態を優先（macOSでのフォーカス問題を回避）
            bool event_focus = paxg::SFML_Event::getInstance()->has_focus;
            bool window_focus = m_window.hasFocus();

            // いずれかがtrueならフォーカスありとする（より寛容な判定）
            return event_focus || window_focus;
        }

        void clear() override {
            m_window.clear(backgroundColor);
        }

        void display() override {
            m_window.display();
        }

        void close() override {
            m_window.close();
        }
    };

} // namespace paxg

#endif // PAXS_USING_SFML

#endif // !PAX_GRAPHICA_SFML_WINDOW_IMPL_HPP
