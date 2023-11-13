/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_LANGUAGE_FONTS_HPP
#define PAX_MAHOROBA_LANGUAGE_FONTS_HPP

/*##########################################################################################

##########################################################################################*/

#include <cstdint>
#include <string>
#include <unordered_map>

#include <PAX_GRAPHICA/String.hpp> // Font

namespace paxs {

    // 言語用のフォントを管理
    class LanguageFonts {
    private:
        std::string default_path{};
        std::unordered_map<std::uint_least64_t, paxg::Font> fonts{};
        std::unordered_map<std::uint_least32_t, std::string> paths{};

        constexpr std::uint_least64_t createIndex(const std::uint_least32_t language_key_, const std::uint_least8_t size_, const std::uint_least8_t buffer_thickness_) const {
            return (static_cast<std::uint_least64_t>(language_key_) << 32)
                + (static_cast<std::uint_least64_t>(size_) << 8)
                + (static_cast<std::uint_least64_t>(buffer_thickness_));
        }
    public:

        void setDefaultPath(const std::string& path_) {
            default_path = path_;
        }

        void add(const std::string& path_, const std::uint_least32_t language_key_, const std::uint_least8_t size_, const std::uint_least8_t buffer_thickness_) {
            const std::uint_least64_t key = createIndex(language_key_, size_, buffer_thickness_);
            if (fonts.find(key) == fonts.end()) { // 登録されていない場合は登録する
                fonts.emplace(
                    key,
                    // const int size_, const std::string& path, const int buffer_thickness
                    paxg::Font(static_cast<int>(size_),
                        path_,
                        static_cast<int>(buffer_thickness_))
                );
            }
            if (paths.find(language_key_) == paths.end()) { // 登録されていない場合は登録する
                paths.emplace(language_key_, path_);
            }
        }

        paxg::Font* getAndAdd(const std::uint_least32_t language_key_, const std::uint_least8_t size_, const std::uint_least8_t buffer_thickness_) {
            const std::uint_least64_t key = createIndex(language_key_, size_, buffer_thickness_);
            if (fonts.find(key) == fonts.end()) { // 登録されていない場合は登録する
                if (paths.find(language_key_) != paths.end()) {
                    fonts.emplace(
                        key,
                        // const int size_, const std::string& path, const int buffer_thickness
                        paxg::Font(static_cast<int>(size_),
                            paths.at(language_key_),
                            static_cast<int>(buffer_thickness_))
                    );
                }
                // else return nullptr;
                // パスがない場合に設定する（試験用）
                else {
                    fonts.emplace(
                        key,
                        // const int size_, const std::string& path, const int buffer_thickness
                        paxg::Font(static_cast<int>(size_),
                            default_path,
                            static_cast<int>(buffer_thickness_))
                    );
                }
            }
            return &(fonts.at(key));
        }

    };

}

#endif // !PAX_MAHOROBA_LANGUAGE_FONTS_HPP
