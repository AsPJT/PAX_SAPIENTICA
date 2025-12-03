/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_CIRCLE_IMPL_HPP
#define PAX_GRAPHICA_CIRCLE_IMPL_HPP

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxg {

    /// @brief Abstract base class for Circle implementation (Strategy pattern)
    /// @note Each graphics library provides its own implementation
    class CircleImpl {
    public:
        virtual ~CircleImpl() = default;

        /// @brief Draw the circle without color
        virtual void draw() const = 0;

        /// @brief Draw the circle with specified color
        /// @param color The color to draw the circle
        virtual void draw(const Color& color) const = 0;

        /// @brief Get the position of the circle center
        /// @return Position as Vector2<float>
        virtual paxs::Vector2<float> getPosition() const = 0;

        /// @brief Get the radius of the circle
        virtual float getRadius() const = 0;
    };

}

#endif // !PAX_GRAPHICA_CIRCLE_IMPL_HPP
