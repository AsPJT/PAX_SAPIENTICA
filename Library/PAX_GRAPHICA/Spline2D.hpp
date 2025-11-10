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

#if defined(PAXS_USING_SIV3D)
#include <Siv3D/Color.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

namespace paxg {

    /// @brief 2Dスプライン曲線クラス
    /// @brief 複数の点を通るなめらかな曲線を描画する
    class Spline2D {
    private:
#ifdef PAXS_USING_SIV3D
        s3d::Spline2D spline;
#endif

    public:
        /// @brief デフォルトコンストラクタ
        Spline2D() = default;

        /// @brief Vec2fの配列から構築
        /// @param points 制御点の配列
        explicit Spline2D(const std::vector<Vec2f>& points) {
#ifdef PAXS_USING_SIV3D
            s3d::Array<s3d::Vec2> siv3d_points;
            for (const auto& p : points) {
                siv3d_points << s3d::Vec2{ p.x(), p.y() };
            }
            spline = s3d::Spline2D(siv3d_points);
#else
            (void)points;
#endif
        }

        /// @brief Vec2iの配列から構築
        /// @param points 制御点の配列
        explicit Spline2D(const std::vector<Vec2i>& points) {
#ifdef PAXS_USING_SIV3D
            s3d::Array<s3d::Vec2> siv3d_points;
            for (const auto& p : points) {
                siv3d_points << s3d::Vec2{ static_cast<double>(p.x()), static_cast<double>(p.y()) };
            }
            spline = s3d::Spline2D(siv3d_points);
#else
            (void)points;
#endif
        }

        /// @brief スプライン曲線を描画
        /// @param thickness 線の太さ
        /// @param color 線の色
        void draw(double thickness, const paxg::Color& color) const {
#ifdef PAXS_USING_SIV3D
            spline.draw(thickness, s3d::ColorF(color.color));
#elif defined(PAXS_USING_DXLIB)
            (void)thickness;
            (void)color;
#elif defined(PAXS_USING_SFML)
            (void)thickness;
            (void)color;
#else
            (void)thickness;
            (void)color;
#endif
        }

        /// @brief スプライン曲線を描画（デフォルトの太さ）
        /// @param color 線の色
        void draw(const Color& color) const {
            draw(1.0, color);
        }
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_SPLINE_2D_HPP
