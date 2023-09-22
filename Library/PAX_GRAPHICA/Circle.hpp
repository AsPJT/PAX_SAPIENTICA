/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_CIRCLE_HPP
#define PAX_GRAPHICA_CIRCLE_HPP

/*##########################################################################################

##########################################################################################*/

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#endif

#include <PAX_GRAPHICA/IDrawable.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxg {

    struct Circle : public IDrawable
    {
#if defined(PAXS_USING_SIV3D)
        s3d::Circle circle;
        constexpr Circle(const float x, const float y, const float r) : circle(x, y, r) {}
        constexpr Circle(const paxg::Vec2i& pos, const float r) : circle(pos.x(), pos.y(), r) {}
        constexpr operator s3d::Circle() const { return circle; }
#elif defined(PAXS_USING_SFML)
        sf::CircleShape circle;
        Circle(float x, float y, float r) : circle(r) { circle.setPosition(x, y); }
        Circle(const sf::Vector2i& pos, float r) : circle(r) { circle.setPosition(pos.x, pos.y); }
        operator sf::CircleShape() const { return circle; }
#else
        float x, y, r;
        constexpr Circle(const float x, const float y, const float r) : x(x), y(y), r(r) {}
        constexpr Circle(const paxg::Vec2i& pos, const float r) : x(static_cast<float>(pos.x())), y(static_cast<float>(pos.y())), r(r) {}
#endif
        void draw() const override {
#if defined(PAXS_USING_SIV3D)
            circle.draw();
#elif defined(PAXS_USING_SFML)
            Window::window.draw(circle);
#endif
        }

        void drawAt([[maybe_unused]] const Vec2f& pos) const override {}
    };
}

#endif // !PAX_GRAPHICA_CIRCLE_HPP
