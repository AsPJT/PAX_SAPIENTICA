/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CORE_TYPE_VECTOR2_HPP
#define PAX_SAPIENTICA_CORE_TYPE_VECTOR2_HPP

#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>

namespace paxs {

    /// @brief 2D Vector class.
    template<typename T>
    struct Vector2 {
        constexpr explicit Vector2() noexcept = default;
        constexpr Vector2(T x, T y) noexcept : x(x), y(y) {}

        T x;
        T y;

        [[nodiscard]] constexpr static Vector2<T> zero() noexcept { return Vector2<T>(0, 0); }

        [[nodiscard]] constexpr Vector2<T> operator-() const noexcept {
            return Vector2<T>(-x, -y);
        }

        [[nodiscard]] constexpr Vector2<T> operator+(const Vector2<T>& v) const noexcept {
            return Vector2<T>(x + v.x, y + v.y);
        }
        constexpr Vector2<T>& operator+=(const Vector2<T>& v) noexcept {
            x += v.x;
            y += v.y;
            return *this;
        }
        [[nodiscard]] constexpr Vector2<T> operator-(const Vector2<T>& v) const noexcept {
            return Vector2<T>(x - v.x, y - v.y);
        }
        constexpr Vector2<T>& operator-=(const Vector2<T>& v) noexcept {
            x -= v.x;
            y -= v.y;
            return *this;
        }
        [[nodiscard]] constexpr Vector2<T> operator*(const Vector2<T>& v) const noexcept {
            return Vector2<T>(x * v.x, y * v.y);
        }

        template<typename U>
        [[nodiscard]] constexpr Vector2<T> operator*(const U& t) const noexcept {
            return Vector2<T>(static_cast<T>(x * t), static_cast<T>(y * t));
        }

        template<typename U>
        constexpr Vector2<T>& operator*=(const U& t) noexcept {
            x *= t;
            y *= t;
            return *this;
        }

        [[nodiscard]] constexpr Vector2<T> operator/(const Vector2<T>& v) const noexcept {
            return Vector2<T>(x / v.x, y / v.y);
        }

        template<typename U>
        [[nodiscard]] constexpr Vector2<T> operator/(const U& t) const noexcept {
            return Vector2<T>(static_cast<T>(x / t), static_cast<T>(y / t));
        }

        template<typename U>
        constexpr Vector2<T>& operator/=(const U& t) noexcept {
            x /= t;
            y /= t;
            return *this;
        }

        template<typename U>
        explicit constexpr operator Vector2<U>() const noexcept {
            return Vector2<U>(static_cast<U>(x), static_cast<U>(y));
        }

        auto operator<=>(const Vector2<T>& v) const noexcept = default;
        /// @brief Get the length of the vector
        /// @brief べクトルの長さを取得。
        [[nodiscard]] double length() const noexcept {
            return std::sqrt((x * x) + (y * y));
        }

        /// @brief  Get the distance between two vectors
        /// @brief  二つのベクトルの距離を取得。
        [[nodiscard]] double distance(const Vector2<T>& v) const noexcept {
            return std::sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
        }
        /// @brief  Get the distance^2 between two vectors
        /// @brief  二つのベクトルの距離の２乗を取得。
        [[nodiscard]] double distance_pow2(const Vector2<T>& v) const noexcept {
            return ((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
        }
        friend std::ostream& operator<<(std::ostream& os, const Vector2<T>& v) noexcept {
            os << "(" << v.x << ", " << v.y << ")";
            return os;
        }

        [[nodiscard]] std::string toString() const noexcept {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }

        [[nodiscard]] std::uint_least64_t to(std::uint_least64_t) const noexcept {
            return static_cast<std::uint_least64_t>(x) << 32 | static_cast<std::uint_least64_t>(y);
        }
        [[nodiscard]] std::int_least64_t to(std::int_least64_t) const noexcept {
            return static_cast<std::int_least64_t>(x) << 32 | static_cast<std::int_least64_t>(y);
        }
        [[nodiscard]] std::uint_least32_t to(std::uint_least32_t) const noexcept {
            return static_cast<std::uint_least32_t>(x) << 16 | static_cast<std::uint_least32_t>(y);
        }
        [[nodiscard]] std::int_least32_t to(std::int_least32_t) const noexcept {
            return static_cast<std::int_least32_t>(x) << 16 | static_cast<std::int_least32_t>(y);
        }
        [[nodiscard]] std::uint_least16_t to(std::uint_least16_t) const noexcept {
            return static_cast<std::uint_least16_t>(x) << 8 | static_cast<std::uint_least16_t>(y);
        }
        [[nodiscard]] std::int_least16_t to(std::int_least16_t) const noexcept {
            return static_cast<std::int_least16_t>(x) << 8 | static_cast<std::int_least16_t>(y);
        }

        [[nodiscard]] static Vector2<T> from(const std::uint_least64_t u64) noexcept {
            return Vector2<T>(static_cast<T>(u64 >> 32), static_cast<T>(u64 & 0x00000000ffffffff));
        }
        [[nodiscard]] static Vector2<T> from(const std::uint_least32_t u32) noexcept {
            return Vector2<T>(static_cast<T>(u32 >> 16), static_cast<T>(u32 & 0x0000ffff));
        }
        [[nodiscard]] static Vector2<T> from(const std::uint_least16_t u16) noexcept {
            return Vector2<T>(static_cast<T>(u16 >> 8), static_cast<T>(u16 & 0x00ff));
        }

    };
}

#endif // !PAX_SAPIENTICA_CORE_TYPE_VECTOR2_HPP
