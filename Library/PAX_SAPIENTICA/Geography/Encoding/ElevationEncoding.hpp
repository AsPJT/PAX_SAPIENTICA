/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_GEOGRAPHY_ENCODING_ELEVATION_ENCODING_HPP
#define PAX_SAPIENTICA_GEOGRAPHY_ENCODING_ELEVATION_ENCODING_HPP

#include <cmath>
#include <cstdint>
#include <limits>

namespace paxs {

    /// @brief Elevation encoding utilities
    /// @brief 標高エンコーディングユーティリティ
    struct ElevationEncoding {
        static constexpr double s16_max_p_log2_elevation_max = 2440.16038278159; // 32760 / log2(11000 + 1)

        // 標高を int16 から double (m) へ変換
        static constexpr double elevationLog2S16ToF64(const std::int_least16_t char_value_) {
            return (char_value_ >= 32761) ? std::numeric_limits<double>::quiet_NaN() : // 32761 以上は NaN
                ((char_value_ < 0) ? -std::pow(2, -char_value_ / s16_max_p_log2_elevation_max) + 1.0 : // 32760 以下は値を変換
                    std::pow(2, char_value_ / s16_max_p_log2_elevation_max) - 1.0); // 32760 以下は値を変換
        }

        // 標高 (m) を double から int16 へ変換
        static constexpr std::int_least16_t elevationF64ToLog2S16(const double float_value_) {
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
                static_cast<std::int_least16_t>(-std::ceil(std::log2(-float_value_ + 1.0) * s16_max_p_log2_elevation_max)) :
                static_cast<std::int_least16_t>(std::ceil(std::log2(float_value_ + 1.0) * s16_max_p_log2_elevation_max));
        }
    };
}

#endif // !PAX_SAPIENTICA_GEOGRAPHY_ENCODING_ELEVATION_ENCODING_HPP
