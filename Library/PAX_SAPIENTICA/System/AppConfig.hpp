/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_APP_CONFIG_HPP
#define PAX_SAPIENTICA_APP_CONFIG_HPP

#include <fstream>
#include <string>
#include <utility>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>
#include <PAX_SAPIENTICA/Utility/StringUtils.hpp>

namespace paxs {

    // BOM constants
    namespace BOMConstants {
        constexpr unsigned char bom_byte_1 = 0xEF;
        constexpr unsigned char bom_byte_2 = 0xBB;
        constexpr unsigned char bom_byte_3 = 0xBF;
        constexpr std::size_t bom_size = 3;
    }

    // 実行時定数
    class AppConfig {
    public:
        // コピー・ムーブを禁止
        AppConfig(const AppConfig&) = delete;
        auto operator=(const AppConfig&) -> AppConfig& = delete;
        AppConfig(AppConfig&&) = delete;
        auto operator=(AppConfig&&) -> AppConfig& = delete;

        // Meyer's Singleton パターン
        static AppConfig& getInstance() {
            static AppConfig instance;
            return instance;
        }

        // ルートパスを取得
        [[nodiscard]] std::string getRootPath() const {
            return root_path;
        }

        [[nodiscard]] std::string getSettingPath(const std::uint_least32_t key) const {
            if (!data_settings.contains(key)) {
                PAXS_WARNING("Data settings key " + std::to_string(key) + " not found.");
                return std::string{};
            }
            return data_settings.at(key);
        }
        template<typename Func_>
        void ifSettingExists(const std::uint_least32_t key, Func_&& func) const {
            // 指定したキーのデータ設定が存在している場合は処理をする
            if (hasDataSettings(key)) {
                std::forward<Func_>(func)(getSettingPath(key));
            } else {
                PAXS_WARNING("Data settings for key " + std::to_string(key) + " is missing.");
            }
        }

    private:
        paxs::UnorderedMap<std::uint_least32_t, std::string> data_settings;
        std::string root_path;

        AppConfig() {
            // Config.tsvからroot_pathを読み込み
            root_path = loadKeyValueTSV("Config.tsv", MurMur3::calcHash("asset_file"));
            if (root_path.empty()) {
                PAXS_ERROR("Failed to load asset_file from Config.tsv");
                root_path = ""; // デフォルト値
            }

            // Data/Settings.tsvからdata_settingsを読み込み
            if (!loadAllKeyValueTSV(root_path + "Data/Settings.tsv", data_settings)) {
                PAXS_ERROR("Failed to load AppConfig Data Settings: " + root_path + "Data/Settings.tsv");
            }
        }

        ~AppConfig() = default;

        // @brief 指定したキーのデータ設定が存在しているか
        [[nodiscard]] bool hasDataSettings(const std::uint_least32_t key_) const {
            if (!data_settings.contains(key_)) {
                return false;
            }
            return (data_settings.at(key_).size() != 0);
        }

        /// @brief TSVファイルから key-value ペアを全て読み込む
        static bool loadAllKeyValueTSV(const std::string& file_path,
                                       paxs::UnorderedMap<std::uint_least32_t, std::string>& map) {
            std::ifstream file(file_path);
            if (!file.is_open()) {
                return false;
            }

            std::string line;
            // 1行目（ヘッダー）を読み込み
            if (!std::getline(file, line)) {
                return false;
            }

            // BOM削除
            if (line.size() >= BOMConstants::bom_size &&
                static_cast<unsigned char>(line[0]) == BOMConstants::bom_byte_1 &&
                static_cast<unsigned char>(line[1]) == BOMConstants::bom_byte_2 &&
                static_cast<unsigned char>(line[2]) == BOMConstants::bom_byte_3) {
                line = line.substr(BOMConstants::bom_size);
            }

            std::vector<std::string> headers = paxs::StringUtils::split(line, '\t');

            // key, value カラムのインデックスを取得
            std::size_t key_index = SIZE_MAX;
            std::size_t value_index = SIZE_MAX;
            for (std::size_t i = 0; i < headers.size(); ++i) {
                if (headers[i] == "key") {
                    key_index = i;
                }
                if (headers[i] == "value") {
                    value_index = i;
                }
            }

            if (key_index == SIZE_MAX || value_index == SIZE_MAX) {
                PAXS_ERROR("TSV file is missing 'key' or 'value' column: " + file_path);
                return false;
            }

            // データ行を読み込み
            while (std::getline(file, line)) {
                std::vector<std::string> columns = paxs::StringUtils::split(line, '\t');

                if (key_index >= columns.size() || value_index >= columns.size()) {
                    continue;
                }

                const std::string& key_str = columns[key_index];
                const std::string& value_str = columns[value_index];

                if (key_str.empty()) {
                    continue;
                }

                map.emplace(
                    MurMur3::calcHash(key_str.size(), key_str.c_str()),
                    value_str
                );
            }

            return true;
        }

        /// @brief TSVファイルから特定のキーの値を取得
        static std::string loadKeyValueTSV(const std::string& file_path, std::uint_least32_t target_key) {
            paxs::UnorderedMap<std::uint_least32_t, std::string> temp_map;
            if (!loadAllKeyValueTSV(file_path, temp_map)) {
                return std::string{};
            }
            return temp_map.contains(target_key) ? temp_map.at(target_key) : std::string{};
        }

    };

}

#endif // !PAX_SAPIENTICA_APP_CONFIG_HPP
