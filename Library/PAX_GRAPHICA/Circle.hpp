/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_CIRCLE_HPP
#define PAX_GRAPHICA_CIRCLE_HPP

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#include <PAX_GRAPHICA/SFML_Circle.hpp>
#endif

#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxg {

    struct Circle
    {
#if defined(PAXS_USING_SIV3D)
        s3d::Circle circle;
        constexpr Circle(const float x, const float y, const float r) : circle(x, y, r) {}
        constexpr Circle(const paxg::Vec2i& pos, const float r) : circle(pos.x(), pos.y(), r) {}
        constexpr Circle(const paxg::Vec2<double>& pos, const float r) : circle(static_cast<float>(pos.x()), static_cast<float>(pos.y()), r) {}
        constexpr operator s3d::Circle() const { return circle; }
#else
        float x, y, r;
        constexpr Circle(const float x, const float y, const float r) : x(x), y(y), r(r) {}
        constexpr Circle(const paxg::Vec2i& pos, const float r) : x(static_cast<float>(pos.x())), y(static_cast<float>(pos.y())), r(r) {}
        constexpr Circle(const paxg::Vec2<double>& pos, const float r) : x(static_cast<float>(pos.x())), y(static_cast<float>(pos.y())), r(r) {}
#endif
        void draw() const {
#if defined(PAXS_USING_SIV3D)
            circle.draw();

#elif defined(PAXS_USING_SFML)
            SFML_Circle::getInstance()->circle.setRadius(r);
            SFML_Circle::getInstance()->circle.setPosition({ x, y });
            paxg::Window::window().draw(SFML_Circle::getInstance()->circle);
#endif
        }

#if defined(PAXS_USING_SIV3D)
        void draw(const paxg::Color& c_) const {
            circle.draw(c_.color);
        }
#elif defined(PAXS_USING_DXLIB)
        void draw(const paxg::Color& c_) const {
            DxLib::DrawCircle(
                static_cast<int>(x), static_cast<int>(y), static_cast<int>(r),
                DxLib::GetColor(c_.r, c_.g, c_.b), TRUE);
        }
#elif defined(PAXS_USING_SFML)
        void draw(const paxg::Color& c_) const {
            SFML_Circle::getInstance()->circle.setRadius(r);
            SFML_Circle::getInstance()->circle.setPosition({ x, y });

            SFML_Circle::getInstance()->circle.setFillColor(c_.color);
            paxg::Window::window().draw(SFML_Circle::getInstance()->circle);
        }
#else
        void draw(const paxg::Color&) const {}
#endif
    };
}

#endif // !PAX_GRAPHICA_CIRCLE_HPP
