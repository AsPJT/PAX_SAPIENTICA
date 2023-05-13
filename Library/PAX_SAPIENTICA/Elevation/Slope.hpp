/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_ELEVATION_SLOPE_HPP
#define PAX_SAPIENTICA_ELEVATION_SLOPE_HPP

/*##########################################################################################

##########################################################################################*/

#include <cmath>

namespace paxs {
	enum class SlopeGridType {
		a, b, c, d, e, f, g, h, i
	};

	double getSlopeRad(const double* const grid[]) {
		const double* const ap = grid[std::size_t(SlopeGridType::a)];
		const double* const bp = grid[std::size_t(SlopeGridType::b)];
		const double* const cp = grid[std::size_t(SlopeGridType::c)];
		const double* const dp = grid[std::size_t(SlopeGridType::d)];
		const double* const ep = grid[std::size_t(SlopeGridType::e)];
		const double* const fp = grid[std::size_t(SlopeGridType::f)];
		const double* const gp = grid[std::size_t(SlopeGridType::g)];
		const double* const hp = grid[std::size_t(SlopeGridType::h)];
		const double* const ip = grid[std::size_t(SlopeGridType::i)];

		const double a = ((ap == nullptr) ? 0 : *ap);
		const double b = ((bp == nullptr) ? 0 : *bp);
		const double c = ((cp == nullptr) ? 0 : *cp);
		const double d = ((dp == nullptr) ? 0 : *dp);
		const double e = ((ep == nullptr) ? 0 : *ep);
		const double f = ((fp == nullptr) ? 0 : *fp);
		const double g = ((gp == nullptr) ? 0 : *gp);
		const double h = ((hp == nullptr) ? 0 : *hp);
		const double i = ((ip == nullptr) ? 0 : *ip);

		const double aw = ((ap == nullptr) ? 0 : 1);
		const double bw = ((bp == nullptr) ? 0 : 1);
		const double cw = ((cp == nullptr) ? 0 : 1);
		const double dw = ((dp == nullptr) ? 0 : 1);
		const double ew = ((ep == nullptr) ? 0 : 1);
		const double fw = ((fp == nullptr) ? 0 : 1);
		const double gw = ((gp == nullptr) ? 0 : 1);
		const double hw = ((hp == nullptr) ? 0 : 1);
		const double iw = ((ip == nullptr) ? 0 : 1);

		const double x_cellsize = 5;
		const double y_cellsize = 5;

		double wght1 = cw + 2 * fw + iw;
		double wght2 = aw + 2 * dw + gw;
		double wght3 = gw + 2 * hw + iw;
		double wght4 = aw + 2 * bw + cw;

	   // 中央セル e の X 方向の変化率
		double dz_p_dx =
			(((wght1 == 0.0) ? 0 : (c + 2 * f + i) / wght1) -
				((wght2 == 0.0) ? 0 : (a + 2 * d + g) / wght2)) / (2 * x_cellsize);
		// 中央セル e の Y 方向の変化率
		double dz_p_dy =
			(((wght3 == 0.0) ? 0 : (g + 2 * h + i) / wght3) -
				((wght4 == 0.0) ? 0 : (a + 2 * b + c) / wght4)) / (2 * y_cellsize);
		// 中央セル e の傾斜角
		double rise_run = std::sqrt(dz_p_dx * dz_p_dx + dz_p_dy * dz_p_dy);
		double slope_rad = std::atan(rise_run);
		//double slope_degrees = std::atan(rise_run) * 57.29578;

		return slope_rad;
	}
}

#endif // !PAX_SAPIENTICA_ELEVATION_SLOPE_HPP