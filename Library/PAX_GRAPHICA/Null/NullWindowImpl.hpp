/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_NULL_WINDOW_IMPL_HPP
#define PAX_GRAPHICA_NULL_WINDOW_IMPL_HPP

#include <PAX_GRAPHICA/Interface/WindowImpl.hpp>

namespace paxg {

    /// @brief Null implementation for when no graphics library is available
    class NullWindowImpl : public WindowImpl {
    public:
        void init(int, int, const std::string&) override {}
        bool update() override { return false; }

        void setTitle(const std::string&) override {}
        void setSize(int, int) override {}
        void setPosition(const paxs::Vector2<int>&) override {}
        void setIcon(const std::string&) override {}
        void setVisible(bool) override {}
        void setVSync(bool) override {}
        void setFPS(int) override {}
        void setMouseCursorVisible(bool) override {}
        void setMouseCursor(const std::string&) override {}
        void setMousePosition(const paxs::Vector2<int>&) override {}
        void setBackgroundColor(const Color) override {}
        void setLetterbox(const Color) override {}
        void setResizable(bool) override {}
        void setDecorated(bool) override {}

        paxs::Vector2<int> center() const override { return {0, 0}; }
        int width() const override { return 1; }
        int height() const override { return 1; }
        paxs::Vector2<int> size() const override { return {0, 0}; }
        paxs::Vector2<int> getMousePosition() const override { return {0, 0}; }
        bool hasFocus() const override { return false; }

        void clear() override {}
        void display() override {}
        void close() override {}
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_NULL_WINDOW_IMPL_HPP
