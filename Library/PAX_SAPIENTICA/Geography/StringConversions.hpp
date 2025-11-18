/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_GEOGRAPHY_STRING_CONVERSIONS_HPP
#define PAX_SAPIENTICA_GEOGRAPHY_STRING_CONVERSIONS_HPP

#include <cstddef>
#include <cstdint>
#include <string>

#include <PAX_SAPIENTICA/Core/Utility/StringUtils.hpp>
#include <PAX_SAPIENTICA/Geography/Encoding/ElevationEncoding.hpp>
#include <PAX_SAPIENTICA/Geography/Encoding/SlopeEncoding.hpp>

namespace paxs {

/// @brief 地理情報に関する文字列変換ユーティリティ
/// @brief Geographic information string conversion utilities
struct GeoStringConversions {

    /// @brief 傾斜度の文字列を分割してUInt8配列に変換
    /// @brief Split slope degree string and convert to UInt8 array
    /// @param input 入力文字列 / Input string
    /// @param delimiter 区切り文字 / Delimiter character
    /// @param result 結果を格納する配列 / Array to store results
    /// @param size 配列のサイズ / Array size
    static void splitSlopeDegU8(const std::string& input, const char delimiter, unsigned char* const result, const std::size_t size) noexcept {
        std::size_t start_pos = 0;
        std::size_t end_pos = 0;
        std::size_t field_index = 0;

        while ((end_pos = input.find(delimiter, start_pos)) != std::string::npos) {
            if (field_index >= size) break;
            std::string field = input.substr(start_pos, end_pos - start_pos);
            result[field_index++] = (field.empty() || field == "nan") ? 251 : paxs::SlopeEncoding::slopeDegF64ToLog2U8(StringUtils::safeStod(field, 0.0, false));
            start_pos = end_pos + 1;
        }
        // 最後のフィールド
        if (field_index < size && start_pos < input.size()) {
            std::string field = input.substr(start_pos);
            result[field_index++] = (field.empty() || field == "nan") ? 251 : paxs::SlopeEncoding::slopeDegF64ToLog2U8(StringUtils::safeStod(field, 0.0, false));
        }
    }

    /// @brief 標高の文字列を分割してInt16配列に変換
    /// @brief Split elevation string and convert to Int16 array
    /// @param input 入力文字列 / Input string
    /// @param delimiter 区切り文字 / Delimiter character
    /// @param result 結果を格納する配列 / Array to store results
    /// @param size 配列のサイズ / Array size
    static void splitElevationS16(const std::string& input, const char delimiter, std::int_least16_t* const result, const std::size_t size) noexcept {
        std::size_t start_pos = 0;
        std::size_t end_pos = 0;
        std::size_t field_index = 0;

        while ((end_pos = input.find(delimiter, start_pos)) != std::string::npos) {
            if (field_index >= size) break;
            std::string field = input.substr(start_pos, end_pos - start_pos);
            result[field_index++] = (field.empty() || field == "nan") ? 32761 : paxs::ElevationEncoding::elevationF64ToLog2S16(StringUtils::safeStod(field, 0.0, false));
            start_pos = end_pos + 1;
        }
        // 最後のフィールド
        if (field_index < size && start_pos < input.size()) {
            std::string field = input.substr(start_pos);
            result[field_index++] = (field.empty() || field == "nan") ? 32761 : paxs::ElevationEncoding::elevationF64ToLog2S16(StringUtils::safeStod(field, 0.0, false));
        }
    }
};

} // namespace paxs

#endif // !PAX_SAPIENTICA_GEOGRAPHY_STRING_CONVERSIONS_HPP
