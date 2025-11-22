/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_TILE_TILE_COORDINATE_HPP
#define PAX_SAPIENTICA_MAP_TILE_TILE_COORDINATE_HPP

#include <cmath>

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxs {

    /// @brief タイル座標計算を担当するクラス
    /// @brief Tile coordinate calculation class
    /// Web Mercator投影法のタイル座標を計算します。
    class TileCoordinate {
    public:
        /// @brief コンストラクタ
        /// @param z ズームレベル
        /// @param z_num 2^z の値（タイル数）
        TileCoordinate(unsigned int z, unsigned int z_num)
            : z_(z), z_num_(z_num) {}

        /// @brief ズームレベルを自動計算
        /// @brief Calculate zoom level automatically
        /// @param map_view_height マップビューの高さ（度）
        /// @return 計算されたズームレベル
        static int calculateZoomLevel(double map_view_height) {
            return static_cast<int>(-std::log2(map_view_height) + 12.5);
        }

        /// @brief 始点セルを計算
        /// @brief Calculate start cell
        /// @param map_view_center マップビュー中心座標（経度, 緯度） / Map view center (longitude, latitude)
        /// @param map_view_size マップビューサイズ（幅, 高さ） / Map view size (width, height)
        /// @return 始点セルの座標 / Start cell coordinates
        Vector2<int> calculateStartCell(
            const Vector2<double>& map_view_center,
            const Vector2<double>& map_view_size
        ) const {
            const int start_x = static_cast<int>(
                (((map_view_center.x - map_view_size.x / 2) + 180.0) / 360.0) * z_num_ + z_num_
            ) - static_cast<int>(z_num_);

            const int start_y = static_cast<int>(
                ((360.0 - ((map_view_center.y + map_view_size.y / 2) + 180.0)) / 360.0) * z_num_ + z_num_
            ) - static_cast<int>(z_num_);

            return Vector2<int>{start_x, start_y};
        }

        /// @brief 終点セルを計算
        /// @brief Calculate end cell
        /// @param map_view_center マップビュー中心座標（経度, 緯度） / Map view center (longitude, latitude)
        /// @param map_view_size マップビューサイズ（幅, 高さ） / Map view size (width, height)
        /// @return 終点セルの座標 / End cell coordinates
        Vector2<int> calculateEndCell(
            const Vector2<double>& map_view_center,
            const Vector2<double>& map_view_size
        ) const {
            const int end_x = static_cast<int>(
                (((map_view_center.x + map_view_size.x / 2) + 180.0) / 360.0) * z_num_ + z_num_
            ) - static_cast<int>(z_num_);

            const int end_y = static_cast<int>(
                ((360.0 - ((map_view_center.y - map_view_size.y / 2) + 180.0)) / 360.0) * z_num_ + z_num_
            ) - static_cast<int>(z_num_);

            return Vector2<int>{end_x, end_y};
        }

        /// @brief 2^z を計算
        /// @brief Calculate 2^z
        /// @param z ズームレベル
        /// @return 2^z の値
        static unsigned int calculateZNum(unsigned int z) {
            return 1U << z;
        }

        /// @brief ズームレベルを取得
        /// @brief Get zoom level
        unsigned int getZ() const { return z_; }

        /// @brief z_num（2^z）を取得
        /// @brief Get z_num (2^z)
        unsigned int getZNum() const { return z_num_; }

    private:
        unsigned int z_;       ///< ズームレベル
        unsigned int z_num_;   ///< 2^z の値（タイル数）
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_MAP_TILE_TILE_COORDINATE_HPP
