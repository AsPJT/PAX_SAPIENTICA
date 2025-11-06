/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_COLOR_RGBA_HPP
#define PAX_SAPIENTICA_COLOR_RGBA_HPP

#include <cstring>
#include <stdexcept>

namespace paxs {

    struct RGBA {
    public:
        unsigned char r = 0, g = 0, b = 0, a = 255; //赤, 緑, 青, 不透過

        constexpr RGBA() = default;
        constexpr RGBA(const unsigned char r_, const unsigned char g_, const unsigned char b_, const unsigned char a_ = 255) :r(r_), g(g_), b(b_), a(a_) {}
        template <size_t N>
        constexpr RGBA(const char (&str_)[N]) :
            // コンパイル時にサイズをチェック
            r((N != 8 || str_[0] != '#') ? throw std::logic_error("Invalid hex format") : convertHexToRGB(str_[1], str_[2])),
            g((N != 8 || str_[0] != '#') ? throw std::logic_error("Invalid hex format") : convertHexToRGB(str_[3], str_[4])),
            b((N != 8 || str_[0] != '#') ? throw std::logic_error("Invalid hex format") : convertHexToRGB(str_[5], str_[6]))
        {
            if (N != 8) {
                throw std::logic_error("Hex color string must be 8 chars long (e.g. \"#RRGGBB\")");
             }
            if (str_[0] != '#') {
                throw std::logic_error("Hex color string must start with '#'");
            }
        }

    private:
        /// @brief 16進数文字1つを数値(0-15)に変換する
        // @param hex 16進数文字
        // @return 変換後の数値(0-15)
        // @note 不正な文字の場合は255を返す
        static constexpr unsigned char hexToValue(const char hex) {
            if (hex >= '0' && hex <= '9') return hex - '0';
            if (hex >= 'a' && hex <= 'f') return hex - 'a' + 10;
            if (hex >= 'A' && hex <= 'F') return hex - 'A' + 10;
            return 255;
        }

        /// @brief 16進数文字2つを組み合わせてRGB値(0-255)に変換する
        static constexpr unsigned char convertHexToRGB(const char hex1, const char hex2) {
            const unsigned char high = hexToValue(hex1);
            const unsigned char low = hexToValue(hex2);
            if (high == 255 || low == 255) {
                throw std::invalid_argument("Invalid hex character");
            }
            return high * 16 + low;
        }
    };

}

#endif // !PAX_SAPIENTICA_COLOR_RGBA_HPP
