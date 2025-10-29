/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_WINDOW_IMPL_HPP
#define PAX_GRAPHICA_WINDOW_IMPL_HPP

#include <string>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

namespace paxg {

    /// @brief Abstract base class for window implementations
    class WindowImpl {
    public:
        virtual ~WindowImpl() = default;

        virtual void init(int width, int height, const std::string& title) = 0;
        virtual bool update() = 0;

        virtual void setTitle(const std::string& title) = 0;
        virtual void setSize(int width, int height) = 0;
        virtual void setPosition(int x, int y) = 0;
        virtual void setPosition(const Vec2i& pos) = 0;
        virtual void setIcon(const std::string& path) = 0;
        virtual void setVisible(bool visible) = 0;
        virtual void setVSync(bool vsync) = 0;
        virtual void setFPS(int fps) = 0;
        virtual void setMouseCursorVisible(bool visible) = 0;
        virtual void setMouseCursorGrabbed(bool grabbed) = 0;
        virtual void setMouseCursor(const std::string& path) = 0;
        virtual void setMousePosition(int x, int y) = 0;
        virtual void setMousePosition(const Vec2i& pos) = 0;
        virtual void setKeyRepeat(bool repeat) = 0;
        virtual void setBackgroundColor(const paxg::Color color) = 0;
        virtual void setLetterbox(const paxg::Color color) = 0;
        virtual void setResizable(bool resizable) = 0;

        virtual paxg::Vec2i center() const = 0;
        virtual int width() const = 0;
        virtual int height() const = 0;
        virtual paxg::Vec2i size() const = 0;
        virtual paxg::Vec2i getMousePosition() const = 0;
        virtual bool hasFocus() const = 0;

        virtual void clear() = 0;
        virtual void display() = 0;
        virtual void close() = 0;
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_WINDOW_IMPL_HPP
