/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_COLOR_HPP
#define PAX_GRAPHICA_COLOR_HPP

#include <cstdint>
#include <type_traits>

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

namespace paxg {

    /// @brief Generic color template class supporting both integer (0-255) and floating-point (0.0-1.0) colors
    /// @tparam T Component type (int or double)
    /// @brief RGBAカラーを扱う汎用テンプレートクラス（整数版と浮動小数点版の両方に対応）
    template <typename T>
    struct ColorT {
        static_assert(std::is_same_v<T, int> || std::is_same_v<T, double>,
            "Color component type must be int or double");

        // Component values (RGBA)
        T r, g, b, a;

        /// @brief Constructor with RGBA components
        /// @param r_ Red component
        /// @param g_ Green component
        /// @param b_ Blue component
        /// @param a_ Alpha component (default: 255 for int, 1.0 for double)
        constexpr ColorT(T r_, T g_, T b_, T a_ = std::is_same_v<T, int> ? T(255) : T(1.0))
            : r(r_), g(g_), b(b_), a(a_) {}

        /// @brief Constructor with grayscale and alpha
        /// @param rgb Grayscale value
        /// @param a_ Alpha component (default: 255 for int, 1.0 for double)
        constexpr ColorT(T rgb, T a_ = std::is_same_v<T, int> ? T(255) : T(1.0))
            : r(rgb), g(rgb), b(rgb), a(a_) {}

#if defined(PAXS_USING_SIV3D)
        // Siv3D conversion operators
        // Use std::conditional to select the correct Siv3D type based on T
        using Siv3DColorType = typename std::conditional_t<
            std::is_same_v<T, double>,
            s3d::ColorF,
            s3d::Color
        >;

        /// @brief Convert to Siv3D color type (s3d::Color or s3d::ColorF)
        operator Siv3DColorType() const {
            if constexpr (std::is_same_v<T, double>) {
                return s3d::ColorF(r, g, b, a);
            } else {
                return s3d::Color(
                    static_cast<s3d::Color::value_type>(r),
                    static_cast<s3d::Color::value_type>(g),
                    static_cast<s3d::Color::value_type>(b),
                    static_cast<s3d::Color::value_type>(a)
                );
            }
        }

#elif defined(PAXS_USING_SFML)
        // SFML only supports integer colors (sf::Color)
        template <typename U = T>
        operator typename std::enable_if_t<std::is_same_v<U, int>, sf::Color>() const {
            return sf::Color(
                static_cast<std::uint8_t>(r),
                static_cast<std::uint8_t>(g),
                static_cast<std::uint8_t>(b),
                static_cast<std::uint8_t>(a)
            );
        }
#endif

        // Equality operators
        constexpr bool operator==(const ColorT& other) const {
            return r == other.r && g == other.g && b == other.b && a == other.a;
        }

        constexpr bool operator!=(const ColorT& other) const {
            return !(*this == other);
        }
    };

    // Type aliases for backward compatibility and convenience
    /// @brief Integer-based color (0-255 range) - backward compatible with old Color
    using Color = ColorT<int>;

    /// @brief Floating-point color (0.0-1.0 range) - backward compatible with old ColorF
    using ColorF = ColorT<double>;

} // namespace paxg

#endif // !PAX_GRAPHICA_COLOR_HPP
