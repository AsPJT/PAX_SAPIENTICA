/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_LOCATION_POINT_HPP
#define PAX_MAHOROBA_LOCATION_POINT_HPP

#include <cstdint>
#include <algorithm>
#include <functional>
#include <string>

#include <PAX_GRAPHICA/Line.hpp>
#include <PAX_GRAPHICA/Texture.hpp>

#include <PAX_MAHOROBA/Core/Init.hpp>

#include <PAX_SAPIENTICA/InputFile.hpp>
#include <PAX_SAPIENTICA/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/MapProjection.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    constexpr double days_in_a_year = 365.2425;
    constexpr double julian_day_on_m1_1_1 = 1721060.0;

    // 地名
    struct LocationPoint {
        constexpr explicit LocationPoint() = default;
        explicit LocationPoint(
            const paxs::UnorderedMap<std::uint_least32_t, std::string>& place_name_,  // 地名
            const paxs::MercatorDeg& coordinate_,  // 経緯度
            std::uint_least16_t x_size_,  // 重ね枚数
            std::uint_least16_t y_size_,  // 重ね枚数
            const double overall_length_,  // 全長
            const double min_view_,  // 可視化する地図の最小範囲
            const double max_view_,  // 可視化する地図の最大範囲
            const int min_year_,  // 可視化する時代（古い年～）
            const int max_year_,  // 可視化する時代（～新しい年）
            const std::uint_least32_t lpe_,  // 対象となる地物の種別
            const std::uint_least32_t place_texture_, // 出典
            const double zoom_ // 拡大率
        ) noexcept
            : place_name(place_name_), coordinate(coordinate_), x_size(x_size_), y_size(y_size_), overall_length(overall_length_),
            min_view(min_view_), max_view(max_view_), min_year(min_year_), max_year(max_year_), lpe(lpe_), place_texture(place_texture_), zoom(zoom_){}

        paxs::UnorderedMap<std::uint_least32_t, std::string> place_name{}; // 地名
        paxs::MercatorDeg coordinate{}; // 経緯度
        std::uint_least16_t x_size = 1;
        std::uint_least16_t y_size = 1;
        double overall_length = 10; // 全長
        double min_view = 0.0, max_view = 9999.0; // 可視化する地図の拡大縮小範囲
        int min_year = -99999999, max_year = 99999999; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t lpe = MurMur3::calcHash("place_name"); // 対象となる地物の種別
        std::uint_least32_t place_texture = 0; // 地物の画像
        double zoom = 1.0; // 拡大率
    };

    // 地物の一覧
    struct LocationPointList {
        std::vector<LocationPoint> location_point_list{}; // 地物の一覧

        paxs::MercatorDeg start_coordinate{}; // 経緯度
        paxs::MercatorDeg end_coordinate{}; // 経緯度
        double min_view = 0.0, max_view = 9999.0; // 可視化する地図の拡大縮小範囲
        int min_year = -99999999, max_year = 99999999; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t lpe = MurMur3::calcHash("place_name"); // 対象となる地物の種別
        std::uint_least32_t place_texture = 0; // 地物の画像

        constexpr explicit LocationPointList() = default;
        explicit LocationPointList(
            const std::vector<LocationPoint>& location_point_list_,  // 地物
            paxs::MercatorDeg start_coordinate_, // 経緯度
            paxs::MercatorDeg end_coordinate_, // 経緯度
            const double min_view_,  // 可視化する地図の最小範囲
            const double max_view_,  // 可視化する地図の最大範囲
            const int min_year_,  // 可視化する時代（古い年～）
            const int max_year_,  // 可視化する時代（～新しい年）
            const std::uint_least32_t lpe_,  // 対象となる地物の種別
            const std::uint_least32_t place_texture_ // 出典
        ) noexcept
            : location_point_list(location_point_list_),
            start_coordinate(start_coordinate_), end_coordinate(end_coordinate_),
            min_view(min_view_), max_view(max_view_), min_year(min_year_), max_year(max_year_), lpe(lpe_), place_texture(place_texture_) {}

    };

}

#endif // !PAX_MAHOROBA_LOCATION_POINT_HPP
