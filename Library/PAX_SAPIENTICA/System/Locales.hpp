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
#include <PAX_SAPIENTICA/Interface/IProgressReporter.hpp>
#include <PAX_SAPIENTICA/IO/Data/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/System/InputFile.hpp>
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
        /// @brief 進捗報告インターフェース
        /// @brief Progress reporter interface
        IProgressReporter* progress_reporter_ = nullptr;

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
        /// @param domain_name ドメイン名 / Domain name
        /// @param locale_name ロケール名 / Locale name
        /// @param file_path ファイルパス / File path
        void loadFile(const std::string& domain_name, const std::string& locale_name, const std::string& file_path) {
            KeyValueTSV<std::string> kv_tsv;
            if (!kv_tsv.input(file_path)) {
                // en-US ファイルが存在しない場合のみエラー、その他の言語はスキップ
                if (locale_name == default_locale_name_) {
                    PAXS_ERROR("Required en-US locale file not found: " + file_path);
                }
                return;
            }

            // ドメイン名とロケール名のキーを計算
            const std::uint_least32_t domain_key = MurMur3::calcHash(
                domain_name.size(),
                domain_name.c_str()
            );
            const std::uint_least32_t locale_key = MurMur3::calcHash(
                locale_name.size(),
                locale_name.c_str()
            );

            // KeyValueTSVから全エントリを取得
            UnorderedMap<std::uint_least32_t, std::string>& entries = kv_tsv.get();
            std::size_t line_count = 0;

            // ハッシュテーブルを事前予約（リハッシュを回避）
            text_dictionary_.reserve(text_dictionary_.size() + entries.size());

            for (auto& [text_key, value_str] : entries) {
                // 辞書に追加
                const CombinedKey combined_key(domain_key, text_key, locale_key);
                const auto [iter, inserted] = text_dictionary_.try_emplace(combined_key, std::move(value_str));
                (void)iter;  // 未使用変数の警告を抑制
                if (inserted) {
                    ++line_count;
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
                const auto [iter, inserted] = locale_key_to_index_.try_emplace(locale_key, ordered_locales_.size());
                (void)iter;  // 未使用変数の警告を抑制
                if (inserted) {
                    ordered_locales_.emplace_back(locale_key);
                    locale_key_to_name_.emplace(locale_key, locale_name);
                } else {
                    PAXS_WARNING("Duplicate locale key in Locales.tsv: " + locale_name);
                }
            });
        }

        /// @brief ファイルパスからドメイン名を取得
        /// @brief Get domain name from file path using domain.tsv
        /// @param file_path ファイルパス / File path
        /// @param locales_dir ロケールディレクトリのルート / Locales directory root
        /// @return ドメイン名、見つからない場合は空文字列 / Domain name, empty string if not found
        std::string getDomainNameFromPath(const std::string& file_path, const std::string& locales_dir) {
            // ファイルパスからディレクトリパスを抽出
            const std::size_t last_slash = file_path.find_last_of("/\\");
            if (last_slash == std::string::npos) {
                return std::string{};
            }
            const std::string directory_path = file_path.substr(0, last_slash);

            // 親ディレクトリパスを取得
            const std::size_t parent_slash = directory_path.find_last_of("/\\");
            if (parent_slash == std::string::npos) {
                return std::string{};
            }
            const std::string parent_directory_path = directory_path.substr(0, parent_slash);

            // domain.tsv のパスを構築
            const std::string domain_tsv_path = parent_directory_path + "/domain.tsv";

            if (!FileSystem::exists(domain_tsv_path)) {
                return std::string{};
            }

            // domain.tsv を読み込む
            KeyValueTSV<std::string> kv_tsv;
            if (!kv_tsv.input(domain_tsv_path)) {
                return std::string{};
            }

            // ディレクトリ名を取得（最後のパス要素）
            const std::string dir_name = directory_path.substr(parent_slash + 1);

            // ディレクトリ名のハッシュを計算
            const std::uint_least32_t dir_name_hash = MurMur3::calcHash(dir_name.size(), dir_name.c_str());

            // マッピングから取得
            const UnorderedMap<std::uint_least32_t, std::string>& entries = kv_tsv.get();
            const auto it = entries.find(dir_name_hash);
            if (it != entries.end()) {
                return it->second;
            }

            return std::string{};
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

                // domain.tsv からドメイン名を取得
                const std::string domain_name = getDomainNameFromPath(file_path, locales_dir);
                if (domain_name.empty()) {
                    PAXS_WARNING("domain.tsv not found or domain mapping not found for: " + file_path);
                    continue;  // ドメイン名が取得できない場合はスキップ
                }

                // ファイルを読み込む（ドメイン名を使用）
                loadFile(domain_name, locale_name, file_path);
                ++file_count;
            }
        }

    public:
        /// @brief コンストラクタ
        /// @brief Constructor
        /// @param reporter 進捗報告インターフェース（オプショナル） / Progress reporter (optional)
        /// @param progress_start 進捗開始値（0.0～1.0） / Progress start value (0.0 to 1.0)
        /// @param progress_end 進捗終了値（0.0～1.0） / Progress end value (0.0 to 1.0)
        explicit Locales(IProgressReporter* reporter = nullptr, float progress_start = 0.0f, float progress_end = 1.0f)
            : progress_reporter_(reporter) {

            if (progress_reporter_) {
                progress_reporter_->reportProgress(progress_start, "Loading locale list...");
            }
            loadLocaleList();

            const float progress_mid = progress_start + (progress_end - progress_start) * 0.1f;
            if (progress_reporter_) {
                progress_reporter_->reportProgress(progress_mid, "Loading locale files...");
            }
            loadAllDomains();

            if (progress_reporter_) {
                progress_reporter_->reportProgress(progress_end, "Locales initialized");
            }
        }

        /// @brief テキストを取得（ドメイン名ハッシュ版）
        /// @brief Get text string (domain name hash version)
        /// @param domain_name_hash ドメイン名のハッシュ値 / Domain name hash
        /// @param text_key_hash テキストキーのハッシュ値 / Text key hash
        /// @param locale_key ロケールキーのハッシュ値 / Locale key hash
        /// @param suppress_warning 警告を抑制するか（デフォルト: false） / Suppress warning (default: false)
        /// @return テキストへのポインタ、見つからない場合はnullptr
        ///         Pointer to text string, nullptr if not found
        const std::string* getStringPtr(
            const std::uint_least32_t domain_name_hash,
            const std::uint_least32_t text_key_hash,
            const std::uint_least32_t locale_key,
            const bool suppress_warning = false) const {

            // 指定されたロケールでテキストを検索
            const CombinedKey combined_key{domain_name_hash, text_key_hash, locale_key};
            const auto iterator = text_dictionary_.find(combined_key);
            if (iterator != text_dictionary_.end()) {
                return &(iterator->second);
            }

            // フォールバック処理：en-USで再検索
            const CombinedKey fallback_key{domain_name_hash, text_key_hash, getDefaultLocaleKey()};
            const auto fallback_text_it = text_dictionary_.find(fallback_key);
            if (fallback_text_it == text_dictionary_.end()) {
                // テキストキー自体が登録されていない
                if (!suppress_warning) {
                    PAXS_WARNING("[Locales::getStringPtr] Text key NOT found in default locale (en-US) - returning nullptr");
                }
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

        /// @brief ロケールキーが有効かを検証
        /// @brief Check if locale key is valid
        /// @param key ロケールキー / Locale key
        /// @return 有効であれば true / true if valid
        [[nodiscard]] bool isValidLocaleKey(std::uint_least32_t key) const {
            return locale_key_to_index_.contains(key);
        }

        /// @brief ロケールキーからインデックスへのマッピングを取得
        /// @brief Get locale key to index mapping
        /// @return ロケールキー → インデックス のマップ / Map from locale key to index
        [[nodiscard]] const paxs::UnorderedMap<std::uint_least32_t, std::size_t>& getLocaleKeyToIndex() const {
            return locale_key_to_index_;
        }

        /// @brief ロケールキーから名前へのマッピングを取得
        /// @brief Get locale key to name mapping
        /// @return ロケールキー → 名前 のマップ / Map from locale key to name
        [[nodiscard]] const paxs::UnorderedMap<std::uint_least32_t, std::string>& getLocaleKeyToName() const {
            return locale_key_to_name_;
        }

        /// @brief デフォルトロケールのキーを取得
        /// @brief Get default locale key
        /// @return デフォルトロケールのキー / Default locale key
        [[nodiscard]] static std::uint_least32_t getDefaultLocaleKey() {
            return MurMur3::calcHash(default_locale_name_);
        }

        /// @brief ロケール名からキーを取得
        /// @brief Get locale key from name
        /// @param locale_name ロケール名（例: "en-US"） / Locale name (e.g., "en-US")
        /// @return ロケールキー / Locale key
        [[nodiscard]] static std::uint_least32_t getKeyByName(const std::string& locale_name) {
            return MurMur3::calcHash(locale_name.c_str());
        }

    };

}

#endif // !PAX_SAPIENTICA_SYSTEM_LOCALES_HPP
