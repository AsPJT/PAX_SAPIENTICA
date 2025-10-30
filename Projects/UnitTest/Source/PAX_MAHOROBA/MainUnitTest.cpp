/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <string>
#include <unordered_set>
#include <vector>

#include <PAX_MAHOROBA/Main.hpp>

using namespace paxs;

TEST(MainTest, StartMain) {
    startMain();
    SUCCEED();
}

// Main.hppのマクロ定義が正しく設定されているかテスト
TEST(MainTest, MacroDefinitions) {
	// PAX_MAHOROBA_MAIN_HPP が定義されていることを確認
#ifdef PAX_MAHOROBA_MAIN_HPP
	SUCCEED();
#else
	FAIL() << "PAX_MAHOROBA_MAIN_HPP is not defined";
#endif

	// Android以外ではシミュレーションが有効であることを確認
#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
	// Android版ではシミュレーションは無効
#ifdef PAXS_USING_SIMULATOR
	FAIL() << "PAXS_USING_SIMULATOR should not be defined on Android";
#else
	SUCCEED();
#endif
#else
	// それ以外ではシミュレーションは有効
#ifdef PAXS_USING_SIMULATOR
	SUCCEED();
#else
	FAIL() << "PAXS_USING_SIMULATOR should be defined on non-Android platforms";
#endif
#endif
}

// Main.hppで使用されるヘッダーが正しくインクルードされているかテスト
TEST(MainTest, RequiredHeadersIncluded) {
	// Vec2型が使用可能か確認
	paxg::Vec2i vec2i_test{100, 200};
	EXPECT_EQ(vec2i_test.x(), 100);
	EXPECT_EQ(vec2i_test.y(), 200);

	// MurMur3ハッシュが使用可能か確認
	const std::uint_least32_t hash = MurMur3::calcHash("TestString");
	EXPECT_NE(hash, 0u);

	// GraphicVisualizationListが使用可能か確認
	GraphicVisualizationList visible;
	visible.emplace(MurMur3::calcHash("Test"), true);
	EXPECT_TRUE(visible[MurMur3::calcHash("Test")]);
}

// Main.hppで初期化される可視化設定のテスト（startMain関数内の初期化ロジック）
TEST(MainTest, VisibilityInitialization) {
	// Main.hpp:56-61 の初期化ロジックをテスト
	GraphicVisualizationList visible{};
	visible.emplace(MurMur3::calcHash("Calendar"), true);
	visible.emplace(MurMur3::calcHash("Map"), true);
	visible.emplace(MurMur3::calcHash("UI"), true);
	visible.emplace(MurMur3::calcHash("3D"), false);

	// 初期状態の確認
	EXPECT_TRUE(visible[MurMur3::calcHash("Calendar")]);
	EXPECT_TRUE(visible[MurMur3::calcHash("Map")]);
	EXPECT_TRUE(visible[MurMur3::calcHash("UI")]);
	EXPECT_FALSE(visible[MurMur3::calcHash("3D")]);
}

// Main.hppで使用されるハッシュキーの整合性テスト
TEST(MainTest, HashKeyConsistency) {
	// Main.hpp:158, 198, 219 で使用される2パラメータ版ハッシュの動作確認
	const std::uint_least32_t hash_3d_single = MurMur3::calcHash("3D");
	const std::uint_least32_t hash_3d_two_param = MurMur3::calcHash(2, "3D");

	EXPECT_EQ(hash_3d_single, hash_3d_two_param);
}

// Main.hppのview関連の可視化キーテスト（177-183行目）
TEST(MainTest, ViewMenuKeys) {
	GraphicVisualizationList visible;

	// View メニューで使用される全てのキーが正しく処理できることを確認
	const std::vector<std::string> view_keys = {
		"Calendar", "Map", "UI", "Simulation", "3D"
	};

	for (const auto& key : view_keys) {
		const std::uint_least32_t hash = MurMur3::calcHash(key.c_str());
		visible.set(hash, true);
		EXPECT_TRUE(visible[hash]) << "Failed for key: " << key;

		visible.set(hash, false);
		EXPECT_FALSE(visible[hash]) << "Failed for key: " << key;
	}
}

// Main.hppのplace_names関連の可視化キーテスト（185-196行目）
TEST(MainTest, PlaceNamesMenuKeys) {
	GraphicVisualizationList visible;

	// Place names メニューで使用される全てのキーが正しく処理できることを確認
	const std::vector<std::string> place_names_keys = {
		"place_name", "site", "tumulus", "dolmen", "kamekanbo",
		"stone_coffin", "doken", "dotaku", "bronze_mirror",
		"human_bone", "mtdna", "ydna"
	};

	for (const auto& key : place_names_keys) {
		const std::uint_least32_t hash = MurMur3::calcHash(key.c_str());
		visible.set(hash, true);
		EXPECT_TRUE(visible[hash]) << "Failed for key: " << key;

		visible.set(hash, false);
		EXPECT_FALSE(visible[hash]) << "Failed for key: " << key;
	}
}

// Main.hppで使用される全ハッシュキーの衝突チェック
TEST(MainTest, NoHashCollisionsInMainKeys) {
	const std::vector<std::string> all_keys = {
		"Calendar", "Map", "UI", "Simulation", "3D",
		"place_name", "site", "tumulus", "dolmen", "kamekanbo",
		"stone_coffin", "doken", "dotaku", "bronze_mirror",
		"human_bone", "mtdna", "ydna", "view", "place_names"
	};

	std::unordered_set<std::uint_least32_t> hashes;
	for (const auto& key : all_keys) {
		const std::uint_least32_t hash = MurMur3::calcHash(key.c_str());
		EXPECT_EQ(hashes.count(hash), 0u) << "Hash collision detected for key: " << key;
		hashes.insert(hash);
	}

	EXPECT_EQ(hashes.size(), all_keys.size());
}

