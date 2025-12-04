/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_NULL_TRIANGLE_HPP
#define PAX_GRAPHICA_NULL_TRIANGLE_HPP

#include <PAX_GRAPHICA/Interface/Triangle.hpp>

namespace paxg {

    // TriangleShape Implementation
    inline constexpr TriangleShape::TriangleShape(const float radius_, const float rotation_)
        : offset1_x(0.0f), offset1_y(0.0f),
          offset2_x(0.0f), offset2_y(0.0f),
          offset3_x(0.0f), offset3_y(0.0f),
          radius(radius_), rotation(rotation_)
    {}

    inline constexpr float TriangleShape::constexprSin(float) { return 0.0f; }
    inline constexpr float TriangleShape::constexprCos(float) { return 0.0f; }

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

    inline void Triangle::draw(const paxg::Color&) const {
        // Do nothing
    }

} // namespace paxg

#endif // PAX_GRAPHICA_NULL_TRIANGLE_HPP
