/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TYPE_VECTOR2_HPP
#define PAX_SAPIENTICA_TYPE_VECTOR2_HPP

/*##########################################################################################

##########################################################################################*/

#include <cmath>
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

        constexpr static Vector2<T> zero() noexcept { return Vector2<T>(0, 0); }

        constexpr Vector2<T> operator-() const noexcept {
            return Vector2<T>(-x, -y);
        }

        constexpr Vector2<T> operator+(const Vector2<T>& v) const noexcept {
            return Vector2<T>(x + v.x, y + v.y);
        }
        constexpr Vector2<T>& operator+=(const Vector2<T>& v) noexcept {
            x += v.x;
            y += v.y;
            return *this;
        }
        constexpr Vector2<T> operator-(const Vector2<T>& v) const noexcept {
            return Vector2<T>(x - v.x, y - v.y);
        }
        constexpr Vector2<T>& operator-=(const Vector2<T>& v) noexcept {
            x -= v.x;
            y -= v.y;
            return *this;
        }
        constexpr Vector2<T> operator*(const Vector2<T>& v) const noexcept {
            return Vector2<T>(x * v.x, y * v.y);
        }

        template<typename U>
        constexpr Vector2<T> operator*(const U& t) const noexcept {
            return Vector2<T>(static_cast<T>(x * t), static_cast<T>(y * t));
        }

        template<typename U>
        constexpr Vector2<T>& operator*=(const U& t) noexcept {
            x *= t;
            y *= t;
            return *this;
        }

        constexpr Vector2<T> operator/(const Vector2<T>& v) const noexcept {
            return Vector2<T>(x / v.x, y / v.y);
        }

        template<typename U>
        constexpr Vector2<T> operator/(const U& t) const noexcept {
            return Vector2<T>(static_cast<T>(x / t), static_cast<T>(y / t));
        }

        template<typename U>
        constexpr Vector2<T>& operator/=(const U& t) noexcept {
            x /= t;
            y /= t;
            return *this;
        }
#ifdef USING_CPP20
        auto operator<=>(const Vector2<T>& v) const noexcept = default;
#else
        bool operator!=(const Vector2<T>& v) const noexcept {
            return !(v.x == x && v.y == y);
        }
        bool operator==(const Vector2<T>& v) const noexcept {
            return (v.x == x && v.y == y);
        }
#endif // USING_CPP20
        /// @brief Get the length of the vector
        /// @brief べクトルの長さを取得。
        double length() const noexcept {
            return std::sqrt(x * x + y * y);
        }

        /// @brief  Get the distance between two vectors
        /// @brief  二つのベクトルの距離を取得。
        double distance(const Vector2<T>& v) const noexcept {
            return std::sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
        }
        friend std::ostream& operator<<(std::ostream& os, const Vector2<T>& v) noexcept {
            os << "(" << v.x << ", " << v.y << ")";
            return os;
        }

        std::string toString() const noexcept {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }

        template<typename T>
        T to() const noexcept {
            return static_cast<T>(x) << 32 | static_cast<T>(y);
        }
        template<>
        std::uint_least64_t to<std::uint_least64_t>() const noexcept {
            return static_cast<std::uint_least64_t>(x) << 32 | static_cast<T>(y);
        }
        template<>
        std::int_least64_t to<std::int_least64_t>() const noexcept {
            return static_cast<std::int_least64_t>(x) << 32 | static_cast<T>(y);
        }
        template<>
        std::uint_least32_t to<std::uint_least32_t>() const noexcept {
            return static_cast<std::uint_least32_t>(x) << 16 | static_cast<T>(y);
        }
        template<>
        std::int_least32_t to<std::int_least32_t>() const noexcept {
            return static_cast<std::int_least32_t>(x) << 16 | static_cast<T>(y);
        }

        std::uint_least64_t toU64() const noexcept {
            return static_cast<std::uint_least64_t>(x) << 32 | static_cast<std::uint_least64_t>(y);
        }

        static Vector2<T> fromU64(const std::uint_least64_t u64) noexcept {
            return Vector2<T>(static_cast<T>(u64 >> 32), static_cast<T>(u64 & 0x00000000ffffffff));
        }
    };
}

#endif // !PAX_SAPIENTICA_TYPE_VECTOR2_HPP
