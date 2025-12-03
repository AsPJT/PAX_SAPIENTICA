/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SIV3D_CIRCLE_IMPL_HPP
#define PAX_GRAPHICA_SIV3D_CIRCLE_IMPL_HPP

#include <PAX_GRAPHICA/Interface/CircleImpl.hpp>

#ifdef PAXS_USING_SIV3D
#include <Siv3D.hpp>
#endif

namespace paxg {

    struct Color;

#ifdef PAXS_USING_SIV3D

    /// @brief Siv3D implementation of CircleImpl
    class Siv3DCircleImpl : public CircleImpl {
    private:
        s3d::Circle circle;

    public:
        /// @brief Constructor with position and radius
        /// @param x X-coordinate of the circle center
        /// @param y Y-coordinate of the circle center
        /// @param r Radius of the circle
        constexpr Siv3DCircleImpl(float x, float y, float r)
            : circle(x, y, r) {}

        /// @brief Draw the circle without color
        void draw() const override {
            circle.draw();
        }

        /// @brief Draw the circle with specified color
        /// @param color The color to draw the circle
        void draw(const Color& color) const override {
            circle.draw(color.color);
        }

        /// @brief Get the position of the circle center
        /// @return Position as Vector2<float>
        paxs::Vector2<float> getPosition() const override {
            return paxs::Vector2<float>(static_cast<float>(circle.x), static_cast<float>(circle.y));
        }

        /// @brief Get the radius of the circle
        float getRadius() const override {
            return static_cast<float>(circle.r);
        }

        /// @brief Get the underlying Siv3D circle
        constexpr const s3d::Circle& getNativeCircle() const {
            return circle;
        }
    };

#endif // PAXS_USING_SIV3D

}

#endif // !PAX_GRAPHICA_SIV3D_CIRCLE_IMPL_HPP
