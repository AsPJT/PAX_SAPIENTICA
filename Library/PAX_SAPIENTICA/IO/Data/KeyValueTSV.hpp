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
    の２つの TSV/Binary 読み込み行うクラス
##########################################################################################*/

#include <string>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/IO/Data/UnifiedTable.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    template<typename Value>
    class KeyValueTSV {
    private:
        UnorderedMap<std::uint_least32_t, Value> path_list;
        bool is_successfully_loaded{ false };
        bool is_loaded{ false };

    public:

        void emplace(const std::uint_least32_t key_, const Value& value_) {
            path_list.emplace(key_, value_);
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

        /// @brief TSV/Binaryファイルを読み込む (UnifiedTable使用)
        /// @param relative_path アセットルートからの相対パス（拡張子なし）
        /// @param func 値を変換する関数
        /// @return 成功時true
        template<typename Func>
        bool input(const std::string& relative_path, Func&& func) {
            if (is_loaded) return true;
            is_loaded = true;

            // UnifiedTableを使用（.binがあれば優先、なければ.tsvを読み込む）
            paxs::UnifiedTable table(relative_path);
            if (!table.isSuccessfullyLoaded()) {
                PAXS_ERROR(relative_path + " (.tsv or .bin) is missing or failed to load.");
                return false;
            }

            // 必須カラムの存在確認
            if (!table.hasColumn("key")) {
                PAXS_ERROR(relative_path + " is missing a 'key' column.");
                return false;
            }
            if (!table.hasColumn("value")) {
                PAXS_ERROR(relative_path + " is missing a 'value' column.");
                return false;
            }

            const std::uint_least32_t key_hash = MurMur3::calcHash("key");
            const std::uint_least32_t value_hash = MurMur3::calcHash("value");

            // 1 行ずつ読み込み
            table.forEachRow([&](std::size_t row_index, const std::vector<std::string>&) {
                const std::string& value_str = table.get(row_index, value_hash);

                // バイナリ形式の場合: keyハッシュを直接取得
                std::uint_least32_t key_hashed = 0;
                if (table.isBinary()) {
                    key_hashed = table.getKeyHash(row_index);
                    if (key_hashed == 0) return;  // ハッシュが取得できない場合はスキップ
                } else {
                    // TSV形式の場合: keyを文字列として取得してハッシュ化
                    const std::string& key_str = table.get(row_index, key_hash);
                    if (key_str.empty()) return;  // keyが空の場合は読み込まない
                    key_hashed = MurMur3::calcHash(key_str.size(), key_str.c_str());
                }

                // keyハッシュと値を格納
                path_list.emplace(key_hashed, func(value_str));
            });

            is_successfully_loaded = true;
            return true;
        }

        /// @brief TSV/Binaryファイルを読み込む（デフォルト変換関数使用）
        /// @param relative_path アセットルートからの相対パス（拡張子なし）
        /// @return 成功時true
        bool input(const std::string& relative_path) {
            return input(relative_path, [](const std::string& value_) { return Value(value_); });
        }

        /// @brief 要素にアクセス
        const Value& at(std::uint_least32_t key_) const {
            return path_list.at(key_);
        }

        /// @brief 要素にアクセス、存在しない場合は nullptr を返す
        const Value* try_get(std::uint_least32_t key_) const {
            return path_list.try_get(key_);
        }
    };

}

#endif // !PAX_SAPIENTICA_IO_DATA_KEY_VALUE_TSV_HPP
