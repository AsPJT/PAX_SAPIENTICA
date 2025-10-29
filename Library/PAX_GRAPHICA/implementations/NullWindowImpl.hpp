/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_NULL_WINDOW_IMPL_HPP
#define PAX_GRAPHICA_NULL_WINDOW_IMPL_HPP

#include <PAX_GRAPHICA/WindowImpl.hpp>

namespace paxg {

    /// @brief Null implementation for when no graphics library is available
    class NullWindowImpl : public WindowImpl {
    public:
        void init(int, int, const std::string&) override {}
        bool update() override { return false; }

        void setTitle(const std::string&) override {}
        void setSize(int, int) override {}
        void setPosition(int, int) override {}
        void setPosition(const Vec2i&) override {}
        void setIcon(const std::string&) override {}
        void setVisible(bool) override {}
        void setVSync(bool) override {}
        void setFPS(int) override {}
        void setMouseCursorVisible(bool) override {}
        void setMouseCursorGrabbed(bool) override {}
        void setMouseCursor(const std::string&) override {}
        void setMousePosition(int, int) override {}
        void setMousePosition(const Vec2i&) override {}
        void setKeyRepeat(bool) override {}
        void setBackgroundColor(const Color) override {}
        void setLetterbox(const Color) override {}
        void setResizable(bool) override {}

        Vec2i center() const override { return Vec2i{}; }
        int width() const override { return 1; }
        int height() const override { return 1; }
        Vec2i size() const override { return Vec2i{}; }
        Vec2i getMousePosition() const override { return Vec2i(0, 0); }
        bool hasFocus() const override { return false; }

        void clear() override {}
        void display() override {}
        void close() override {}
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_NULL_WINDOW_IMPL_HPP
