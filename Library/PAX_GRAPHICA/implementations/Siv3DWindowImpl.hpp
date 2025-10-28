/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SIV3D_WINDOW_IMPL_HPP
#define PAX_GRAPHICA_SIV3D_WINDOW_IMPL_HPP

#if defined(PAXS_USING_SIV3D)

#include <Siv3D.hpp>

#include <PAX_GRAPHICA/WindowImpl.hpp>

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

        void setPosition(int x, int y) override {
            s3d::Window::SetPos(x, y);
        }

        void setPosition(const Vec2i& pos) override {
            s3d::Window::SetPos(static_cast<s3d::int32>(pos.x()), static_cast<s3d::int32>(pos.y()));
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

        void setMouseCursorGrabbed(bool) override {
            // Siv3D does not support mouse cursor grabbing
        }

        void setMouseCursor(const std::string& path) override {
            s3d::Cursor::RequestStyle(s3d::Unicode::FromUTF8(path));
        }

        void setMousePosition(int x, int y) override {
            s3d::Cursor::SetPos(x, y);
        }

        void setMousePosition(const Vec2i& pos) override {
            s3d::Cursor::SetPos(static_cast<s3d::int32>(pos.x()), static_cast<s3d::int32>(pos.y()));
        }

        void setKeyRepeat(bool) override {
            // Siv3D does not support key repeat control
        }

        void setBackgroundColor(const Color color) override {
            s3d::Scene::SetBackground(color.color);
        }

        void setLetterbox(const Color color) override {
            s3d::Scene::SetLetterbox(color.color);
        }

        void setResizable(bool resizable) override {
            if (resizable) {
                s3d::Window::SetStyle(s3d::WindowStyle::Sizable);
            } else {
                s3d::Window::SetStyle(s3d::WindowStyle::Fixed);
            }
        }

        Vec2i center() const override {
            return Vec2i(s3d::Scene::Center().x, s3d::Scene::Center().y);
        }

        int width() const override {
            return s3d::Scene::Width();
        }

        int height() const override {
            return s3d::Scene::Height();
        }

        Vec2i size() const override {
            return Vec2i(s3d::Scene::Width(), s3d::Scene::Height());
        }

        Vec2i getMousePosition() const override {
            return Vec2i(static_cast<std::int_least32_t>(s3d::Cursor::PosF().x),
                        static_cast<std::int_least32_t>(s3d::Cursor::PosF().y));
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

#endif // defined(PAXS_USING_SIV3D)

#endif // !PAX_GRAPHICA_SIV3D_WINDOW_IMPL_HPP
