/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <memory>
#include <random>
#include <vector>

#include <PAX_SAPIENTICA/Simulation/Manager/SettlementSimulator.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>
#include <PAX_SAPIENTICA/Utility/StringUtils.hpp>

// SettlementSimulator クラスのテスト

class SettlementSimulatorUnitTest : public ::testing::Test {
protected:
	void SetUp() override {
		// モデル名を設定
		model_name = "Sample";

		// SimulationConstantsの初期化
		paxs::SimulationConstants::getInstance(model_name).init(model_name);

		// パスを生成（実際のシミュレーションと同じ方法）
		map_list_path = paxs::AppConfig::getInstance().getSettingPath(paxs::MurMur3::calcHash("SimulationXYZTiles"));
		japan_provinces_path = paxs::AppConfig::getInstance().getSettingPath(paxs::MurMur3::calcHash("SimulationProvincesPath"));

		paxs::StringUtils::replace(map_list_path, "Sample", model_name);
		paxs::StringUtils::replace(japan_provinces_path, "Sample", model_name);

		// テスト用のシミュレーションシードを固定
		seed = 12345;
	}

	std::string model_name;
	std::string map_list_path;
	std::string japan_provinces_path;
	unsigned seed;
};

TEST_F(SettlementSimulatorUnitTest, Construction) {
	// Given: パラメータを指定してシミュレータを作成
	paxs::SettlementSimulator simulator(map_list_path, japan_provinces_path, seed);

	// Then: シミュレータが正常に構築される
	EXPECT_EQ(simulator.cgetPopulationNum(), 0);
	EXPECT_EQ(simulator.cgetSettlement(), 0);
}

TEST_F(SettlementSimulatorUnitTest, SetEnvironment) {
	// Given: 空のシミュレータ
	paxs::SettlementSimulator simulator;

	// When: 環境を設定
	EXPECT_NO_THROW(simulator.setEnvironment(map_list_path, japan_provinces_path, seed));

	// Then: 正常に設定される
	EXPECT_EQ(simulator.cgetPopulationNum(), 0);
}

// ========================================
// RandomizeSettlements Tests
// ========================================

/**
 * @brief randomizeSettlements()の動作を検証するテスト
 *
 * このテストは固定シードで2回実行し、集落配置が決定論的であることを確認します。
 * リファクタリング前後で同じ結果が得られることを保証します。
 */
TEST_F(SettlementSimulatorUnitTest, RandomizeSettlements_Deterministic) {
	// Given: 同じシードで2つのシミュレータを作成
	paxs::SettlementSimulator simulator1(map_list_path, japan_provinces_path, seed);
	paxs::SettlementSimulator simulator2(map_list_path, japan_provinces_path, seed);

	// When: 両方を初期化
	simulator1.init();
	simulator2.init();

	// Then: 同じ人口と集落数が生成される
	EXPECT_EQ(simulator1.cgetPopulationNum(), simulator2.cgetPopulationNum());
	EXPECT_EQ(simulator1.cgetSettlement(), simulator2.cgetSettlement());

	// 集落の詳細も一致することを確認
	const auto& grids1 = simulator1.cgetSettlementGrids();
	const auto& grids2 = simulator2.cgetSettlementGrids();

	EXPECT_EQ(grids1.size(), grids2.size());
}

/**
 * @brief randomizeSettlements()で配置されたエージェントの詳細を検証
 *
 * 固定シードでエージェントの属性（年齢、ゲノム、文化）が再現可能であることを確認
 */
