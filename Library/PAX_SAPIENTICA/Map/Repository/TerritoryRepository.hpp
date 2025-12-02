/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_REPOSITORY_TERRITORY_REPOSITORY_HPP
#define PAX_SAPIENTICA_MAP_REPOSITORY_TERRITORY_REPOSITORY_HPP

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/Core/Type/Range.hpp>
#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/IO/Data/TsvTable.hpp>
#include <PAX_SAPIENTICA/Map/TerritoryLocationPoint.hpp>
#include <PAX_SAPIENTICA/Map/Repository/FeatureListLoader.hpp>
#include <PAX_SAPIENTICA/Map/Repository/LocationDataLoader.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief 領域データの読み込みを担当する構造体 (Infrastructure Layer)
    /// @brief Repository for loading territory data (Infrastructure Layer)
    struct TerritoryRepository {
        /// @brief 領域リストを読み込み、全てのTerritoryLocationDataを返す
        /// @brief Load territory data file list and return all TerritoryLocationData
        static std::vector<std::pair<TerritoryLocationData, TerritoryLocationGroup>> loadTerritoryList() {
            std::vector<std::pair<TerritoryLocationData, TerritoryLocationGroup>> all_territories;

            FeatureListLoader::loadFeatureList("Territories", [&all_territories](const FeatureListParams& params) {
                auto loaded = loadTerritoryFromFile(params);
                for (const auto& territory_data : loaded.territory_list) {
                    all_territories.emplace_back(territory_data, loaded);
                }
                });

            return all_territories;
        }

        /// @brief 個別ファイルから領域データを読み込み
        /// @brief Load territory data from individual file
        static TerritoryLocationGroup loadTerritoryFromFile(const FeatureListParams& params) {
            std::vector<TerritoryLocationData> territory_list{}; // 領域の一覧 / List of territories
            const std::uint_least32_t feature_type_hash = MurMur3::calcHash(params.type.size(), params.type.c_str());

            const std::string territory_key = params.key;
            if (territory_key.empty()) {
                PAXS_WARNING("Territories.tsv key is empty for file: " + params.file_path);
                return TerritoryLocationGroup();
            }

            paxs::TsvTable table(params.file_path);
            if (!table.isSuccessfullyLoaded()) {
                PAXS_WARNING("Failed to load territory file: " + params.file_path);
                return TerritoryLocationGroup();
            }

            // 必須カラムの検証
            if (!table.hasColumn("longitude")) {
                PAXS_ERROR("TerritoryFile is missing required column: longitude");
                return TerritoryLocationGroup();
            }
            if (!table.hasColumn("latitude")) {
                PAXS_ERROR("TerritoryFile is missing required column: latitude");
                return TerritoryLocationGroup();
            }

            // カラムハッシュの設定
            const std::uint_least32_t longitude_hash = MurMur3::calcHash("longitude");
            const std::uint_least32_t latitude_hash = MurMur3::calcHash("latitude");

            // 座標列を読み込み
            std::vector<paxs::WebMercatorDeg> coordinates;

            // 1 行ずつ読み込み
            table.forEachRow([&](std::size_t row_index, const std::vector<std::string>&) {
                const std::string& longitude_str = table.get(row_index, longitude_hash);
                const std::string& latitude_str = table.get(row_index, latitude_hash);

                if (longitude_str.empty() || latitude_str.empty()) {
                    PAXS_WARNING("Skipping row " + std::to_string(row_index) + " in " + params.file_path + ": missing coordinates");
                    return;
                }

                // 経度の変換
                auto lon_opt = StringUtils::toDouble(longitude_str);
                if (!lon_opt) {
                    PAXS_WARNING("Invalid longitude at row " + std::to_string(row_index) + " in " + params.file_path + ": \"" + longitude_str + "\"");
                    return;
                }
                double point_longitude = *lon_opt;

                // 緯度の変換
                auto lat_opt = StringUtils::toDouble(latitude_str);
                if (!lat_opt) {
                    PAXS_WARNING("Invalid latitude at row " + std::to_string(row_index) + " in " + params.file_path + ": \"" + latitude_str + "\"");
                    return;
                }
                double point_latitude = *lat_opt;

                // EPSG:3857(Webメルカトル) 座標に変換して格納
                coordinates.emplace_back(
                    paxs::EPSG4326_WGS84Deg(
                        paxs::Vector2<double>(point_longitude, point_latitude)
                    ).toWebMercatorDeg()
                );
                });

            // 座標を何も読み込んでいない場合は何もしないで終わる
            if (coordinates.empty()) {
                PAXS_WARNING("No valid coordinates loaded from file: " + params.file_path);
                return TerritoryLocationGroup();
            }

            // TerritoryLocationDataを作成
            territory_list.emplace_back(
                territory_key,
                std::move(coordinates),
                params.zoom_range,
                params.year_range,
                feature_type_hash
            );

            // Territories.tsvからcolor文字列とline_widthを取得
            // Get color string and line_width from Territories.tsv
            const std::string territory_color_string = params.color_string;  // colorカラムの文字列を直接使用
            const float territory_line_width = static_cast<float>(params.line_width);  // line_widthカラムを使用

            // 読み込んだファイルを格納する
            return TerritoryLocationGroup(
                territory_list,
                params.zoom_range,
                params.year_range,
                feature_type_hash,
                territory_color_string,
                territory_line_width
            );
        }
    };

}

#endif // !PAX_SAPIENTICA_MAP_REPOSITORY_TERRITORY_REPOSITORY_HPP
