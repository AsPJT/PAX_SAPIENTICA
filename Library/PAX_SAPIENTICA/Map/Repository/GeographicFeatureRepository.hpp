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
#include <PAX_SAPIENTICA/Map/Repository/LocationDataLoader.hpp>
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

            // 共通ヘルパーを使用
            ColumnHashes hashes;
            ColumnFlags flags;
            flags.setFromTable(table, hashes);
            CoordinateBounds bounds;

            // 1 行ずつ読み込み
            table.forEachRow([&](std::size_t row_index, const std::vector<std::string>& row) {
                auto row_data_opt = LocationDataLoader::loadRowData(table, row_index, hashes, flags, params);
                if (!row_data_opt.has_value()) {
                    return; // 経度・緯度が空の場合はスキップ
                }

                const auto& data = row_data_opt.value();

                // 経緯度の範囲を更新
                bounds.update(data.longitude, data.latitude);

                // LocationPointを構築
                location_point_list.emplace_back(
                    data.names,
                    paxs::EquirectangularDeg(
                        paxs::Vector2<double>(data.longitude, data.latitude)).toMercatorDeg(),
                    data.x_size,
                    data.y_size,
                    data.overall_length,
                    data.min_size,
                    data.max_size,
                    data.first_julian_day,
                    data.last_julian_day,
                    lpe,
                    data.texture_hash,
                    params.zoom
                );
            });

            if (location_point_list.size() == 0) {
                PAXS_WARNING("No valid geographic features loaded from file: " + params.file_path);
                return LocationPointList{};
            }

            return LocationPointList(location_point_list,
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(bounds.start_longitude, bounds.start_latitude)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(bounds.end_longitude, bounds.end_latitude)).toMercatorDeg(),
                params.min_view, params.max_view, params.min_year, params.max_year,
                lpe, params.texture_hash);
        }

    };

}

#endif // !PAX_SAPIENTICA_MAP_REPOSITORY_GEOGRAPHIC_FEATURE_REPOSITORY_HPP
