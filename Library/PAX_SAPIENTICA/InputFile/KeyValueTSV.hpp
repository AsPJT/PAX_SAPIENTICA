/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_KEY_VALUE_TSV_HPP
#define PAX_SAPIENTICA_KEY_VALUE_TSV_HPP

/*##########################################################################################
    * std::uint_least32_t Key
    * std::string Value
    の２つの TSV 読み込み行うクラス
##########################################################################################*/

#include <string>

#include <PAX_SAPIENTICA/InputFile.hpp>
#include <PAX_SAPIENTICA/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>

namespace paxs {

    template<typename Value>
    class KeyValueTSV {
    private:
        UnorderedMap<std::uint_least32_t, Value> path_list;
    private:
        // 項目の ID を返す
        inline std::size_t inputPathGetMenuIndex(const paxs::UnorderedMap<std::uint_least32_t, std::size_t>& menu, const std::uint_least32_t& str_) {
            return  (menu.find(str_) != menu.end()) ? menu.at(str_) : SIZE_MAX;
        }
    public:

        void emplace(const std::uint_least32_t key_, const Value& value_) {
            path_list.emplace(key_, value_);
        }
        bool contains(const std::uint_least32_t key_) {
            return path_list.contains(key_);
        }

        UnorderedMap<std::uint_least32_t, Value>& get() {
            return path_list;
        }

        // ルートパスを読み込む true 成功
        template<typename Func>
        bool input(const std::string& str_, Func&& func) {

            paxs::InputFile pifs(str_);
            if (pifs.fail()) {
                PAXS_ERROR(str_ + " is missing.");
                return false;
            }
            // 1 行目を読み込む
            if (!(pifs.getLine())) {
                PAXS_ERROR("The first line of " + str_ + " could not be read.");
                return false;
            }
            // BOM を削除
            pifs.deleteBOM();
            // 1 行目を分割する
            paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu = pifs.splitHashMapMurMur3('\t');

            const std::size_t file_path = inputPathGetMenuIndex(menu, MurMur3::calcHash("value"));
            if (file_path == SIZE_MAX) {
                PAXS_ERROR(str_ + " is missing a Value on the first line.");
                return false; // Value がないのはデータにならない
            }
            const std::size_t file_type = inputPathGetMenuIndex(menu, MurMur3::calcHash("key"));
            if (file_type == SIZE_MAX) {
                PAXS_ERROR(str_ + " is missing a Key on the first line.");
                return false; // Key がないのはデータにならない
            }

            // 1 行ずつ読み込み（区切りはタブ）
            while (pifs.getLine()) {
                std::vector<std::string> strvec = pifs.split('\t');

                // 列数が項目より小さい場合は読み込まない
                if (file_type >= strvec.size()) continue;
                if (file_path >= strvec.size()) continue;

                // テクスチャ名が空の場合は読み込まない
                if (strvec[file_type].size() == 0) continue;

                // テクスチャを追加
                path_list.emplace(MurMur3::calcHash(strvec[file_type].size(), strvec[file_type].c_str()), func(strvec[file_path]));
            }
            return true;
        }
        bool input(const std::string& str_) {
            return input(str_, [](const std::string& value_) { return Value(value_); });
        }

        Value operator[](std::uint_least32_t key_) const {
            return path_list.contains(key_) ? path_list.at(key_) : Value{};
        }

    };

}

#endif // !PAX_SAPIENTICA_KEY_VALUE_TSV_HPP
