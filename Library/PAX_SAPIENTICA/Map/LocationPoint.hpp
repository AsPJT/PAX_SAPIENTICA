/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_LOCATION_POINT_HPP
#define PAX_SAPIENTICA_MAP_LOCATION_POINT_HPP

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <PAX_SAPIENTICA/Core/Type/Range.hpp>
#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    // 地物の位置情報
    struct LocationPoint {
        explicit LocationPoint() = default;
        explicit LocationPoint(
            std::string  key_,  // 地物の一意キー
            const paxs::WebMercatorDeg& coordinate_,  // 経緯度
            const double overall_length_,  // 全長
            const Range<double>& zoom_range_,  // 表示するズームレベル範囲
            const Range<double>& year_range_,  // 可視化する時代範囲
            const std::uint_least32_t feature_type_hash_,  // 地物の種別を識別するハッシュ値
            const std::uint_least32_t texture_key_, // テクスチャキー
            const double zoom_, // 拡大率
            const paxs::UnorderedMap<std::uint_least32_t, std::string>& extra_data_ = {},  // 追加カラムデータ
            const std::uint_least32_t key_hash_ = 0  // keyのハッシュ値
        ) noexcept
            : key(std::move(key_)), coordinate(coordinate_), overall_length(overall_length_),
            zoom_range(zoom_range_), year_range(year_range_),
            feature_type_hash(feature_type_hash_), texture_key(texture_key_), zoom(zoom_),
            extra_data(extra_data_), key_hash(key_hash_){}

        std::string key;  // 地物の一意キー（TSVのkeyカラムから、バイナリの場合は空）
        std::uint_least32_t key_hash = 0;  // keyのハッシュ値（バイナリから読み込んだ場合に使用）
        paxs::WebMercatorDeg coordinate; // 経緯度
        double overall_length = 10; // 全長
        Range<double> zoom_range{0.0, 9999.0}; // 表示するズームレベル範囲
        Range<double> year_range{-99999999.0, 99999999.0}; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t feature_type_hash = MurMur3::calcHash("place_name"); // 地物の種別を識別するハッシュ値（例: "tomb", "site", "person"）
        std::uint_least32_t texture_key = 0; // テクスチャキー（ハッシュ値）
        double zoom = 1.0; // 拡大率

        /// @brief 追加カラムデータ（TSVの拡張情報）
        /// @brief Extra column data from TSV files
        paxs::UnorderedMap<std::uint_least32_t, std::string> extra_data;

        /// @brief 追加カラムの値を取得（ハッシュ値で直接アクセス）
        /// @brief Get extra column value by hash
        std::string getExtraData(std::uint_least32_t column_hash) const {
            const auto iterator = extra_data.find(column_hash);
            return (iterator != extra_data.end()) ? iterator->second : "";
        }

        /// @brief 追加カラムが存在するか確認（ハッシュ値）
        /// @brief Check if extra column exists by hash
        bool hasExtraData(std::uint_least32_t column_hash) const {
            return extra_data.contains(column_hash);
        }
    };

    // 地物のグループ（ファイル単位のメタデータを含む）
    struct LocationPointGroup {
        std::vector<LocationPoint> location_point_list; // 地物の一覧

        paxs::WebMercatorDeg start_coordinate; // 経緯度
        paxs::WebMercatorDeg end_coordinate; // 経緯度
        Range<double> zoom_range{0.0, 9999.0}; // 表示するズームレベル範囲
        Range<double> year_range{-99999999.0, 99999999.0}; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t feature_type_hash = MurMur3::calcHash("place_name"); // 地物の種別を識別するハッシュ値（例: "tomb", "site", "person"）
        std::uint_least32_t texture_key = 0; // テクスチャキー（ハッシュ値）

        explicit LocationPointGroup() = default;
        explicit LocationPointGroup(
            const std::vector<LocationPoint>& location_point_list_,  // 地物
            paxs::WebMercatorDeg start_coordinate_, // 経緯度
            paxs::WebMercatorDeg end_coordinate_, // 経緯度
            const Range<double>& zoom_range_,  // 表示するズームレベル範囲
            const Range<double>& year_range_,  // 可視化する時代範囲
            const std::uint_least32_t feature_type_hash_,  // 地物の種別を識別するハッシュ値
            const std::uint_least32_t texture_key_ // テクスチャキー
        ) noexcept
            : location_point_list(location_point_list_),
            start_coordinate(start_coordinate_), end_coordinate(end_coordinate_),
            zoom_range(zoom_range_), year_range(year_range_),
            feature_type_hash(feature_type_hash_), texture_key(texture_key_) {}

    };

}

#endif // !PAX_MAHOROBA_LOCATION_POINT_HPP
