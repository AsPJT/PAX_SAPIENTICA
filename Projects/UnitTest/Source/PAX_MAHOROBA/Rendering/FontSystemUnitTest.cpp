/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2025 As Project
    [Production]    2023-2025 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

namespace paxs {

class FontSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 各テスト前に初期化
        // Note: 実際のフォントファイルが必要なため、一部のテストはスキップされる可能性があります
    }

    void TearDown() override {
        // 各テスト後にクリーンアップ
        Fonts().clearForTesting();
    }
};

// Singleton パターンのテスト
TEST_F(FontSystemTest, SingletonInstance) {
    FontSystem& instance1 = FontSystem::getInstance();
    FontSystem& instance2 = FontSystem::getInstance();
    EXPECT_EQ(&instance1, &instance2);
}

// ヘルパー関数テスト
TEST_F(FontSystemTest, HelperFunction) {
    FontSystem& instance1 = FontSystem::getInstance();
    FontSystem& instance2 = Fonts();
    EXPECT_EQ(&instance1, &instance2);
}

// 初期化前のアクセス（安全性テスト）
TEST_F(FontSystemTest, AccessBeforeInitialization) {
    // 初期化前でもクラッシュしないことを確認
    EXPECT_FALSE(Fonts().isInitialized());
}

// 初期化テスト
TEST_F(FontSystemTest, Initialization) {
    EXPECT_FALSE(Fonts().isInitialized());
    Fonts().initialize();
    EXPECT_TRUE(Fonts().isInitialized());
}

// 多重初期化のテスト
TEST_F(FontSystemTest, MultipleInitialization) {
    Fonts().initialize();
    EXPECT_TRUE(Fonts().isInitialized());

    // 二重初期化は無視される
    Fonts().initialize();
    EXPECT_TRUE(Fonts().isInitialized());
}

// 存在しないプロファイルでフォント取得
TEST_F(FontSystemTest, GetFontNonexistentProfile) {
    Fonts().initialize();

    auto* font = Fonts().getFont("nonexistent_profile");
    EXPECT_EQ(font, nullptr);
}

// 言語選択テスト（キーベース）
TEST_F(FontSystemTest, LanguageSelection) {
    Fonts().initialize();

    // Locales から登録されている言語リストを取得
    const std::vector<std::uint_least32_t>& locale_keys = Fonts().getOrderedLocales();

    // 最初の言語 (en-US) を選択
    if (!locale_keys.empty()) {
        Fonts().setLanguageKey(locale_keys[0]);
        EXPECT_EQ(Fonts().getSelectedLanguageKey(), locale_keys[0]);
    }

    // 2番目の言語 (ja-JP) を選択
    if (locale_keys.size() > 1) {
        Fonts().setLanguageKey(locale_keys[1]);
        EXPECT_EQ(Fonts().getSelectedLanguageKey(), locale_keys[1]);
    }
}

// 言語キーで選択テスト（setLanguage と同じ動作を確認）
TEST_F(FontSystemTest, LanguageSelectionByKey) {
    Fonts().initialize();

    // Locales から登録されている言語リストを取得
    const std::vector<std::uint_least32_t>& locale_keys = Fonts().getOrderedLocales();

    // 最初の言語 (en-US) を選択
    if (!locale_keys.empty()) {
        Fonts().setLanguageKey(locale_keys[0]); // en-US
        EXPECT_EQ(Fonts().getSelectedLanguageKey(), locale_keys[0]);
    }

    // 2番目の言語 (ja-JP) を選択
    if (locale_keys.size() > 1) {
        Fonts().setLanguageKey(locale_keys[1]); // ja-JP
        EXPECT_EQ(Fonts().getSelectedLanguageKey(), locale_keys[1]);
    }
}

// デフォルトプロファイルでのフォント取得テスト
TEST_F(FontSystemTest, DefaultProfileFontRetrieval) {
    Fonts().initialize();

    // デフォルトプロファイルでフォントが取得できることを確認
    auto* main_font = Fonts().getFont(FontProfiles::MAIN);
    EXPECT_NE(main_font, nullptr);

    auto* pulldown_font = Fonts().getFont(FontProfiles::PULLDOWN);
    EXPECT_NE(pulldown_font, nullptr);

    auto* koyomi_font = Fonts().getFont(FontProfiles::KOYOMI);
    EXPECT_NE(koyomi_font, nullptr);
}

// 空のプロファイル名
TEST_F(FontSystemTest, EmptyProfileName) {
    Fonts().initialize();

    auto* font = Fonts().getFont("");
    EXPECT_EQ(font, nullptr);
}

// 初期化・シャットダウンサイクルテスト
TEST_F(FontSystemTest, InitializeShutdownCycle) {
    // 初回初期化
    EXPECT_FALSE(Fonts().isInitialized());
    Fonts().initialize();
    EXPECT_TRUE(Fonts().isInitialized());

    // シャットダウン
    Fonts().clearForTesting();
    EXPECT_FALSE(Fonts().isInitialized());

    // 再初期化
    Fonts().initialize();
    EXPECT_TRUE(Fonts().isInitialized());
}

} // namespace paxs
