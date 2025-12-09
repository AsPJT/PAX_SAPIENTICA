/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_INTERFACE_TRIANGLE_HPP
#define PAX_GRAPHICA_INTERFACE_TRIANGLE_HPP

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_SAPIENTICA/Core/Math/Math.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxg {

    struct TriangleShape;

    struct Triangle {
    private:
        float center_x_{};
        float center_y_{};
        float radius_{};
        float rotation_{};
        const TriangleShape* shape_{ nullptr };

    public:
        constexpr Triangle();
        constexpr Triangle(const float center_x, const float center_y, const float radius, const float rotation = 0.0f);
        constexpr Triangle(const paxs::Vector2<float>& center, const float radius, const float rotation = 0.0f);
        constexpr Triangle(const float center_x, const float center_y, const TriangleShape& shape);
        constexpr Triangle(const paxs::Vector2<float>& center, const TriangleShape& shape);

        constexpr float centerX() const;
        constexpr float centerY() const;
        constexpr float radius() const;
        constexpr float rotation() const;

        void setCenterX(const float x);
        void setCenterY(const float y);
        void setCenter(const float x, const float y);
        void setCenter(const paxs::Vector2<float>& center);
        void setRadius(const float r);
        void setRotation(const float rot);

        void draw(const paxg::Color& color = paxg::Color(0, 0, 0)) const;
    };

    struct TriangleShape {
        float offset1_x, offset1_y;
        float offset2_x, offset2_y;
        float offset3_x, offset3_y;
        float radius, rotation;

        constexpr TriangleShape(const float radius_, const float rotation_);

    private:
        static constexpr float constexprSin(float x);
        static constexpr float constexprCos(float x);
    };

} // namespace paxg

#endif // PAX_GRAPHICA_INTERFACE_TRIANGLE_HPP
