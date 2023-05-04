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

namespace paxs {
    template<typename T>
    class Vector2 {
    public:
        Vector2(T x, T y) : x(x), y(y) {}

        T x;
        T y;

        Vector2<T> operator+(const Vector2<T>& v) const {
            return Vector2<T>(x + v.x, y + v.y);
        }
        Vector2<T> operator-(const Vector2<T>& v) const {
            return Vector2<T>(x - v.x, y - v.y);
        }
        Vector2<T> operator*(const Vector2<T>& v) const {
            return Vector2<T>(x * v.x, y * v.y);
        }
        Vector2<T> operator*(const T& t) const {
            return Vector2<T>(x * t, y * t);
        }
        Vector2<T> operator/(const Vector2<T>& v) const {
            return Vector2<T>(x / v.x, y / v.y);
        }
        Vector2<T> operator/(const T& t) const {
            return Vector2<T>(x / t, y / t);
        }
        bool operator==(const Vector2<T>& v) const {
            return v.x == x && v.y == y;
        }
        bool operator<(const Vector2<T>& v) const {
            if (x == v.x) {
                return y < v.y;
            }
            return x < v.x;
        }
    };
}

#endif // !PAX_SAPIENTICA_TYPE_VECTOR2_HPP