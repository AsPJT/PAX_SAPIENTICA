/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SFML_TRIANGLE_HPP
#define PAX_GRAPHICA_SFML_TRIANGLE_HPP

#ifdef PAXS_USING_SFML
#include <cmath>

#include <SFML/Graphics.hpp>

#include <PAX_GRAPHICA/Interface/Triangle.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxg {

    // TriangleShape Implementation
    inline constexpr TriangleShape::TriangleShape(const float radius_, const float rotation_)
        : offset1_x(radius_ * constexprCos(rotation_ - paxs::Math<float>::piHalf()))
        , offset1_y(radius_ * constexprSin(rotation_ - paxs::Math<float>::piHalf()))
        , offset2_x(radius_ * constexprCos(rotation_ - paxs::Math<float>::piHalf() + paxs::Math<float>::pi2() / 3.0f))
        , offset2_y(radius_ * constexprSin(rotation_ - paxs::Math<float>::piHalf() + paxs::Math<float>::pi2() / 3.0f))
        , offset3_x(radius_ * constexprCos(rotation_ - paxs::Math<float>::piHalf() + paxs::Math<float>::pi2() * 2.0f / 3.0f))
        , offset3_y(radius_ * constexprSin(rotation_ - paxs::Math<float>::piHalf() + paxs::Math<float>::pi2() * 2.0f / 3.0f))
        , radius(radius_)
        , rotation(rotation_)
    {}

    inline constexpr float TriangleShape::constexprSin(float x) {
        while (x > paxs::Math<float>::pi()) x -= paxs::Math<float>::pi2();
        while (x < -paxs::Math<float>::pi()) x += paxs::Math<float>::pi2();
        const float x2 = x * x;
        return x * (1.0f - x2 / 6.0f * (1.0f - x2 / 20.0f * (1.0f - x2 / 42.0f)));
    }

    inline constexpr float TriangleShape::constexprCos(float x) {
        return constexprSin(x + paxs::Math<float>::piHalf());
    }

    // Triangle Implementation
    inline constexpr Triangle::Triangle() = default;

    inline constexpr Triangle::Triangle(const float center_x, const float center_y, const float radius, const float rotation)
        : center_x_(center_x), center_y_(center_y), radius_(radius), rotation_(rotation) {}

    inline constexpr Triangle::Triangle(const paxs::Vector2<float>& center, const float radius, const float rotation)
        : center_x_(center.x), center_y_(center.y), radius_(radius), rotation_(rotation) {}

    inline constexpr Triangle::Triangle(const float center_x, const float center_y, const TriangleShape& shape)
        : center_x_(center_x), center_y_(center_y), radius_(shape.radius), rotation_(shape.rotation), shape_(&shape) {}

    inline constexpr Triangle::Triangle(const paxs::Vector2<float>& center, const TriangleShape& shape)
        : center_x_(center.x), center_y_(center.y), radius_(shape.radius), rotation_(shape.rotation), shape_(&shape) {}

    inline constexpr float Triangle::centerX() const { return center_x_; }
    inline constexpr float Triangle::centerY() const { return center_y_; }
    inline constexpr float Triangle::radius() const { return radius_; }
    inline constexpr float Triangle::rotation() const { return rotation_; }

    inline void Triangle::setCenterX(const float x) { center_x_ = x; }
    inline void Triangle::setCenterY(const float y) { center_y_ = y; }
    inline void Triangle::setCenter(const float x, const float y) { center_x_ = x; center_y_ = y; }
    inline void Triangle::setCenter(const paxs::Vector2<float>& center) { center_x_ = center.x; center_y_ = center.y; }
    inline void Triangle::setRadius(const float r) { radius_ = r; }
    inline void Triangle::setRotation(const float rot) { rotation_ = rot; }

    inline void Triangle::draw(const paxg::Color& color) const {
        sf::ConvexShape triangle;
        triangle.setPointCount(3);

        if (shape_) {
            triangle.setPoint(0, sf::Vector2f(center_x_ + shape_->offset1_x, center_y_ + shape_->offset1_y));
            triangle.setPoint(1, sf::Vector2f(center_x_ + shape_->offset2_x, center_y_ + shape_->offset2_y));
            triangle.setPoint(2, sf::Vector2f(center_x_ + shape_->offset3_x, center_y_ + shape_->offset3_y));
        } else {
            const float angle1 = rotation_ - paxs::Math<float>::piHalf();
            const float angle2 = angle1 + paxs::Math<float>::pi2() / 3.0f;
            const float angle3 = angle2 + paxs::Math<float>::pi2() / 3.0f;

            triangle.setPoint(0, sf::Vector2f(center_x_ + radius_ * std::cos(angle1), center_y_ + radius_ * std::sin(angle1)));
            triangle.setPoint(1, sf::Vector2f(center_x_ + radius_ * std::cos(angle2), center_y_ + radius_ * std::sin(angle2)));
            triangle.setPoint(2, sf::Vector2f(center_x_ + radius_ * std::cos(angle3), center_y_ + radius_ * std::sin(angle3)));
        }

        triangle.setFillColor(color);
        paxg::Window::window().draw(triangle);
    }

} // namespace paxg

#endif // PAXS_USING_SFML

#endif // PAX_GRAPHICA_SFML_TRIANGLE_HPP
