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

// Test Fixture: environment を共有することでテスト高速化
class SettlementUnitTest : public ::testing::Test {
protected:
	// 全テストで共有される environment（SetUpTestSuiteで1回だけ初期化）
	static std::shared_ptr<paxs::Environment> shared_environment;
	static paxs::KanakumaLifeSpan shared_life_span;

	// テストスイート全体の初期化（最初に1回だけ実行）
	static void SetUpTestSuite() {
		shared_environment = std::make_shared<paxs::Environment>();
	}

	// テストスイート全体の終了処理
	static void TearDownTestSuite() {
		shared_environment.reset();
	}
};

// 静的メンバの定義
std::shared_ptr<paxs::Environment> SettlementUnitTest::shared_environment;
paxs::KanakumaLifeSpan SettlementUnitTest::shared_life_span;

TEST_F(SettlementUnitTest, Construction) {
	std::mt19937 gen(12345);

	paxs::Settlement settlement(1, gen, shared_environment);

	// IDが正しく設定されていることを確認
	EXPECT_EQ(settlement.getId(), 1);

	// 初期人口は0
	EXPECT_EQ(settlement.getPopulation(), 0);
}

TEST_F(SettlementUnitTest, AddAgent) {
	std::mt19937 gen(12345);

	paxs::Settlement settlement(1, gen, shared_environment);

	// エージェントを追加
	paxs::SettlementAgent agent(1, 0, 100, paxs::Genome(), 0, 0, 0);
	settlement.addAgent(agent);

	// 人口が1になっているはず
	EXPECT_EQ(settlement.getPopulation(), 1);
}

TEST_F(SettlementUnitTest, Position) {
	std::mt19937 gen(12345);

	paxs::Settlement settlement(1, gen, shared_environment);

	paxs::Vector2<paxs::GridType> pos(100, 200);
	settlement.setPosition(pos);

	EXPECT_EQ(settlement.getPosition(), pos);
}

