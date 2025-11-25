/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CORE_MATH_CONVERSIONS_HPP
#define PAX_SAPIENTICA_CORE_MATH_CONVERSIONS_HPP

#include <PAX_SAPIENTICA/Core/Math/Constants.hpp>

namespace paxs {

    template<typename T>
    struct MathConversions {
        // ラジアンから度への変換係数
        constexpr static T radToDeg() noexcept { return static_cast<T>(180.0 / PAX_SAPIENTICA_PI); }

        // 度からラジアンへの変換係数
        constexpr static T degToRad() noexcept { return static_cast<T>(PAX_SAPIENTICA_PI / 180.0); }

        // ラジアンから度へ変換
        constexpr static T radToDeg(const T value) noexcept { return static_cast<T>(value * radToDeg()); }

        // 度からラジアンへ変換
        constexpr static T degToRad(const T value) noexcept { return static_cast<T>(value * degToRad()); }
    };
}

#endif // !PAX_SAPIENTICA_CORE_MATH_CONVERSIONS_HPP
