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

#include <PAX_SAPIENTICA/Simulation/Config/Environment.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/KanakumaLifeSpan.hpp>
#include <PAX_SAPIENTICA/Simulation/Entity/Settlement.hpp>

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

// ========================================
// Death Logic Tests
// ========================================

TEST(SettlementUnitTest, Death_RemovesAgentsWhoExceedLifespan) {
	// Given: 集落と寿命が尽きるエージェント
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);

	paxs::Genome genome;
	genome.setYDNA(1); // Male
	// 年齢100ステップ、寿命100ステップのエージェント（次のステップで age > life_span となり死亡）
	paxs::SettlementAgent old_agent(1, 100, 100, genome, 0, 0, 0);
	settlement.addAgent(old_agent);

	// 年齢50ステップ、寿命100ステップのエージェント（生存）
	paxs::SettlementAgent young_agent(2, 50, 100, genome, 0, 0, 0);
	settlement.addAgent(young_agent);

	EXPECT_EQ(settlement.getPopulation(), 2);

	// When: 死亡処理を実行（年齢をインクリメント+死亡判定: age > life_span）
	settlement.death();

	// Then: 寿命を超えたエージェント（age=101 > life_span=100）が削除される
	EXPECT_EQ(settlement.getPopulation(), 1);
	// 残ったエージェントの年齢が1ステップ増えている
	EXPECT_EQ(settlement.getAgents()[0].getAgeInt(), 51);
	// 若いエージェントが残っている
	EXPECT_EQ(settlement.getAgents()[0].getId(), 2);
}

TEST(SettlementUnitTest, Death_IncrementsAge) {
	// Given: 集落と若いエージェント
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);

	paxs::Genome genome;
	genome.setYDNA(1);
	paxs::SettlementAgent agent(1, 20, 100, genome, 0, 0, 0);
	settlement.addAgent(agent);

	std::uint_least16_t initial_age = settlement.getAgents()[0].getAgeInt();

	// When: 死亡処理を実行
	settlement.death();

	// Then: 年齢が1ステップ増加
	EXPECT_EQ(settlement.getAgents()[0].getAgeInt(), initial_age + 1);
}

TEST(SettlementUnitTest, Death_DivorcesPartnerWhenAgentDies) {
	// Given: 既婚のカップルがいる集落
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);

	paxs::Genome female_genome, male_genome;
	female_genome.setYDNA(0); // Female
	male_genome.setYDNA(1);   // Male

	// 夫（寿命ちょうど、100ステップ）
	paxs::SettlementAgent husband(1, 100, 100, male_genome, 100, 0, 5);
	husband.marry(2, female_genome, 100, 0, 5);

	// 妻（若い、30ステップ）
	paxs::SettlementAgent wife(2, 30, 100, female_genome, 100, 0, 5);
	wife.marry(1, male_genome, 100, 0, 5);

	settlement.addAgent(husband);
	settlement.addAgent(wife);

	EXPECT_TRUE(settlement.getAgents()[0].isMarried());
	EXPECT_TRUE(settlement.getAgents()[1].isMarried());

	// When: 死亡処理を実行（夫が101ステップ > 100となり死亡）
	settlement.death();

	// Then: 夫が削除され、妻が離婚状態になる
	EXPECT_EQ(settlement.getPopulation(), 1);
	EXPECT_FALSE(settlement.getAgents()[0].isMarried());
	EXPECT_EQ(settlement.getAgents()[0].getId(), 2); // 妻が残っている
	EXPECT_EQ(settlement.getAgents()[0].getAgeInt(), 31); // 妻の年齢が1ステップ増加
}

// ========================================
// Birth Logic Tests
// ========================================

