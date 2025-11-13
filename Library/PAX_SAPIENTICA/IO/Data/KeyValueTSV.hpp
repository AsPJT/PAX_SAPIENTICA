/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_IO_DATA_KEY_VALUE_TSV_HPP
#define PAX_SAPIENTICA_IO_DATA_KEY_VALUE_TSV_HPP

/*##########################################################################################
    * std::uint_least32_t Key
    * std::string Value
    の２つの TSV 読み込み行うクラス
##########################################################################################*/

#include <string>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/System/InputFile.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxs {

    template<typename Value>
    class KeyValueTSV {
    private:
        UnorderedMap<std::uint_least32_t, Value> path_list;
        bool is_successfully_loaded{ false };
        bool is_loaded{ false };

        // 項目の ID を返す
        inline std::size_t inputPathGetMenuIndex(const paxs::UnorderedMap<std::uint_least32_t, std::size_t>& menu, const std::uint_least32_t& str_) {
            return  (menu.find(str_) != menu.end()) ? menu.at(str_) : SIZE_MAX;
        }
    public:

        void emplace(const std::uint_least32_t key_, const Value& value_) {
            path_list.emplace(key_, value_);
        }
        bool contains(const std::uint_least32_t key_) const {
            return path_list.contains(key_);
        }

        UnorderedMap<std::uint_least32_t, Value>& get() {
            return path_list;
        }

        const UnorderedMap<std::uint_least32_t, Value>& get() const {
            return path_list;
        }

        bool isLoaded() const {
            return is_loaded;
        }

        bool isSuccessfullyLoaded() const {
            return is_successfully_loaded;
        }

        /// @brief TSVファイルを読み込む
        /// @param relative_path アセットルートからの相対パス
        /// @param func 値を変換する関数
        /// @return 成功時true
        template<typename Func>
        bool input(const std::string& relative_path, Func&& func) {
            if (is_loaded) return true;
            is_loaded = true;

            paxs::InputFile input_file(relative_path);
            if (input_file.fail()) {
                PAXS_ERROR(relative_path + " is missing.");
                return false;
            }
            // 1 行目を読み込む
            if (!(input_file.getLine())) {
                PAXS_ERROR("The first line of " + relative_path + " could not be read.");
                return false;
            }
            // BOM を削除
            input_file.deleteBOM();
            // 1 行目を分割する
            paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu = input_file.splitHashMapMurMur3('\t');

            const std::size_t file_path = inputPathGetMenuIndex(menu, MurMur3::calcHash("value"));
            if (file_path == SIZE_MAX) {
                PAXS_ERROR(relative_path + " is missing a Value on the first line.");
                return false; // Value がないのはデータにならない
            }
            const std::size_t file_type = inputPathGetMenuIndex(menu, MurMur3::calcHash("key"));
            if (file_type == SIZE_MAX) {
                PAXS_ERROR(relative_path + " is missing a Key on the first line.");
                return false; // Key がないのはデータにならない
            }

            // 1 行ずつ読み込み（区切りはタブ）
            while (input_file.getLine()) {
                std::vector<std::string> strvec = input_file.split('\t');

                // 列数が項目より小さい場合は読み込まない
                if (file_type >= strvec.size()) continue;
                if (file_path >= strvec.size()) continue;

                // テクスチャ名が空の場合は読み込まない
                if (strvec[file_type].size() == 0) continue;

                // テクスチャを追加
                path_list.emplace(MurMur3::calcHash(strvec[file_type].size(), strvec[file_type].c_str()), func(strvec[file_path]));
            }
            is_successfully_loaded = true;
            return true;
        }
        /// @brief TSVファイルを読み込む（デフォルト変換関数使用）
        /// @param relative_path アセットルートからの相対パス
        /// @return 成功時true
        bool input(const std::string& relative_path) {
            return input(relative_path, [](const std::string& value_) { return Value(value_); });
        }

        Value operator[](std::uint_least32_t key_) const {
            return path_list.contains(key_) ? path_list.at(key_) : Value{};
        }

    };

}

#endif // !PAX_SAPIENTICA_IO_DATA_KEY_VALUE_TSV_HPP
