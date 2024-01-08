/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_GEOGRAPHIC_INFORMATION_ELEVATION_HPP
#define PAX_SAPIENTICA_GEOGRAPHIC_INFORMATION_ELEVATION_HPP

/*##########################################################################################

##########################################################################################*/

#include <array>
#include <cmath>
#include <stdexcept>
#include <string>

namespace paxs {

    /// @brief A class that handles elevation.
    struct Grid3x3 {
        Grid3x3(const std::array<double, 9>& grid) : grid(grid) {}
        std::array<double, 9> grid;
        constexpr const double* operator[](const int i) const {
            if (i < 0 || i > 8) {
                throw std::out_of_range("Grid3x3::operator[] out of range.\n0 <= i <= 8 is required.\ni = " + std::to_string(i));
            }
            return &grid[std::size_t(i)];
        }
    };

    /// @brief A class that handles elevation.
    class Elevation {
    public:
        /// @brief Get the slope.
        /// @param grid List of 9 grids.
        static double getSlopeRad(const Grid3x3& grid,
            const double x_cell_size,
            const double y_cell_size
            //const int z
        ) {
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

            //const double z_num = 156543.03390624999884 / std::pow(2, z);
            //const double x_cell_size = z_num;
            //const double y_cell_size = z_num;

            double wght1 = cw + 2 * fw + iw;
            double wght2 = aw + 2 * dw + gw;
            double wght3 = gw + 2 * hw + iw;
            double wght4 = aw + 2 * bw + cw;

            double wght5 = bw + 2 * ew + hw;
            double wght6 = dw + 2 * ew + fw;

            // 中央セル e の X 方向の変化率
            double dz_p_dx = 0.0;
            if (wght1 == 0.0) {
                if (wght2 == 0.0) {}
                else dz_p_dx = (((b + 2 * e + h) / wght5) - ((a + 2 * d + g) / wght2)) / (x_cell_size);
            }
            else {
                if (wght2 == 0.0) dz_p_dx = (((c + 2 * f + i) / wght1) - ((b + 2 * e + h) / wght5)) / (x_cell_size);
                else dz_p_dx = (((c + 2 * f + i) / wght1) - ((a + 2 * d + g) / wght2)) / (2 * x_cell_size);
            }
            // 中央セル e の Y 方向の変化率
            double dz_p_dy = 0.0;
            if (wght3 == 0.0) {
                if (wght4 == 0.0) {}
                else dz_p_dy = (((d + 2 * e + f) / wght6) - ((a + 2 * b + c) / wght4)) / (y_cell_size);
            }
            else {
                if (wght4 == 0.0) dz_p_dy = (((g + 2 * h + i) / wght3) - ((d + 2 * e + f) / wght6)) / (y_cell_size);
                else dz_p_dy = (((g + 2 * h + i) / wght3) - ((a + 2 * b + c) / wght4)) / (2 * y_cell_size);
            }
            // 中央セル e の傾斜角
            double rise_run = std::sqrt(dz_p_dx * dz_p_dx + dz_p_dy * dz_p_dy);
            double slope_rad = std::atan(rise_run);
            //double slope_degrees = std::atan(rise_run) * 57.29578;

            return slope_rad;
        }
    private:
        /// @brief A enum class that represents the grid type of the slope.
        enum class SlopeGridType {
            a, b, c,
            d, e, f,
            g, h, i
        };
    };
}

#endif // !PAX_SAPIENTICA_GEOGRAPHIC_INFORMATION_ELEVATION_HPP