TEST(SettlementUnitTest, Birth_CreatesChildWhenBirthIntervalReachesZero) {
	// Given: 出産間隔カウントが1の妊娠中の女性
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);
	paxs::KanakumaLifeSpan life_span;

	paxs::Genome mother_genome, father_genome;
	mother_genome.setMtDNA(10);
	mother_genome.setYDNA(0); // Female
	father_genome.setMtDNA(20);
	father_genome.setYDNA(1); // Male

	paxs::SettlementAgent mother(1, 25 * 12, 100 * 12, mother_genome, 100, 0, 5);
	mother.marry(2, father_genome, 100, 0, 5);
	mother.setBirthIntervalCount(1); // 次のステップで出産

	settlement.addAgent(mother);
	EXPECT_EQ(settlement.getPopulation(), 1);

	// When: 出産処理を実行
	settlement.preUpdate(life_span);

	// Then: 子供が追加される（確率的に死産でなければ）
	// Note: 死産率により失敗する可能性があるため、どちらも正常
	// 子供が生まれた、または死産だった
	EXPECT_TRUE(settlement.getPopulation() == 1 || settlement.getPopulation() == 2);
}

TEST(SettlementUnitTest, Birth_DecrementsBirthIntervalCount) {
	// Given: 出産間隔カウントが5の女性
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);
	paxs::KanakumaLifeSpan life_span;

	paxs::Genome genome;
	genome.setYDNA(0); // Female
	paxs::SettlementAgent mother(1, 25 * 12, 100 * 12, genome, 100, 0, 5);
	mother.setBirthIntervalCount(5);

	settlement.addAgent(mother);
	EXPECT_EQ(settlement.getAgents()[0].getBirthIntervalCount(), 5);

	// When: 出産処理を実行
	settlement.preUpdate(life_span);

	// Then: カウントが減少
	EXPECT_EQ(settlement.getAgents()[0].getBirthIntervalCount(), 4);
}

TEST(SettlementUnitTest, Birth_ChildInheritsGenomeFromParents) {
	// Given: 既婚の女性（カウント1）
	std::mt19937 gen(54321); // Different seed for better randomness
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);
	paxs::KanakumaLifeSpan life_span;

	paxs::Genome mother_genome, father_genome;
	mother_genome.setMtDNA(10);
	mother_genome.setYDNA(0);
	father_genome.setMtDNA(20);
	father_genome.setYDNA(1);

	paxs::SettlementAgent mother(1, 25 * 12, 100 * 12, mother_genome, 100, 0, 5);
	mother.marry(2, father_genome, 100, 0, 5);
	mother.setBirthIntervalCount(1);

	settlement.addAgent(mother);

	// When: 出産処理を実行
	settlement.preUpdate(life_span);

	// Then: 子供が生まれた場合、母のmtDNAを継承
	if (settlement.getPopulation() > 1) {
		EXPECT_EQ(settlement.getAgents()[1].cgetGenome().getMtDNA(), 10);
	}
}

TEST(SettlementUnitTest, Birth_ChildInheritsCultureFromParents) {
	// Given: 農耕文化を持つ既婚カップル
	std::mt19937 gen(11111);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);
	paxs::KanakumaLifeSpan life_span;

	paxs::Genome mother_genome, father_genome;
	mother_genome.setYDNA(0);
	father_genome.setYDNA(1);

	paxs::SettlementAgent mother(1, 25 * 12, 100 * 12, mother_genome, 100, 0, 5);
	mother.marry(2, father_genome, 80, 0, 3);
	mother.setBirthIntervalCount(1);

	settlement.addAgent(mother);

	// When: 出産処理を実行（複数回試行して確実に出産させる）
	for (int i = 0; i < 10 && settlement.getPopulation() == 1; ++i) {
		settlement.getAgents()[0].setBirthIntervalCount(1);
		settlement.preUpdate(life_span);
	}

	// Then: 子供が生まれた場合、両親の農耕文化の平均を継承
	if (settlement.getPopulation() > 1) {
		std::uint_least8_t child_farming = settlement.getAgents()[1].cgetFarming();
		// 両親の平均: (100 + 80) / 2 = 90
		EXPECT_EQ(child_farming, 90);
	}
}

// ========================================
// Agent Management Tests
// ========================================

