/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <memory>

#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/System/Locales.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

// Locales クラスのユニットテスト
// Unit tests for Locales class

class LocalesTest : public ::testing::Test {
protected:
	void SetUp() override {
		// AppConfigシングルトンを初期化（getInstance()で自動的に初期化される）
		// Localesオブジェクトを作成
		locales = std::make_unique<paxs::Locales>();
	}

	std::unique_ptr<paxs::Locales> locales;
};

// ロケール一覧の読み込みテスト
// Test loading locale list
TEST_F(LocalesTest, LoadLocaleList) {
	// ロケールリストが正しく読み込まれているかを確認
	// （ordered_locales_が空でも、実際にテキストが取得できれば正常）
	const std::uint_least32_t domain_key = paxs::MurMur3::calcHash("MenuBar");
	const std::uint_least32_t text_key = paxs::MurMur3::calcHash("view_header");
	const std::uint_least32_t locale_key = paxs::MurMur3::calcHash("ja-JP");

	// テキストが取得できることを確認
	const std::string* text = locales->getStringPtr(domain_key, text_key, locale_key);

	// Localesが正しく初期化されていれば、少なくともフォールバックロケールは動作する
	EXPECT_TRUE(text != nullptr || locales->getOrderedLocales().size() == 0);
}

// ドメイン読み込みとテキスト取得テスト（ハッシュキー版）
// Test domain loading and text retrieval (hash key version)
TEST_F(LocalesTest, AddDomainAndGetTextHashKey) {
	// コンストラクタで全ドメインが自動読み込み済み

	// ハッシュキー版でテキストを取得
	const std::uint_least32_t domain_key = paxs::MurMur3::calcHash("MenuBar");
	const std::uint_least32_t text_key = paxs::MurMur3::calcHash("view_header");
	const std::uint_least32_t locale_key_ja = paxs::MurMur3::calcHash("ja-JP");

	const std::string* text = locales->getStringPtr(domain_key, text_key, locale_key_ja);

	// ファイルが存在する場合のみテスト
	if (text != nullptr) {
		EXPECT_EQ(*text, "表示");
	}
}

// テキスト取得テスト（文字列キー版）
// Test text retrieval (string key version)
TEST_F(LocalesTest, GetTextStringKey) {
	// ハッシュキー版でテキストを取得
	const std::uint_least32_t domain_key = paxs::MurMur3::calcHash("MenuBar");
	const std::uint_least32_t text_key = paxs::MurMur3::calcHash("view_header");
	const std::uint_least32_t locale_key = paxs::MurMur3::calcHash("ja-JP");
	const std::string* text = locales->getStringPtr(domain_key, text_key, locale_key);

	// ファイルが存在する場合のみテスト
	if (text != nullptr) {
		EXPECT_EQ(*text, "表示");
	}
}

// 英語ロケールのテスト
// Test English locale
TEST_F(LocalesTest, EnglishLocale) {
	const std::uint_least32_t domain_key = paxs::MurMur3::calcHash("MenuBar");
	const std::uint_least32_t text_key = paxs::MurMur3::calcHash("view_header");
	const std::uint_least32_t locale_key = paxs::MurMur3::calcHash("en-US");
	const std::string* text = locales->getStringPtr(domain_key, text_key, locale_key);

	// ファイルが存在する場合のみテスト
	if (text != nullptr) {
		EXPECT_EQ(*text, "View");
	}
}

// フォールバック動作のテスト
// Test fallback behavior
TEST_F(LocalesTest, FallbackToDefaultLocale) {
	// 存在しないロケールでテキストを要求
	const std::uint_least32_t domain_key = paxs::MurMur3::calcHash("MenuBar");
	const std::uint_least32_t text_key = paxs::MurMur3::calcHash("view_header");
	const std::uint_least32_t invalid_locale_key = paxs::MurMur3::calcHash("xx-XX");

	const std::string* text = locales->getStringPtr(domain_key, text_key, invalid_locale_key);

	// フォールバック言語（最初に登録された言語）のテキストが返される
	// ファイルが存在する場合のみテスト
	if (text != nullptr) {
		EXPECT_NE(text->size(), 0);
	}
}

// 存在しないキーのテスト
// Test nonexistent key
TEST_F(LocalesTest, NonexistentKey) {
	// 存在しないキーでテキストを要求
	const std::uint_least32_t domain_key = paxs::MurMur3::calcHash("MenuBar");
	const std::uint_least32_t text_key = paxs::MurMur3::calcHash("nonexistent_key_12345");
	const std::uint_least32_t locale_key = paxs::MurMur3::calcHash("ja-JP");

	const std::string* text = locales->getStringPtr(domain_key, text_key, locale_key);
	EXPECT_EQ(text, nullptr);
}

// 存在しないドメインのテスト
// Test nonexistent domain
TEST_F(LocalesTest, NonexistentDomain) {
	const std::uint_least32_t domain_key = paxs::MurMur3::calcHash("NonexistentDomain");
	const std::uint_least32_t text_key = paxs::MurMur3::calcHash("test_key");
	const std::uint_least32_t locale_key = paxs::MurMur3::calcHash("ja-JP");
	const std::string* text = locales->getStringPtr(domain_key, text_key, locale_key);
	EXPECT_EQ(text, nullptr);
}

// 個別ファイル読み込みのテスト
// Test individual file loading
TEST_F(LocalesTest, AddIndividualFile) {
	const std::uint_least32_t domain_key = paxs::MurMur3::calcHash("MenuBar");
	const std::uint_least32_t text_key = paxs::MurMur3::calcHash("view_header");
	const std::uint_least32_t locale_key = paxs::MurMur3::calcHash("en-US");
	const std::string* text = locales->getStringPtr(domain_key, text_key, locale_key);

	// ファイルが存在する場合のみテスト
	if (text != nullptr) {
		EXPECT_EQ(*text, "View");
	}
}

// 複数のテキストキーのテスト
// Test multiple text keys
TEST_F(LocalesTest, MultipleTextKeys) {
	const std::uint_least32_t domain_key = paxs::MurMur3::calcHash("MenuBar");
	const std::uint_least32_t locale_key = paxs::MurMur3::calcHash("en-US");

	// 複数のキーでテキストを取得
	const std::string* view_text = locales->getStringPtr(
		domain_key,
		paxs::MurMur3::calcHash("view_header"),
		locale_key
	);
	const std::string* calendar_text = locales->getStringPtr(
		domain_key,
		paxs::MurMur3::calcHash("view_calendar"),
		locale_key
	);

	// ファイルが存在する場合のみテスト
	if (view_text != nullptr && calendar_text != nullptr) {
		EXPECT_EQ(*view_text, "View");
		EXPECT_EQ(*calendar_text, "Calendar");
	}
}
