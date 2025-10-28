/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2025 As Project
    [Production]    2023-2025 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_TRIANGLE_HPP
#define PAX_GRAPHICA_TRIANGLE_HPP

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#include <cmath>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxg {

    /// @brief Triangle class that draws a triangle with center, radius and rotation
    /// @details Provides a cross-platform abstraction for drawing triangles
    struct Triangle {
    private:
        float center_x_{};
        float center_y_{};
        float radius_{};
        float rotation_{}; // Rotation angle in radians (0 = pointing right, π = pointing down)

    public:
        constexpr Triangle() = default;

        /// @brief Constructor with center position, radius, and rotation angle
        /// @param center_x X coordinate of the center
        /// @param center_y Y coordinate of the center
        /// @param radius Radius of the circumscribed circle
        /// @param rotation Rotation angle in radians (0 = right, π/2 = down, π = left, 3π/2 = up)
        constexpr Triangle(const float center_x, const float center_y, const float radius, const float rotation = 0.0f)
            : center_x_(center_x), center_y_(center_y), radius_(radius), rotation_(rotation) {}

        constexpr Triangle(const Vec2f& center, const float radius, const float rotation = 0.0f)
            : center_x_(center.x()), center_y_(center.y()), radius_(radius), rotation_(rotation) {}

        // Getters
        constexpr float centerX() const { return center_x_; }
        constexpr float centerY() const { return center_y_; }
        constexpr float radius() const { return radius_; }
        constexpr float rotation() const { return rotation_; }

        // Setters
        void setCenterX(const float x) { center_x_ = x; }
        void setCenterY(const float y) { center_y_ = y; }
        void setCenter(const float x, const float y) { center_x_ = x; center_y_ = y; }
        void setCenter(const Vec2f& center) { center_x_ = center.x(); center_y_ = center.y(); }
        void setRadius(const float r) { radius_ = r; }
        void setRotation(const float rot) { rotation_ = rot; }

        /// @brief Draw the triangle with specified color (defaults to black)
        void draw(const Color& color = Color(0, 0, 0)) const {
#if defined(PAXS_USING_SIV3D)
            s3d::Triangle{ center_x_, center_y_, radius_, rotation_ }.draw(s3d::ColorF{ color.color });

#elif defined(PAXS_USING_SFML)
            // Calculate three vertices of the triangle
            // SFML's rotation is measured from the positive X-axis (pointing right)
            // We need to add 90 degrees (π/2) because our convention is 0 = right, π/2 = down
            const float angle1 = rotation_ - 1.5707963267948966f; // -π/2 (pointing up in our convention)
            const float angle2 = angle1 + 2.0943951023931953f;    // +2π/3
            const float angle3 = angle2 + 2.0943951023931953f;    // +2π/3

            sf::ConvexShape triangle;
            triangle.setPointCount(3);
            triangle.setPoint(0, sf::Vector2f(
                center_x_ + radius_ * std::cos(angle1),
                center_y_ + radius_ * std::sin(angle1)
            ));
            triangle.setPoint(1, sf::Vector2f(
                center_x_ + radius_ * std::cos(angle2),
                center_y_ + radius_ * std::sin(angle2)
            ));
            triangle.setPoint(2, sf::Vector2f(
                center_x_ + radius_ * std::cos(angle3),
                center_y_ + radius_ * std::sin(angle3)
            ));
            triangle.setFillColor(color.color);
            triangle.setOutlineColor(color.color);
            triangle.setOutlineThickness(0);
            paxg::Window::window().draw(triangle);

#elif defined(PAXS_USING_DXLIB)
            // Calculate three vertices of the triangle
            const float angle1 = rotation_ - 1.5707963267948966f; // -π/2
            const float angle2 = angle1 + 2.0943951023931953f;    // +2π/3
            const float angle3 = angle2 + 2.0943951023931953f;    // +2π/3

            const int x1 = static_cast<int>(center_x_ + radius_ * std::cos(angle1));
            const int y1 = static_cast<int>(center_y_ + radius_ * std::sin(angle1));
            const int x2 = static_cast<int>(center_x_ + radius_ * std::cos(angle2));
            const int y2 = static_cast<int>(center_y_ + radius_ * std::sin(angle2));
            const int x3 = static_cast<int>(center_x_ + radius_ * std::cos(angle3));
            const int y3 = static_cast<int>(center_y_ + radius_ * std::sin(angle3));

            DxLib::DrawTriangle(x1, y1, x2, y2, x3, y3, DxLib::GetColor(color.r, color.g, color.b), TRUE);
#endif
        }
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_TRIANGLE_HPP
