/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_REPOSITORY_LOCATION_DATA_LOADER_HPP
#define PAX_SAPIENTICA_MAP_REPOSITORY_LOCATION_DATA_LOADER_HPP

#include <cstdint>
#include <string>
#include <optional>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/IO/Data/TsvTable.hpp>
#include <PAX_SAPIENTICA/Map/Repository/FeatureListLoader.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>
#include <PAX_SAPIENTICA/Calendar/JulianDayNumber.hpp>

namespace paxs {

    // 内部実装用の名前空間
    namespace location_data_loader_detail {
        // 日付計算用の定数（Repository用の簡易版）
        // Note: LocationPoint.hppには365.2425と1721060.0が定義されているが、
        // 既存のRepository実装との互換性のため、ここでは整数値を使用
        // TODO: 確認
        constexpr int days_in_a_year = 365;
        constexpr int julian_day_on_m1_1_1 = 1721058;
    }

    /// @brief TSVカラムのハッシュ値を事前計算して保持する構造体
    /// @brief Struct to hold pre-computed hash values for TSV columns
    struct ColumnHashes {
        // 必須カラム
        std::uint_least32_t longitude = MurMur3::calcHash("longitude");
        std::uint_least32_t latitude = MurMur3::calcHash("latitude");

        // オプションカラム
        std::uint_least32_t overall_length = MurMur3::calcHash("overall_length");
        std::uint_least32_t x_size = MurMur3::calcHash("x_size");
        std::uint_least32_t y_size = MurMur3::calcHash("y_size");
        std::uint_least32_t min_size = MurMur3::calcHash("min_size");
        std::uint_least32_t max_size = MurMur3::calcHash("max_size");
        std::uint_least32_t first_julian_day = MurMur3::calcHash("first_julian_day");
        std::uint_least32_t last_julian_day = MurMur3::calcHash("last_julian_day");
        std::uint_least32_t first_year = MurMur3::calcHash("first_year");
        std::uint_least32_t last_year = MurMur3::calcHash("last_year");
        std::uint_least32_t texture = MurMur3::calcHash("texture");
        std::uint_least32_t ja_jp = MurMur3::calcHash("ja-JP");
        std::uint_least32_t en_us = MurMur3::calcHash("en-US");
    };

    // location_data_loader_detailの追加関数
    namespace location_data_loader_detail {
        /// @brief 標準カラムかどうかを判定
        /// @brief Check if column is a standard column
        inline bool isStandardColumn(std::uint_least32_t column_hash, const ColumnHashes& hashes) {
            return column_hash == hashes.longitude ||
                   column_hash == hashes.latitude ||
                   column_hash == hashes.overall_length ||
                   column_hash == hashes.x_size ||
                   column_hash == hashes.y_size ||
                   column_hash == hashes.min_size ||
                   column_hash == hashes.max_size ||
                   column_hash == hashes.first_julian_day ||
                   column_hash == hashes.last_julian_day ||
                   column_hash == hashes.first_year ||
                   column_hash == hashes.last_year ||
                   column_hash == hashes.texture ||
                   column_hash == hashes.ja_jp ||
                   column_hash == hashes.en_us;
        }
    }

    /// @brief カラムの存在フラグを保持する構造体
    /// @brief Struct to hold column existence flags
    struct ColumnFlags {
        bool has_overall_length = false;
        bool has_x_size = false;
        bool has_y_size = false;
        bool has_min_size = false;
        bool has_max_size = false;
        bool has_first_julian_day = false;
        bool has_last_julian_day = false;
        bool has_first_year = false;
        bool has_last_year = false;
        bool has_texture = false;
        bool has_ja_jp = false;
        bool has_en_us = false;

