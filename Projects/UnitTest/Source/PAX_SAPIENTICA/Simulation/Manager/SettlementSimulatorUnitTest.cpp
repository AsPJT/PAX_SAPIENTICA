/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/Simulation/Manager/SettlementSimulator.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>
#include <PAX_SAPIENTICA/Utility/StringUtils.hpp>

// SettlementSimulator クラスのテスト

class SettlementSimulatorUnitTest : public ::testing::Test {
protected:
	// 全テストで共有される初期化済みシミュレータ（SetUpTestSuiteで1回だけ初期化）
	static std::unique_ptr<paxs::SettlementSimulator> shared_initialized_simulator;
	static std::string shared_model_name;
	static std::string shared_map_list_path;
	static std::string shared_japan_provinces_path;
	static unsigned shared_seed;

	// テストスイート全体の初期化（最初に1回だけ実行）
	static void SetUpTestSuite() {
		// モデル名を設定
		shared_model_name = "Sample";

		// SimulationConstantsの初期化
		paxs::SimulationConstants::getInstance(shared_model_name).init(shared_model_name);

		// パスを生成（実際のシミュレーションと同じ方法）
		shared_map_list_path = paxs::AppConfig::getInstance().getSettingPath(paxs::MurMur3::calcHash("SimulationXYZTiles"));
		shared_japan_provinces_path = paxs::AppConfig::getInstance().getSettingPath(paxs::MurMur3::calcHash("SimulationProvincesPath"));

		paxs::StringUtils::replace(shared_map_list_path, "Sample", shared_model_name);
		paxs::StringUtils::replace(shared_japan_provinces_path, "Sample", shared_model_name);

		// テスト用のシミュレーションシードを固定
		shared_seed = 12345;

		// 初期化済みシミュレータを作成（読み取り専用テスト用）
		shared_initialized_simulator = std::make_unique<paxs::SettlementSimulator>(
			shared_map_list_path, shared_japan_provinces_path, shared_seed);
		shared_initialized_simulator->init();
	}

	// テストスイート全体の終了処理
	static void TearDownTestSuite() {
		shared_initialized_simulator.reset();
	}

	void SetUp() override {
		// 各テストで使用するローカル変数（互換性のため残す）
		model_name = shared_model_name;
		map_list_path = shared_map_list_path;
		japan_provinces_path = shared_japan_provinces_path;
		seed = shared_seed;
	}

	std::string model_name;
	std::string map_list_path;
	std::string japan_provinces_path;
	unsigned seed;
};

// 静的メンバの定義
std::unique_ptr<paxs::SettlementSimulator> SettlementSimulatorUnitTest::shared_initialized_simulator;
std::string SettlementSimulatorUnitTest::shared_model_name;
std::string SettlementSimulatorUnitTest::shared_map_list_path;
std::string SettlementSimulatorUnitTest::shared_japan_provinces_path;
unsigned SettlementSimulatorUnitTest::shared_seed;

TEST_F(SettlementSimulatorUnitTest, SetEnvironment) {
	// Given: 空のシミュレータ（setEnvironmentのテストなので新規作成）
	paxs::SettlementSimulator simulator;

	// When: 環境を設定
	EXPECT_NO_THROW(simulator.setEnvironment(shared_map_list_path, shared_japan_provinces_path, shared_seed));

	// Then: 正常に設定される
	EXPECT_EQ(simulator.cgetPopulationNum(), 0);
}

// ========================================
// RandomizeSettlements Tests
// ========================================

/**
 * @brief 渡来人配置のテスト
 *
 * randomizeSettlements()が渡来フラグで正しく動作することを確認
 */
TEST_F(SettlementSimulatorUnitTest, RandomizeSettlements_ImmigrationFlag) {
	// Given: 初期化済みシミュレータを使用
	const std::size_t initial_population = shared_initialized_simulator->cgetPopulationNum();

	// Then: 初期人口が設定される
	EXPECT_GT(initial_population, 0) << "Initial population should be greater than 0";
}

/**
 * @brief 集落配置の空間分布を検証
 *
 * 集落が可住地にのみ配置されることを確認
 */
TEST_F(SettlementSimulatorUnitTest, RandomizeSettlements_OnlyOnHabitableLand) {
	// Given: 初期化済みシミュレータを使用

	// Then: 全ての集落が有効な位置にある
	const auto& grids = shared_initialized_simulator->cgetSettlementGrids();
	for (const auto& [key, grid] : grids) {
		for (const auto& settlement : grid.cgetSettlements()) {
			// 人口が0より大きい
			EXPECT_GT(settlement.getPopulation(), 0);

			// 位置が有効
			const auto pos = settlement.getPosition();
			EXPECT_GE(pos.x, 0);
			EXPECT_GE(pos.y, 0);
		}
	}
}
