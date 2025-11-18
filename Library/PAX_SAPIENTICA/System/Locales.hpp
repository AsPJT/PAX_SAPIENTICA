/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SYSTEM_LOCALES_HPP
#define PAX_SAPIENTICA_SYSTEM_LOCALES_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/IO/Data/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/IO/Data/TsvTable.hpp>
#include <PAX_SAPIENTICA/IO/File/FileSystem.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief ロケール管理クラス
    /// @brief Locale management class
    class Locales {
    private:
        /// @brief デフォルトロケール名（フォールバック言語）
        /// @brief Default locale name (fallback language)
        static constexpr const char* default_locale_name_ = "en-US";

        /// @brief 3次元キー構造（ドメイン + テキスト + ロケール）
        /// @brief 3-dimensional key structure (domain + text + locale)
        struct CombinedKey {
            std::uint_least32_t domain_key;
            std::uint_least32_t text_key;
            std::uint_least32_t locale_key;

            bool operator==(const CombinedKey& other) const {
                return domain_key == other.domain_key
                    && text_key == other.text_key
                    && locale_key == other.locale_key;
            }
        };

        /// @brief CombinedKeyのハッシュ関数
        /// @brief Hash function for CombinedKey
        struct CombinedKeyHash {
            std::size_t operator()(const CombinedKey& key) const {
                return static_cast<std::size_t>(key.domain_key)
                    ^ (static_cast<std::size_t>(key.text_key) << 11)
                    ^ (static_cast<std::size_t>(key.locale_key) << 22);
            }
        };

        /// @brief フォールバック用2次元キー（ドメイン + テキスト）
        /// @brief 2-dimensional key for fallback (domain + text)
        struct DomainTextKey {
            std::uint_least32_t domain_key;
            std::uint_least32_t text_key;

            bool operator==(const DomainTextKey& other) const {
                return domain_key == other.domain_key && text_key == other.text_key;
            }
        };

        /// @brief DomainTextKeyのハッシュ関数
        /// @brief Hash function for DomainTextKey
        struct DomainTextKeyHash {
            std::size_t operator()(const DomainTextKey& key) const {
                return static_cast<std::size_t>(key.domain_key)
                    ^ (static_cast<std::size_t>(key.text_key) << 16);
            }
        };

        /// @brief 登録されたロケールのリスト（登録順）
        /// @brief List of registered locales (in registration order)
        std::vector<std::uint_least32_t> ordered_locales_;

        /// @brief ロケールキー → インデックス のマッピング
        /// @brief Mapping from locale key to index
        paxs::UnorderedMap<std::uint_least32_t, std::size_t> locale_key_to_index_;

        /// @brief ロケールキー → ロケール名 の逆引きマッピング
        /// @brief Reverse mapping from locale key to locale name
        paxs::UnorderedMap<std::uint_least32_t, std::string> locale_key_to_name_;

        /// @brief (ドメインキー + テキストキー + ロケールキー) → テキスト のマッピング
        /// @brief Mapping from (domain key + text key + locale key) to text
        paxs::UnorderedMap<CombinedKey, std::string, CombinedKeyHash> text_dictionary_;

        /// @brief (ドメインキー + テキストキー) → フォールバックキー のマッピング
        /// @brief Mapping from (domain key + text key) to fallback key
        paxs::UnorderedMap<DomainTextKey, CombinedKey, DomainTextKeyHash> fallback_text_key_;

        /// @brief ロケールキーからロケール名を取得
        /// @brief Get locale name from locale key
        /// @param locale_key ロケールキー / Locale key
        /// @return ロケール名、見つからない場合は空文字列 / Locale name, empty string if not found
        std::string getLocaleNameFromKey(std::uint_least32_t locale_key) const {
            const auto iterator = locale_key_to_name_.find(locale_key);
            if (iterator != locale_key_to_name_.end()) {
                return iterator->second;
            }
            return std::string{};
        }

        /// @brief ファイルからロケールデータを読み込む
        /// @brief Load locale data from file
        /// @param domain_path ドメインパス / Domain path
        /// @param locale_name ロケール名 / Locale name
        /// @param file_path ファイルパス / File path
        void loadFile(const std::string& domain_path, const std::string& locale_name, const std::string& file_path) {
            KeyValueTSV<std::string> kv_tsv;
            if (!kv_tsv.input(file_path)) {
                // en-US ファイルが存在しない場合のみエラー、その他の言語はスキップ
                if (locale_name == default_locale_name_) {
                    PAXS_ERROR("Required en-US locale file not found: " + file_path);
                }
                return;
            }

            // ドメインキーとロケールキーを計算
            const std::uint_least32_t domain_key = MurMur3::calcHash(
                domain_path.size(),
                domain_path.c_str()
            );
            const std::uint_least32_t locale_key = MurMur3::calcHash(
                locale_name.size(),
                locale_name.c_str()
            );

            // KeyValueTSVから全エントリを取得
            const UnorderedMap<std::uint_least32_t, std::string>& entries = kv_tsv.get();
            std::size_t line_count = 0;

            for (const auto& [text_key, value_str] : entries) {
                // 辞書に追加
                CombinedKey combined_key{domain_key, text_key, locale_key};
                if (!text_dictionary_.contains(combined_key)) {
                    text_dictionary_.emplace(combined_key, value_str);
                    ++line_count;
                }

                // フォールバック用キーを登録（en-US のみ）
                if (locale_name == default_locale_name_) {
                    DomainTextKey dt_key{domain_key, text_key};
                    if (!fallback_text_key_.contains(dt_key)) {
                        fallback_text_key_.emplace(dt_key, combined_key);
                    }
                }
            }
        }

        /// @brief ロケール一覧を読み込む
        /// @brief Load locale list
        void loadLocaleList() {
            const std::string locales_tsv_path = AppConfig::getInstance().getSettingPath(MurMur3::calcHash("Locales"));

            if (locales_tsv_path.empty()) {
                PAXS_WARNING("Locales.tsv path not found in Settings.tsv");
                return;
            }

            paxs::TsvTable table(locales_tsv_path);
            if (!table.isSuccessfullyLoaded()) {
                PAXS_WARNING("Failed to load Locales.tsv: " + locales_tsv_path);
                return;
            }

            // "key"カラムの存在チェック
            if (!table.hasColumn("key")) {
                PAXS_ERROR("Locales.tsv must have 'key' column");
                return;
            }

            const std::uint_least32_t key_hash = MurMur3::calcHash("key");

            // 各行を読み込む（keyカラムだけを読み込む）
            table.forEachRow([&](std::size_t row_index, const std::vector<std::string>& row) -> void {
                const std::string& locale_name = table.get(row_index, key_hash);

                if (locale_name.empty()) {
                    return;
                }

                const std::uint_least32_t locale_key = MurMur3::calcHash(locale_name.size(), locale_name.c_str());

                // 重複チェック
                if (!locale_key_to_index_.contains(locale_key)) {
                    locale_key_to_index_.emplace(locale_key, ordered_locales_.size());
                    ordered_locales_.emplace_back(locale_key);
                    locale_key_to_name_.emplace(locale_key, locale_name);
                } else {
                    PAXS_WARNING("Duplicate locale key in Locales.tsv: " + locale_name);
                }
            });
        }

        /// @brief Data/Locales/以下の全てのロケールファイルを再帰的に読み込む
        /// @brief Load all locale files under Data/Locales/ recursively
        void loadAllDomains() {
            const std::string locales_dir = "Data/Locales/";

            if (!FileSystem::exists(locales_dir)) {
                PAXS_WARNING("Data/Locales/ directory does not exist");
                return;
            }

            // Data/Locales/以下の全ファイルを再帰的に取得
            const std::vector<std::string> all_files = FileSystem::getFilePathsRecursive(locales_dir);
            std::size_t file_count = 0;

            for (const std::string& file_path : all_files) {
                // .tsvファイルのみを対象
                if (file_path.size() < 4 || file_path.substr(file_path.size() - 4) != ".tsv") {
                    continue;
                }

                // ファイル名を抽出（パスの最後の部分）
                const std::size_t last_slash = file_path.find_last_of("/\\");
                const std::string file_name = (last_slash != std::string::npos)
                    ? file_path.substr(last_slash + 1)
                    : file_path;

                // ファイル名からロケール名を抽出（例: "ja-JP.tsv" -> "ja-JP"）
                const std::string locale_name = file_name.substr(0, file_name.size() - 4);

                // 登録されているロケールかチェック
                const std::uint_least32_t locale_key = MurMur3::calcHash(locale_name.size(), locale_name.c_str());
                if (!locale_key_to_name_.contains(locale_key)) {
                    continue;  // 未登録のロケールはスキップ
                }

                // ドメインパスを抽出（Data/Locales/からの相対パス、ファイル名を除く）
                // 例: "Data/Locales/Features/Persons/Names/ja-JP.tsv" -> "Features/Persons/Names"
                std::string domain_path;
                if (file_path.size() > locales_dir.size() && last_slash != std::string::npos) {
                    const std::string path_after_locales = file_path.substr(locales_dir.size());
                    const std::size_t domain_slash = path_after_locales.find_last_of("/\\");
                    if (domain_slash != std::string::npos) {
                        domain_path = path_after_locales.substr(0, domain_slash);
                    }
                }

                // ファイルを読み込む
                loadFile(domain_path, locale_name, file_path);
                ++file_count;
            }
        }

    public:
        Locales() {
            loadLocaleList();
            loadAllDomains();
        }

        /// @brief テキストを取得（文字列キー版）
        /// @brief Get text string (string key version)
        /// @param domain_path ドメインパス / Domain path
        /// @param text_key テキストキー（文字列） / Text key (string)
        /// @param locale_name ロケール名 / Locale name
        /// @return テキストへのポインタ、見つからない場合はnullptr
        ///         Pointer to text string, nullptr if not found
        const std::string* getStringPtr(
            const std::string& domain_path,
            const std::string& text_key,
            const std::string& locale_name) const {

            const std::uint_least32_t domain_key = MurMur3::calcHash(
                domain_path.size(),
                domain_path.c_str()
            );
            const std::uint_least32_t text_key_hash = MurMur3::calcHash(
                text_key.size(),
                text_key.c_str()
            );
            const std::uint_least32_t locale_key = MurMur3::calcHash(
                locale_name.size(),
                locale_name.c_str()
            );

            return getStringPtr(domain_key, text_key_hash, locale_key);
        }

        /// @brief テキストを取得（ハッシュキー版）
        /// @brief Get text string (hash key version)
        /// @param domain_key ドメインキー（ハッシュ値） / Domain key (hash value)
        /// @param text_key テキストキー（ハッシュ値） / Text key (hash value)
        /// @param locale_key ロケールキー（ハッシュ値） / Locale key (hash value)
        /// @return テキストへのポインタ、見つからない場合はnullptr
        ///         Pointer to text string, nullptr if not found
        /// @details 指定言語で見つからない場合、フォールバック言語のテキストを返す
        ///          If not found in specified language, returns text in fallback language
        const std::string* getStringPtr(
            const std::uint_least32_t domain_key,
            const std::uint_least32_t text_key,
            const std::uint_least32_t locale_key) const {

            // 指定されたロケールでテキストを検索
            CombinedKey combined_key{domain_key, text_key, locale_key};
            const auto iterator = text_dictionary_.find(combined_key);
            if (iterator != text_dictionary_.end()) {
                return &(iterator->second);
            }

            // フォールバック処理
            DomainTextKey dt_key(domain_key, text_key);
            const auto fallback_it = fallback_text_key_.find(dt_key);
            if (fallback_it == fallback_text_key_.end()) {
                // テキストキー自体が登録されていない
                PAXS_WARNING("[Locales::getStringPtr] Fallback key NOT found - returning nullptr");
                return nullptr;
            }

            const CombinedKey& fallback_key = fallback_it->second;
            const auto fallback_text_it = text_dictionary_.find(fallback_key);
            if (fallback_text_it == text_dictionary_.end()) {
                // フォールバックテキストも見つからない（内部エラー）
                PAXS_ERROR("[Locales::getStringPtr] INTERNAL ERROR: fallback text not found in dictionary");
                return nullptr;
            }

            return &(fallback_text_it->second);
        }

        /// @brief 登録されているロケール一覧を取得
        /// @brief Get list of registered locales
        /// @return ロケールキーのベクタ / Vector of locale keys
        const std::vector<std::uint_least32_t>& getOrderedLocales() const {
            return ordered_locales_;
        }

        /// @brief ロケールキーからロケール名を取得（公開版）
        /// @brief Get locale name from locale key (public version)
        /// @param locale_key ロケールキー / Locale key
        /// @return ロケール名、見つからない場合は空文字列 / Locale name, empty string if not found
        std::string getLocaleNameFromKeyPublic(std::uint_least32_t locale_key) const {
            return getLocaleNameFromKey(locale_key);
        }
    };

}

#endif // !PAX_SAPIENTICA_SYSTEM_LOCALES_HPP
