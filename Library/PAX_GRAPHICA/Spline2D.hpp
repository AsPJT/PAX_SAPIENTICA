/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SPLINE_2D_HPP
#define PAX_GRAPHICA_SPLINE_2D_HPP

#include <vector>
#include <cmath>

#if defined(PAXS_USING_SIV3D)
#include <Siv3D/Color.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Line.hpp>

namespace paxg {

    /// @brief 2Dスプライン曲線クラス
    class Spline2D {
    private:
#ifdef PAXS_USING_SIV3D
        s3d::Spline2D spline;
#else
        std::vector<Vec2f> points_;
#endif

        static Vec2f calculateCatmullRom(const Vec2f& p0, const Vec2f& p1, const Vec2f& p2, const Vec2f& p3, float t) {
            float t2 = t * t;
            float t3 = t2 * t;

            float v0 = (p2.x() - p0.x()) * 0.5f;
            float v1 = (p3.x() - p1.x()) * 0.5f;
            float x = (2 * p1.x() - 2 * p2.x() + v0 + v1) * t3 + (-3 * p1.x() + 3 * p2.x() - 2 * v0 - v1) * t2 + v0 * t + p1.x();

            v0 = (p2.y() - p0.y()) * 0.5f;
            v1 = (p3.y() - p1.y()) * 0.5f;
            float y = (2 * p1.y() - 2 * p2.y() + v0 + v1) * t3 + (-3 * p1.y() + 3 * p2.y() - 2 * v0 - v1) * t2 + v0 * t + p1.y();

            return Vec2f{ x, y };
        }

    public:
        Spline2D() = default;

        explicit Spline2D(const std::vector<Vec2f>& points) {
#ifdef PAXS_USING_SIV3D
            s3d::Array<s3d::Vec2> siv3d_points;
            for (const auto& p : points) {
                siv3d_points << s3d::Vec2{ p.x(), p.y() };
            }
            spline = s3d::Spline2D(siv3d_points);
#else
            points_ = points;
#endif
        }

        explicit Spline2D(const std::vector<Vec2i>& points) {
#ifdef PAXS_USING_SIV3D
            s3d::Array<s3d::Vec2> siv3d_points;
            for (const auto& p : points) {
                siv3d_points << s3d::Vec2{ static_cast<double>(p.x()), static_cast<double>(p.y()) };
            }
            spline = s3d::Spline2D(siv3d_points);
#else
            for (const auto& p : points) {
                points_.emplace_back(static_cast<float>(p.x()), static_cast<float>(p.y()));
            }
#endif
        }

        void draw(double thickness, const paxg::Color& color) const {
#ifdef PAXS_USING_SIV3D
            spline.draw(thickness, s3d::ColorF(color.color));
#else
            if (points_.size() < 2) return;

            const int divisions = 20;

            for (std::size_t i = 0; i < points_.size() - 1; ++i) {
                Vec2f p0 = (i == 0) ? points_[0] : points_[i - 1];
                Vec2f p1 = points_[i];
                Vec2f p2 = points_[i + 1];
                Vec2f p3 = (i + 2 >= points_.size()) ? points_[i + 1] : points_[i + 2];

                Vec2f current_pos = p1;

                for (int j = 1; j <= divisions; ++j) {
                    float t = static_cast<float>(j) / divisions;
                    Vec2f next_pos = calculateCatmullRom(p0, p1, p2, p3, t);

                    // 修正: Lineのコンストラクタに float x, y を明示的に渡すことで曖昧さを回避
                    paxg::Line(current_pos.x(), current_pos.y(), next_pos.x(), next_pos.y()).draw(thickness, color);

                    current_pos = next_pos;
                }
            }
#endif
        }

        void draw(const Color& color) const {
            draw(1.0, color);
        }
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_SPLINE_2D_HPP
