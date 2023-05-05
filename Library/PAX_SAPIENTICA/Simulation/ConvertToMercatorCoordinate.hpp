/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_CONVERT_TO_MERCATOR_COORDINATE_HPP
#define PAX_SAPIENTICA_SIMULATION_CONVERT_TO_MERCATOR_COORDINATE_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/Type/Vector2.hpp>

namespace paxs {
    template <typename T>
    Vector2<double> convertToMercatorCoordinate(const Vector2<T>& position, const int z, const int pixel_size) {
        const int pixel_sum = pixel_size * std::pow(2, z);
        const double u = (double)position.x / pixel_sum;
        const double v = (double)position.y / pixel_sum;

        const double lon = u * 360.0 - 180.0;
        const double lat = v * 360.0 - 180.0;

        return Vector2<double>(lon, lat);
    }
}

#endif // !PAX_SAPIENTICA_SIMULATION_CONVERT_TO_MERCATOR_COORDINATE_HPP