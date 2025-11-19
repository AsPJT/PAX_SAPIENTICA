/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_REPOSITORY_FEATURE_LIST_LOADER_HPP
#define PAX_SAPIENTICA_MAP_REPOSITORY_FEATURE_LIST_LOADER_HPP

#include <cstdint>
#include <functional>
#include <string>

#include <PAX_SAPIENTICA/Calendar/JulianDayNumber.hpp>
#include <PAX_SAPIENTICA/Core/Type/Range.hpp>
#include <PAX_SAPIENTICA/Core/Utility/StringUtils.hpp>
#include <PAX_SAPIENTICA/IO/Data/TsvTable.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief 地物リスト読み込みの共通パラメータ
    /// @brief Common parameters for feature list loading
    struct FeatureListParams {
        std::string key;        // 地物の一意キー（TSVのkeyカラムから）
        std::string file_path;
        std::string type;
        Range<double> zoom_range;
        Range<double> year_range;
        std::uint_least32_t texture_hash;
        double zoom;
    };

    /// @brief 地物リストの共通読み込みロジック
    /// @brief Common loading logic for feature lists
    struct FeatureListLoader {
        /// @brief TSVから地物リストを読み込み
        /// @brief Load feature list from TSV
        /// @param config_key 設定ファイルのキー（"Genomes", "PlaceNames", "PersonNames"など）
        /// @param callback 各行を処理するコールバック関数
        static void loadFeatureList(
            const std::string& config_key,
            const std::function<void(const FeatureListParams&)>& callback
        ) {
            const std::string setting_path = AppConfig::getInstance().getSettingPath(MurMur3::calcHash(config_key.c_str()));
            if (setting_path.empty()) {
                PAXS_WARNING(config_key + " configuration path is empty");
                return;
            }

            paxs::TsvTable table(setting_path);
            if (!table.isSuccessfullyLoaded()) {
                PAXS_WARNING("Failed to load " + config_key + " list: " + setting_path);
                return;
            }

            // 必須カラムの存在確認
            if (!table.hasColumn("file_path")) {
                PAXS_ERROR(config_key + " list is missing required column: file_path");
                return;
            }

            // カラムハッシュキーを取得
            const std::uint_least32_t key_hash = MurMur3::calcHash("key");
            const std::uint_least32_t file_path_hash = MurMur3::calcHash("file_path");
            const std::uint_least32_t type_hash = MurMur3::calcHash("type");
            const std::uint_least32_t min_size_hash = MurMur3::calcHash("min_size");
            const std::uint_least32_t max_size_hash = MurMur3::calcHash("max_size");
            const std::uint_least32_t first_julian_day_hash = MurMur3::calcHash("first_julian_day");
            const std::uint_least32_t last_julian_day_hash = MurMur3::calcHash("last_julian_day");
            const std::uint_least32_t texture_hash = MurMur3::calcHash("texture");
            const std::uint_least32_t zoom_hash = MurMur3::calcHash("zoom");
            const std::uint_least32_t first_year_hash = MurMur3::calcHash("first_year");
            const std::uint_least32_t last_year_hash = MurMur3::calcHash("last_year");

            const bool has_key = table.hasColumn(key_hash);
            const bool has_type = table.hasColumn(type_hash);
            const bool has_min_size = table.hasColumn(min_size_hash);
            const bool has_max_size = table.hasColumn(max_size_hash);
            const bool has_first_julian_day = table.hasColumn(first_julian_day_hash);
            const bool has_last_julian_day = table.hasColumn(last_julian_day_hash);
            const bool has_texture = table.hasColumn(texture_hash);
            const bool has_zoom = table.hasColumn(zoom_hash);
            const bool has_first_year = table.hasColumn(first_year_hash);
            const bool has_last_year = table.hasColumn(last_year_hash);

            // 定数
            constexpr double days_in_a_year = 365.24;
            constexpr double julian_day_on_m1_1_1 = 1721423.5;

            // 1 行ずつ読み込み
            table.forEachRow([&](std::size_t row_index, const std::vector<std::string>& row) {
                (void)row;
                const std::string key_str = has_key ? table.get(row_index, key_hash) : "";

                const std::string& file_path_str = table.get(row_index, file_path_hash);

                // パスが空の場合は読み込まない
                if (file_path_str.empty()) {
                    PAXS_WARNING("Skipping row " + std::to_string(row_index) + " in " + setting_path + ": file_path is empty");
                    return;
                }

                // 対象となる地物の種別
                const std::string type_str = has_type ? table.get(row_index, type_hash) : "";

                // typeが空の場合はスキップ
                if (type_str.empty()) {
                    PAXS_WARNING("Skipping row " + std::to_string(row_index) + " in " + setting_path + ": type is empty");
                    return;
                }

                // 表示する最小ズームレベル
                const std::string& min_size_str = has_min_size ? table.get(row_index, min_size_hash) : "";
                const double min_zoom_level = StringUtils::safeStod(min_size_str, 0.0, true);

                // 表示する最大ズームレベル
                const std::string& max_size_str = has_max_size ? table.get(row_index, max_size_hash) : "";
                const double max_zoom_level = StringUtils::safeStod(max_size_str, 99999999.0, true);

                // 可視化する時代（古い年～）
                const std::string& first_jd_str = has_first_julian_day ? table.get(row_index, first_julian_day_hash) : "";
                const std::string& first_year_str = has_first_year ? table.get(row_index, first_year_hash) : "";
                const int min_year = !first_jd_str.empty() ?
                    StringUtils::safeStoi(first_jd_str, -99999999, true) :
                    (!first_year_str.empty() ?
                        static_cast<int>(StringUtils::safeStod(first_year_str, -99999999.0, true) * days_in_a_year + julian_day_on_m1_1_1) :
                        -99999999);

                // 可視化する時代（～新しい年）
                const std::string& last_jd_str = has_last_julian_day ? table.get(row_index, last_julian_day_hash) : "";
                const std::string& last_year_str = has_last_year ? table.get(row_index, last_year_hash) : "";
                const int max_year = !last_jd_str.empty() ?
                    StringUtils::safeStoi(last_jd_str, 99999999, true) :
                    (!last_year_str.empty() ?
                        static_cast<int>(StringUtils::safeStod(last_year_str, 99999999.0, true) * days_in_a_year + julian_day_on_m1_1_1) :
                        99999999);

                // 画像
                const std::string& texture_str = has_texture ? table.get(row_index, texture_hash) : "";
                const std::uint_least32_t texture_hash_value = texture_str.empty() ? 0 :
                    MurMur3::calcHash(texture_str.size(), texture_str.c_str());

                // ズームサイズ
                const std::string& zoom_str = has_zoom ? table.get(row_index, zoom_hash) : "";
                const double zoom = StringUtils::safeStod(zoom_str, 1.0, true);

                // コールバックを呼び出し
                callback(FeatureListParams(
                    key_str,
                    file_path_str,
                    type_str,
                    Range<double>(min_zoom_level, max_zoom_level),
                    Range<double>(static_cast<double>(min_year), static_cast<double>(max_year)),
                    texture_hash_value,
                    zoom
                ));
            });
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_MAP_REPOSITORY_FEATURE_LIST_LOADER_HPP
