/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_SPACE_GRID_HPP
#define PAX_SAPIENTICA_SIMULATION_SPACE_GRID_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/Type/Coordinate.hpp>

namespace paxs {

	// 空間グリッド
	template<typename T>
    class SpaceGrid{
	public:
		SpaceGrid(type::CoordinateRange<T>& coordinate_range, type::Coordinate<int_fast32_t> division_num_) {
			start = coordinate_range.start;
			end = coordinate_range.end;
			division_num = division_num_;
		}
    private:
		type::Coordinate<T> start;
		type::Coordinate<T> end;
		type::Coordinate<int_fast32_t> division_num;
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_SPACE_GRID_HPP