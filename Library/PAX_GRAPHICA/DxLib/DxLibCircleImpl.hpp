/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_DXLIB_CIRCLE_IMPL_HPP
#define PAX_GRAPHICA_DXLIB_CIRCLE_IMPL_HPP

#include <PAX_GRAPHICA/Interface/CircleImpl.hpp>

#ifdef PAXS_USING_DXLIB
#include <DxLib.h>
#include <PAX_GRAPHICA/Color.hpp>
#endif

namespace paxg {

    struct Color;

#ifdef PAXS_USING_DXLIB

    /// @brief DxLib implementation of CircleImpl
    class DxLibCircleImpl : public CircleImpl {
    private:
        float x, y, r;

    public:
        /// @brief Constructor with position and radius
        /// @param x X-coordinate of the circle center
        /// @param y Y-coordinate of the circle center
        /// @param r Radius of the circle
        constexpr DxLibCircleImpl(float x, float y, float r)
            : x(x), y(y), r(r) {}

        /// @brief Draw the circle without color
        /// @note DxLib requires color parameter, so this implementation is not supported
        void draw() const override {
            // DxLib does not support drawing without specifying color
            // This is a no-op implementation
        }

        /// @brief Draw the circle with specified color
        /// @param color The color to draw the circle
        void draw(const Color& color) const override {
            DxLib::DrawCircle(
                static_cast<int>(x), static_cast<int>(y), static_cast<int>(r),
                DxLib::GetColor(color.r, color.g, color.b), TRUE);
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

#endif // PAXS_USING_DXLIB

}

#endif // !PAX_GRAPHICA_DXLIB_CIRCLE_IMPL_HPP