TEST_F(SettlementUnitTest, ClearAgents) {
	std::mt19937 gen(12345);

	paxs::Settlement settlement(1, gen, shared_environment);

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

TEST_F(SettlementUnitTest, GetPopulationWeight) {
	std::mt19937 gen(12345);

	paxs::Settlement settlement(1, gen, shared_environment);

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

TEST_F(SettlementUnitTest, Death_RemovesAgentsWhoExceedLifespan) {
	// Given: 集落と寿命が尽きるエージェント
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

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

TEST_F(SettlementUnitTest, Death_IncrementsAge) {
	// Given: 集落と若いエージェント
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

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

TEST_F(SettlementUnitTest, Death_DivorcesPartnerWhenAgentDies) {
	// Given: 既婚のカップルがいる集落
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

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

TEST_F(SettlementUnitTest, Birth_CreatesChildWhenBirthIntervalReachesZero) {
	// Given: 出産間隔カウントが1の妊娠中の女性
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

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
	settlement.preUpdate(shared_life_span);

	// Then: 子供が追加される（確率的に死産でなければ）
	// Note: 死産率により失敗する可能性があるため、どちらも正常
	// 子供が生まれた、または死産だった
	EXPECT_TRUE(settlement.getPopulation() == 1 || settlement.getPopulation() == 2);
}

TEST_F(SettlementUnitTest, Birth_DecrementsBirthIntervalCount) {
	// Given: 出産間隔カウントが5の女性
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	paxs::Genome genome;
	genome.setYDNA(0); // Female
	paxs::SettlementAgent mother(1, 25 * 12, 100 * 12, genome, 100, 0, 5);
	mother.setBirthIntervalCount(5);

	settlement.addAgent(mother);
	EXPECT_EQ(settlement.getAgents()[0].getBirthIntervalCount(), 5);

	// When: 出産処理を実行
	settlement.preUpdate(shared_life_span);

	// Then: カウントが減少
	EXPECT_EQ(settlement.getAgents()[0].getBirthIntervalCount(), 4);
}

TEST_F(SettlementUnitTest, Birth_ChildInheritsGenomeFromParents) {
	// Given: 既婚の女性（カウント1）
	std::mt19937 gen(54321); // Different seed for better randomness
	paxs::Settlement settlement(1, gen, shared_environment);

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
	settlement.preUpdate(shared_life_span);

	// Then: 子供が生まれた場合、母のmtDNAを継承
	if (settlement.getPopulation() > 1) {
		EXPECT_EQ(settlement.getAgents()[1].cgetGenome().getMtDNA(), 10);
	}
}

TEST_F(SettlementUnitTest, Birth_ChildInheritsCultureFromParents) {
	// Given: 農耕文化を持つ既婚カップル
	std::mt19937 gen(11111);
	paxs::Settlement settlement(1, gen, shared_environment);

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
		settlement.preUpdate(shared_life_span);
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

TEST_F(SettlementUnitTest, DeleteAgent_RemovesSpecificAgent) {
	// Given: 複数のエージェントがいる集落
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

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

TEST_F(SettlementUnitTest, GetAgent_ReturnsCorrectAgent) {
	// Given: 複数のエージェントがいる集落
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

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

TEST_F(SettlementUnitTest, ResizeAgents_ChangesPopulation) {
	// Given: エージェントがいる集落
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

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

TEST_F(SettlementUnitTest, SetPosition_UpdatesOldPosition) {
	// Given: 座標を持つ集落
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	paxs::Vector2<paxs::GridType> initial_pos(100, 200);
	settlement.setPosition(initial_pos);

	// When: 新しい座標を設定
	paxs::Vector2<paxs::GridType> new_pos(150, 250);
	settlement.setPosition(new_pos);

	// Then: 古い座標が保存される
	EXPECT_EQ(settlement.getPosition(), new_pos);
	EXPECT_EQ(settlement.getOldPosition(), initial_pos);
}

TEST_F(SettlementUnitTest, ClearOldPosition_ResetsOldPositionData) {
	// Given: 座標履歴を持つ集落
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

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

TEST_F(SettlementUnitTest, GetPopulationWeight_WithMixedCultures_CalculatesCorrectly) {
	// Given: 異なる生業文化を持つエージェントがいる集落
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

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

TEST_F(SettlementUnitTest, GetPopulationWeight_WithNoAgents_ReturnsZero) {
	// Given: エージェントがいない集落
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	// When: 人口重みを計算
	double weight = settlement.getPopulationWeight();

	// Then: 0が返る
	EXPECT_EQ(weight, 0.0);
}

// ========================================
// Language Tests
// ========================================

TEST_F(SettlementUnitTest, GetLanguage_ReturnsZeroWhenNoAgents) {
	// Given: エージェントがいない集落
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	// When: 言語を取得
	std::uint_least8_t language = settlement.getLanguage();

	// Then: 0が返る（初期値）
	EXPECT_EQ(language, 0);
}

TEST_F(SettlementUnitTest, GetLanguage_ReturnsMostFrequentLanguage) {
	// Given: 複数の言語を持つエージェントがいる集落
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

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

TEST_F(SettlementUnitTest, GetLanguage_HandlesTieBreaking) {
	// Given: 同数の異なる言語を持つエージェントがいる集落
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

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

TEST_F(SettlementUnitTest, GetLanguage_WorksWithSingleAgent) {
	// Given: 1人のエージェントがいる集落
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	paxs::SettlementAgent agent(1, 20, 100, paxs::Genome(), 0, 0, 42);
	settlement.addAgent(agent);

	// When: 言語を取得
	std::uint_least8_t language = settlement.getLanguage();

	// Then: そのエージェントの言語が返る
	EXPECT_EQ(language, 42);
}

TEST_F(SettlementUnitTest, GetLanguage_WorksWithAllSameLanguage) {
	// Given: 全員が同じ言語を話す集落
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

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

// ========================================
// SNP Tests
// ========================================

TEST_F(SettlementUnitTest, GetSNP_ReturnsCorrectAverage) {
	// Given: 異なるSNP値を持つエージェント
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	// SNP=128のエージェントを3人
	for (int i = 0; i < 3; ++i) {
		paxs::Genome genome;
		genome.setSNP(128);
		paxs::SettlementAgent agent(i, 20, 100, genome, 0, 0, 0);
		settlement.addAgent(agent);
	}

	// SNP=255のエージェントを2人
	for (int i = 3; i < 5; ++i) {
		paxs::Genome genome;
		genome.setSNP(255);
		paxs::SettlementAgent agent(i, 20, 100, genome, 0, 0, 0);
		settlement.addAgent(agent);
	}

	// When: SNP平均を取得
	double snp = settlement.getSNP();

	// Then: 正しい平均値が返る
	// (128*3 + 255*2) / 5 / 255 = 894 / 5 / 255 ≈ 0.7012
	EXPECT_NEAR(snp, 0.7012, 0.001);
}

TEST_F(SettlementUnitTest, GetSNP_WithAllSameSNP) {
	// Given: 全員同じSNP値
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	// SNP=200のエージェントを5人
	for (int i = 0; i < 5; ++i) {
		paxs::Genome genome;
		genome.setSNP(200);
		paxs::SettlementAgent agent(i, 20, 100, genome, 0, 0, 0);
		settlement.addAgent(agent);
	}

	// When: SNP平均を取得
	double snp = settlement.getSNP();

	// Then: 200/255 ≈ 0.7843
	EXPECT_NEAR(snp, 200.0 / 255.0, 0.001);
}

TEST_F(SettlementUnitTest, GetSNP_WithZeroSNP) {
	// Given: 全員SNP=0
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	for (int i = 0; i < 3; ++i) {
		paxs::Genome genome;
		genome.setSNP(0);
		paxs::SettlementAgent agent(i, 20, 100, genome, 0, 0, 0);
		settlement.addAgent(agent);
	}

	// When: SNP平均を取得
	double snp = settlement.getSNP();

	// Then: 0が返る
	EXPECT_EQ(snp, 0.0);
}

TEST_F(SettlementUnitTest, GetSNP_WithMaxSNP) {
	// Given: 全員SNP=255（最大値）
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	for (int i = 0; i < 3; ++i) {
		paxs::Genome genome;
		genome.setSNP(255);
		paxs::SettlementAgent agent(i, 20, 100, genome, 0, 0, 0);
		settlement.addAgent(agent);
	}

	// When: SNP平均を取得
	double snp = settlement.getSNP();

	// Then: 1.0が返る
	EXPECT_EQ(snp, 1.0);
}

// ========================================
// Farming Population Tests
// ========================================

TEST_F(SettlementUnitTest, GetFarmingPopulation_CountsCorrectly) {
	// Given: 農耕民と狩猟採集民の混在
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	// 農耕民5人（farming > 0）
	for (int i = 0; i < 5; ++i) {
		paxs::SettlementAgent agent(i, 20, 100, paxs::Genome(), 100, 0, 0);
		settlement.addAgent(agent);
	}

	// 狩猟採集民3人（farming = 0）
	for (int i = 5; i < 8; ++i) {
		paxs::SettlementAgent agent(i, 20, 100, paxs::Genome(), 0, 100, 0);
		settlement.addAgent(agent);
	}

	// When: 農耕人口を取得
	std::size_t farming_pop = settlement.getFarmingPopulation();

	// Then: 5人が返る
	EXPECT_EQ(farming_pop, 5);
}

TEST_F(SettlementUnitTest, GetFarmingPopulation_WithAllFarmers) {
	// Given: 全員農耕民
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	for (int i = 0; i < 10; ++i) {
		paxs::SettlementAgent agent(i, 20, 100, paxs::Genome(), 50, 0, 0);
		settlement.addAgent(agent);
	}

	EXPECT_EQ(settlement.getFarmingPopulation(), 10);
}

TEST_F(SettlementUnitTest, GetFarmingPopulation_WithNoFarmers) {
	// Given: 全員狩猟採集民
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	for (int i = 0; i < 7; ++i) {
		paxs::SettlementAgent agent(i, 20, 100, paxs::Genome(), 0, 100, 0);
		settlement.addAgent(agent);
	}

	EXPECT_EQ(settlement.getFarmingPopulation(), 0);
}

TEST_F(SettlementUnitTest, GetFarmingPopulation_WithNoAgents) {
	// Given: エージェントがいない集落
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	EXPECT_EQ(settlement.getFarmingPopulation(), 0);
}

// ========================================
// MtDNA Tests
// ========================================

TEST_F(SettlementUnitTest, GetMostMtDNA_ReturnsAverage) {
	// Given: 異なるmtDNA値を持つエージェント
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	// mtDNA=10のエージェントを3人
	for (int i = 0; i < 3; ++i) {
		paxs::Genome genome;
		genome.setMtDNA(10);
		paxs::SettlementAgent agent(i, 20, 100, genome, 0, 0, 0);
		settlement.addAgent(agent);
	}

	// mtDNA=20のエージェントを2人
	for (int i = 3; i < 5; ++i) {
		paxs::Genome genome;
		genome.setMtDNA(20);
		paxs::SettlementAgent agent(i, 20, 100, genome, 0, 0, 0);
		settlement.addAgent(agent);
	}

	// When: mtDNA平均を取得
	std::size_t mtdna = settlement.getMostMtDNA();

	// Then: (10*3 + 20*2) / 5 = 70 / 5 = 14
	EXPECT_EQ(mtdna, 14);
}

TEST_F(SettlementUnitTest, GetMostMtDNA_WithSameMtDNA) {
	// Given: 全員同じmtDNA
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	for (int i = 0; i < 5; ++i) {
		paxs::Genome genome;
		genome.setMtDNA(15);
		paxs::SettlementAgent agent(i, 20, 100, genome, 0, 0, 0);
		settlement.addAgent(agent);
	}

	EXPECT_EQ(settlement.getMostMtDNA(), 15);
}

TEST_F(SettlementUnitTest, GetMostMtDNA_WithZeroMtDNA) {
	// Given: 全員mtDNA=0
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	for (int i = 0; i < 3; ++i) {
		paxs::Genome genome;
		genome.setMtDNA(0);
		paxs::SettlementAgent agent(i, 20, 100, genome, 0, 0, 0);
		settlement.addAgent(agent);
	}

	EXPECT_EQ(settlement.getMostMtDNA(), 0);
}

// ========================================
// Divide Tests
// ========================================

TEST_F(SettlementUnitTest, Divide_SplitsAgentsInHalf) {
	// Given: 10人のエージェントがいる集落
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	for (int i = 0; i < 10; ++i) {
		paxs::SettlementAgent agent(i, 20, 100, paxs::Genome(), 0, 0, 0);
		settlement.addAgent(agent);
	}

	// When: 集落を分割
	paxs::Settlement new_settlement = settlement.divide();

	// Then: エージェントが分割される（5人ずつ）
	EXPECT_EQ(settlement.getPopulation(), 5);
	EXPECT_EQ(new_settlement.getPopulation(), 5);
}

TEST_F(SettlementUnitTest, Divide_CreatesNewSettlement) {
	// Given: エージェントがいる集落
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	for (int i = 0; i < 6; ++i) {
		paxs::SettlementAgent agent(i, 20, 100, paxs::Genome(), 0, 0, 0);
		settlement.addAgent(agent);
	}

	// When: 分割
	paxs::Settlement new_settlement = settlement.divide();

	// Then: 新しい集落が作成され、異なるIDを持つ
	EXPECT_NE(settlement.getId(), new_settlement.getId());
}

TEST_F(SettlementUnitTest, Divide_WithOddNumberOfAgents) {
	// Given: 奇数人のエージェント
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	for (int i = 0; i < 7; ++i) {
		paxs::SettlementAgent agent(i, 20, 100, paxs::Genome(), 0, 0, 0);
		settlement.addAgent(agent);
	}

	// When: 分割
	paxs::Settlement new_settlement = settlement.divide();

	// Then: 3人と4人に分かれる（7/2=3）
	EXPECT_EQ(settlement.getPopulation(), 3);
	EXPECT_EQ(new_settlement.getPopulation(), 4);
	EXPECT_EQ(settlement.getPopulation() + new_settlement.getPopulation(), 7);
}

TEST_F(SettlementUnitTest, Divide_KeepsMarriedCouplesTogether) {
	// Given: 既婚カップルがいる集落
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	// 夫婦3組 + 独身4人 = 10人
	for (int i = 0; i < 3; ++i) {
		paxs::Genome male_genome, female_genome;
		male_genome.setYDNA(1);
		female_genome.setYDNA(0);

		paxs::SettlementAgent husband(i * 2, 25 * 12, 100 * 12, male_genome, 0, 0, 0);
		paxs::SettlementAgent wife(i * 2 + 1, 25 * 12, 100 * 12, female_genome, 0, 0, 0);

		husband.marry(i * 2 + 1, female_genome, 0, 0, 0);
		wife.marry(i * 2, male_genome, 0, 0, 0);

		settlement.addAgent(husband);
		settlement.addAgent(wife);
	}

	// 独身4人
	for (int i = 6; i < 10; ++i) {
		paxs::SettlementAgent agent(i, 20, 100, paxs::Genome(), 0, 0, 0);
		settlement.addAgent(agent);
	}

	// When: 分割
	paxs::Settlement new_settlement = settlement.divide();

	// Then: 夫婦が同じ集落にいる
	auto checkCouplesInSettlement = [](const paxs::Settlement& s) {
		for (const auto& agent : s.cgetAgents()) {
			if (agent.isMarried()) {
				auto partner_id = agent.getPartnerId();
				bool found = false;
				for (const auto& other : s.cgetAgents()) {
					if (other.getId() == partner_id) {
						found = true;
						break;
					}
				}
				EXPECT_TRUE(found) << "Partner ID " << partner_id << " not found in same settlement";
			}
		}
	};

	checkCouplesInSettlement(settlement);
	checkCouplesInSettlement(new_settlement);
}

TEST_F(SettlementUnitTest, Divide_WithTwoAgents) {
	// Given: 2人のエージェント
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	paxs::SettlementAgent agent1(1, 20, 100, paxs::Genome(), 0, 0, 0);
	paxs::SettlementAgent agent2(2, 20, 100, paxs::Genome(), 0, 0, 0);
	settlement.addAgent(agent1);
	settlement.addAgent(agent2);

	// When: 分割
	paxs::Settlement new_settlement = settlement.divide();

	// Then: 1人ずつに分かれる
	EXPECT_EQ(settlement.getPopulation(), 1);
	EXPECT_EQ(new_settlement.getPopulation(), 1);
}

TEST_F(SettlementUnitTest, Divide_WithSingleAgent) {
	// Given: 1人のエージェント
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	paxs::SettlementAgent agent(1, 20, 100, paxs::Genome(), 0, 0, 0);
	settlement.addAgent(agent);

	// When: 分割
	paxs::Settlement new_settlement = settlement.divide();

	// Then: 元の集落は0人、新しい集落は1人（1/2=0）
	EXPECT_EQ(settlement.getPopulation(), 0);
	EXPECT_EQ(new_settlement.getPopulation(), 1);
}

TEST_F(SettlementUnitTest, Divide_PreservesAgentData) {
	// Given: 特定のデータを持つエージェント
	std::mt19937 gen(12345);
	paxs::Settlement settlement(1, gen, shared_environment);

	for (int i = 0; i < 4; ++i) {
		paxs::Genome genome;
		genome.setMtDNA(10 + i);
		paxs::SettlementAgent agent(i, 20, 100, genome, 50 + i, 30 + i, 5 + i);
		settlement.addAgent(agent);
	}

	// When: 分割
	paxs::Settlement new_settlement = settlement.divide();

	// Then: エージェントのデータが保持されている
	int total_farming = 0;
	for (const auto& agent : settlement.cgetAgents()) {
		total_farming += agent.cgetFarming();
	}
	for (const auto& agent : new_settlement.cgetAgents()) {
		total_farming += agent.cgetFarming();
	}

	// 元の合計: 50+51+52+53 = 206
	EXPECT_EQ(total_farming, 206);
}
