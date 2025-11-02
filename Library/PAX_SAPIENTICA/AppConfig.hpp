/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_APP_CONFIG_HPP
#define PAX_SAPIENTICA_APP_CONFIG_HPP

#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>

namespace paxs {

    // 実行時定数
    class AppConfig {
    public:
        // インスタンスを取得
        static AppConfig* getInstance() {
            if (instance == nullptr) {
                instance = new AppConfig();
            }
            return instance;
        }

        // ルートパスを取得
        std::string getRootPath() const {
            return root_path;
        }

        std::string getDataSettings(const std::uint_least32_t key_) const {
            if (!data_settings.contains(key_)) {
                PAXS_WARNING("Data settings key " + std::to_string(key_) + " not found.");
                return std::string{};
            }
            return data_settings[key_];
        }
        // 指定したキーのデータ設定が存在している場合は true を返す
        bool isDataSettings(const std::uint_least32_t key_) const {
            if (!data_settings.contains(key_)) {
                return false;
            }
            return (data_settings[key_].size() != 0);
        }
        template<typename Func_>
        void calcDataSettings(const std::uint_least32_t key_, Func_&& func_) const {
            // 指定したキーのデータ設定が存在している場合は処理をする
            if (isDataSettings(key_)) {
                func_(
                    getRootPath() + getDataSettings(key_));
            } else {
                PAXS_WARNING("Data settings for key " + std::to_string(key_) + " is missing.");
            }
        }
        template<typename Func_>
        void calcDataSettingsNotPath(const std::uint_least32_t key_, Func_&& func_) const {
            // 指定したキーのデータ設定が存在している場合は処理をする
            if (isDataSettings(key_)) {
                func_(
                    getDataSettings(key_));
            } else {
                PAXS_WARNING("Data settings for key " + std::to_string(key_) + " is missing.");
            }
        }

    private:
        static AppConfig* instance;

        KeyValueTSV<std::string> data_settings;
        std::string root_path = "";

        AppConfig() {
            KeyValueTSV<std::string> key_value_tsv;
            if (!key_value_tsv.input("Config.tsv")) {
                PAXS_ERROR("Failed to load AppConfig: Config.tsv");
                key_value_tsv.emplace(
                    MurMur3::calcHash("asset_file"),
                    "./"
                ); // デフォルト値
            }
            root_path = key_value_tsv[MurMur3::calcHash("asset_file")];

            if (!data_settings.input(root_path + "Data/Settings.tsv")) {
                PAXS_ERROR("Failed to load AppConfig Data Settings: " + root_path + "Data/Settings.tsv");
                return;
            }
        }

        ~AppConfig() {
            delete instance;
        }

    };

    AppConfig* AppConfig::instance = nullptr;

}

#endif // !PAX_SAPIENTICA_APP_CONFIG_HPP
