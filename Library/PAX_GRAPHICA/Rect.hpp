/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_RECT_HPP
#define PAX_GRAPHICA_RECT_HPP

/*##########################################################################################

##########################################################################################*/

#ifdef PAXS_USING_SIV3D
#include <Siv3D.hpp>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#include <PAX_GRAPHICA/IDrawable.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxg {

    struct Rect : public paxg::IDrawable {
#ifdef PAXS_USING_SIV3D
        s3d::RectF rect;
        Rect(float x, float y, float w, float h) : rect(x, y, w, h) {}
        Rect(const Vec2& pos, const Vec2& size) : rect(pos.x, pos.y, size.x, size.y) {}
        Rect(const Vec2& pos, float w, float h) : rect(pos.x, pos.y, w, h) {}
        Rect(float x, float y, const Vec2& size) : rect(x, y, size.x, size.y) {}
        operator s3d::RectF() const { return rect; }
#elif defined(PAXS_USING_SFML)
        sf::RectangleShape rect;
        Rect(float x, float y, float w, float h) : rect(sf::Vector2f(w, h)) { rect.setPosition(x, y); }
        Rect(const sf::Vector2i& pos, const sf::Vector2i& size) : rect(sf::Vector2f(size.x, size.y)) { rect.setPosition(pos.x, pos.y); }
        Rect(const sf::Vector2i& pos, float w, float h) : rect(sf::Vector2f(w, h)) { rect.setPosition(pos.x, pos.y); }
        Rect(float x, float y, const sf::Vector2i& size) : rect(sf::Vector2f(size.x, size.y)) { rect.setPosition(x, y); }
        operator sf::RectangleShape() const { return rect; }
#else
        float x, y, w, h;
        Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
        Rect(const Vec2& pos, const Vec2& size) : x(pos.x), y(pos.y), w(size.x), h(size.y) {}
        Rect(const Vec2& pos, float w, float h) : x(pos.x), y(pos.y), w(w), h(h) {}
        Rect(float x, float y, const Vec2& size) : x(x), y(y), w(size.x), h(size.y) {}
#endif
        void draw() const override {
#ifdef PAXS_USING_SIV3D
            rect.draw();
#elif defined(PAXS_USING_SFML)
            Window::window.draw(rect);
#endif
        }

        void drawAt(const Vec2& pos) const override {}
    };
}

#endif // !PAX_GRAPHICA_RECT_HPP