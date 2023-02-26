/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TYPE_GRID_HPP
#define PAX_SAPIENTICA_TYPE_GRID_HPP

/*##########################################################################################

##########################################################################################*/

#include <cstdint>

#include <PAX_SAPIENTICA/Type/Coordinate.hpp>

namespace paxs {

	namespace type {

		// 座標系のXY構造体
		template<typename T>
		struct Grid {
			T data; // 二次元配列のようなもの
			paxs::type::Coordinate<std::int_fast32_t> division_num;
			// C++座標系の範囲startの座標XとYがendの座標XとYよりも大きくなることはない（）

			Grid() = default;
			Grid(const paxs::type::Coordinate<std::int_fast32_t>& coordinate_) :division_num(coordinate_) {}

		};

	}

}

#endif // !PAX_SAPIENTICA_TYPE_GRID_HPP