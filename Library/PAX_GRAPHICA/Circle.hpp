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

#ifdef PAXS_USING_SIV3D
#include <Siv3D.hpp>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#include <PAX_GRAPHICA/IDrawable.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxg {

    struct Circle : public IDrawable
    {
#ifdef PAXS_USING_SIV3D
        s3d::Circle circle;
        Circle(float x, float y, float r) : circle(x, y, r) {}
        Circle(const Vec2& pos, float r) : circle(pos.x, pos.y, r) {}
        operator s3d::Circle() const { return circle; }
#elif defined(PAXS_USING_SFML)
        sf::CircleShape circle;
        Circle(float x, float y, float r) : circle(r) { circle.setPosition(x, y); }
        Circle(const sf::Vector2i& pos, float r) : circle(r) { circle.setPosition(pos.x, pos.y); }
        operator sf::CircleShape() const { return circle; }
#else
        float x, y, r;
        Circle(float x, float y, float r) : x(x), y(y), r(r) {}
        Circle(const Vec2& pos, float r) : x(pos.x), y(pos.y), r(r) {}
#endif
        void draw() const override {
#ifdef PAXS_USING_SIV3D
            circle.draw();
#elif defined(PAXS_USING_SFML)
            Window::window.draw(circle);
#endif
        }

        void drawAt(const Vec2& pos) const override {}
    };
}

#endif // !PAX_GRAPHICA_CIRCLE_HPP