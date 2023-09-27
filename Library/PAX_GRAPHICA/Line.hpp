/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_LINE_HPP
#define PAX_GRAPHICA_LINE_HPP

/*##########################################################################################

##########################################################################################*/

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#endif

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/IDrawable.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxg {

    struct Line {
        constexpr Line() = default;
#if defined(PAXS_USING_SIV3D)
        s3d::Line line{};
        constexpr Line(const float sx, const float sy, const float ex, const float ey)
            : line(sx, sy, ex, ey) {}
        constexpr Line(const float sx, const float sy, const Vec2i& e)
            : line(sx, sy, e.x(), e.y()) {}
        constexpr Line(const Vec2i& s, const Vec2i& e)
            : line(s.x(), s.y(), e.x(), e.y()) {}

        void draw(const double thickness, const paxg::Color& color) const {
            line.draw(thickness, color.color);
        }
#elif defined(PAXS_USING_SFML)
        sf::Vertex line[2]{};
        Line(const float sx, const float sy, const float ex, const float ey) {
            line[0].position = sf::Vector2f(sx, sy);
            line[1].position = sf::Vector2f(ex, ey);
        }
        Line(const float sx, const float sy, const Vec2i& e) {
            line[0].position = sf::Vector2f(sx, sy);
            line[1].position = sf::Vector2f(e.x(), e.y());
        }
        Line(const Vec2i& s, const Vec2i& e) {
            line[0].position = sf::Vector2f(s.x(), s.y());
            line[1].position = sf::Vector2f(e.x(), e.y());
        }

        void draw(const double thickness, const paxg::Color& color) {
            line[0].color = color;
            line[1].color = color;
            Window::window.draw(line, 2, sf::Lines);
        }
#elif defined(PAXS_USING_DXLIB)
        float x0{}, y0{}, w0{}, h0{};
        constexpr Line(const float x, const float y, const float w, const float h) :
            x0(x), y0(y), w0(w), h0(h) {}
        constexpr Line(const Vec2i& pos, const Vec2i& size)
            : x0(static_cast<float>(pos.x())), y0(static_cast<float>(pos.y())),
            w0(static_cast<float>(size.x())), h0(static_cast<float>(size.y())) {}
        constexpr Line(const Vec2i& pos, const float w, const float h) :
            x0(static_cast<float>(pos.x())), y0(static_cast<float>(pos.y())), w0(w), h0(h) {}
        constexpr Line(const float x, const float y, const Vec2i& size)
            : x0(x), y0(y), w0(static_cast<float>(size.x())), h0(static_cast<float>(size.y())) {}

        void draw(const double thickness, const paxg::Color& color) const {
            // 直線の場合
            if (y0 == h0 || x0 == w0) {
                const int x1 = static_cast<int>(x0 - thickness / 2);
                const int y1 = static_cast<int>(y0 - thickness / 2);
                const int w1 = static_cast<int>(w0 + thickness / 2);
                const int h1 = static_cast<int>(h0 + thickness / 2);

                DxLib::DrawBox(
                    x1,
                    y1,
                    (w1 == x1) ? w1 + 1 : w1,
                    (h1 == y1) ? h1 + 1 : h1,
                    DxLib::GetColor(color.r, color.g, color.b), TRUE);
            }
        }
#else
        float sx0{}, sy0{}, ex0{}, ey0{};
        constexpr Line(const float sx, const float sy, const float ex, const float ey)
            : sx0(sx), sy0(sy), ex0(ex), ey0(ey) {}
        constexpr Line(const float sx, const float sy, const Vec2i& e)
            : sx0(sx), sy0(sy), ex0(static_cast<float>(e.x())), ey0(static_cast<float>(e.y())) {}
        constexpr Line(const Vec2i& s, const Vec2i& e)
            : sx0(static_cast<float>(s.x())), sy0(static_cast<float>(s.y())),
            ex0(static_cast<float>(e.x())), ey0(static_cast<float>(e.y())) {}

        void draw([[maybe_unused]] const double thickness, [[maybe_unused]] const paxg::Color& color) const {
        }
#endif
    };
}

#endif // !PAX_GRAPHICA_RECT_HPP
