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

    private:
        static AppConfig* instance;
        std::string root_path = "";

        AppConfig() {
            KeyValueTSV<std::string> key_value_tsv;
            key_value_tsv.input("Path.tsv");
            root_path = key_value_tsv[MurMur3::calcHash("asset_file")];
        }

        ~AppConfig() {
            delete instance;
        }

    };

    AppConfig* AppConfig::instance = nullptr;

}

#endif // !PAX_SAPIENTICA_APP_CONFIG_HPP
