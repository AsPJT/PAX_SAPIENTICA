/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CORE_MATH_MATH_HPP
#define PAX_SAPIENTICA_CORE_MATH_MATH_HPP

#include <PAX_SAPIENTICA/Core/Math/Constants.hpp>
#include <PAX_SAPIENTICA/Core/Math/Conversions.hpp>

namespace paxs {

    // 互換性のための統合型
    template<typename T>
    struct Math : public MathConstants<T>, public MathConversions<T> {
    };

    using MathF32 = Math<float>;
    using MathF64 = Math<double>;

}

#endif // !PAX_SAPIENTICA_CORE_MATH_MATH_HPP