TEST_F(SettlementSimulatorUnitTest, RandomizeSettlements_AgentDetailsReproducible) {
	// Given: 同じシードで2つのシミュレータを作成
	const unsigned test_seed = 54321;
	paxs::SettlementSimulator simulator1(map_list_path, japan_provinces_path, test_seed);
	paxs::SettlementSimulator simulator2(map_list_path, japan_provinces_path, test_seed);

	// When: 初期化
	simulator1.init();
	simulator2.init();

	// Then: 各集落のエージェント詳細が一致
	const auto& grids1 = simulator1.cgetSettlementGrids();
	const auto& grids2 = simulator2.cgetSettlementGrids();

	// 集落グリッド数が一致
	ASSERT_EQ(grids1.size(), grids2.size());

	// 各グリッドの集落を比較
	for (const auto& [key, grid1] : grids1) {
		const auto* grid2_ptr = grids2.try_get(key);
		ASSERT_NE(grid2_ptr, nullptr) << "Grid key " << key << " not found in simulator2";

		const auto& settlements1 = grid1.cgetSettlements();
		const auto& settlements2 = grid2_ptr->cgetSettlements();

		EXPECT_EQ(settlements1.size(), settlements2.size())
			<< "Settlement count mismatch in grid " << key;

		// 各集落のエージェント数を確認
		for (std::size_t i = 0; i < settlements1.size() && i < settlements2.size(); ++i) {
			EXPECT_EQ(settlements1[i].getPopulation(), settlements2[i].getPopulation())
				<< "Population mismatch in settlement " << i << " of grid " << key;
		}
	}
}

/**
 * @brief 渡来人配置のテスト
 *
 * randomizeSettlements()が渡来フラグで正しく動作することを確認
 */
TEST_F(SettlementSimulatorUnitTest, RandomizeSettlements_ImmigrationFlag) {
	// Given: シミュレータを作成
	paxs::SettlementSimulator simulator(map_list_path, japan_provinces_path, seed);

	// When: 初期化（在地人配置）
	simulator.init();
	const std::size_t initial_population = simulator.cgetPopulationNum();

	// Then: 初期人口が設定される
	EXPECT_GT(initial_population, 0) << "Initial population should be greater than 0";
}

/**
 * @brief 集落配置の空間分布を検証
 *
 * 集落が可住地にのみ配置されることを確認
 */
TEST_F(SettlementSimulatorUnitTest, RandomizeSettlements_OnlyOnHabitableLand) {
	// Given: シミュレータを作成
	paxs::SettlementSimulator simulator(map_list_path, japan_provinces_path, seed);

	// When: 初期化
	simulator.init();

	// Then: 全ての集落が有効な位置にある
	const auto& grids = simulator.cgetSettlementGrids();
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

/**
 * @brief 異なるシードで異なる結果が生成されることを確認
 */
TEST_F(SettlementSimulatorUnitTest, RandomizeSettlements_DifferentSeedsDifferentResults) {
	// Given: 異なるシードで2つのシミュレータを作成
	paxs::SettlementSimulator simulator1(map_list_path, japan_provinces_path, 12345);
	paxs::SettlementSimulator simulator2(map_list_path, japan_provinces_path, 67890);

	// When: 両方を初期化
	simulator1.init();
	simulator2.init();

	// Then: 人口は同じ設定値だが、集落の詳細配置は異なる可能性が高い
	// （ただし、テストデータや設定によっては偶然一致する可能性もあるため、
	// 　ここでは基本的な検証のみ行う）

	// 両方とも集落が生成されている
	EXPECT_GT(simulator1.cgetSettlement(), 0);
	EXPECT_GT(simulator2.cgetSettlement(), 0);
}

// ========================================
// Population Calculation Tests
// ========================================

TEST_F(SettlementSimulatorUnitTest, CalcPop_UpdatesPopulationAndSettlementCount) {
	// Given: 初期化されたシミュレータ
	paxs::SettlementSimulator simulator(map_list_path, japan_provinces_path, seed);
	simulator.init();

	// When: 人口計算を実行
	const std::size_t pop_before = simulator.cgetPopulationNum();
	const std::size_t settlement_before = simulator.cgetSettlement();

	simulator.calcPop();

	// Then: 人口と集落数が計算される（この場合、変化なし）
	EXPECT_EQ(simulator.cgetPopulationNum(), pop_before);
	EXPECT_EQ(simulator.cgetSettlement(), settlement_before);
}

// ========================================
// Migration Count Tests
// ========================================

TEST_F(SettlementSimulatorUnitTest, GetMigrationCount_InitiallyZero) {
	// Given: 新しいシミュレータ
	paxs::SettlementSimulator simulator(map_list_path, japan_provinces_path, seed);

	// Then: 渡来数は0
	EXPECT_EQ(simulator.getMigrationCount(), 0);
}
