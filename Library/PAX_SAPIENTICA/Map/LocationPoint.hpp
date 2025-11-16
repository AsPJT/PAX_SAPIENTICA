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

#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    constexpr double days_in_a_year = 365.2425;
    constexpr double julian_day_on_m1_1_1 = 1721060.0;

    // 地物の位置情報
    struct LocationPoint {
        constexpr explicit LocationPoint() = default;
        explicit LocationPoint(
            const paxs::UnorderedMap<std::uint_least32_t, std::string>& place_name_,  // 地物名（多言語対応）
            const paxs::MercatorDeg& coordinate_,  // 経緯度
            std::uint_least16_t x_size_,  // 重ね枚数
            std::uint_least16_t y_size_,  // 重ね枚数
            const double overall_length_,  // 全長
            const double min_zoom_level_,  // 表示する最小ズームレベル
            const double max_zoom_level_,  // 表示する最大ズームレベル
            const int min_year_,  // 可視化する時代（古い年～）
            const int max_year_,  // 可視化する時代（～新しい年）
            const std::uint_least32_t feature_type_hash_,  // 地物の種別を識別するハッシュ値
            const std::uint_least32_t texture_key_, // テクスチャキー
            const double zoom_, // 拡大率
            const paxs::UnorderedMap<std::uint_least32_t, std::string>& extra_data_ = {}  // 追加カラムデータ
        ) noexcept
            : place_name(place_name_), coordinate(coordinate_), x_size(x_size_), y_size(y_size_), overall_length(overall_length_),
            min_zoom_level(min_zoom_level_), max_zoom_level(max_zoom_level_), min_year(min_year_), max_year(max_year_), feature_type_hash(feature_type_hash_), texture_key(texture_key_), zoom(zoom_), extra_data(extra_data_){}

        paxs::UnorderedMap<std::uint_least32_t, std::string> place_name{}; // 地物名（多言語対応：地名、遺跡名、古墳名など）
        paxs::MercatorDeg coordinate{}; // 経緯度
        std::uint_least16_t x_size = 1;
        std::uint_least16_t y_size = 1;
        double overall_length = 10; // 全長
        double min_zoom_level = 0.0, max_zoom_level = 9999.0; // 表示するズームレベル範囲
        int min_year = -99999999, max_year = 99999999; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t feature_type_hash = MurMur3::calcHash("place_name"); // 地物の種別を識別するハッシュ値（例: "tomb", "site", "person"）
        std::uint_least32_t texture_key = 0; // テクスチャキー（ハッシュ値）
        double zoom = 1.0; // 拡大率

        /// @brief 追加カラムデータ（TSVの拡張情報）
        /// @brief Extra column data from TSV files
        paxs::UnorderedMap<std::uint_least32_t, std::string> extra_data{};

        /// @brief 追加カラムの値を取得（ハッシュ値で直接アクセス）
        /// @brief Get extra column value by hash
        std::string getExtraData(std::uint_least32_t column_hash) const {
            auto it = extra_data.find(column_hash);
            return (it != extra_data.end()) ? it->second : "";
        }

        /// @brief 追加カラムが存在するか確認（ハッシュ値）
        /// @brief Check if extra column exists by hash
        bool hasExtraData(std::uint_least32_t column_hash) const {
            return extra_data.find(column_hash) != extra_data.end();
        }
    };

    // 地物の一覧
    struct LocationPointList {
        std::vector<LocationPoint> location_point_list{}; // 地物の一覧

        paxs::MercatorDeg start_coordinate{}; // 経緯度
        paxs::MercatorDeg end_coordinate{}; // 経緯度
        double min_zoom_level = 0.0, max_zoom_level = 9999.0; // 表示するズームレベル範囲
        int min_year = -99999999, max_year = 99999999; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t feature_type_hash = MurMur3::calcHash("place_name"); // 地物の種別を識別するハッシュ値（例: "tomb", "site", "person"）
        std::uint_least32_t texture_key = 0; // テクスチャキー（ハッシュ値）

        constexpr explicit LocationPointList() = default;
        explicit LocationPointList(
            const std::vector<LocationPoint>& location_point_list_,  // 地物
            paxs::MercatorDeg start_coordinate_, // 経緯度
            paxs::MercatorDeg end_coordinate_, // 経緯度
            const double min_zoom_level_,  // 表示する最小ズームレベル
            const double max_zoom_level_,  // 表示する最大ズームレベル
            const int min_year_,  // 可視化する時代（古い年～）
            const int max_year_,  // 可視化する時代（～新しい年）
            const std::uint_least32_t feature_type_hash_,  // 地物の種別を識別するハッシュ値
            const std::uint_least32_t texture_key_ // テクスチャキー
        ) noexcept
            : location_point_list(location_point_list_),
            start_coordinate(start_coordinate_), end_coordinate(end_coordinate_),
            min_zoom_level(min_zoom_level_), max_zoom_level(max_zoom_level_), min_year(min_year_), max_year(max_year_), feature_type_hash(feature_type_hash_), texture_key(texture_key_) {}

    };

}

#endif // !PAX_MAHOROBA_LOCATION_POINT_HPP