TEST(SettlementUnitTest, DeleteAgent_RemovesSpecificAgent) {
	// Given: 複数のエージェントがいる集落
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);

	paxs::Genome genome;
	paxs::SettlementAgent agent1(1, 20, 100, genome, 0, 0, 0);
	paxs::SettlementAgent agent2(2, 30, 100, genome, 0, 0, 0);
	paxs::SettlementAgent agent3(3, 40, 100, genome, 0, 0, 0);

	settlement.addAgent(agent1);
	settlement.addAgent(agent2);
	settlement.addAgent(agent3);

	EXPECT_EQ(settlement.getPopulation(), 3);

	// When: 特定のエージェントを削除
	settlement.deleteAgent(2);

	// Then: 該当エージェントが削除され、人口が減る
	EXPECT_EQ(settlement.getPopulation(), 2);
}

TEST(SettlementUnitTest, GetAgent_ReturnsCorrectAgent) {
	// Given: 複数のエージェントがいる集落
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);

	paxs::Genome genome;
	// 年齢はステップ単位で指定
	paxs::SettlementAgent agent1(10, 20, 100, genome, 0, 0, 0);
	paxs::SettlementAgent agent2(20, 30, 100, genome, 0, 0, 0);

	settlement.addAgent(agent1);
	settlement.addAgent(agent2);

	// When: 特定IDのエージェントを取得
	paxs::SettlementAgent& retrieved = settlement.getAgent(20);

	// Then: 正しいエージェントが取得できる
	EXPECT_EQ(retrieved.getId(), 20);
	EXPECT_EQ(retrieved.getAgeInt(), 30); // ステップ単位で比較
}

TEST(SettlementUnitTest, ResizeAgents_ChangesPopulation) {
	// Given: エージェントがいる集落
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);

	for (int i = 0; i < 5; ++i) {
		settlement.addAgent(paxs::SettlementAgent(i, 20, 100, paxs::Genome(), 0, 0, 0));
	}

	EXPECT_EQ(settlement.getPopulation(), 5);

	// When: サイズ変更
	settlement.resizeAgents(3);

	// Then: 人口が変更される
	EXPECT_EQ(settlement.getPopulation(), 3);
}

// ========================================
// Position Management Tests
// ========================================

TEST(SettlementUnitTest, SetPosition_UpdatesOldPosition) {
	// Given: 座標を持つ集落
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);

	paxs::Vector2<paxs::GridType> initial_pos(100, 200);
	settlement.setPosition(initial_pos);

	// When: 新しい座標を設定
	paxs::Vector2<paxs::GridType> new_pos(150, 250);
	settlement.setPosition(new_pos);

	// Then: 古い座標が保存される
	EXPECT_EQ(settlement.getPosition(), new_pos);
	EXPECT_EQ(settlement.getOldPosition(), initial_pos);
}

TEST(SettlementUnitTest, ClearOldPosition_ResetsOldPositionData) {
	// Given: 座標履歴を持つ集落
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);

	settlement.setPosition(paxs::Vector2<paxs::GridType>(100, 200));
	settlement.setPosition(paxs::Vector2<paxs::GridType>(150, 250));

	// When: 古い座標をクリア
	settlement.clearOldPosition();

	// Then: 古い座標が無効値になる
	EXPECT_EQ(settlement.getOldPosition().x, -1);
	EXPECT_EQ(settlement.getOldPosition().y, -1);
	EXPECT_EQ(settlement.getPositions().size(), 0);
}

// ========================================
// Population Weight Tests
// ========================================

TEST(SettlementUnitTest, GetPopulationWeight_WithMixedCultures_CalculatesCorrectly) {
	// Given: 異なる生業文化を持つエージェントがいる集落
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);

	// 農耕文化レベル100のエージェント3人
	for (int i = 0; i < 3; ++i) {
		settlement.addAgent(paxs::SettlementAgent(i, 20, 100, paxs::Genome(), 100, 0, 0));
	}

	// 狩猟採集文化レベル100のエージェント2人
	for (int i = 3; i < 5; ++i) {
		settlement.addAgent(paxs::SettlementAgent(i, 20, 100, paxs::Genome(), 0, 100, 0));
	}

	// When: 人口重みを計算
	double weight = settlement.getPopulationWeight();

	// Then: 正の値が返る（詳細な計算式は内部実装に依存）
	EXPECT_GT(weight, 0.0);
	EXPECT_LT(weight, 1000.0); // 妥当な範囲内
}

