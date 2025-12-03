/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SFML_CIRCLE_IMPL_HPP
#define PAX_GRAPHICA_SFML_CIRCLE_IMPL_HPP

#include <PAX_GRAPHICA/Interface/CircleImpl.hpp>

#ifdef PAXS_USING_SFML
#include <SFML/Graphics.hpp>
#include <PAX_GRAPHICA/Window.hpp>
#include <PAX_GRAPHICA/Color.hpp>
#endif

namespace paxg {

#ifdef PAXS_USING_SFML

    /// @brief SFML implementation of CircleImpl
    class SFMLCircleImpl : public CircleImpl {
    private:
        float x, y, r;
        mutable sf::CircleShape circleShape;

    public:
        /// @brief Constructor with position and radius
        /// @param x X-coordinate of the circle center
        /// @param y Y-coordinate of the circle center
        /// @param r Radius of the circle
        SFMLCircleImpl(float x, float y, float r)
            : x(x), y(y), r(r), circleShape(r) {}

        /// @brief Draw the circle without color
        void draw() const override {
            circleShape.setPosition({x, y});
            Window::window().draw(circleShape);
        }

        /// @brief Draw the circle with specified color
        /// @param color The color to draw the circle
        void draw(const Color& color) const override {
            circleShape.setRadius(r);
            circleShape.setPosition({x, y});
            circleShape.setFillColor(color);
            Window::window().draw(circleShape);
        }

        /// @brief Get the position of the circle center
        /// @return Position as Vector2<float>
        paxs::Vector2<float> getPosition() const override {
            return paxs::Vector2<float>(x, y);
        }

        /// @brief Get the radius of the circle
        float getRadius() const override {
            return r;
        }
    };

#endif // PAXS_USING_SFML

}

#endif // !PAX_GRAPHICA_SFML_CIRCLE_IMPL_HPP
