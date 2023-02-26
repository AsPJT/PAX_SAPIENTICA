/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TYPE_COORDINATE_HPP
#define PAX_SAPIENTICA_TYPE_COORDINATE_HPP

/*##########################################################################################

##########################################################################################*/

namespace paxs {

	namespace type {

		// 座標系のXY構造体
		template<typename T>
		struct Coordinate {
			T x;
			T y;

			bool operator!=(const Coordinate<T>& coordinate_) const {
				return x != coordinate_.x || y != coordinate_.y;
			}

			Coordinate() = default;
			Coordinate(const T& x_, const T& y_) :x(x_), y(y_) {}

		};

		// 座標系の範囲
		template<typename T>
		struct CoordinateRange {
			Coordinate<T> start;
			Coordinate<T> end;

			CoordinateRange() = default;
			CoordinateRange(const T& start_, const T& end_) :start(start_), end(end_) {}

		};

	}

}

#endif // !PAX_SAPIENTICA_TYPE_COORDINATE_HPP