/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_UTILS_HPP
#define PAX_SAPIENTICA_MAP_UTILS_HPP

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>

namespace paxs {

    /**
     * @brief Map Utils
     * @brief 地図のユーティリティ
    */
    struct MapUtils
    {
        /**
         * @brief Convert to Mercator Coordinate
         * @brief メルカトル座標に変換
         * @return Mercator Coordinate
        */
        static paxs::Vector2<double> convertToMercatorCoordinate(const paxs::Vector2<GridType>& start_position, const paxs::Vector2<GridType>& position, const int z) noexcept {
            const double n = std::pow(2, z);
            const double pixel_sum = pixel_size * n;
            const double u = (double)position.x / pixel_sum + start_position.x / n;
            const double v = (double)position.y / pixel_sum + start_position.y / n;

            const double lon = u * 360.0 - 180.0;
            const double lat = -v * 360.0 + 180.0;

            return paxs::Vector2<double>(lon, lat);
        }
    };


}

#endif // !PAX_SAPIENTICA_MAP_UTILS_HPP
