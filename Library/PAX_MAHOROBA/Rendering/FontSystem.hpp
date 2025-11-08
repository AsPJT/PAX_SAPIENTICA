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
#include <string>

#include <PAX_GRAPHICA/Font.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/Key/LanguageKeys.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Type/UnorderedMap.hpp>

namespace paxs {

    /// @brief フォントプロファイル名定数
    /// @brief Font profile name constants
    namespace FontProfiles {
        constexpr const char* MAIN = "main";           // カレンダー・地名・人名
        constexpr const char* PULLDOWN = "pulldown";   // プルダウンメニュー
        constexpr const char* PINYIN = "pinyin";       // ピンイン表示
        constexpr const char* ENGLISH = "english";     // 英語表示
        constexpr const char* KOYOMI = "koyomi";       // 暦表示
        constexpr const char* UI_SMALL = "ui_small";   // 小さいUIテキスト
        constexpr const char* UI_MEDIUM = "ui_medium"; // 中サイズUIテキスト
        constexpr const char* UI_LARGE = "ui_large";   // 大きいUIテキスト
    }

    /// @brief 言語辞書のドメイン
    /// @brief Language dictionary domain
    enum class LanguageDomain : std::uint8_t {
        UI = 0,          //
        SIMULATION = 1,  // シミュレーションモデル用 / For simulation models
    };

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
        std::string default_font_path_;

        // 言語辞書管理
        paxs::UnorderedMap<LanguageDomain, paxs::Language> languages_;

        // 選択言語
        SelectLanguage select_language_;

        // フォント設定プロファイル
        struct FontProfile {
            std::uint_least8_t size;
            std::uint_least8_t buffer_thickness;
        };

        // プロファイル定義
        paxs::UnorderedMap<std::string, FontProfile> profiles_;

        // 初期化フラグ
        bool initialized_ = false;

        /// @brief コンストラクタ（private）
        FontSystem() = default;

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

            // レガシー互換プロファイル
            registerProfile(FontProfiles::PINYIN, 14, 2);
            registerProfile(FontProfiles::ENGLISH, 20, 2);

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
            static constexpr std::array path_list = {
                "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
                "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
                "Data/Font/noto-sans-sc/NotoSansSC-Regular.otf",
                "Data/Font/noto-sans-sc/NotoSansSC-Regular.otf",
                "Data/Font/noto-sans-kr/NotoSansKR-Regular.otf",
                "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
                "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
                "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
                "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
                "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
                "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
                "Data/Font/noto-sans/NotoSans-Regular.ttf",
                "Data/Font/noto-sans/NotoSans-Regular.ttf",
                "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
                "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
                "Data/Font/noto-sans/NotoSans-Regular.ttf",
                "Data/Font/noto-sans/NotoSans-Regular.ttf",
                "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
                "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
                "Data/Font/noto-sans-ar/NotoNaskhArabic-VariableFont_wght.ttf",
                "Data/Font/noto-sans-ar/NotoNaskhArabic-VariableFont_wght.ttf",
                "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf"
            };

