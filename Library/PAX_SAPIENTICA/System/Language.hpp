/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SYSTEM_LANGUAGE_HPP
#define PAX_SAPIENTICA_SYSTEM_LANGUAGE_HPP

#include <cstddef>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/System/InputFile.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    // 選択言語
    class SelectLanguage {
    private:
        std::size_t select_language = 0; // 選択している言語
        std::uint_least32_t select_key = 0; // 選択している言語
    public:
        constexpr void set(const std::size_t select_language_) { select_language = select_language_; }
        constexpr void setKey(const std::uint_least32_t select_key_) { select_key = select_key_; }
        constexpr std::size_t get() const { return select_language; }
        constexpr std::uint_least32_t getKey() const { return select_key; }
    };

    /// @brief 多言語テキスト辞書クラス
    /// @brief Multi-language text dictionary class
    /// @details TSVファイルから多言語テキストを読み込み、言語キーとテキストキーで検索可能にする
    ///          Loads multi-language text from TSV files and makes it searchable by language key and text key
    class Language {
    private:
        /// @brief 登録された言語のリスト（登録順）
        /// @brief List of registered languages (in registration order)
        std::vector<std::uint_least32_t> ordered_languages_;

        /// @brief 言語キー → インデックス のマッピング
        /// @brief Mapping from language key to index
        paxs::UnorderedMap<std::uint_least32_t, std::size_t> language_key_to_index_;

        /// @brief (テキストキー + 言語キー) → テキスト のマッピング
        /// @brief Mapping from (text key + language key) to text
        /// @details 上位32bitがテキストキー、下位32bitが言語キー
        ///          Upper 32 bits: text key, Lower 32 bits: language key
        paxs::UnorderedMap<std::uint_least64_t, std::string> text_dictionary_;

        /// @brief テキストキー → フォールバック用の64bitキー のマッピング
        /// @brief Mapping from text key to fallback 64-bit key
        /// @details 指定言語でテキストが見つからない場合のフォールバック用
        ///          Used as fallback when text is not found in specified language
        paxs::UnorderedMap<std::uint_least32_t, std::uint_least64_t> fallback_text_key_;

        /// @brief TSVヘッダーから指定キーのカラムインデックスを取得
        /// @brief Get column index for specified key from TSV header
        /// @param header_map ヘッダーマップ / Header map
        /// @param key 検索するキー / Key to search
        /// @return カラムインデックス、見つからない場合はSIZE_MAX
        ///         Column index, SIZE_MAX if not found
        static std::size_t getColumnIndex(
            const paxs::UnorderedMap<std::uint_least32_t, std::size_t>& header_map,
            const std::uint_least32_t key) {
            auto iterator = header_map.find(key);
            return (iterator != header_map.end()) ? iterator->second : SIZE_MAX;
        }

    public:
        /// @brief 64bitキーが辞書に存在するかチェック
        /// @brief Check if 64-bit key exists in dictionary
        /// @param key 64bitキー（テキストキー + 言語キー） / 64-bit key (text key + language key)
        /// @return 存在すればtrue / true if exists
        bool hasText(const std::uint_least64_t key) const {
            return text_dictionary_.find(key) != text_dictionary_.end();
        }

        /// @brief テキストキーが辞書に存在するかチェック
        /// @brief Check if text key exists in dictionary
        /// @param key テキストキー / Text key
        /// @return 存在すればtrue / true if exists
        bool hasTextKey(const std::uint_least32_t key) const {
            return fallback_text_key_.find(key) != fallback_text_key_.end();
        }

        /// @brief テキストキーと言語キーから64bitキーを生成
        /// @brief Generate 64-bit key from text key and language key
        /// @param text_key テキストキー / Text key
        /// @param language_key 言語キー / Language key
        /// @return 64bitキー（上位32bit: テキストキー, 下位32bit: 言語キー）
        ///         64-bit key (upper 32 bits: text key, lower 32 bits: language key)
        std::uint_least64_t createCombinedKey(
            const std::uint_least32_t text_key,
            const std::uint_least32_t language_key) const {
            return (static_cast<std::uint_least64_t>(text_key) << 32)
                + static_cast<std::uint_least64_t>(language_key);
        }

        /// @brief テキストを取得
        /// @brief Get text string
        /// @param text_key テキストキー / Text key
        /// @param language_key 言語キー / Language key
        /// @return テキストへのポインタ、見つからない場合はnullptr
        ///         Pointer to text string, nullptr if not found
        /// @details 指定言語で見つからない場合、フォールバック言語のテキストを返す
        ///          If not found in specified language, returns text in fallback language
        const std::string* getStringPtr(
            const std::uint_least32_t text_key,
            const std::uint_least32_t language_key) const {
            const std::uint_least64_t combined_key = createCombinedKey(text_key, language_key);

            // 指定した言語でテキストが存在する場合はそれを返す
            if (combined_key != 0 && hasText(combined_key)) {
                return &(text_dictionary_.at(combined_key));
            }

            // フォールバック処理：デフォルト言語のテキストを返す
            if (!hasTextKey(text_key)) {
                return nullptr; // テキストキー自体が登録されていない
            }

            const std::uint_least64_t fallback_key = fallback_text_key_.at(text_key);
            if (!hasText(fallback_key)) {
                return nullptr; // フォールバックテキストも見つからない
            }

            return &(text_dictionary_.at(fallback_key));
        }

        /// @brief TSVファイルから多言語テキストを読み込む
        /// @brief Load multi-language text from TSV file
        /// @param relative_path 相対ファイルパス / Relative file path
        /// @details TSV形式：1行目がヘッダー（key, ja-JP, en-US, ...）、2行目以降がデータ
        ///          TSV format: 1st row is header (key, ja-JP, en-US, ...), subsequent rows are data
        void add(const std::string& relative_path) {
            paxs::InputFile input_file(relative_path);
            if (input_file.fail()) {
                PAXS_WARNING("Failed to load language file: " + relative_path);
                return;
            }

            // ヘッダー行を読み込む（1行目）
            if (!input_file.getLine()) {
                PAXS_WARNING("Failed to load language file: " + relative_path);
                return; // ファイルが空の場合
            }

            // BOMを削除
            input_file.deleteBOM();

            // ヘッダーを解析（タブ区切り）
            paxs::UnorderedMap<std::uint_least32_t, std::size_t> header_map = input_file.splitHashMapMurMur3('\t');
            std::vector<std::uint_least32_t> language_keys = input_file.splitHashMapMurMur3Vector('\t');

            // "key"カラムのインデックスを取得
            const std::size_t key_column_index = getColumnIndex(header_map, MurMur3::calcHash("key"));
            if (key_column_index == SIZE_MAX) {
                PAXS_WARNING("Failed to load language file: " + relative_path);
                return; // "key"カラムがない場合は処理できない
            }

            // "key"カラム自体を言語リストに登録（内部管理用）
            const std::uint_least32_t key_hash = MurMur3::calcHash("key");
            if (language_key_to_index_.find(key_hash) == language_key_to_index_.end()) {
                language_key_to_index_.emplace(key_hash, SIZE_MAX);
            }

            // ヘッダーに含まれる言語を登録
            for (const std::uint_least32_t lang_key : language_keys) {
                if (language_key_to_index_.find(lang_key) == language_key_to_index_.end()) {
                    language_key_to_index_.emplace(lang_key, ordered_languages_.size());
                    ordered_languages_.emplace_back(lang_key);
                }
            }

            // データ行を1行ずつ読み込み
            while (input_file.getLine()) {
                std::vector<std::string> columns = input_file.split('\t');
                if (key_column_index >= columns.size()) {
                    continue; // keyカラムがない行はスキップ
                }

                // 各カラム（各言語）のテキストを処理
                for (std::size_t i = 0; i < columns.size() && i < language_keys.size(); ++i) {
                    if (i == key_column_index) {
                        continue; // keyカラム自体はスキップ
                    }
                    if (columns[i].empty()) {
                        continue;     // 空のテキストはスキップ
                    }

                    // テキストキーを生成（keyカラムの値をハッシュ化）
                    const std::uint_least32_t text_key = MurMur3::calcHash(
                        columns[key_column_index].size(),
                        columns[key_column_index].c_str()
                    );

                    // 64bitキーを生成（テキストキー + 言語キー）
                    const std::uint_least64_t combined_key = createCombinedKey(text_key, language_keys[i]);

                    // 辞書に追加（重複は無視）
                    if (!hasText(combined_key)) {
                        text_dictionary_.emplace(combined_key, columns[i]);
                    }

                    // フォールバック用キーを登録（最初に見つかった言語を使用）
                    if (!hasTextKey(text_key)) {
                        fallback_text_key_.emplace(text_key, combined_key);
                    }
                }
            }
        }
        Language() = default;
    };

}

#endif // !PAX_SAPIENTICA_SYSTEM_LANGUAGE_HPP
