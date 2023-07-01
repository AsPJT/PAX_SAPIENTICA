/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TYPE_VECTOR2_HPP
#define PAX_SAPIENTICA_TYPE_VECTOR2_HPP

/*##########################################################################################

##########################################################################################*/

#include <iostream>

namespace paxs {
    
    /// @brief 2D Vector class.
    template<typename T>
    class Vector2 {
    public:
        constexpr Vector2() = default;
        constexpr Vector2(T x, T y) : x(x), y(y) {}

        T x;
        T y;

        constexpr Vector2<T> operator+(const Vector2<T>& v) const {
            return Vector2<T>(x + v.x, y + v.y);
        }
        constexpr Vector2<T>& operator+=(const Vector2<T>& v) {
            x += v.x;
            y += v.y;
            return *this;
        }
        constexpr Vector2<T> operator-(const Vector2<T>& v) const {
            return Vector2<T>(x - v.x, y - v.y);
        }
        constexpr Vector2<T>& operator-=(const Vector2<T>& v) {
            x -= v.x;
            y -= v.y;
            return *this;
        }
        constexpr Vector2<T> operator*(const Vector2<T>& v) const {
            return Vector2<T>(x * v.x, y * v.y);
        }
        constexpr Vector2<T> operator*(const int& t) const {
            return Vector2<T>(x * t, y * t);
        }
        constexpr Vector2<T>& operator*=(const int& t) {
            x *= t;
            y *= t;
            return *this;
        }
        constexpr Vector2<T> operator*(const double& t) const {
            return Vector2<T>(x * T(t), y * T(t));
        }
        constexpr Vector2<T>& operator*=(const double& t) {
            x *= t;
            y *= t;
            return *this;
        }
        constexpr Vector2<T> operator/(const Vector2<T>& v) const {
            return Vector2<T>(x / v.x, y / v.y);
        }
        constexpr Vector2<T> operator/(const int& t) const {
            return Vector2<T>(x / t, y / t);
        }
        constexpr Vector2<T>& operator/=(const int& t) {
            x /= t;
            y /= t;
            return *this;
        }
        constexpr Vector2<T> operator/(const double& t) const {
            return Vector2<T>(x / t, y / t);
        }
        constexpr Vector2<T>& operator/=(const double& t) {
            x /= t;
            y /= t;
            return *this;
        }
        constexpr bool operator==(const Vector2<T>& v) const {
            return v.x == x && v.y == y;
        }
        constexpr bool operator!=(const Vector2<T>& v) const {
            return v.x != x || v.y != y;
        }
        constexpr bool operator<(const Vector2<T>& v) const {
            if (x == v.x) {
                return y < v.y;
            }
            return x < v.x;
        }
        constexpr bool operator>(const Vector2<T>& v) const {
            if (x == v.x) {
                return y > v.y;
            }
            return x > v.x;
        }
        constexpr bool operator<=(const Vector2<T>& v) const {
            if (x == v.x) {
                return y <= v.y;
            }
            return x <= v.x;
        }
        constexpr bool operator>=(const Vector2<T>& v) const {
            if (x == v.x) {
                return y >= v.y;
            }
            return x >= v.x;
        }

        /// @brief Get the length of the vector
        double length() const {
            return sqrt(x * x + y * y);
        }

        /// @brief  Get the distance between two vectors
        /// @param v Another vector
        double distance(const Vector2<T>& v) const {
            return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
        }
        friend std::ostream& operator<<(std::ostream& os, const Vector2<T>& v) {
            os << "(" << v.x << ", " << v.y << ")";
            return os;
        }
    };
}

#endif // !PAX_SAPIENTICA_TYPE_VECTOR2_HPP