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

#include <PAX_SAPIENTICA/Simulation/Entity/Settlement.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/Environment.hpp>

// Settlement クラスの基本的なテスト

TEST(SettlementUnitTest, Construction) {
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();

	paxs::Settlement settlement(1, gen, environment);

	// IDが正しく設定されていることを確認
	EXPECT_EQ(settlement.getId(), 1);

	// 初期人口は0
	EXPECT_EQ(settlement.getPopulation(), 0);
}

TEST(SettlementUnitTest, AddAgent) {
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();

	paxs::Settlement settlement(1, gen, environment);

	// エージェントを追加
	paxs::SettlementAgent agent(1, 0, 100, paxs::Genome(), 0, 0, 0);
	settlement.addAgent(agent);

	// 人口が1になっているはず
	EXPECT_EQ(settlement.getPopulation(), 1);
}

TEST(SettlementUnitTest, Position) {
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();

	paxs::Settlement settlement(1, gen, environment);

	paxs::Vector2<paxs::GridType> pos(100, 200);
	settlement.setPosition(pos);

	EXPECT_EQ(settlement.getPosition(), pos);
}

TEST(SettlementUnitTest, ClearAgents) {
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();

	paxs::Settlement settlement(1, gen, environment);

	// エージェントを複数追加
	for (int i = 0; i < 5; ++i) {
		paxs::SettlementAgent agent(i, 0, 100, paxs::Genome(), 0, 0, 0);
		settlement.addAgent(agent);
	}

	EXPECT_EQ(settlement.getPopulation(), 5);

	// クリア
	settlement.clearAgents();

	EXPECT_EQ(settlement.getPopulation(), 0);
}

TEST(SettlementUnitTest, GetPopulationWeight) {
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();

	paxs::Settlement settlement(1, gen, environment);

	// 農耕民を追加
	paxs::SettlementAgent farming_agent(1, 0, 100, paxs::Genome(), 100, 0, 0);
	settlement.addAgent(farming_agent);

	// 狩猟採集民を追加
	paxs::SettlementAgent hunter_agent(2, 0, 100, paxs::Genome(), 0, 100, 0);
	settlement.addAgent(hunter_agent);

	// 重み人口が計算されているはず
	double weight = settlement.getPopulationWeight();
	EXPECT_GT(weight, 0.0);
}
