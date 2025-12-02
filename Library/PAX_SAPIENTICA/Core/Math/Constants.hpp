/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CORE_MATH_CONSTANTS_HPP
#define PAX_SAPIENTICA_CORE_MATH_CONSTANTS_HPP

#include <numbers>

namespace paxs {

    template<typename T>
    struct MathConstants {
        constexpr static T pi() noexcept { return std::numbers::pi_v<T>; } // π
        constexpr static T pi2() noexcept { return std::numbers::pi_v<T> * 2.0; } // 2π
        constexpr static T piHalf() noexcept { return std::numbers::pi_v<T> * 0.5; } // π/2
        constexpr static T invPi() noexcept { return std::numbers::inv_pi_v<T>; } // 1/π

        constexpr static T deg0() noexcept { return static_cast<T>(0); }   // 0度
        constexpr static T deg90() noexcept { return static_cast<T>(90); } // 90度（π/2 ラジアン相当）
        constexpr static T deg180() noexcept { return static_cast<T>(180); } // 180度（π ラジアン相当）
        constexpr static T deg270() noexcept { return static_cast<T>(270); } // 270度（3π/2 ラジアン相当）
        constexpr static T deg360() noexcept { return static_cast<T>(360); } // 360度（2π ラジアン相当）
        constexpr static T invDeg180() noexcept { return static_cast<T>(1.0 / 180.0); } // 1/180度
        constexpr static T invDeg360() noexcept { return static_cast<T>(1.0 / 360.0); } // 1/360度

        constexpr static T e() noexcept { return std::numbers::e_v<T>; } // e
        constexpr static T log2e() noexcept { return std::numbers::log2e_v<T>; } // log2(e)
        constexpr static T log10e() noexcept { return std::numbers::log10e_v<T>; } // log10(e)
        constexpr static T ln2() noexcept { return std::numbers::ln2_v<T>; } // ln(2)
        constexpr static T ln10() noexcept { return std::numbers::ln10_v<T>; } // ln(10)

        constexpr static T sqrt2() noexcept { return std::numbers::sqrt2_v<T>; } // √2
        constexpr static T sqrt3() noexcept { return std::numbers::sqrt3_v<T>; } // √3
        constexpr static T invSqrt3() noexcept { return std::numbers::inv_sqrt3_v<T>; } // 1/
    };
}

#endif // !PAX_SAPIENTICA_CORE_MATH_CONSTANTS_HPP
