/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SIV3D_SPLINE_2D_IMPL_HPP
#define PAX_GRAPHICA_SIV3D_SPLINE_2D_IMPL_HPP

#ifdef PAXS_USING_SIV3D
#include <vector>

#include <Siv3D.hpp>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Interface/Spline2DImpl.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxg {

    /// @brief Siv3D-specific implementation of 2D spline using s3d::Spline2D
    class Siv3DSpline2DImpl : public Spline2DImpl {
    private:
        s3d::Spline2D spline;

    public:
        /// @brief Constructor with Vector2<double> points
        /// @param points Control points for the spline
        /// @param is_closed Whether the spline is a closed loop (unused for Siv3D)
        Siv3DSpline2DImpl(const std::vector<paxs::Vector2<double>>& points, bool /*is_closed*/ = false) {
            s3d::Array<s3d::Vec2> siv3d_points;
            for (const auto& p : points) {
                siv3d_points << s3d::Vec2{ p.x, p.y };
            }
            spline = s3d::Spline2D(siv3d_points);
        }

        /// @brief Constructor with Vector2<float> points
        /// @param points Control points for the spline
        /// @param is_closed Whether the spline is a closed loop (unused for Siv3D)
        Siv3DSpline2DImpl(const std::vector<paxs::Vector2<float>>& points, bool /*is_closed*/ = false) {
            s3d::Array<s3d::Vec2> siv3d_points;
            for (const auto& p : points) {
                siv3d_points << s3d::Vec2{ p.x, p.y };
            }
            spline = s3d::Spline2D(siv3d_points);
        }

        /// @brief Constructor with Vector2<int> points
        /// @param points Control points for the spline
        /// @param is_closed Whether the spline is a closed loop (unused for Siv3D)
        Siv3DSpline2DImpl(const std::vector<paxs::Vector2<int>>& points, bool /*is_closed*/ = false) {
            s3d::Array<s3d::Vec2> siv3d_points;
            for (const auto& p : points) {
                siv3d_points << s3d::Vec2{ static_cast<double>(p.x), static_cast<double>(p.y) };
            }
            spline = s3d::Spline2D(siv3d_points);
        }

        /// @brief Draw the spline with specified thickness and color
        /// @param thickness Line thickness
        /// @param color Line color
        void draw(double thickness, const Color& color) const override {
            spline.draw(thickness, s3d::ColorF(color));
        }

        /// @brief Get the native Siv3D spline object
        /// @return Reference to the s3d::Spline2D object
        const s3d::Spline2D& getNativeSpline() const {
            return spline;
        }
    };

} // namespace paxg

#endif // PAXS_USING_SIV3D

#endif // !PAX_GRAPHICA_SIV3D_SPLINE_2D_IMPL_HPP
