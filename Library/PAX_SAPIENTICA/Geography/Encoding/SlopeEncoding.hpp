/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_GEOGRAPHY_ENCODING_SLOPE_ENCODING_HPP
#define PAX_SAPIENTICA_GEOGRAPHY_ENCODING_SLOPE_ENCODING_HPP

#include <cmath>
#include <limits>

namespace paxs {

    /// @brief Slope encoding utilities
    /// @brief 傾斜エンコーディングユーティリティ
    struct SlopeEncoding {
        static constexpr double u8_max_p_log2_slope_max = 38.4154715724661; // 250 / log2(91)

        // 傾斜を uint8 (0-250) から double （度）へ変換
        static constexpr double slopeDegLog2U8ToF64(const unsigned char char_value_) {
            return (char_value_ >= 251u) ? std::numeric_limits<double>::quiet_NaN() : // 251 以上は NaN
                std::pow(2, char_value_ / u8_max_p_log2_slope_max) - 1.0; // 250 以下は値を変換
        }

        // 傾斜（度）を double から uint8 (0-250) へ変換
        static constexpr unsigned char slopeDegF64ToLog2U8(const double float_value_) {
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
            return static_cast<unsigned char>(std::ceil(std::log2(float_value_ + 1.0) * u8_max_p_log2_slope_max));
        }
    };
}

#endif // !PAX_SAPIENTICA_GEOGRAPHY_ENCODING_SLOPE_ENCODING_HPP