        /// @brief TsvTableから各カラムの存在を確認して設定
        /// @brief Set column existence flags from TsvTable
        void setFromTable(const TsvTable& table, const ColumnHashes& hashes) {
            has_overall_length = table.hasColumn(hashes.overall_length);
            has_x_size = table.hasColumn(hashes.x_size);
            has_y_size = table.hasColumn(hashes.y_size);
            has_min_size = table.hasColumn(hashes.min_size);
            has_max_size = table.hasColumn(hashes.max_size);
            has_first_julian_day = table.hasColumn(hashes.first_julian_day);
            has_last_julian_day = table.hasColumn(hashes.last_julian_day);
            has_first_year = table.hasColumn(hashes.first_year);
            has_last_year = table.hasColumn(hashes.last_year);
            has_texture = table.hasColumn(hashes.texture);
            has_ja_jp = table.hasColumn(hashes.ja_jp);
            has_en_us = table.hasColumn(hashes.en_us);
        }
    };

    /// @brief 座標範囲を管理する構造体
    /// @brief Struct to manage coordinate bounds
    struct CoordinateBounds {
        double start_longitude = 180.0;
        double end_longitude = -180.0;
        double start_latitude = 90.0;
        double end_latitude = -90.0;

        /// @brief 座標を追加して範囲を更新
        /// @brief Update bounds with new coordinates
        void update(double longitude, double latitude) {
            start_longitude = (std::min)(start_longitude, longitude);
            start_latitude = (std::min)(start_latitude, latitude);
            end_longitude = (std::max)(end_longitude, longitude);
            end_latitude = (std::max)(end_latitude, latitude);
        }
    };

    /// @brief TSVから読み込んだ行データを保持する構造体
    /// @brief Struct to hold row data loaded from TSV
    struct LocationRowData {
        double longitude = 0.0;
        double latitude = 0.0;
        double overall_length = 10.0;
        std::uint_least16_t x_size = 1;
        std::uint_least16_t y_size = 1;
        double min_size = 0.0;
        double max_size = 0.0;
        int first_julian_day = 0;
        int last_julian_day = 0;
        std::uint_least32_t texture_hash = 0;
        paxs::UnorderedMap<std::uint_least32_t, std::string> names;

        /// @brief 追加カラムデータ
        /// @brief Extra column data
        paxs::UnorderedMap<std::uint_least32_t, std::string> extra_data;
    };

    /// @brief 位置データ読み込みのヘルパークラス
    /// @brief Helper class for loading location data
    class LocationDataLoader {
    public:
        /// @brief 多言語名を読み込む
        /// @brief Load multilingual names
        static paxs::UnorderedMap<std::uint_least32_t, std::string> loadNames(
            const TsvTable& table,
            std::size_t row_index,
            const ColumnHashes& hashes,
            const ColumnFlags& flags
        ) {
            paxs::UnorderedMap<std::uint_least32_t, std::string> names;

            if (flags.has_ja_jp) {
                const std::string& ja_jp_str = table.get(row_index, hashes.ja_jp);
                if (!ja_jp_str.empty()) {
                    names.emplace(MurMur3::calcHash("ja-JP"), ja_jp_str);
                }
            }
            if (flags.has_en_us) {
                const std::string& en_us_str = table.get(row_index, hashes.en_us);
                if (!en_us_str.empty()) {
                    names.emplace(MurMur3::calcHash("en-US"), en_us_str);
                }
            }

            return names;
        }

        /// @brief 日付範囲を計算（ユリウス日または年から）
        /// @brief Calculate date range from Julian Day or year
        static int calculateJulianDay(
            const std::string& jd_str,
            const std::string& year_str,
            int default_value
        ) {
            if (!jd_str.empty()) {
                return static_cast<int>(std::stod(jd_str));
            }
            if (!year_str.empty()) {
                return static_cast<int>((std::stod(year_str) * location_data_loader_detail::days_in_a_year) + location_data_loader_detail::julian_day_on_m1_1_1);
            }
            return default_value;
        }

        /// @brief オプション値を取得（空の場合はデフォルト値）
        /// @brief Get optional value or default
        template<typename T>
        static T getOptionalValue(
            const std::string& str,
            T default_value
        ) {
            if (str.empty()) {
                return default_value;
            }
            if constexpr (std::is_same_v<T, std::uint_least16_t>) {
                return static_cast<std::uint_least16_t>(std::stod(str));
            } else if constexpr (std::is_same_v<T, double>) {
                return std::stod(str);
            } else if constexpr (std::is_same_v<T, int>) {
                return static_cast<int>(std::stod(str));
            }
            return default_value;
        }

