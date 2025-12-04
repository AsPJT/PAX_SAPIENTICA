/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_NULL_CIRCLE_IMPL_HPP
#define PAX_GRAPHICA_NULL_CIRCLE_IMPL_HPP

#include <PAX_GRAPHICA/Interface/CircleImpl.hpp>

namespace paxg {

    /// @brief Null (no-op) implementation of CircleImpl
    /// @note Used when no graphics library is available
    class NullCircleImpl : public CircleImpl {
    private:
        float x, y, r;

    public:
        /// @brief Constructor with position and radius
        /// @param x X-coordinate of the circle center
        /// @param y Y-coordinate of the circle center
        /// @param r Radius of the circle
        constexpr NullCircleImpl(float x, float y, float r)
            : x(x), y(y), r(r) {}

        /// @brief Draw the circle without color (no-op)
        void draw() const override {
            // No operation
        }

        /// @brief Draw the circle with specified color (no-op)
        /// @param color The color to draw the circle (unused)
        void draw(const Color&) const override {
            // No operation
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

}

#endif // !PAX_GRAPHICA_NULL_CIRCLE_IMPL_HPP
