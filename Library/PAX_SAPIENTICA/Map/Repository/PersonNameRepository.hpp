/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_REPOSITORY_PERSON_NAME_REPOSITORY_HPP
#define PAX_SAPIENTICA_MAP_REPOSITORY_PERSON_NAME_REPOSITORY_HPP

#include <algorithm>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/IO/Data/TsvTable.hpp>
#include <PAX_SAPIENTICA/Map/Repository/FeatureListLoader.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {
    // 人物の位置情報
    struct PersonLocationPoint {
        constexpr explicit PersonLocationPoint() = default;
        explicit PersonLocationPoint(
            const paxs::UnorderedMap<std::uint_least32_t, std::string>& person_name_,  // 人物名
            const paxs::MercatorDeg& start_coordinate_,  // 経緯度
            const paxs::MercatorDeg& end_coordinate_,  // 経緯度
            const double overall_length_,  // 全長
            const double min_zoom_level_,  // 表示する最小ズームレベル
            const double max_zoom_level_,  // 表示する最大ズームレベル
            const double min_year_,  // 可視化する時代（古い年～）
            const double max_year_,  // 可視化する時代（～新しい年）
            const std::uint_least32_t feature_type_hash_,  // 地物の種別を識別するハッシュ値
            const std::uint_least32_t texture_key_ // テクスチャキー
        ) noexcept
            : person_name(person_name_), start_coordinate(start_coordinate_), end_coordinate(end_coordinate_), overall_length(overall_length_),
            min_zoom_level(min_zoom_level_), max_zoom_level(max_zoom_level_), min_year(min_year_), max_year(max_year_), feature_type_hash(feature_type_hash_), texture_key(texture_key_) {}

        paxs::UnorderedMap<std::uint_least32_t, std::string> person_name{}; // 人物名
        paxs::MercatorDeg start_coordinate{}; // 経緯度
        paxs::MercatorDeg end_coordinate{}; // 経緯度
        double overall_length = 10; // 全長
        double min_zoom_level = 0.0, max_zoom_level = 9999.0; // 表示するズームレベル範囲
        double min_year = -99999999, max_year = 99999999; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t feature_type_hash = MurMur3::calcHash("place_name"); // 地物の種別を識別するハッシュ値（例: "person"）
        std::uint_least32_t texture_key = 0; // テクスチャキー
    };

    // 人物の一覧
    struct PersonLocationList {
        std::vector<PersonLocationPoint> person_location_list{}; // 人物の一覧

        paxs::MercatorDeg start_start_coordinate{}; // 経緯度
        paxs::MercatorDeg start_end_coordinate{}; // 経緯度
        paxs::MercatorDeg end_start_coordinate{}; // 経緯度
        paxs::MercatorDeg end_end_coordinate{}; // 経緯度
        double min_zoom_level = 0.0, max_zoom_level = 9999.0; // 表示するズームレベル範囲
        double min_year = -99999999, max_year = 99999999; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t feature_type_hash = MurMur3::calcHash("place_name"); // 地物の種別を識別するハッシュ値（例: "person"）
        std::uint_least32_t texture_key = 0; // テクスチャキー

        constexpr explicit PersonLocationList() = default;
        explicit PersonLocationList(
            const std::vector<PersonLocationPoint>& location_point_list_,  // 人物
            paxs::MercatorDeg start_start_coordinate_, // 経緯度
            paxs::MercatorDeg start_end_coordinate_, // 経緯度
            paxs::MercatorDeg end_start_coordinate_, // 経緯度
            paxs::MercatorDeg end_end_coordinate_, // 経緯度
            const double min_zoom_level_,  // 表示する最小ズームレベル
            const double max_zoom_level_,  // 表示する最大ズームレベル
            const double min_year_,  // 可視化する時代（古い年～）
            const double max_year_,  // 可視化する時代（～新しい年）
            const std::uint_least32_t feature_type_hash_,  // 地物の種別を識別するハッシュ値
            const std::uint_least32_t texture_key_ // テクスチャキー
        ) noexcept
            : person_location_list(location_point_list_),
            start_start_coordinate(start_start_coordinate_), start_end_coordinate(start_end_coordinate_),
            end_start_coordinate(end_start_coordinate_), end_end_coordinate(end_end_coordinate_),
            min_zoom_level(min_zoom_level_), max_zoom_level(max_zoom_level_), min_year(min_year_), max_year(max_year_), feature_type_hash(feature_type_hash_), texture_key(texture_key_) {}

    };

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

            double start_start_longitude = 180.0; // 始点の経度
            double start_end_longitude = -180.0; // 終点の経度
            double start_start_latitude = 90.0; // 始点の緯度
            double start_end_latitude = -90.0; // 終点の緯度

            double end_start_longitude = 180.0; // 始点の経度
            double end_end_longitude = -180.0; // 終点の経度
            double end_start_latitude = 90.0; // 始点の緯度
            double end_end_latitude = -90.0; // 終点の緯度

            // カラムハッシュキーを作成
            const std::uint_least32_t start_longitude_hash = MurMur3::calcHash("start_longitude");
            const std::uint_least32_t start_latitude_hash = MurMur3::calcHash("start_latitude");
            const std::uint_least32_t end_longitude_hash = MurMur3::calcHash("end_longitude");
            const std::uint_least32_t end_latitude_hash = MurMur3::calcHash("end_latitude");
            const std::uint_least32_t overall_length_hash = MurMur3::calcHash("overall_length");
            const std::uint_least32_t min_size_hash = MurMur3::calcHash("min_size");
            const std::uint_least32_t max_size_hash = MurMur3::calcHash("max_size");
            const std::uint_least32_t first_julian_day_hash = MurMur3::calcHash("first_julian_day");
            const std::uint_least32_t last_julian_day_hash = MurMur3::calcHash("last_julian_day");
            const std::uint_least32_t texture_hash = MurMur3::calcHash("texture");
            const std::uint_least32_t ja_jp_hash = MurMur3::calcHash("ja-JP");
            const std::uint_least32_t en_us_hash = MurMur3::calcHash("en-US");

            // オプショナルカラムの存在確認
            const bool has_overall_length = table.hasColumn(overall_length_hash);
            const bool has_min_size = table.hasColumn(min_size_hash);
            const bool has_max_size = table.hasColumn(max_size_hash);
            const bool has_first_julian_day = table.hasColumn(first_julian_day_hash);
            const bool has_last_julian_day = table.hasColumn(last_julian_day_hash);
            const bool has_texture = table.hasColumn(texture_hash);
            const bool has_ja_jp = table.hasColumn(ja_jp_hash);
            const bool has_en_us = table.hasColumn(en_us_hash);

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

                // 人物名
                paxs::UnorderedMap<std::uint_least32_t, std::string> person_name{};
                if (has_ja_jp) {
                    const std::string& ja_jp_str = table.get(row_index, ja_jp_hash);
                    if (!ja_jp_str.empty()) {
                        person_name.emplace(MurMur3::calcHash("ja-JP"), ja_jp_str);
                    }
                }
                if (has_en_us) {
                    const std::string& en_us_str = table.get(row_index, en_us_hash);
                    if (!en_us_str.empty()) {
                        person_name.emplace(MurMur3::calcHash("en-US"), en_us_str);
                    }
                }

                const std::string& overall_length_str = has_overall_length ? table.get(row_index, overall_length_hash) : "";

                double start_point_longitude = std::stod(start_longitude_str); // 経度
                double start_point_latitude = std::stod(start_latitude_str); // 緯度
                // 経緯度の範囲を求める
                start_start_longitude = (std::min)(start_start_longitude, start_point_longitude);
                start_start_latitude = (std::min)(start_start_latitude, start_point_latitude);
                start_end_longitude = (std::max)(start_end_longitude, start_point_longitude);
                start_end_latitude = (std::max)(start_end_latitude, start_point_latitude);

                double end_point_longitude = std::stod(end_longitude_str); // 経度
                double end_point_latitude = std::stod(end_latitude_str); // 緯度
                // 経緯度の範囲を求める
                end_start_longitude = (std::min)(end_start_longitude, end_point_longitude);
                end_start_latitude = (std::min)(end_start_latitude, end_point_latitude);
                end_end_longitude = (std::max)(end_end_longitude, end_point_longitude);
                end_end_latitude = (std::max)(end_end_latitude, end_point_latitude);

                // オプショナル値の取得
                const std::string& min_size_str = has_min_size ? table.get(row_index, min_size_hash) : "";
                const std::string& max_size_str = has_max_size ? table.get(row_index, max_size_hash) : "";
                const std::string& first_julian_day_str = has_first_julian_day ? table.get(row_index, first_julian_day_hash) : "";
                const std::string& last_julian_day_str = has_last_julian_day ? table.get(row_index, last_julian_day_hash) : "";
                const std::string& texture_str = has_texture ? table.get(row_index, texture_hash) : "";

                // 格納
                person_location_list.emplace_back(
                    person_name,
                    paxs::EquirectangularDeg(
                        paxs::Vector2<double>(
                            start_point_longitude, // 経度
                            start_point_latitude)).toMercatorDeg(), // 緯度
                    paxs::EquirectangularDeg(
                        paxs::Vector2<double>(
                            end_point_longitude, // 経度
                            end_point_latitude)).toMercatorDeg(), // 緯度
                    overall_length_str.empty() ? 1000.0 : std::stod(overall_length_str), // 全長
                    min_size_str.empty() ? params.min_zoom_level : std::stod(min_size_str), // 最小サイズ
                    max_size_str.empty() ? params.max_zoom_level : std::stod(max_size_str), // 最大サイズ
                    first_julian_day_str.empty() ? params.min_year : std::stod(first_julian_day_str), // 最小時代
                    last_julian_day_str.empty() ? params.max_year : std::stod(last_julian_day_str), // 最大時代
                    feature_type_hash,
                    texture_str.empty() ? params.texture_hash : MurMur3::calcHash(texture_str.size(), texture_str.c_str()) // テクスチャの Key
                );
            });

            // 地物を何も読み込んでいない場合は何もしないで終わる
            if (person_location_list.size() == 0) {
                PAXS_WARNING("No valid person locations loaded from file: " + params.file_path);
                return PersonLocationList();
            }

            // 読み込んだファイルを格納する
            return PersonLocationList(person_location_list,
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(start_start_longitude/* 経度 */, start_start_latitude/* 緯度 */)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(start_end_longitude/* 経度 */, start_end_latitude/* 緯度 */)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(end_start_longitude/* 経度 */, end_start_latitude/* 緯度 */)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(end_end_longitude/* 経度 */, end_end_latitude/* 緯度 */)).toMercatorDeg(),
                params.min_zoom_level, params.max_zoom_level, params.min_year, params.max_year,
                feature_type_hash, params.texture_hash);
        }

    };

}

#endif // !PAX_SAPIENTICA_MAP_REPOSITORY_PERSON_NAME_REPOSITORY_HPP
