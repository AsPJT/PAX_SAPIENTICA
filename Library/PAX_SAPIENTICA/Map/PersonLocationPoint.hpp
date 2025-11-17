/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_PERSON_LOCATION_POINT_HPP
#define PAX_SAPIENTICA_MAP_PERSON_LOCATION_POINT_HPP

#include <cstdint>
#include <string>

#include <PAX_SAPIENTICA/Core/Type/Range.hpp>
#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    // 人物の位置情報
    struct PersonLocationPoint {
        explicit PersonLocationPoint() = default;
        explicit PersonLocationPoint(
            const std::string& key_,  // 人物の一意キー
            const paxs::UnorderedMap<std::uint_least32_t, std::string>& names_,
            const paxs::MercatorDeg& start_coordinate_,
            const paxs::MercatorDeg& end_coordinate_,
            const Range<double>& zoom_range_,
            const Range<double>& year_range_,
            const std::uint_least32_t feature_type_hash_,
            const std::uint_least32_t texture_key_
        ) noexcept
            : key(key_), names(names_), start_coordinate(start_coordinate_), end_coordinate(end_coordinate_),
            zoom_range(zoom_range_), year_range(year_range_), feature_type_hash(feature_type_hash_), texture_key(texture_key_) {}

        std::string key;  // 人物の一意キー（ファイル名から）
        paxs::UnorderedMap<std::uint_least32_t, std::string> names; // 人物名（多言語対応）
        paxs::MercatorDeg start_coordinate; // 開始時の経緯度
        paxs::MercatorDeg end_coordinate; // 終了時の経緯度
        Range<double> zoom_range{0.0, 9999.0}; // 表示するズームレベル範囲
        Range<double> year_range{-99999999.0, 99999999.0}; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t feature_type_hash = MurMur3::calcHash("person"); // 地物の種別を識別するハッシュ値
        std::uint_least32_t texture_key = 0; // テクスチャキー（ハッシュ値）
    };

    // 人物の一覧
    struct PersonLocationList {
        std::vector<PersonLocationPoint> person_location_list; // 人物の一覧

        paxs::MercatorDeg start_coordinate; // 開始時の経緯度
        paxs::MercatorDeg end_coordinate; // 終了時の経緯度
        Range<double> zoom_range{0.0, 9999.0}; // 表示するズームレベル範囲
        Range<double> year_range{-99999999.0, 99999999.0}; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t feature_type_hash = MurMur3::calcHash("person"); // 地物の種別を識別するハッシュ値
        std::uint_least32_t texture_key = 0; // テクスチャキー（ハッシュ値）

        explicit PersonLocationList() = default;
        explicit PersonLocationList(
            const std::vector<PersonLocationPoint>& person_location_list_,  // 人物
            const Range<double>& zoom_range_,  // 表示するズームレベル範囲
            const Range<double>& year_range_,  // 可視化する時代範囲
            const std::uint_least32_t feature_type_hash_,  // 地物の種別を識別するハッシュ値
            const std::uint_least32_t texture_key_ // テクスチャキー
        ) noexcept
            : person_location_list(person_location_list_),
            zoom_range(zoom_range_), year_range(year_range_),
            feature_type_hash(feature_type_hash_), texture_key(texture_key_) {}

    };

}

#endif // !PAX_SAPIENTICA_MAP_PERSON_LOCATION_POINT_HPP
