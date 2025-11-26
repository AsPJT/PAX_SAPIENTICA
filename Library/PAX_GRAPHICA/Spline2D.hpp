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
        bool is_closed_ = false;  // 閉じたループかどうか / Whether it's a closed loop
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

        /// @brief 閉じたループかどうかを自動判定し、重複する最後の点を除去
        /// @brief Automatically detect closed loop and remove duplicate last point
        /// @return true if closed loop detected (and duplicate removed)
        bool detectAndFixClosedLoop(std::vector<Vec2f>& points) const {
            if (points.size() < 3) return false;
            
            const Vec2f& first = points.front();
            const Vec2f& last = points.back();
            
            // 最初と最後の点が十分近ければ閉じたループとみなす
            // Consider it a closed loop if first and last points are close enough
            constexpr float threshold = 0.1f;
            float dx = first.x() - last.x();
            float dy = first.y() - last.y();
            float dist_sq = dx * dx + dy * dy;
            
            if (dist_sq < threshold * threshold) {
                // 閉じたループの場合、最後の重複点を除去
                // Remove duplicate last point for closed loops
                points.pop_back();
                return true;
            }
            
            return false;
        }

    public:
        Spline2D() = default;

        explicit Spline2D(const std::vector<Vec2f>& points, bool is_closed = false) {
#ifdef PAXS_USING_SIV3D
            s3d::Array<s3d::Vec2> siv3d_points;
            for (const auto& p : points) {
                siv3d_points << s3d::Vec2{ p.x(), p.y() };
            }
            spline = s3d::Spline2D(siv3d_points);
#else
            points_ = points;
            // 閉じたループを自動判定し、重複する最後の点を除去
            // Auto-detect closed loop and remove duplicate last point
            if (!is_closed) {
                is_closed_ = detectAndFixClosedLoop(points_);
            } else {
                is_closed_ = true;
                // 明示的に閉じたループが指定された場合も重複チェック
                // Also check for duplicates when explicitly specified as closed
                if (points_.size() >= 2) {
                    const Vec2f& first = points_.front();
                    const Vec2f& last = points_.back();
                    constexpr float threshold = 0.1f;
                    float dx = first.x() - last.x();
                    float dy = first.y() - last.y();
                    float dist_sq = dx * dx + dy * dy;
                    
                    if (dist_sq < threshold * threshold) {
                        points_.pop_back();
                    }
                }
            }
#endif
        }

        explicit Spline2D(const std::vector<Vec2i>& points, bool is_closed = false) {
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
            
            // 閉じたループを自動判定し、重複する最後の点を除去
            // Auto-detect closed loop and remove duplicate last point
            if (!is_closed) {
                is_closed_ = detectAndFixClosedLoop(points_);
            } else {
                is_closed_ = true;
                // 明示的に閉じたループが指定された場合も重複チェック
                // Also check for duplicates when explicitly specified as closed
                if (points_.size() >= 2) {
                    const Vec2f& first = points_.front();
                    const Vec2f& last = points_.back();
                    constexpr float threshold = 0.1f;
                    float dx = first.x() - last.x();
                    float dy = first.y() - last.y();
                    float dist_sq = dx * dx + dy * dy;
                    
                    if (dist_sq < threshold * threshold) {
                        points_.pop_back();
                    }
                }
            }
#endif
        }

        void draw(double thickness, const paxg::Color& color) const {
#ifdef PAXS_USING_SIV3D
            spline.draw(thickness, s3d::ColorF(color.color));
#else
            if (points_.size() < 2) return;

            const int divisions = 20;
            const std::size_t point_count = points_.size();

            // 閉じたループの場合、最後の点から最初の点への繋ぎも描画
            // For closed loops, also draw the connection from the last point to the first
            const std::size_t loop_count = is_closed_ ? point_count : (point_count - 1);

            for (std::size_t i = 0; i < loop_count; ++i) {
                // 閉じたループの場合、インデックスを循環させる
                // For closed loops, wrap indices around
                Vec2f p0, p1, p2, p3;
                
                if (is_closed_) {
                    // 閉じたループ：全ての点でモジュロ演算を使用
                    // Closed loop: use modulo for all point indices
                    p0 = points_[(i + point_count - 1) % point_count];
                    p1 = points_[i];
                    p2 = points_[(i + 1) % point_count];
                    p3 = points_[(i + 2) % point_count];
                } else {
                    // 開いた曲線：既存のロジック
                    // Open curve: existing logic
                    p0 = (i == 0) ? points_[0] : points_[i - 1];
                    p1 = points_[i];
                    p2 = points_[i + 1];
                    p3 = (i + 2 >= point_count) ? points_[i + 1] : points_[i + 2];
                }

                Vec2f current_pos = p1;

                for (int j = 1; j <= divisions; ++j) {
                    float t = static_cast<float>(j) / divisions;
                    Vec2f next_pos = calculateCatmullRom(p0, p1, p2, p3, t);

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
