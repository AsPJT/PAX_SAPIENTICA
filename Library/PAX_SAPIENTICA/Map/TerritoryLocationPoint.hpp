/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_TERRITORY_LOCATION_POINT_HPP
#define PAX_SAPIENTICA_MAP_TERRITORY_LOCATION_POINT_HPP

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <PAX_SAPIENTICA/Core/Type/Range.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief 領域の位置情報（スプライン曲線用の点列）
    /// @brief Territory location information (points for spline curves)
    struct TerritoryLocationData {
        explicit TerritoryLocationData() = default;
        explicit TerritoryLocationData(
            std::string  key_,  // 領域の一意キー / Unique key for territory
            std::vector<paxs::WebMercatorDeg>&& coordinates_,  // スプライン曲線の座標列 / Coordinates for spline curve
            const Range<double>& zoom_range_,
            const Range<double>& year_range_,
            const std::uint_least32_t feature_type_hash_
        ) noexcept
            : key(std::move(key_)),
              coordinates(std::move(coordinates_)),
              zoom_range(zoom_range_),
              year_range(year_range_),
              feature_type_hash(feature_type_hash_) {}

        std::string key;  // 領域の一意キー / Unique key for territory
        std::vector<paxs::WebMercatorDeg> coordinates; // スプライン曲線の座標列 / Coordinates for spline curve
        Range<double> zoom_range{0.0, 9999.0}; // 表示するズームレベル範囲 / Zoom level range
        Range<double> year_range{-99999999.0, 99999999.0}; // 可視化する時代 / Time range
        std::uint_least32_t feature_type_hash = MurMur3::calcHash("territory"); // 地物の種別 / Feature type hash
    };

    /// @brief 領域のグループ
    /// @brief Territory group
    struct TerritoryLocationGroup {
        std::vector<TerritoryLocationData> territory_list; // 領域の一覧 / List of territories

        Range<double> zoom_range{0.0, 9999.0}; // 表示するズームレベル範囲 / Zoom level range
        Range<double> year_range{-99999999.0, 99999999.0}; // 可視化する時代範囲 / Time range
        std::uint_least32_t feature_type_hash = MurMur3::calcHash("territory"); // 地物の種別 / Feature type hash
        std::uint_least32_t color_hash = 0; // 色情報のハッシュ / Color hash
        float line_width = 2.0f; // 線の太さ / Line width

        explicit TerritoryLocationGroup() = default;
        explicit TerritoryLocationGroup(
            const std::vector<TerritoryLocationData>& territory_list_,  // 領域リスト / Territory list
            const Range<double>& zoom_range_,  // 表示するズームレベル範囲 / Zoom level range
            const Range<double>& year_range_,  // 可視化する時代範囲 / Time range
            const std::uint_least32_t feature_type_hash_,  // 地物の種別 / Feature type hash
            const std::uint_least32_t color_hash_,  // 色情報のハッシュ / Color hash
            float line_width_  // 線の太さ / Line width
        ) noexcept
            : territory_list(territory_list_),
              zoom_range(zoom_range_),
              year_range(year_range_),
              feature_type_hash(feature_type_hash_),
              color_hash(color_hash_),
              line_width(line_width_) {}
    };

}

#endif // !PAX_SAPIENTICA_MAP_TERRITORY_LOCATION_POINT_HPP