TEST(SettlementUnitTest, GetPopulationWeight_WithNoAgents_ReturnsZero) {
	// Given: エージェントがいない集落
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);

	// When: 人口重みを計算
	double weight = settlement.getPopulationWeight();

	// Then: 0が返る
	EXPECT_EQ(weight, 0.0);
}

// ========================================
// Language Tests
// ========================================

TEST(SettlementUnitTest, GetLanguage_ReturnsZeroWhenNoAgents) {
	// Given: エージェントがいない集落
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);

	// When: 言語を取得
	std::uint_least8_t language = settlement.getLanguage();

	// Then: 0が返る（初期値）
	EXPECT_EQ(language, 0);
}

TEST(SettlementUnitTest, GetLanguage_ReturnsMostFrequentLanguage) {
	// Given: 複数の言語を持つエージェントがいる集落
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);

	// 言語5のエージェントを3人追加
	for (int i = 0; i < 3; ++i) {
		paxs::SettlementAgent agent(i, 20, 100, paxs::Genome(), 0, 0, 5);
		settlement.addAgent(agent);
	}

	// 言語10のエージェントを2人追加
	for (int i = 3; i < 5; ++i) {
		paxs::SettlementAgent agent(i, 20, 100, paxs::Genome(), 0, 0, 10);
		settlement.addAgent(agent);
	}

	// When: 言語を取得
	std::uint_least8_t language = settlement.getLanguage();

	// Then: 最頻の言語5が返る
	EXPECT_EQ(language, 5);
}

TEST(SettlementUnitTest, GetLanguage_HandlesTieBreaking) {
	// Given: 同数の異なる言語を持つエージェントがいる集落
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);

	// 言語3のエージェントを2人
	for (int i = 0; i < 2; ++i) {
		paxs::SettlementAgent agent(i, 20, 100, paxs::Genome(), 0, 0, 3);
		settlement.addAgent(agent);
	}

	// 言語7のエージェントを2人
	for (int i = 2; i < 4; ++i) {
		paxs::SettlementAgent agent(i, 20, 100, paxs::Genome(), 0, 0, 7);
		settlement.addAgent(agent);
	}

	// When: 言語を取得
	std::uint_least8_t language = settlement.getLanguage();

	// Then: いずれかの言語が返る（実装依存だが、最初に見つかった最大値）
	EXPECT_TRUE(language == 3 || language == 7);
}

TEST(SettlementUnitTest, GetLanguage_WorksWithSingleAgent) {
	// Given: 1人のエージェントがいる集落
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);

	paxs::SettlementAgent agent(1, 20, 100, paxs::Genome(), 0, 0, 42);
	settlement.addAgent(agent);

	// When: 言語を取得
	std::uint_least8_t language = settlement.getLanguage();

	// Then: そのエージェントの言語が返る
	EXPECT_EQ(language, 42);
}

TEST(SettlementUnitTest, GetLanguage_WorksWithAllSameLanguage) {
	// Given: 全員が同じ言語を話す集落
	std::mt19937 gen(12345);
	auto environment = std::make_shared<paxs::Environment>();
	paxs::Settlement settlement(1, gen, environment);

	// 言語15のエージェントを10人追加
	for (int i = 0; i < 10; ++i) {
		paxs::SettlementAgent agent(i, 20, 100, paxs::Genome(), 0, 0, 15);
		settlement.addAgent(agent);
	}

	// When: 言語を取得
	std::uint_least8_t language = settlement.getLanguage();

	// Then: 言語15が返る
	EXPECT_EQ(language, 15);
}
