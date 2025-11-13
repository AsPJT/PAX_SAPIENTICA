/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_REPOSITORY_PLACE_NAME_REPOSITORY_HPP
#define PAX_SAPIENTICA_MAP_REPOSITORY_PLACE_NAME_REPOSITORY_HPP

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/IO/Data/TsvTable.hpp>
#include <PAX_SAPIENTICA/Map/LocationPoint.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief 地名のデータ読み込みを担当するクラス (Infrastructure Layer)
    class PlaceNameRepository {
    public:
        PlaceNameRepository() = default;

        /// @brief 地名データファイルのリストを読み込み、各ファイルをinputPlace経由で処理
        void loadPlaceNameList(
            const std::function<void(const std::string&, double, double, int, int, std::uint_least32_t, std::uint_least32_t, double)>& inputPlaceFunc
        ) const {
            const std::string setting_path = AppConfig::getInstance()->getSettingPath(MurMur3::calcHash("PlaceNames"));
            if (setting_path.size() == 0) {
                PAXS_WARNING("PlaceNames configuration path is empty");
                return;
            }

            paxs::TsvTable table(setting_path);
            if (!table.isSuccessfullyLoaded()) {
                PAXS_WARNING("Failed to load place name list: " + setting_path);
                return;
            }

            // 必須カラムの存在確認
            if (!table.hasColumn("file_path")) {
                PAXS_ERROR("PlaceNameList is missing required column: file_path");
                return;
            }

            // カラムハッシュキーを取得
            const std::uint_least32_t file_path_hash = MurMur3::calcHash("file_path");
            const std::uint_least32_t place_type_hash = MurMur3::calcHash("place_type");
            const std::uint_least32_t min_size_hash = MurMur3::calcHash("min_size");
            const std::uint_least32_t max_size_hash = MurMur3::calcHash("max_size");
            const std::uint_least32_t first_julian_day_hash = MurMur3::calcHash("first_julian_day");
            const std::uint_least32_t last_julian_day_hash = MurMur3::calcHash("last_julian_day");
            const std::uint_least32_t texture_hash = MurMur3::calcHash("texture");
            const std::uint_least32_t zoom_hash = MurMur3::calcHash("zoom");
            const std::uint_least32_t first_year_hash = MurMur3::calcHash("first_year");
            const std::uint_least32_t last_year_hash = MurMur3::calcHash("last_year");

            const bool has_place_type = table.hasColumn(place_type_hash);
            const bool has_min_size = table.hasColumn(min_size_hash);
            const bool has_max_size = table.hasColumn(max_size_hash);
            const bool has_first_julian_day = table.hasColumn(first_julian_day_hash);
            const bool has_last_julian_day = table.hasColumn(last_julian_day_hash);
            const bool has_texture = table.hasColumn(texture_hash);
            const bool has_zoom = table.hasColumn(zoom_hash);
            const bool has_first_year = table.hasColumn(first_year_hash);
            const bool has_last_year = table.hasColumn(last_year_hash);

            // 1 行ずつ読み込み
            table.forEachRow([&](std::size_t row_index, const std::vector<std::string>& row) {
                const std::string& file_path_str = table.get(row_index, file_path_hash);

                // パスが空の場合は読み込まない
                if (file_path_str.empty()) {
                    PAXS_WARNING("Skipping row " + std::to_string(row_index) + " in " + setting_path + ": file_path is empty");
                    return;
                }

                // 対象となる地物の種別
                const std::string& place_type_str = has_place_type ? table.get(row_index, place_type_hash) : "";
                const std::uint_least32_t type = place_type_str.empty() ?
                    MurMur3::calcHash("place_name") :
                    MurMur3::calcHash(place_type_str.size(), place_type_str.c_str());

                // 可視化する地図の最小範囲
                const std::string& min_size_str = has_min_size ? table.get(row_index, min_size_hash) : "";
                const double min_view = min_size_str.empty() ? 0 : std::stod(min_size_str);

                // 可視化する地図の最大範囲
                const std::string& max_size_str = has_max_size ? table.get(row_index, max_size_hash) : "";
                const double max_view = max_size_str.empty() ? 99999999.0 : std::stod(max_size_str);

                // 可視化する時代（古い年～）
                const std::string& first_jd_str = has_first_julian_day ? table.get(row_index, first_julian_day_hash) : "";
                const std::string& first_year_str = has_first_year ? table.get(row_index, first_year_hash) : "";
                const int min_year = !first_jd_str.empty() ?
                    std::stoi(first_jd_str) :
                    (!first_year_str.empty() ?
                        int(std::stod(first_year_str) * days_in_a_year + julian_day_on_m1_1_1) :
                        -99999999);

                // 可視化する時代（～新しい年）
                const std::string& last_jd_str = has_last_julian_day ? table.get(row_index, last_julian_day_hash) : "";
                const std::string& last_year_str = has_last_year ? table.get(row_index, last_year_hash) : "";
                const int max_year = !last_jd_str.empty() ?
                    std::stoi(last_jd_str) :
                    (!last_year_str.empty() ?
                        int(std::stod(last_year_str) * days_in_a_year + julian_day_on_m1_1_1) :
                        99999999);

                // 画像
                const std::string& texture_str = has_texture ? table.get(row_index, texture_hash) : "";
                const std::uint_least32_t place_texture_hash = texture_str.empty() ? 0 :
                    MurMur3::calcHash(texture_str.size(), texture_str.c_str());

                // ズームサイズ
                const std::string& zoom_str = has_zoom ? table.get(row_index, zoom_hash) : "";
                const double zoom = zoom_str.empty() ? 1.0 : std::stod(zoom_str);

                // 地物を追加（コールバック経由）
                inputPlaceFunc(file_path_str, min_view, max_view, min_year, max_year, type, place_texture_hash, zoom);
            });
        }

        /// @brief 個別の地名ファイルを読み込んでLocationPointListを返す
        LocationPointList loadPlaceFromFile(
            const std::string& str_,
            const double min_view_,
            const double max_view_,
            const int min_year_,
            const int max_year_,
            const std::uint_least32_t lpe_,
            const std::uint_least32_t place_texture_,
            const double zoom_
        ) const {
            std::vector<LocationPoint> location_point_list{}; // 地物の一覧

            paxs::TsvTable table(str_);
            if (!table.isSuccessfullyLoaded()) {
                PAXS_WARNING("Failed to load place file: " + str_);
                return LocationPointList{};
            }

            // 必須カラムの存在確認
            if (!table.hasColumn("longitude")) {
                PAXS_ERROR("PlaceFile is missing required column: longitude");
                return LocationPointList{};
            }
            if (!table.hasColumn("latitude")) {
                PAXS_ERROR("PlaceFile is missing required column: latitude");
                return LocationPointList{};
            }

            double start_longitude = 180.0; // 始点の経度
            double end_longitude = -180.0; // 終点の経度
            double start_latitude = 90.0; // 始点の緯度
            double end_latitude = -90.0; // 終点の緯度

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

            // 1 行ずつ読み込み
            table.forEachRow([&](std::size_t row_index, const std::vector<std::string>& row) {
                const std::string& longitude_str = table.get(row_index, longitude_hash);
                const std::string& latitude_str = table.get(row_index, latitude_hash);

                // 経度・緯度の文字列が空の場合は読み込まない
                if (longitude_str.empty() || latitude_str.empty()) {
                    // TODO: データを整備してコメントアウトを解除する
                    // PAXS_WARNING("Skipping row " + std::to_string(row_index) + " in " + str_ + ": missing longitude or latitude");
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
                const std::string& x_size_str = has_x_size ? table.get(row_index, x_size_hash) : "";
                const std::string& y_size_str = has_y_size ? table.get(row_index, y_size_hash) : "";

                double point_longitude = std::stod(longitude_str); // 経度
                double point_latitude = std::stod(latitude_str); // 緯度
                // 経緯度の範囲を求める
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
                        min_year_);

                const std::string& last_jd_str = has_last_julian_day ? table.get(row_index, last_julian_day_hash) : "";
                const std::string& last_year_str = has_last_year ? table.get(row_index, last_year_hash) : "";
                int last_jd = !last_jd_str.empty() ?
                    int(std::stod(last_jd_str)) :
                    (!last_year_str.empty() ?
                        int(std::stod(last_year_str) * days_in_a_year + julian_day_on_m1_1_1) :
                        max_year_);

                const std::string& min_size_str = has_min_size ? table.get(row_index, min_size_hash) : "";
                const std::string& max_size_str = has_max_size ? table.get(row_index, max_size_hash) : "";
                const std::string& texture_str = has_texture ? table.get(row_index, texture_hash) : "";

                // 格納
                location_point_list.emplace_back(
                    place_name,
                    paxs::EquirectangularDeg(
                        paxs::Vector2<double>(
                            point_longitude, // 経度
                            point_latitude)).toMercatorDeg(), // 緯度
                    x_size_str.empty() ? 1 : static_cast<std::uint_least16_t>(std::stod(x_size_str)), // 横の枚数
                    y_size_str.empty() ? 1 : static_cast<std::uint_least16_t>(std::stod(y_size_str)), // 縦の枚数
                    overall_length_str.empty() ? 10.0 : std::stod(overall_length_str), // 全長
                    min_size_str.empty() ? min_view_ : std::stod(min_size_str), // 最小サイズ
                    max_size_str.empty() ? max_view_ : std::stod(max_size_str), // 最大サイズ
                    first_jd, // 最小時代
                    last_jd, // 最大時代
                    lpe_,
                    texture_str.empty() ? place_texture_ : MurMur3::calcHash(texture_str.size(), texture_str.c_str()), // テクスチャの Key
                    zoom_
                );
            });
            // 地物を何も読み込んでいない場合は空のLocationPointListを返す
            if (location_point_list.size() == 0) {
                PAXS_WARNING("No valid location points loaded from file: " + str_);
                return LocationPointList{};
            }

            // LocationPointListを構築して返す
            return LocationPointList(location_point_list,
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(start_longitude/* 経度 */, start_latitude/* 緯度 */)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(end_longitude/* 経度 */, end_latitude/* 緯度 */)).toMercatorDeg(),
                min_view_, max_view_, min_year_, max_year_
                , lpe_, place_texture_);
        }

    };

}

#endif // !PAX_SAPIENTICA_MAP_REPOSITORY_PLACE_NAME_REPOSITORY_HPP
