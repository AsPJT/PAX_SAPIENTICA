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
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {
    // 人物の位置情報
    struct PersonLocationPoint {
        constexpr explicit PersonLocationPoint() = default;
        explicit PersonLocationPoint(
            const paxs::UnorderedMap<std::uint_least32_t, std::string>& place_name_,  // 地名
            const paxs::MercatorDeg& start_coordinate_,  // 経緯度
            const paxs::MercatorDeg& end_coordinate_,  // 経緯度
            const double overall_length_,  // 全長
            const double min_view_,  // 可視化する地図の最小範囲
            const double max_view_,  // 可視化する地図の最大範囲
            const double min_year_,  // 可視化する時代（古い年～）
            const double max_year_,  // 可視化する時代（～新しい年）
            const std::uint_least32_t lpe_,  // 対象となる地物の種別
            const std::uint_least32_t place_texture_ // 出典
        ) noexcept
            : place_name(place_name_), start_coordinate(start_coordinate_), end_coordinate(end_coordinate_), overall_length(overall_length_),
            min_view(min_view_), max_view(max_view_), min_year(min_year_), max_year(max_year_), lpe(lpe_), place_texture(place_texture_) {}

        paxs::UnorderedMap<std::uint_least32_t, std::string> place_name{}; // 地名
        paxs::MercatorDeg start_coordinate{}; // 経緯度
        paxs::MercatorDeg end_coordinate{}; // 経緯度
        double overall_length = 10; // 全長
        double min_view = 0.0, max_view = 9999.0; // 可視化する地図の拡大縮小範囲
        double min_year = -99999999, max_year = 99999999; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t lpe = MurMur3::calcHash("place_name"); // 対象となる地物の種別
        std::uint_least32_t place_texture = 0; // 地物の画像
    };

    // 地物の一覧
    struct PersonLocationList {
        std::vector<PersonLocationPoint> person_location_list{}; // 地物の一覧

        paxs::MercatorDeg start_start_coordinate{}; // 経緯度
        paxs::MercatorDeg start_end_coordinate{}; // 経緯度
        paxs::MercatorDeg end_start_coordinate{}; // 経緯度
        paxs::MercatorDeg end_end_coordinate{}; // 経緯度
        double min_view = 0.0, max_view = 9999.0; // 可視化する地図の拡大縮小範囲
        double min_year = -99999999, max_year = 99999999; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t lpe = MurMur3::calcHash("place_name"); // 対象となる地物の種別
        std::uint_least32_t place_texture = 0; // 地物の画像

        constexpr explicit PersonLocationList() = default;
        explicit PersonLocationList(
            const std::vector<PersonLocationPoint>& location_point_list_,  // 地物
            paxs::MercatorDeg start_start_coordinate_, // 経緯度
            paxs::MercatorDeg start_end_coordinate_, // 経緯度
            paxs::MercatorDeg end_start_coordinate_, // 経緯度
            paxs::MercatorDeg end_end_coordinate_, // 経緯度
            const double min_view_,  // 可視化する地図の最小範囲
            const double max_view_,  // 可視化する地図の最大範囲
            const double min_year_,  // 可視化する時代（古い年～）
            const double max_year_,  // 可視化する時代（～新しい年）
            const std::uint_least32_t lpe_,  // 対象となる地物の種別
            const std::uint_least32_t place_texture_ // 出典
        ) noexcept
            : person_location_list(location_point_list_),
            start_start_coordinate(start_start_coordinate_), start_end_coordinate(start_end_coordinate_),
            end_start_coordinate(end_start_coordinate_), end_end_coordinate(end_end_coordinate_),
            min_view(min_view_), max_view(max_view_), min_year(min_year_), max_year(max_year_), lpe(lpe_), place_texture(place_texture_) {}

    };

    /// @brief 人物名のデータ読み込みを担当するクラス (Infrastructure Layer)
    class PersonNameRepository {
    public:
        PersonNameRepository() = default;

        /// @brief 人物名リストを読み込み
        void loadPersonNameList(
            const std::function<void(const std::string&, double, double, int, int,
                                     std::uint_least32_t, std::uint_least32_t)>& inputPlaceFunc
        ) const {
            std::string str = "";
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("PersonNames"),
                [&](const std::string& path_) {str = path_; });
            if (str.size() == 0) {
                PAXS_WARNING("PersonNames configuration path is empty");
                return;
            }

            paxs::TsvTable table(str);
            if (!table.isSuccessfullyLoaded()) {
                PAXS_WARNING("Failed to load person name list: " + str);
                return;
            }

            // 必須カラムの検証
            if (!table.hasColumn("file_path")) {
                PAXS_ERROR("PersonNameList is missing required column: file_path");
                return;
            }

            // カラムハッシュキーを作成
            const std::uint_least32_t file_path_hash = MurMur3::calcHash("file_path");
            const std::uint_least32_t place_type_hash = MurMur3::calcHash("place_type");
            const std::uint_least32_t min_size_hash = MurMur3::calcHash("min_size");
            const std::uint_least32_t max_size_hash = MurMur3::calcHash("max_size");
            const std::uint_least32_t first_julian_day_hash = MurMur3::calcHash("first_julian_day");
            const std::uint_least32_t last_julian_day_hash = MurMur3::calcHash("last_julian_day");
            const std::uint_least32_t texture_hash = MurMur3::calcHash("texture");

            // オプショナルカラムの存在確認
            const bool has_place_type = table.hasColumn(place_type_hash);
            const bool has_min_size = table.hasColumn(min_size_hash);
            const bool has_max_size = table.hasColumn(max_size_hash);
            const bool has_first_julian_day = table.hasColumn(first_julian_day_hash);
            const bool has_last_julian_day = table.hasColumn(last_julian_day_hash);
            const bool has_texture = table.hasColumn(texture_hash);

            // 1 行ずつ読み込み
            table.forEachRow([&](std::size_t row_index, const std::vector<std::string>& row) {
                const std::string& file_path_str = table.get(row_index, file_path_hash);
                if (file_path_str.empty()) {
                    PAXS_WARNING("Skipping row " + std::to_string(row_index) + " in " + str + ": file_path is empty");
                    return;
                }

                // 対象となる地物の種別
                const std::string& place_type_str = has_place_type ? table.get(row_index, place_type_hash) : "";
                const std::uint_least32_t type = place_type_str.empty() ?
                    MurMur3::calcHash("place_name") :
                    MurMur3::calcHash(place_type_str.size(), place_type_str.c_str());

                // 可視化する地図の最小範囲
                const std::string& min_size_str = has_min_size ? table.get(row_index, min_size_hash) : "";
                const double min_view = min_size_str.empty() ? 0.0 : std::stod(min_size_str);

                // 可視化する地図の最大範囲
                const std::string& max_size_str = has_max_size ? table.get(row_index, max_size_hash) : "";
                const double max_view = max_size_str.empty() ? 99999999.0 : std::stod(max_size_str);

                // 可視化する時代（古い年～）
                const std::string& first_julian_day_str = has_first_julian_day ? table.get(row_index, first_julian_day_hash) : "";
                const int min_year = first_julian_day_str.empty() ? -99999999 : std::stoi(first_julian_day_str);

                // 可視化する時代（～新しい年）
                const std::string& last_julian_day_str = has_last_julian_day ? table.get(row_index, last_julian_day_hash) : "";
                const int max_year = last_julian_day_str.empty() ? 99999999 : std::stoi(last_julian_day_str);

                // 画像
                const std::string& texture_str = has_texture ? table.get(row_index, texture_hash) : "";
                const std::uint_least32_t place_texture_hash = texture_str.empty() ?
                    0 : MurMur3::calcHash(texture_str.size(), texture_str.c_str());

                // 地物を追加 (callback)
                inputPlaceFunc(file_path_str, min_view, max_view, min_year, max_year, type, place_texture_hash);
            });
        }

        /// @brief 個別ファイルから人物データを読み込み
        PersonLocationList loadPersonFromFile(
            const std::string& str_,
            const double min_view_,
            const double max_view_,
            const int min_year_,
            const int max_year_,
            const std::uint_least32_t lpe_,
            const std::uint_least32_t place_texture_
        ) const {
            std::vector<PersonLocationPoint> person_location_list{}; // 地物の一覧

            paxs::TsvTable table(str_);
            if (!table.isSuccessfullyLoaded()) {
                PAXS_WARNING("Failed to load person file: " + str_);
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
                    PAXS_WARNING("Skipping row " + std::to_string(row_index) + " in " + str_ + ": missing coordinates");
                    return;
                }

                // 地名
                paxs::UnorderedMap<std::uint_least32_t, std::string> place_name{};
                if (has_ja_jp) {
                    const std::string& ja_jp_str = table.get(row_index, ja_jp_hash);
                    if (!ja_jp_str.empty()) {
                        place_name.emplace(MurMur3::calcHash("ja-JP"), ja_jp_str);
                    }
                }
                if (has_en_us) {
                    const std::string& en_us_str = table.get(row_index, en_us_hash);
                    if (!en_us_str.empty()) {
                        place_name.emplace(MurMur3::calcHash("en-US"), en_us_str);
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
                    place_name,
                    paxs::EquirectangularDeg(
                        paxs::Vector2<double>(
                            start_point_longitude, // 経度
                            start_point_latitude)).toMercatorDeg(), // 緯度
                    paxs::EquirectangularDeg(
                        paxs::Vector2<double>(
                            end_point_longitude, // 経度
                            end_point_latitude)).toMercatorDeg(), // 緯度
                    overall_length_str.empty() ? 1000.0 : std::stod(overall_length_str), // 全長
                    min_size_str.empty() ? min_view_ : std::stod(min_size_str), // 最小サイズ
                    max_size_str.empty() ? max_view_ : std::stod(max_size_str), // 最大サイズ
                    first_julian_day_str.empty() ? min_year_ : std::stod(first_julian_day_str), // 最小時代
                    last_julian_day_str.empty() ? max_year_ : std::stod(last_julian_day_str), // 最大時代
                    lpe_,
                    texture_str.empty() ? place_texture_ : MurMur3::calcHash(texture_str.size(), texture_str.c_str()) // テクスチャの Key
                );
            });

            // 地物を何も読み込んでいない場合は何もしないで終わる
            if (person_location_list.size() == 0) {
                PAXS_WARNING("No valid person locations loaded from file: " + str_);
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
                min_view_, max_view_, min_year_, max_year_
                , lpe_, place_texture_);
        }

    };

}

#endif // !PAX_SAPIENTICA_MAP_REPOSITORY_PERSON_NAME_REPOSITORY_HPP
