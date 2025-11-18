/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_LINE_HPP
#define PAX_GRAPHICA_LINE_HPP

#include <cmath>

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxg {

    struct Line {
        constexpr Line() = default;
#if defined(PAXS_USING_SIV3D)
        s3d::Line line{};
        constexpr Line(const float sx, const float sy, const float ex, const float ey)
            : line(sx, sy, ex, ey) {}
        constexpr Line(const float sx, const float sy, const paxg::Vec2i& e)
            : line(sx, sy, e.x(), e.y()) {}
        constexpr Line(const paxg::Vec2i& s, const paxg::Vec2i& e)
            : line(s.x(), s.y(), e.x(), e.y()) {}

        // Vec2<double> コンストラクタ (conversion from double to float)
        constexpr Line(const paxg::Vec2<double>& s, const paxg::Vec2<double>& e)
            : line(static_cast<float>(s.x()), static_cast<float>(s.y()),
                   static_cast<float>(e.x()), static_cast<float>(e.y())) {}

        void draw(const double thickness, const paxg::Color& color) const {
            line.draw(thickness, color.color);
        }

        void drawArrow(const double thickness, const paxg::Vec2f& arrowSize, const paxg::Color& color) const {
            line.drawArrow(thickness, s3d::Vec2(arrowSize.x(), arrowSize.y()), color.color);
        }

#elif defined(PAXS_USING_DXLIB)
        float x0{}, y0{}, w0{}, h0{};
        constexpr Line(const float x, const float y, const float w, const float h) :
            x0(x), y0(y), w0(w), h0(h) {}
        constexpr Line(const paxg::Vec2i& pos, const paxg::Vec2i& size)
            : x0(static_cast<float>(pos.x())), y0(static_cast<float>(pos.y())),
            w0(static_cast<float>(size.x())), h0(static_cast<float>(size.y())) {}
        constexpr Line(const paxg::Vec2i& pos, const float w, const float h) :
            x0(static_cast<float>(pos.x())), y0(static_cast<float>(pos.y())), w0(w), h0(h) {}
        constexpr Line(const float x, const float y, const paxg::Vec2i& size)
            : x0(x), y0(y), w0(static_cast<float>(size.x())), h0(static_cast<float>(size.y())) {}

        // Vec2<double> コンストラクタ (conversion from double to float)
        constexpr Line(const paxg::Vec2<double>& s, const paxg::Vec2<double>& e)
            : x0(static_cast<float>(s.x())), y0(static_cast<float>(s.y())),
              w0(static_cast<float>(e.x())), h0(static_cast<float>(e.y())) {}

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

        void drawArrow(const double thickness, const paxg::Vec2f& arrowSize, const paxg::Color& color) const {
            // Draw the main line
            draw(thickness, color);

            // Calculate direction vector
            const float dx = w0 - x0;
            const float dy = h0 - y0;
            const float length = std::sqrt(dx * dx + dy * dy);
            if (length < 0.001f) return; // Too short to draw arrow

            // Adjust arrow size based on line length
            // If line is shorter than arrow, scale down the arrow proportionally
            const float arrowLength = arrowSize.y();
            const float arrowWidth = arrowSize.x();
            const float scale = (length < arrowLength) ? (length / arrowLength) : 1.0f;
            const float adjustedArrowLength = arrowLength * scale;
            const float adjustedArrowWidth = arrowWidth * scale;

            // Normalize direction
            const float ndx = dx / length;
            const float ndy = dy / length;

            // Perpendicular vector
            const float pdx = -ndy;
            const float pdy = ndx;

            // Arrow head vertices
            const float tipX = w0;
            const float tipY = h0;
            const float baseX = w0 - ndx * adjustedArrowLength;
            const float baseY = h0 - ndy * adjustedArrowLength;
            const float leftX = baseX + pdx * adjustedArrowWidth / 2;
            const float leftY = baseY + pdy * adjustedArrowWidth / 2;
            const float rightX = baseX - pdx * adjustedArrowWidth / 2;
            const float rightY = baseY - pdy * adjustedArrowWidth / 2;

            // Draw arrow head lines
            const unsigned int dxColor = DxLib::GetColor(color.r, color.g, color.b);
            DxLib::DrawLine(static_cast<int>(tipX), static_cast<int>(tipY),
                           static_cast<int>(leftX), static_cast<int>(leftY), dxColor);
            DxLib::DrawLine(static_cast<int>(tipX), static_cast<int>(tipY),
                           static_cast<int>(rightX), static_cast<int>(rightY), dxColor);
        }

#elif defined(PAXS_USING_SFML)
        sf::Vertex line[2]{};
        Line(const float sx, const float sy, const float ex, const float ey) {
            line[0].position = sf::Vector2f(sx, sy);
            line[1].position = sf::Vector2f(ex, ey);
        }
        Line(const float sx, const float sy, const paxg::Vec2i& e) {
            line[0].position = sf::Vector2f(sx, sy);
            line[1].position = sf::Vector2f(static_cast<float>(e.x()), static_cast<float>(e.y()));
        }
        Line(const paxg::Vec2i& s, const paxg::Vec2i& e) {
            line[0].position = sf::Vector2f(static_cast<float>(s.x()), static_cast<float>(s.y()));
            line[1].position = sf::Vector2f(static_cast<float>(e.x()), static_cast<float>(e.y()));
        }

        // Vec2<double> コンストラクタ (conversion from double to float)
        Line(const paxg::Vec2<double>& s, const paxg::Vec2<double>& e) {
            line[0].position = sf::Vector2f(static_cast<float>(s.x()), static_cast<float>(s.y()));
            line[1].position = sf::Vector2f(static_cast<float>(e.x()), static_cast<float>(e.y()));
        }

        void draw(const double /*thickness*/, const paxg::Color& color) {
            line[0].color = color;
            line[1].color = color;
            paxg::Window::window().draw(line, 2, sf::PrimitiveType::Lines);
        }

        void drawArrow(const double thickness, const paxg::Vec2f& arrowSize, const paxg::Color& color) {
            // Draw the main line
            draw(thickness, color);

            // Calculate direction vector
            const float dx = line[1].position.x - line[0].position.x;
            const float dy = line[1].position.y - line[0].position.y;
            const float length = std::sqrt(dx * dx + dy * dy);
            if (length < 0.001f) return; // Too short to draw arrow

            // Adjust arrow size based on line length
            // If line is shorter than arrow, scale down the arrow proportionally
            const float arrowLength = arrowSize.y();
            const float arrowWidth = arrowSize.x();
            const float scale = (length < arrowLength) ? (length / arrowLength) : 1.0f;
            const float adjustedArrowLength = arrowLength * scale;
            const float adjustedArrowWidth = arrowWidth * scale;

            // Normalize direction
            const float ndx = dx / length;
            const float ndy = dy / length;

            // Perpendicular vector
            const float pdx = -ndy;
            const float pdy = ndx;

            // Arrow head vertices
            const float tipX = line[1].position.x;
            const float tipY = line[1].position.y;
            const float baseX = tipX - ndx * adjustedArrowLength;
            const float baseY = tipY - ndy * adjustedArrowLength;
            const float leftX = baseX + pdx * adjustedArrowWidth / 2;
            const float leftY = baseY + pdy * adjustedArrowWidth / 2;
            const float rightX = baseX - pdx * adjustedArrowWidth / 2;
            const float rightY = baseY - pdy * adjustedArrowWidth / 2;

            // Draw arrow head lines
            sf::Vertex arrowLeft[2];
            arrowLeft[0].position = sf::Vector2f(tipX, tipY);
            arrowLeft[0].color = color;
            arrowLeft[1].position = sf::Vector2f(leftX, leftY);
            arrowLeft[1].color = color;

            sf::Vertex arrowRight[2];
            arrowRight[0].position = sf::Vector2f(tipX, tipY);
            arrowRight[0].color = color;
            arrowRight[1].position = sf::Vector2f(rightX, rightY);
            arrowRight[1].color = color;

            paxg::Window::window().draw(arrowLeft, 2, sf::PrimitiveType::Lines);
            paxg::Window::window().draw(arrowRight, 2, sf::PrimitiveType::Lines);
        }

#else
        float sx0{}, sy0{}, ex0{}, ey0{};
        constexpr Line(const float sx, const float sy, const float ex, const float ey)
            : sx0(sx), sy0(sy), ex0(ex), ey0(ey) {}
        constexpr Line(const float sx, const float sy, const paxg::Vec2i& e)
            : sx0(sx), sy0(sy), ex0(static_cast<float>(e.x())), ey0(static_cast<float>(e.y())) {}
        constexpr Line(const paxg::Vec2i& s, const paxg::Vec2i& e)
            : sx0(static_cast<float>(s.x())), sy0(static_cast<float>(s.y())),
            ex0(static_cast<float>(e.x())), ey0(static_cast<float>(e.y())) {}

        // Vec2<double> コンストラクタ (conversion from double to float)
        constexpr Line(const paxg::Vec2<double>& s, const paxg::Vec2<double>& e)
            : sx0(static_cast<float>(s.x())), sy0(static_cast<float>(s.y())),
              ex0(static_cast<float>(e.x())), ey0(static_cast<float>(e.y())) {}

        void draw([[maybe_unused]] const double thickness, [[maybe_unused]] const paxg::Color& color) const {
        }

        void drawArrow([[maybe_unused]] const double thickness, [[maybe_unused]] const paxg::Vec2f& arrowSize, [[maybe_unused]] const paxg::Color& color) const {
        }
#endif
    };
}

#endif // !PAX_GRAPHICA_RECT_HPP
