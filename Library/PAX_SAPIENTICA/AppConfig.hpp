/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_APP_CONFIG_HPP
#define PAX_SAPIENTICA_APP_CONFIG_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>

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
            return data_settings[key_];
        }
        // 指定したキーのデータ設定が存在している場合は true を返す
        bool isDataSettings(const std::uint_least32_t key_) const {
            return (data_settings[key_].size() != 0);
        }
        template<typename Func_>
        void calcDataSettings(const std::uint_least32_t key_, Func_&& func_) const {
            // 指定したキーのデータ設定が存在している場合は処理をする
            if (AppConfig::getInstance()->isDataSettings(key_)) {
                func_(
                    AppConfig::getInstance()->getRootPath()
                    + AppConfig::getInstance()->getDataSettings(key_));
            }
        }
        template<typename Func_>
        void calcDataSettingsNotPath(const std::uint_least32_t key_, Func_&& func_) const {
            // 指定したキーのデータ設定が存在している場合は処理をする
            if (AppConfig::getInstance()->isDataSettings(key_)) {
                func_(
                    AppConfig::getInstance()->getDataSettings(key_));
            }
        }

    private:
        static AppConfig* instance;

        KeyValueTSV<std::string> data_settings;
        std::string root_path = "";

        AppConfig() {
            KeyValueTSV<std::string> key_value_tsv;
            key_value_tsv.input("Config.tsv");
            root_path = key_value_tsv[MurMur3::calcHash("asset_file")];

            data_settings.input(root_path + "Data/Settings.tsv");
        }

        ~AppConfig() {
            delete instance;
        }

    };

    AppConfig* AppConfig::instance = nullptr;

}

#endif // !PAX_SAPIENTICA_APP_CONFIG_HPP
