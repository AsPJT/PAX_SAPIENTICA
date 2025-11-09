/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_COLOR_F_HPP
#define PAX_GRAPHICA_COLOR_F_HPP

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#endif

namespace paxg {

    /// @brief Floating-point color class (0.0-1.0 range)
    /// @brief 浮動小数点カラークラス（0.0-1.0の範囲）
    struct ColorF {
        double r, g, b, a;

        /// @brief Constructor with RGB and alpha
        /// @param r_ Red component (0.0-1.0)
        /// @param g_ Green component (0.0-1.0)
        /// @param b_ Blue component (0.0-1.0)
        /// @param a_ Alpha component (0.0-1.0)
        constexpr ColorF(double r_, double g_, double b_, double a_ = 1.0)
            : r(r_), g(g_), b(b_), a(a_) {}

        /// @brief Constructor with grayscale and alpha
        /// @param rgb Grayscale value (0.0-1.0)
        /// @param a_ Alpha component (0.0-1.0)
        constexpr ColorF(double rgb, double a_)
            : r(rgb), g(rgb), b(rgb), a(a_) {}

#if defined(PAXS_USING_SIV3D)
        /// @brief Convert to Siv3D ColorF
        operator s3d::ColorF() const {
            return s3d::ColorF(r, g, b, a);
        }
#endif
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_COLOR_F_HPP