        /// @brief テクスチャハッシュを計算
        /// @brief Calculate texture hash
        static std::uint_least32_t calculateTextureHash(
            const std::string& texture_str,
            std::uint_least32_t default_hash
        ) {
            if (texture_str.empty()) {
                return default_hash;
            }
            return MurMur3::calcHash(texture_str.size(), texture_str.c_str());
        }

        /// @brief 行データを完全に読み込む
        /// @brief Load complete row data
        static std::optional<LocationRowData> loadRowData(
            const TsvTable& table,
            std::size_t row_index,
            const ColumnHashes& hashes,
            const ColumnFlags& flags,
            const FeatureListParams& params
        ) {
            // 必須カラムの取得
            const std::string& longitude_str = table.get(row_index, hashes.longitude);
            const std::string& latitude_str = table.get(row_index, hashes.latitude);

            // 経度・緯度が空の場合はスキップ
            if (longitude_str.empty() || latitude_str.empty()) {
                PAXS_WARNING("Skipping row " + std::to_string(row_index) + " in " + params.file_path + ": missing coordinates");
                return std::nullopt;
            }

            LocationRowData data;
            data.longitude = std::stod(longitude_str);
            data.latitude = std::stod(latitude_str);

            // 多言語名の読み込み
            data.names = loadNames(table, row_index, hashes, flags);

            // オプションカラムの読み込み
            const std::string& overall_length_str = flags.has_overall_length ? table.get(row_index, hashes.overall_length) : "";
            const std::string& x_size_str = flags.has_x_size ? table.get(row_index, hashes.x_size) : "";
            const std::string& y_size_str = flags.has_y_size ? table.get(row_index, hashes.y_size) : "";
            const std::string& min_size_str = flags.has_min_size ? table.get(row_index, hashes.min_size) : "";
            const std::string& max_size_str = flags.has_max_size ? table.get(row_index, hashes.max_size) : "";
            const std::string& texture_str = flags.has_texture ? table.get(row_index, hashes.texture) : "";

            data.overall_length = getOptionalValue(overall_length_str, 10.0);
            data.x_size = getOptionalValue<std::uint_least16_t>(x_size_str, 1);
            data.y_size = getOptionalValue<std::uint_least16_t>(y_size_str, 1);
            data.min_size = getOptionalValue(min_size_str, params.min_zoom_level);
            data.max_size = getOptionalValue(max_size_str, params.max_zoom_level);

            // 日付範囲の計算
            const std::string& first_jd_str = flags.has_first_julian_day ? table.get(row_index, hashes.first_julian_day) : "";
            const std::string& first_year_str = flags.has_first_year ? table.get(row_index, hashes.first_year) : "";
            const std::string& last_jd_str = flags.has_last_julian_day ? table.get(row_index, hashes.last_julian_day) : "";
            const std::string& last_year_str = flags.has_last_year ? table.get(row_index, hashes.last_year) : "";

            data.first_julian_day = calculateJulianDay(first_jd_str, first_year_str, params.min_year);
            data.last_julian_day = calculateJulianDay(last_jd_str, last_year_str, params.max_year);

            // テクスチャハッシュの計算
            data.texture_hash = calculateTextureHash(texture_str, params.texture_hash);

            // 追加カラムの読み込み
            const std::vector<std::uint_least32_t>& header_keys = table.getHeaderKeys();
            for (const auto& column_hash : header_keys) {
                // 標準カラムでない場合は extra_data に格納
                if (!location_data_loader_detail::isStandardColumn(column_hash, hashes)) {
                    const std::string& value = table.get(row_index, column_hash);
                    if (!value.empty()) {
                        data.extra_data[column_hash] = value;
                    }
                }
            }

            return data;
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_MAP_REPOSITORY_LOCATION_DATA_LOADER_HPP
