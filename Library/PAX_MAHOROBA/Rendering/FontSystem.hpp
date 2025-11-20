/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2025 As Project
    [Production]    2023-2025 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_FONT_SYSTEM_HPP
#define PAX_MAHOROBA_FONT_SYSTEM_HPP

#include <cstdint>
#include <memory>
#include <string>

#include <PAX_GRAPHICA/Font.hpp>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Interface/IProgressReporter.hpp>
#include <PAX_SAPIENTICA/IO/Data/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/System/Locales.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief フォントプロファイル名定数
    /// @brief Font profile name constants
    // TODO: スネークケース
    namespace FontProfiles {
        constexpr const char* MAIN = "main";           // カレンダー・地名・人名
        constexpr const char* PULLDOWN = "pulldown";   // プルダウンメニュー
        constexpr const char* KOYOMI = "koyomi";       // 暦表示
        constexpr const char* UI_SMALL = "ui_small";   // 小さいUIテキスト
        constexpr const char* UI_MEDIUM = "ui_medium"; // 中サイズUIテキスト
        constexpr const char* UI_LARGE = "ui_large";   // 大きいUIテキスト
    }

    /// @brief フォント・言語統合管理シングルトンクラス
    /// @brief Font and Language Unified Management Singleton Class
    class FontSystem {
    private:
        // シングルトンインスタンス
        static FontSystem* instance_;

        // フォントキャッシュ（language_key + size + thickness → Font）
        paxs::UnorderedMap<std::uint_least64_t, paxg::Font> font_cache_;

        // 言語ごとのフォントパス
        paxs::UnorderedMap<std::uint_least32_t, std::string> language_font_paths_;

        // Locales システム
        std::unique_ptr<paxs::Locales> locales_;

        // 選択されている言語キー
        std::uint_least32_t selected_language_key_ = 0;

        // フォント設定プロファイル
        struct FontProfile {
            std::uint_least8_t size;
            std::uint_least8_t buffer_thickness;
        };

        // プロファイル定義
        paxs::UnorderedMap<std::string, FontProfile> profiles_;

        // 初期化フラグ
        bool initialized_ = false;
        bool locales_initialized_ = false;

        FontSystem() = default;

        /// @brief カスタムプロファイルを登録
        /// @brief Register custom profile
        /// @param name プロファイル名 / Profile name
        /// @param size フォントサイズ / Font size
        /// @param buffer_thickness バッファー厚 / Buffer thickness
        void registerProfile(const std::string& name,
                             std::uint_least8_t size,
                             std::uint_least8_t buffer_thickness) {
            profiles_.emplace(name, FontProfile( size, buffer_thickness ));
        }

        /// @brief プロファイルが存在するか確認
        /// @brief Check if profile exists
        /// @param name プロファイル名 / Profile name
        /// @return 存在すれば true / true if exists
        [[nodiscard]] bool hasProfile(const std::string& name) const {
            return profiles_.contains(name);
        }

        /// @brief デフォルトプロファイルの登録
        /// @brief Register default profiles
        void registerDefaultProfiles() {
            // PAX_GRAPHICA/FontConfig.hpp の値を使用
            registerProfile(FontProfiles::KOYOMI,
                            paxg::FontConfig::KOYOMI_FONT_SIZE,
                            paxg::FontConfig::KOYOMI_FONT_BUFFER_THICKNESS);

            registerProfile(FontProfiles::PULLDOWN,
                            paxg::FontConfig::PULLDOWN_FONT_SIZE,
                            paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS);

            // 新規プロファイル
            registerProfile(FontProfiles::UI_SMALL, 12, 2);
            registerProfile(FontProfiles::UI_MEDIUM, 16, 2);
            registerProfile(FontProfiles::UI_LARGE, 24, 2);

            // MAIN は KOYOMI と同じ設定
            registerProfile(FontProfiles::MAIN,
                            paxg::FontConfig::KOYOMI_FONT_SIZE,
                            paxg::FontConfig::KOYOMI_FONT_BUFFER_THICKNESS);
        }

        /// @brief フォントキャッシュキーを生成
        /// @brief Create font cache key
        constexpr std::uint_least64_t createFontCacheKey(
            std::uint_least32_t language_key,
            std::uint_least8_t size,
            std::uint_least8_t buffer_thickness) const {
            return (static_cast<std::uint_least64_t>(language_key) << 32)
                + (static_cast<std::uint_least64_t>(size) << 8)
                + (static_cast<std::uint_least64_t>(buffer_thickness));
        }

        /// @brief 言語フォントの設定
        /// @brief Setup language fonts
        void setupLanguageFonts() {
            const std::string font_tsv_path = paxs::AppConfig::getInstance().getSettingPath(paxs::MurMur3::calcHash("Font"));
            paxs::KeyValueTSV<std::string> font_tsv;

            if (!font_tsv.input(font_tsv_path)) {
                PAXS_WARNING("Failed to load Font.tsv: " + font_tsv_path);
                return;
            }

            const paxs::UnorderedMap<std::uint_least32_t, std::string>& font_entries = font_tsv.get();

            // 各言語に対してフォントパスを登録
            for (const auto& [language_key, font_path] : font_entries) {
                language_font_paths_.emplace(language_key, font_path);
            }
        }

    public:
        /// @brief デストラクタ
        ~FontSystem() = default;

        /// @brief シングルトンインスタンス取得
        /// @brief Get singleton instance
        /// @return FontSystem instance reference
        static FontSystem& getInstance() {
            if (instance_ == nullptr) {
                instance_ = new FontSystem();
            }
            return *instance_;
        }

        // コピー・移動禁止
        FontSystem(const FontSystem&) = delete;
        FontSystem& operator=(const FontSystem&) = delete;
        FontSystem(FontSystem&&) = delete;
        FontSystem& operator=(FontSystem&&) = delete;

        /// @brief 初期化
        /// @details アプリケーション起動時に1回だけ呼び出す / Call only once at application startup
        void initialize() {
            if (initialized_) {
                return;
            }

            // デフォルトプロファイルの登録
            registerDefaultProfiles();

            // 言語フォントの設定
            setupLanguageFonts();

            // デフォルト言語キーを設定（Localesの検証は後で行う）
            selected_language_key_ = Locales::getDefaultLocaleKey();

            initialized_ = true;
        }

        /// @brief Localesを初期化
        /// @details 時間がかかるため非同期で呼び出すことを推奨 / Recommended to call asynchronously due to time consumption
        /// @param reporter 進捗報告インターフェース（オプショナル） / Progress reporter (optional)
        /// @param progress_start 進捗開始値（0.0～1.0） / Progress start value (0.0 to 1.0)
        /// @param progress_end 進捗終了値（0.0～1.0） / Progress end value (0.0 to 1.0)
        void initializeLocales(paxs::IProgressReporter* reporter = nullptr, float progress_start = 0.0f, float progress_end = 1.0f) {
            if (locales_initialized_) {
                return;
            }

            // Localesシステムを初期化
            locales_ = std::make_unique<paxs::Locales>(reporter, progress_start, progress_end);

            // デフォルト言語が有効か確認
            if (!locales_->isValidLocaleKey(selected_language_key_)) {
                PAXS_ERROR("FontSystem::initializeLocales - Default locale 'en-US' is not registered in Locales.tsv");
            }

            locales_initialized_ = true;
        }

        /// @brief リソース解放（テスト用）
        /// @brief Release resources (for testing)
        /// @details テストで状態をリセットしたい場合のみ使用 / Use only for resetting state in tests
        static void clearForTesting() {
            if (instance_ != nullptr) {
                instance_->font_cache_.clear();
                instance_->locales_.reset();
                instance_->initialized_ = false;
                instance_->locales_initialized_ = false;
            }
        }

        /// @brief 初期化済みか確認
        bool isInitialized() const { return initialized_; }

        // ========================================
        // フォント取得API
        // Font retrieval API
        // ========================================

        /// @brief プロファイル名でフォントを取得（現在の選択言語）
        /// @brief Get font by profile name (current selected language)
        /// @param profile_name プロファイル名（例: "main", "pulldown"） / Profile name (e.g., "main", "pulldown")
        /// @return フォントへのポインタ、失敗時は nullptr / Pointer to font, nullptr on failure
        paxg::Font* getFont(const std::string& profile_name) {
            return getFont(selected_language_key_, profile_name);
        }

        /// @brief 指定言語のフォントを取得
        /// @brief Get font for specified language
        /// @param language_key 言語キー（MurMur3ハッシュ値） / Language key (MurMur3 hash value)
        /// @param profile_name プロファイル名 / Profile name
        /// @return フォントへのポインタ、失敗時は nullptr / Pointer to font, nullptr on failure
        paxg::Font* getFont(std::uint_least32_t language_key, const std::string& profile_name) {
            if (!hasProfile(profile_name)) {
                return nullptr;
            }
            const auto& profile = profiles_.at(profile_name);
            return getFont(language_key, profile.size, profile.buffer_thickness);
        }

        /// @brief 現在の選択言語でフォントを取得（サイズとバッファー厚を直接指定）
        /// @brief Get font for current selected language (direct size and buffer thickness specification)
        /// @param size フォントサイズ / Font size
        /// @param buffer_thickness バッファー厚 / Buffer thickness
        /// @return フォントへのポインタ、失敗時は nullptr / Pointer to font, nullptr on failure
        paxg::Font* getFont(std::uint_least8_t size, std::uint_least8_t buffer_thickness) {
            return getFont(selected_language_key_, size, buffer_thickness);
        }

        /// @brief 指定言語のフォントを取得（サイズとバッファー厚を直接指定）
        /// @brief Get font for specified language (direct size and buffer thickness specification)
        /// @param language_key 言語キー / Language key
        /// @param size フォントサイズ / Font size
        /// @param buffer_thickness バッファー厚 / Buffer thickness
        /// @return フォントへのポインタ、失敗時は nullptr / Pointer to font, nullptr on failure
        paxg::Font* getFont(std::uint_least32_t language_key,
                            std::uint_least8_t size,
                            std::uint_least8_t buffer_thickness) {
            const std::uint_least64_t cache_key = createFontCacheKey(language_key, size, buffer_thickness);

            // キャッシュに存在すればそれを返す
            const auto iterator = font_cache_.find(cache_key);
            if (iterator != font_cache_.end()) {
                return &(iterator->second);
            }

            // フォントパスを取得
            const auto path_it = language_font_paths_.find(language_key);

            if (path_it == language_font_paths_.end()) {
                PAXS_WARNING("FontSystem::getFont - Font path not found for language key: " + std::to_string(language_key));
                return nullptr;
            }

            // 新しいフォントを作成してキャッシュに追加
            font_cache_.emplace(
                cache_key,
                paxg::Font(static_cast<int>(size), path_it->second, static_cast<int>(buffer_thickness))
            );

            return &(font_cache_.at(cache_key));
        }

        // ========================================
        // 言語選択API
        // Language selection API
        // ========================================

        /// @brief 言語をキーで設定
        /// @brief Set language by key
        /// @param language_key 言語キー（MurMur3ハッシュ値） / Language key (MurMur3 hash value)
        [[nodiscard]] bool setLanguageKey(std::uint_least32_t language_key) {
            if (locales_initialized_ && locales_ && locales_->isValidLocaleKey(language_key)) {
                selected_language_key_ = language_key;
                return true;
            }
            PAXS_WARNING("FontSystem::setLanguageKey - Invalid key: " + std::to_string(language_key));
            return false;
        }

        /// @brief 選択されている言語キーを取得
        /// @brief Get selected language key
        /// @return 言語キー / Language key
        [[nodiscard]] constexpr std::uint_least32_t getSelectedLanguageKey() const {
            return selected_language_key_;
        }

        // ========================================
        // Locales システムアクセス
        // Locales system access
        // ========================================

        /// @brief 登録されているロケール一覧を取得
        /// @brief Get list of registered locales
        /// @return ロケールキーのベクタ / Vector of locale keys
        [[nodiscard]] const std::vector<std::uint_least32_t>& getOrderedLocales() const {
            if (locales_initialized_ && locales_) {
                return locales_->getOrderedLocales();
            }
            static const std::vector<std::uint_least32_t> empty_vector;
            return empty_vector;
        }

        /// @brief 現在選択されている言語でLocalesからテキストを取得（ハッシュキー版）
        /// @brief Get text from Locales with currently selected language (hash key version)
        /// @param domain_key ドメインキー（ハッシュ値） / Domain key (hash value)
        /// @param text_key テキストキー（ハッシュ値） / Text key (hash value)
        /// @param suppress_warning 警告を抑制するか（デフォルト: false） / Suppress warning (default: false)
        /// @return テキストへのポインタ、見つからない場合はnullptr / Pointer to text, nullptr if not found
        [[nodiscard]] const std::string* getLocalesText(
            const std::uint_least32_t domain_key,
            const std::uint_least32_t text_key,
            const bool suppress_warning = false) const {
            if (locales_initialized_ && locales_) {
                return locales_->getStringPtr(domain_key, text_key, selected_language_key_, suppress_warning);
            }
            return nullptr;
        }

    };

    inline FontSystem* FontSystem::instance_ = nullptr;

    /// @brief グローバルアクセス用ヘルパー関数
    inline FontSystem& Fonts() {
        return FontSystem::getInstance();
    }

} // namespace paxs

#endif // !PAX_MAHOROBA_FONT_SYSTEM_HPP
