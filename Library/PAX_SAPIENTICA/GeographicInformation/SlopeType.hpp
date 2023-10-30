/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_GEOGRAPHIC_INFORMATION_SLOPE_TYPE_HPP
#define PAX_SAPIENTICA_GEOGRAPHIC_INFORMATION_SLOPE_TYPE_HPP

/*##########################################################################################

##########################################################################################*/

#include <cmath>
#include <cstdint>
#include <limits>

namespace paxs {

    // 傾斜（度）を double から uint8 (0-250) へ変換
    unsigned char slopeDegF64ToU0To250(const double float_value_) {
        // NaN は 251
        if (float_value_ == std::numeric_limits<double>::quiet_NaN()) {
            return 251u;
        }
        // 90 度以上は 250
        else if (float_value_ >= 90.0) {
            return 250u;
        }
        // 0 度以下は 0
        else if (float_value_ <= 0.0) {
            return 0u;
        }
        return static_cast<unsigned char>(std::ceil(std::log2(float_value_ + 1.0) * 38.4154715724661/* 250 / log2(91) */));
    }

    // 標高 (m) を double から int16 へ変換
    std::int_least16_t elevationS16(const double float_value_) {
        // NaN は 32761
        if (float_value_ == std::numeric_limits<double>::quiet_NaN()) {
            return 32761;
        }
        // 11,000 m 以上は 32760
        else if (float_value_ >= 11000.0) {
            return 32760;
        }
        // -11,000 m 以下は -32760
        else if (float_value_ <= -11000.0) {
            return -32760;
        }
        return (float_value_ < 0) ?
            static_cast<std::int_least16_t>(-std::ceil(std::log2(-float_value_ + 1.0) * 2440.16038278159/* 32760 / log2(11000 + 1) */)) :
            static_cast<std::int_least16_t>(std::ceil(std::log2(float_value_ + 1.0) * 2440.16038278159/* 32760 / log2(11000 + 1) */));
    }

}

#endif // !PAX_SAPIENTICA_GEOGRAPHIC_INFORMATION_SLOPE_TYPE_HPP
