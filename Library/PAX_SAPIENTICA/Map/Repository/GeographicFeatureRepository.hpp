/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_REPOSITORY_GEOGRAPHIC_FEATURE_REPOSITORY_HPP
#define PAX_SAPIENTICA_MAP_REPOSITORY_GEOGRAPHIC_FEATURE_REPOSITORY_HPP

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/IO/Data/TsvTable.hpp>
#include <PAX_SAPIENTICA/Map/LocationPoint.hpp>
#include <PAX_SAPIENTICA/Map/Repository/FeatureListLoader.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief 地理的特徴のデータ読み込みを担当する構造体 (Infrastructure Layer)
    struct GeographicFeatureRepository {
        /// @brief 地理的特徴データファイルのリストを読み込み、全てのLocationPointを返す
        /// @brief Load geographic feature data file list and return all LocationPoints
        static std::vector<LocationPoint> loadGeographicFeatureList() {
            std::vector<LocationPoint> all_points;

            FeatureListLoader::loadFeatureList("GeographicFeatures", [&all_points](const FeatureListParams& params) {
                auto loaded = loadGeographicFeatureFromFile(params);
                all_points.insert(all_points.end(),
                    std::make_move_iterator(loaded.location_point_list.begin()),
                    std::make_move_iterator(loaded.location_point_list.end()));
            });

            return all_points;
        }

        /// @brief 個別の地理的特徴ファイルを読み込んでLocationPointListを返す
        static LocationPointList loadGeographicFeatureFromFile(const FeatureListParams& params) {
            std::vector<LocationPoint> location_point_list{};
            const std::uint_least32_t lpe = MurMur3::calcHash(params.type.size(), params.type.c_str());

            paxs::TsvTable table(params.file_path);
            if (!table.isSuccessfullyLoaded()) {
                PAXS_WARNING("Failed to load geographic feature file: " + params.file_path);
                return LocationPointList{};
            }

            // 必須カラムの存在確認
            if (!table.hasColumn("longitude")) {
                PAXS_ERROR("GeographicFeatureFile is missing required column: longitude");
                return LocationPointList{};
            }
            if (!table.hasColumn("latitude")) {
                PAXS_ERROR("GeographicFeatureFile is missing required column: latitude");
                return LocationPointList{};
            }

            double start_longitude = 180.0;
            double end_longitude = -180.0;
            double start_latitude = 90.0;
            double end_latitude = -90.0;

            // カラムハッシュキーを取得
            const std::uint_least32_t longitude_hash = MurMur3::calcHash("longitude");
            const std::uint_least32_t latitude_hash = MurMur3::calcHash("latitude");
            const std::uint_least32_t overall_length_hash = MurMur3::calcHash("overall_length");
            const std::uint_least32_t x_size_hash = MurMur3::calcHash("x_size");
            const std::uint_least32_t y_size_hash = MurMur3::calcHash("y_size");
            const std::uint_least32_t min_size_hash = MurMur3::calcHash("min_size");
            const std::uint_least32_t max_size_hash = MurMur3::calcHash("max_size");
            const std::uint_least32_t first_julian_day_hash = MurMur3::calcHash("first_julian_day");
            const std::uint_least32_t last_julian_day_hash = MurMur3::calcHash("last_julian_day");
            const std::uint_least32_t texture_hash = MurMur3::calcHash("texture");
            const std::uint_least32_t first_year_hash = MurMur3::calcHash("first_year");
            const std::uint_least32_t last_year_hash = MurMur3::calcHash("last_year");
            const std::uint_least32_t ja_jp_hash = MurMur3::calcHash("ja-JP");
            const std::uint_least32_t en_us_hash = MurMur3::calcHash("en-US");

            const bool has_overall_length = table.hasColumn(overall_length_hash);
            const bool has_x_size = table.hasColumn(x_size_hash);
            const bool has_y_size = table.hasColumn(y_size_hash);
            const bool has_min_size = table.hasColumn(min_size_hash);
            const bool has_max_size = table.hasColumn(max_size_hash);
            const bool has_first_julian_day = table.hasColumn(first_julian_day_hash);
            const bool has_last_julian_day = table.hasColumn(last_julian_day_hash);
            const bool has_texture = table.hasColumn(texture_hash);
            const bool has_first_year = table.hasColumn(first_year_hash);
            const bool has_last_year = table.hasColumn(last_year_hash);
            const bool has_ja_jp = table.hasColumn(ja_jp_hash);
            const bool has_en_us = table.hasColumn(en_us_hash);

            constexpr int days_in_a_year = 365;
            constexpr int julian_day_on_m1_1_1 = 1721058;

            // 1 行ずつ読み込み
            table.forEachRow([&](std::size_t row_index, const std::vector<std::string>& row) {
                const std::string& longitude_str = table.get(row_index, longitude_hash);
                const std::string& latitude_str = table.get(row_index, latitude_hash);

                if (longitude_str.empty() || latitude_str.empty()) {
                    return;
                }

                // 地理的特徴名
                paxs::UnorderedMap<std::uint_least32_t, std::string> feature_name{};
                if (has_ja_jp) {
                    const std::string& ja_jp_str = table.get(row_index, ja_jp_hash);
                    if (!ja_jp_str.empty()) {
                        feature_name.emplace(MurMur3::calcHash("ja-JP"), ja_jp_str);
                    }
                }
                if (has_en_us) {
                    const std::string& en_us_str = table.get(row_index, en_us_hash);
                    if (!en_us_str.empty()) {
                        feature_name.emplace(MurMur3::calcHash("en-US"), en_us_str);
                    }
                }

                const std::string& overall_length_str = has_overall_length ? table.get(row_index, overall_length_hash) : "";
                const std::string& x_size_str = has_x_size ? table.get(row_index, x_size_hash) : "";
                const std::string& y_size_str = has_y_size ? table.get(row_index, y_size_hash) : "";

                double point_longitude = std::stod(longitude_str);
                double point_latitude = std::stod(latitude_str);
                start_longitude = (std::min)(start_longitude, point_longitude);
                start_latitude = (std::min)(start_latitude, point_latitude);
                end_longitude = (std::max)(end_longitude, point_longitude);
                end_latitude = (std::max)(end_latitude, point_latitude);

                // 日付範囲の処理
                const std::string& first_jd_str = has_first_julian_day ? table.get(row_index, first_julian_day_hash) : "";
                const std::string& first_year_str = has_first_year ? table.get(row_index, first_year_hash) : "";
                int first_jd = !first_jd_str.empty() ?
                    int(std::stod(first_jd_str)) :
                    (!first_year_str.empty() ?
                        int(std::stod(first_year_str) * days_in_a_year + julian_day_on_m1_1_1) :
                        params.min_year);

                const std::string& last_jd_str = has_last_julian_day ? table.get(row_index, last_julian_day_hash) : "";
                const std::string& last_year_str = has_last_year ? table.get(row_index, last_year_hash) : "";
                int last_jd = !last_jd_str.empty() ?
                    int(std::stod(last_jd_str)) :
                    (!last_year_str.empty() ?
                        int(std::stod(last_year_str) * days_in_a_year + julian_day_on_m1_1_1) :
                        params.max_year);

                const std::string& min_size_str = has_min_size ? table.get(row_index, min_size_hash) : "";
                const std::string& max_size_str = has_max_size ? table.get(row_index, max_size_hash) : "";
                const std::string& texture_str = has_texture ? table.get(row_index, texture_hash) : "";

                // 格納
                location_point_list.emplace_back(
                    feature_name,
                    paxs::EquirectangularDeg(
                        paxs::Vector2<double>(
                            point_longitude,
                            point_latitude)).toMercatorDeg(),
                    x_size_str.empty() ? 1 : static_cast<std::uint_least16_t>(std::stod(x_size_str)),
                    y_size_str.empty() ? 1 : static_cast<std::uint_least16_t>(std::stod(y_size_str)),
                    overall_length_str.empty() ? 10.0 : std::stod(overall_length_str),
                    min_size_str.empty() ? params.min_view : std::stod(min_size_str),
                    max_size_str.empty() ? params.max_view : std::stod(max_size_str),
                    first_jd,
                    last_jd,
                    lpe,
                    texture_str.empty() ? params.texture_hash : MurMur3::calcHash(texture_str.size(), texture_str.c_str()),
                    params.zoom
                );
            });

            if (location_point_list.size() == 0) {
                PAXS_WARNING("No valid geographic features loaded from file: " + params.file_path);
                return LocationPointList{};
            }

            return LocationPointList(location_point_list,
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(start_longitude, start_latitude)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(end_longitude, end_latitude)).toMercatorDeg(),
                params.min_view, params.max_view, params.min_year, params.max_year,
                lpe, params.texture_hash);
        }

    };

}

#endif // !PAX_SAPIENTICA_MAP_REPOSITORY_GEOGRAPHIC_FEATURE_REPOSITORY_HPP
