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

}

#endif // !PAX_SAPIENTICA_GEOGRAPHIC_INFORMATION_SLOPE_TYPE_HPP
