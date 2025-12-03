/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SIV3D_WINDOW_IMPL_HPP
#define PAX_GRAPHICA_SIV3D_WINDOW_IMPL_HPP

#ifdef PAXS_USING_SIV3D
#include <Siv3D.hpp>

#include <PAX_GRAPHICA/Interface/WindowImpl.hpp>

namespace paxg {

    class Siv3DWindowImpl : public WindowImpl {
    public:
        void init(int width, int height, const std::string& title) override {
            s3d::Window::Resize(width, height);
            s3d::Window::SetTitle(s3d::Unicode::FromUTF8(title));
        }

        bool update() override {
            return s3d::System::Update();
        }

        void setTitle(const std::string& title) override {
            s3d::Window::SetTitle(s3d::Unicode::FromUTF8(title));
        }

        void setSize(int width, int height) override {
            s3d::Window::Resize(width, height);
        }

        void setPosition(const paxs::Vector2<int>& pos) override {
            s3d::Window::SetPos(static_cast<s3d::int32>(pos.x), static_cast<s3d::int32>(pos.y));
        }

        void setIcon(const std::string& /*path*/) override {
            // Siv3D does not support setting icon via path
            // s3d::Window::SetIcon(s3d::Unicode::FromUTF8(path));
        }

        void setVisible(bool) override {
            // Siv3D does not support setVisible
        }

        void setVSync(bool) override {
            // Siv3D does not support VSync control via API
        }

        void setFPS(int) override {
            // Siv3D does not support FPS limit control
        }

        void setMouseCursorVisible(bool) override {
            // Siv3D does not support mouse cursor visibility control
        }

        void setMouseCursor(const std::string& path) override {
            s3d::Cursor::RequestStyle(s3d::Unicode::FromUTF8(path));
        }

        void setMousePosition(const paxs::Vector2<int>& pos) override {
            s3d::Cursor::SetPos(static_cast<s3d::int32>(pos.x), static_cast<s3d::int32>(pos.y));
        }

        void setBackgroundColor(const Color color) override {
            s3d::Scene::SetBackground(color);
        }

        void setLetterbox(const Color color) override {
            s3d::Scene::SetLetterbox(color);
        }

        void setResizable(bool resizable) override {
            if (resizable) {
                s3d::Window::SetStyle(s3d::WindowStyle::Sizable);
            } else {
                s3d::Window::SetStyle(s3d::WindowStyle::Fixed);
            }
        }

        void setDecorated(bool decorated) override {
            if (decorated) {
                s3d::Window::SetStyle(s3d::WindowStyle::Sizable);
            } else {
                s3d::Window::SetStyle(s3d::WindowStyle::Frameless);
            }
        }

        paxs::Vector2<int> center() const override {
            return paxs::Vector2<int>(s3d::Scene::Center().x, s3d::Scene::Center().y);
        }

        int width() const override {
            return s3d::Scene::Width();
        }

        int height() const override {
            return s3d::Scene::Height();
        }

        paxs::Vector2<int> size() const override {
            return paxs::Vector2<int>(s3d::Scene::Width(), s3d::Scene::Height());
        }

        paxs::Vector2<int> getMousePosition() const override {
            return paxs::Vector2<int>(static_cast<std::int_least32_t>(s3d::Cursor::PosF().x),
                        static_cast<std::int_least32_t>(s3d::Cursor::PosF().y));
        }

        bool hasFocus() const override {
            return true;
        }

        void clear() override {
            // Siv3D does not require explicit clear
        }

        void display() override {
            // Siv3D does not require explicit display
        }

        void close() override {
            s3d::System::Exit();
        }
    };

} // namespace paxg

#endif // PAXS_USING_SIV3D

#endif // !PAX_GRAPHICA_SIV3D_WINDOW_IMPL_HPP
