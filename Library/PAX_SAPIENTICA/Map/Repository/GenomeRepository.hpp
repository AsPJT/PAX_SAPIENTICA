/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_REPOSITORY_GENOME_REPOSITORY_HPP
#define PAX_SAPIENTICA_MAP_REPOSITORY_GENOME_REPOSITORY_HPP

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/IO/Data/TsvTable.hpp>
#include <PAX_SAPIENTICA/Map/LocationPoint.hpp>
#include <PAX_SAPIENTICA/Map/Repository/FeatureListLoader.hpp>
#include <PAX_SAPIENTICA/Map/Repository/LocationDataLoader.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief ゲノムデータの読み込みを担当する構造体 (Infrastructure Layer)
    /// @brief Genome data loading repository (Infrastructure Layer)
    struct GenomeRepository {
        /// @brief ゲノムデータファイルのリストを読み込み、全てのLocationPointを返す
        /// @brief Load genome data file list and return all LocationPoints
        static std::vector<LocationPoint> loadGenomeList() {
            std::vector<LocationPoint> all_points;

            FeatureListLoader::loadFeatureList("Genomes", [&all_points](const FeatureListParams& params) {
                auto loaded = loadGenomeFromFile(params);
                all_points.insert(all_points.end(),
                    std::make_move_iterator(loaded.location_point_list.begin()),
                    std::make_move_iterator(loaded.location_point_list.end()));
            });

            return all_points;
        }

        /// @brief 個別のゲノムファイルを読み込んでLocationPointListを返す
        /// @brief Load individual genome file and return LocationPointList
        static LocationPointList loadGenomeFromFile(const FeatureListParams& params) {
            std::vector<LocationPoint> location_point_list{};
            const std::uint_least32_t feature_type_hash = MurMur3::calcHash(params.type.size(), params.type.c_str());

            paxs::TsvTable table(params.file_path);
            if (!table.isSuccessfullyLoaded()) {
                PAXS_WARNING("Failed to load genome file: " + params.file_path);
                return LocationPointList{};
            }

            // 必須カラムの存在確認
            if (!table.hasColumn("longitude")) {
                PAXS_ERROR("GenomeFile is missing required column: longitude");
                return LocationPointList{};
            }
            if (!table.hasColumn("latitude")) {
                PAXS_ERROR("GenomeFile is missing required column: latitude");
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
                    PAXS_WARNING("Skipping row " + std::to_string(row_index) + " in " + params.file_path + ": missing coordinates");
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
                    feature_type_hash,
                    data.texture_hash,
                    params.zoom,
                    data.extra_data
                );
            });

            // ゲノムデータを何も読み込んでいない場合は空のLocationPointListを返す
            if (location_point_list.size() == 0) {
                PAXS_WARNING("No valid genome points loaded from file: " + params.file_path);
                return LocationPointList{};
            }

            // LocationPointListを構築して返す
            return LocationPointList(location_point_list,
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(bounds.start_longitude, bounds.start_latitude)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(bounds.end_longitude, bounds.end_latitude)).toMercatorDeg(),
                params.min_zoom_level, params.max_zoom_level, params.min_year, params.max_year,
                feature_type_hash, params.texture_hash);
        }

    };

}

#endif // !PAX_SAPIENTICA_MAP_REPOSITORY_GENOME_REPOSITORY_HPP