            // 言語ごとのフォントパスを登録
            for (std::size_t i = 0; i < path_list.size(); ++i) {
                language_font_paths_[paxs::LanguageKeys::ALL_LANGUAGE_HASHES[i]] = path_list[i];
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

            // デフォルトフォントパスの設定
            default_font_path_ = "Data/Font/noto-sans-sc/NotoSansSC-Regular.otf";

            // デフォルトプロファイルの登録
            registerDefaultProfiles();

            // 言語フォントの設定
            setupLanguageFonts();

            // 言語辞書の読み込み
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("Languages"),
                [&](const std::string& path_) {
                    addLanguageDictionary(path_, LanguageDomain::UI);
                });

            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("SimulationModels"),
                [&](const std::string& path_) {
                    addLanguageDictionary(path_, LanguageDomain::SIMULATION);
                });

            initialized_ = true;
        }

        /// @brief リソース解放（テスト用）
        /// @brief Release resources (for testing)
        /// @details テストで状態をリセットしたい場合のみ使用 / Use only for resetting state in tests
        static void clearForTesting() {
            if (instance_ != nullptr) {
                instance_->font_cache_.clear();
                instance_->initialized_ = false;
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
            return getFont(select_language_.cgetKey(), profile_name);
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
            return getFont(select_language_.cgetKey(), size, buffer_thickness);
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
            auto it = font_cache_.find(cache_key);
            if (it != font_cache_.end()) {
                return &(it->second);
            }

            // フォントパスを取得
            std::string font_path;
            auto path_it = language_font_paths_.find(language_key);
            if (path_it != language_font_paths_.end()) {
                font_path = path_it->second;
            } else {
                // パスが見つからない場合はデフォルトフォントを使用
                font_path = default_font_path_;
            }

            // 新しいフォントを作成してキャッシュに追加
            font_cache_.emplace(
                cache_key,
                paxg::Font(static_cast<int>(size), font_path, static_cast<int>(buffer_thickness))
            );

            return &(font_cache_.at(cache_key));
        }

        // ========================================
        // 言語辞書API
        // Language dictionary API
        // ========================================

        /// @brief テキストを取得
        /// @brief Get text
        /// @param key テキストキー（例: "menu.file"） / Text key (e.g., "menu.file")
        /// @param domain 言語辞書のドメイン（デフォルト: UI） / Language dictionary domain (default: UI)
        /// @return テキストへのポインタ、失敗時は nullptr / Pointer to text, nullptr on failure
        const std::string* getText(const std::string& key,
                                   LanguageDomain domain = LanguageDomain::UI) const {
            return getText(paxs::MurMur3::calcHash(key), domain);
        }
        const std::string* getText(const char* const key,
                                   LanguageDomain domain = LanguageDomain::UI) const {
            return getText(paxs::MurMur3::calcHash(key), domain);
        }

        /// @brief テキストを取得（ハッシュキー版）
        /// @brief Get text (hash key version)
        /// @param key_hash テキストキーのMurMur3ハッシュ値 / MurMur3 hash value of text key
        /// @param domain 言語辞書のドメイン / Language dictionary domain
        /// @return テキストへのポインタ、失敗時は nullptr / Pointer to text, nullptr on failure
        const std::string* getText(std::uint_least32_t key_hash,
                                   LanguageDomain domain = LanguageDomain::UI) const {
            if (languages_.find(domain) == languages_.end()) {
                return nullptr;
            }
            return languages_.at(domain).getStringPtr(key_hash, select_language_.cgetKey());
        }

        /// @brief 言語辞書を追加
        /// @brief Add language dictionary
        /// @param file_path TSVファイルのパス / Path to TSV file
        /// @param domain 言語辞書のドメイン / Language dictionary domain
        void addLanguageDictionary(const std::string& file_path, LanguageDomain domain) {
            languages_[domain].add(file_path);
        }

        // ========================================
        // 言語選択API
        // Language selection API
        // ========================================

        /// @brief 言語をインデックスで設定
        /// @brief Set language by index
        /// @param language_index 言語インデックス（0から始まる） / Language index (starting from 0)
        void setLanguage(std::size_t language_index) {
            select_language_.set(language_index);
        }

        /// @brief 言語をキーで設定
        /// @brief Set language by key
        /// @param language_key 言語キー（MurMur3ハッシュ値） / Language key (MurMur3 hash value)
        void setLanguageByKey(std::uint_least32_t language_key) {
            select_language_.setKey(language_key);
        }

        /// @brief 選択言語を取得
        /// @brief Get selected language
        /// @return SelectLanguage への const 参照 / const reference to SelectLanguage
        const SelectLanguage& getSelectedLanguage() const {
            return select_language_;
        }

        // ========================================
        // フォントプロファイル管理API
        // Font profile management API
        // ========================================

        /// @brief カスタムプロファイルを登録
        /// @brief Register custom profile
        /// @param name プロファイル名 / Profile name
        /// @param size フォントサイズ / Font size
        /// @param buffer_thickness バッファー厚 / Buffer thickness
        void registerProfile(const std::string& name,
                             std::uint_least8_t size,
                             std::uint_least8_t buffer_thickness) {
            profiles_[name] = FontProfile{ size, buffer_thickness };
        }

        /// @brief プロファイルが存在するか確認
        /// @brief Check if profile exists
        /// @param name プロファイル名 / Profile name
        /// @return 存在すれば true / true if exists
        bool hasProfile(const std::string& name) const {
            return profiles_.find(name) != profiles_.end();
        }

    };

    inline FontSystem* FontSystem::instance_ = nullptr;

    /// @brief グローバルアクセス用ヘルパー関数
    inline FontSystem& Fonts() {
        return FontSystem::getInstance();
    }

} // namespace paxs

#endif // !PAX_MAHOROBA_FONT_SYSTEM_HPP
