/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_UTILS_HPP
#define PAX_SAPIENTICA_MAP_UTILS_HPP

#include <PAX_SAPIENTICA/Core/Math/Math.hpp>
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
         * @brief Convert tile pixel coordinates to longitude and latitude in degrees
         * @brief タイルピクセル座標を正規化座標 (u,v) → -180〜180 の角度空間に変換する
         * @param start_position タイルの開始位置（タイル座標系）
         * @param position ピクセル位置（ピクセル座標系）
         * @param z ズームレベル（0から始まる）
         * @return Vector2(経度 longitude, 緯度 latitude) 度数法、-180～180度
        */
        // TODO: これを使っている場所でメルカトル座標として扱っている箇所を確認する
        static paxs::Vector2<double> tilePixelToAngleSpace(const paxs::Vector2<GridType>& start_position, const paxs::Vector2<GridType>& position, const int z) noexcept {
            const double n = std::pow(2, z); // ズームレベルzにおけるタイル数（2^z）
            const double pixel_sum = pixel_size * n; // 全体のピクセル数
            const double u = (double)position.x / pixel_sum + start_position.x / n; // 正規化座標 u (0～1)
            const double v = (double)position.y / pixel_sum + start_position.y / n; // 正規化座標 v (0～1)

            const double lon = u * Math<double>::deg360() - Math<double>::deg180(); // 経度 (-180～180度)
            const double lat = -v * Math<double>::deg360() + Math<double>::deg180(); // 緯度 (-180～180度)

            return {lon, lat};
        }
    };


}

#endif // !PAX_SAPIENTICA_MAP_UTILS_HPP
