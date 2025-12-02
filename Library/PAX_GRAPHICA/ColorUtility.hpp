/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_COLOR_UTILITY_HPP
#define PAX_GRAPHICA_COLOR_UTILITY_HPP

#include <cstdint>
#include <string>
#include <optional>

#include <PAX_GRAPHICA/Color.hpp>

namespace paxg {

    /// @brief カラー変換ユーティリティクラス
    /// @brief Color conversion utility class
    struct ColorUtility {
        /// @brief 16進数カラー文字列をRGB値に変換
        /// @brief Convert hexadecimal color string to RGB values
        /// @param hex_string 16進数カラー文字列（例: "#fe78ba" または "fe78ba"）/ Hex color string (e.g., "#fe78ba" or "fe78ba")
        /// @return RGB値を持つColorオブジェクト。変換失敗時は std::nullopt / Color object with RGB values, or std::nullopt on failure
        static std::optional<paxg::Color> parseHexColor(const std::string& hex_string) {
            if (hex_string.empty()) {
                return std::nullopt;
            }

            std::string hex = hex_string;
            
            // '#'を除去 / Remove '#' if present
            if (hex[0] == '#') {
                hex = hex.substr(1);
            }

            // 16進数文字列の長さチェック（RGB: 6文字、RGBA: 8文字）
            // Check hex string length (RGB: 6 chars, RGBA: 8 chars)
            if (hex.length() != 6 && hex.length() != 8) {
                return std::nullopt;
            }

            // 16進数文字列を整数に変換
            // Convert hex string to integer
            std::uint32_t hex_value = 0;
            try {
                hex_value = static_cast<std::uint32_t>(std::stoul(hex, nullptr, 16));
            }
            catch (...) {
                return std::nullopt;
            }

            // RGBまたはRGBA値を抽出
            // Extract RGB or RGBA values
            std::uint8_t r, g, b, a = 255;

            if (hex.length() == 6) {
                // RGB形式 / RGB format
                r = static_cast<std::uint8_t>((hex_value >> 16) & 0xFF);
                g = static_cast<std::uint8_t>((hex_value >> 8) & 0xFF);
                b = static_cast<std::uint8_t>(hex_value & 0xFF);
            }
            else {
                // RGBA形式 / RGBA format
                r = static_cast<std::uint8_t>((hex_value >> 24) & 0xFF);
                g = static_cast<std::uint8_t>((hex_value >> 16) & 0xFF);
                b = static_cast<std::uint8_t>((hex_value >> 8) & 0xFF);
                a = static_cast<std::uint8_t>(hex_value & 0xFF);
            }

            return paxg::Color(r, g, b, a);
        }

        /// @brief ハッシュ値を16進数文字列に変換（仮の実装）
        /// @brief Convert hash value to hex string (placeholder implementation)
        /// @param hash_value ハッシュ値 / Hash value
        /// @return 16進数カラー文字列 / Hex color string
        /// @note 実際の実装では、ハッシュ値と色文字列のマッピングテーブルが必要
        /// @note In actual implementation, a mapping table between hash values and color strings is needed
        static std::string hashToHexString(std::uint_least32_t hash_value) {
            // 仮実装: ハッシュ値の下位24ビットを色として使用
            // Placeholder: Use lower 24 bits of hash as color
            const std::uint32_t rgb = hash_value & 0xFFFFFF;
            
            char hex_buffer[8];
            std::snprintf(hex_buffer, sizeof(hex_buffer), "#%06x", rgb);
            
            return std::string(hex_buffer);
        }

        /// @brief ハッシュ値からColorオブジェクトを取得
        /// @brief Get Color object from hash value
        /// @param hash_value ハッシュ値 / Hash value
        /// @return Colorオブジェクト / Color object
        static paxg::Color getColorFromHash(std::uint_least32_t hash_value) {
            if (hash_value == 0) {
                // デフォルト色（黒） / Default color (black)
                return paxg::Color(0, 0, 0, 255);
            }

            // ハッシュ値を16進数文字列に変換してパース
            // Convert hash to hex string and parse
            const std::string hex_string = hashToHexString(hash_value);
            auto color_opt = parseHexColor(hex_string);
            
            if (color_opt.has_value()) {
                return *color_opt;
            }

            // 変換失敗時はデフォルト色 / Return default color on failure
            return paxg::Color(0, 0, 0, 255);
        }
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_COLOR_UTILITY_HPP
