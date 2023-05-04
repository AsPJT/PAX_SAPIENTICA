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

        bool operator==(const paxs::Vector2<T>& v) {
            return v.x == x && v.y == y;
        }
    };
}

#endif // !PAX_SAPIENTICA_TYPE_VECTOR2_HPP