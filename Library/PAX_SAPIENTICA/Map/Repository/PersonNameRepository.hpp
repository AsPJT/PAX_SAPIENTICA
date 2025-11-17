/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_REPOSITORY_PERSON_NAME_REPOSITORY_HPP
#define PAX_SAPIENTICA_MAP_REPOSITORY_PERSON_NAME_REPOSITORY_HPP

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/Core/Type/Range.hpp>
#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Core/Utility/StringUtils.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/IO/Data/TsvTable.hpp>
#include <PAX_SAPIENTICA/Map/PersonLocationPoint.hpp>
#include <PAX_SAPIENTICA/Map/Repository/FeatureListLoader.hpp>
#include <PAX_SAPIENTICA/Map/Repository/LocationDataLoader.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief 人物名のデータ読み込みを担当する構造体 (Infrastructure Layer)
    struct PersonNameRepository {
        /// @brief 人物名リストを読み込み、全てのPersonLocationPointを返す
        /// @brief Load person name data file list and return all PersonLocationPoints
        static std::vector<std::pair<PersonLocationPoint, PersonLocationList>> loadPersonNameList() {
            std::vector<std::pair<PersonLocationPoint, PersonLocationList>> all_persons;

            FeatureListLoader::loadFeatureList("PersonNames", [&all_persons](const FeatureListParams& params) {
                auto loaded = loadPersonFromFile(params);
                for (const auto& person_data : loaded.person_location_list) {
                    all_persons.emplace_back(person_data, loaded);
                }
            });

            return all_persons;
        }

        /// @brief 個別ファイルから人物データを読み込み
        static PersonLocationList loadPersonFromFile(const FeatureListParams& params) {
            std::vector<PersonLocationPoint> person_location_list{}; // 人物の一覧
            const std::uint_least32_t feature_type_hash = MurMur3::calcHash(params.type.size(), params.type.c_str());

            // ファイル名からキーを抽出（拡張子を除く）
            std::string person_key;
            {
                const std::size_t last_slash = params.file_path.find_last_of("/\\");
                const std::size_t last_dot = params.file_path.find_last_of('.');
                const std::size_t start_pos = (last_slash == std::string::npos) ? 0 : last_slash + 1;
                const std::size_t end_pos = (last_dot == std::string::npos || last_dot < start_pos) ? params.file_path.length() : last_dot;
                person_key = params.file_path.substr(start_pos, end_pos - start_pos);
            }

            paxs::TsvTable table(params.file_path);
            if (!table.isSuccessfullyLoaded()) {
                PAXS_WARNING("Failed to load person file: " + params.file_path);
                return PersonLocationList();
            }

            // 必須カラムの検証
            if (!table.hasColumn("start_longitude")) {
                PAXS_ERROR("PersonFile is missing required column: start_longitude");
                return PersonLocationList();
            }
            if (!table.hasColumn("start_latitude")) {
                PAXS_ERROR("PersonFile is missing required column: start_latitude");
                return PersonLocationList();
            }
            if (!table.hasColumn("end_longitude")) {
                PAXS_ERROR("PersonFile is missing required column: end_longitude");
                return PersonLocationList();
            }
            if (!table.hasColumn("end_latitude")) {
                PAXS_ERROR("PersonFile is missing required column: end_latitude");
                return PersonLocationList();
            }


            // カラムハッシュとフラグの設定
            ColumnHashes hashes;
            ColumnFlags flags;

            // Person固有のカラムハッシュを追加
            const std::uint_least32_t start_longitude_hash = MurMur3::calcHash("start_longitude");
            const std::uint_least32_t start_latitude_hash = MurMur3::calcHash("start_latitude");
            const std::uint_least32_t end_longitude_hash = MurMur3::calcHash("end_longitude");
            const std::uint_least32_t end_latitude_hash = MurMur3::calcHash("end_latitude");

            // 共通カラムの存在確認
            flags.setFromTable(table, hashes);

            // 1 行ずつ読み込み
            table.forEachRow([&](std::size_t row_index, const std::vector<std::string>& row) {
                const std::string& start_longitude_str = table.get(row_index, start_longitude_hash);
                const std::string& start_latitude_str = table.get(row_index, start_latitude_hash);
                const std::string& end_longitude_str = table.get(row_index, end_longitude_hash);
                const std::string& end_latitude_str = table.get(row_index, end_latitude_hash);

                if (start_longitude_str.empty() || start_latitude_str.empty() ||
                    end_longitude_str.empty() || end_latitude_str.empty()) {
                    PAXS_WARNING("Skipping row " + std::to_string(row_index) + " in " + params.file_path + ": missing coordinates");
                    return;
                }

                // 人物名（多言語対応）
                paxs::UnorderedMap<std::uint_least32_t, std::string> names =
                    LocationDataLoader::loadNames(table, row_index, hashes, flags);

                // 経度の変換
                auto start_lon_opt = StringUtils::toDouble(start_longitude_str);
                if (!start_lon_opt) {
                    PAXS_WARNING("Invalid start_longitude at row " + std::to_string(row_index) + " in " + params.file_path + ": \"" + start_longitude_str + "\"");
                    return;
                }
                double start_point_longitude = *start_lon_opt; // 経度

                // 緯度の変換
                auto start_lat_opt = StringUtils::toDouble(start_latitude_str);
                if (!start_lat_opt) {
                    PAXS_WARNING("Invalid start_latitude at row " + std::to_string(row_index) + " in " + params.file_path + ": \"" + start_latitude_str + "\"");
                    return;
                }
                double start_point_latitude = *start_lat_opt; // 緯度

                // 終点経度の変換
                auto end_lon_opt = StringUtils::toDouble(end_longitude_str);
                if (!end_lon_opt) {
                    PAXS_WARNING("Invalid end_longitude at row " + std::to_string(row_index) + " in " + params.file_path + ": \"" + end_longitude_str + "\"");
                    return;
                }
                double end_point_longitude = *end_lon_opt; // 経度

                // 終点緯度の変換
                auto end_lat_opt = StringUtils::toDouble(end_latitude_str);
                if (!end_lat_opt) {
                    PAXS_WARNING("Invalid end_latitude at row " + std::to_string(row_index) + " in " + params.file_path + ": \"" + end_latitude_str + "\"");
                    return;
                }
                double end_point_latitude = *end_lat_opt; // 緯度

                // オプショナル値の取得
                const std::string& min_size_str = flags.has_min_size ? table.get(row_index, hashes.min_size) : "";
                const std::string& max_size_str = flags.has_max_size ? table.get(row_index, hashes.max_size) : "";
                const std::string& first_julian_day_str = flags.has_first_julian_day ? table.get(row_index, hashes.first_julian_day) : "";
                const std::string& last_julian_day_str = flags.has_last_julian_day ? table.get(row_index, hashes.last_julian_day) : "";
                const std::string& texture_str = flags.has_texture ? table.get(row_index, hashes.texture) : "";

                // 格納
                person_location_list.emplace_back(
                    person_key,
                    names,
                    paxs::EquirectangularDeg(
                        paxs::Vector2<double>(
                            start_point_longitude, // 経度
                            start_point_latitude)).toMercatorDeg(), // 緯度
                    paxs::EquirectangularDeg(
                        paxs::Vector2<double>(
                            end_point_longitude, // 経度
                            end_point_latitude)).toMercatorDeg(), // 緯度
                    Range<double>(
                        StringUtils::safeStod(min_size_str, params.zoom_range.min, true),
                        StringUtils::safeStod(max_size_str, params.zoom_range.max, true)
                    ), // ズームレベル範囲
                    Range<double>(
                        StringUtils::safeStod(first_julian_day_str, params.year_range.min, true),
                        StringUtils::safeStod(last_julian_day_str, params.year_range.max, true)
                    ), // 時代範囲
                    feature_type_hash,
                    texture_str.empty() ? params.texture_hash : MurMur3::calcHash(texture_str.size(), texture_str.c_str()) // テクスチャの Key
                );
            });

            // 人物を何も読み込んでいない場合は何もしないで終わる
            if (person_location_list.size() == 0) {
                PAXS_WARNING("No valid person locations loaded from file: " + params.file_path);
                return PersonLocationList();
            }

            // 読み込んだファイルを格納する
            return PersonLocationList(
                person_location_list,
                params.zoom_range,
                params.year_range,
                feature_type_hash,
                params.texture_hash
            );
        }

    };

}

#endif // !PAX_SAPIENTICA_MAP_REPOSITORY_PERSON_NAME_REPOSITORY_HPP
