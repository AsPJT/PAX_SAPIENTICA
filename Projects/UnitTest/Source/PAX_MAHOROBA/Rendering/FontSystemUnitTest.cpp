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

    // プロファイル確認（初期化前なので false）
    EXPECT_FALSE(Fonts().hasProfile(FontProfiles::MAIN));
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

// プロファイル登録テスト
TEST_F(FontSystemTest, RegisterProfile) {
    Fonts().initialize();

    Fonts().registerProfile("test_profile", 18, 3);
    EXPECT_TRUE(Fonts().hasProfile("test_profile"));
    EXPECT_FALSE(Fonts().hasProfile("nonexistent_profile"));
}

// デフォルトプロファイルテスト
TEST_F(FontSystemTest, DefaultProfiles) {
    Fonts().initialize();

    EXPECT_TRUE(Fonts().hasProfile(FontProfiles::MAIN));
    EXPECT_TRUE(Fonts().hasProfile(FontProfiles::PULLDOWN));
    EXPECT_TRUE(Fonts().hasProfile(FontProfiles::KOYOMI));
    EXPECT_TRUE(Fonts().hasProfile(FontProfiles::PINYIN));
    EXPECT_TRUE(Fonts().hasProfile(FontProfiles::ENGLISH));
    EXPECT_TRUE(Fonts().hasProfile(FontProfiles::UI_SMALL));
    EXPECT_TRUE(Fonts().hasProfile(FontProfiles::UI_MEDIUM));
    EXPECT_TRUE(Fonts().hasProfile(FontProfiles::UI_LARGE));
}

// 存在しないプロファイルでフォント取得
TEST_F(FontSystemTest, GetFontNonexistentProfile) {
    Fonts().initialize();

    auto* font = Fonts().getFont("nonexistent_profile");
    EXPECT_EQ(font, nullptr);
}

// 言語選択テスト
TEST_F(FontSystemTest, LanguageSelection) {
    Fonts().initialize();

    Fonts().setLanguage(0);
    EXPECT_EQ(Fonts().getSelectedLanguage().get(), 0);

    Fonts().setLanguage(1);
    EXPECT_EQ(Fonts().getSelectedLanguage().get(), 1);
}

// 言語キーで選択テスト
TEST_F(FontSystemTest, LanguageSelectionByKey) {
    Fonts().initialize();

    // Locales から登録されている言語リストを取得
    const std::vector<std::uint_least32_t>& locale_keys = Fonts().getLocales().getOrderedLocales();

    // 最初の言語 (en-US) を選択
    if (!locale_keys.empty()) {
        Fonts().setLanguageByKey(locale_keys[0]); // en-US
        EXPECT_EQ(Fonts().getSelectedLanguage().getKey(), locale_keys[0]);
    }

    // 2番目の言語 (ja-JP) を選択
    if (locale_keys.size() > 1) {
        Fonts().setLanguageByKey(locale_keys[1]); // ja-JP
        EXPECT_EQ(Fonts().getSelectedLanguage().getKey(), locale_keys[1]);
    }
}

// カスタムプロファイルの登録と使用
TEST_F(FontSystemTest, CustomProfileRegistration) {
    Fonts().initialize();

    Fonts().registerProfile("custom_large", 32, 4);
    EXPECT_TRUE(Fonts().hasProfile("custom_large"));
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
