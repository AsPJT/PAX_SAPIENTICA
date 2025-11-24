/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2025 As Project
	[Production]	2023-2025 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/Simulation/Entity/SettlementAgent.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>

// ========================================
// Construction and Basic Properties
// ========================================

TEST(SettlementAgentUnitTest, DefaultConstruction) {
	// Given: デフォルトコンストラクタで作成
	paxs::SettlementAgent agent;

	// Then: 初期値が正しい
	EXPECT_EQ(agent.getId(), 0);
	EXPECT_EQ(agent.getAgeInt(), 0);
	EXPECT_EQ(agent.getLifeSpan(), 0);
	EXPECT_FALSE(agent.isMarried());
	EXPECT_EQ(agent.getBirthIntervalCount(), 0);
}

TEST(SettlementAgentUnitTest, ParameterizedConstruction) {
	// Given: パラメータ指定でエージェントを作成
	paxs::Genome genome;
	genome.setMtDNA(10);
	genome.setYDNA(0);  // Female

	// When: エージェント作成
	paxs::SettlementAgent agent(
		123,     // id
		25,      // age (steps)
		60,      // life_span (steps)
		genome,
		100,     // farming
		50,      // hunter_gatherer
		5        // language
	);

	// Then: 全てのプロパティが正しく設定される
	EXPECT_EQ(agent.getId(), 123);
	EXPECT_EQ(agent.getAgeInt(), 25);
	EXPECT_EQ(agent.getLifeSpan(), 60);
	EXPECT_EQ(agent.cgetFarming(), 100);
	EXPECT_EQ(agent.cgetHunterGatherer(), 50);
	EXPECT_EQ(agent.cgetLanguage(), 5);
	EXPECT_EQ(agent.cgetGenome().getMtDNA(), 10);
	EXPECT_EQ(agent.cgetGenome().getYDNA(), 0);
	EXPECT_FALSE(agent.isMarried());
}

// ========================================
// Gender Tests
// ========================================

TEST(SettlementAgentUnitTest, IsFemale_WhenYDNAIsZero) {
	// Given: Y-DNA = 0のゲノム
	paxs::Genome genome;
	genome.setYDNA(0);

	paxs::SettlementAgent agent(1, 20, 50, genome, 100, 0, 5);

	// Then: 女性として判定される
	EXPECT_TRUE(agent.isFemale());
	EXPECT_FALSE(agent.isMale());
}

TEST(SettlementAgentUnitTest, IsMale_WhenYDNAIsNonZero) {
	// Given: Y-DNA != 0のゲノム
	paxs::Genome genome;
	genome.setYDNA(5);

	paxs::SettlementAgent agent(1, 20, 50, genome, 100, 0, 5);

	// Then: 男性として判定される
	EXPECT_FALSE(agent.isFemale());
	EXPECT_TRUE(agent.isMale());
}

// ========================================
// Age and Lifespan Tests
// ========================================

TEST(SettlementAgentUnitTest, IncrementAge_IncreasesAgeByOne) {
	// Given: 年齢20のエージェント
	paxs::SettlementAgent agent(1, 20, 50, paxs::Genome(), 100, 0, 5);

	// When: 年齢をインクリメント
	agent.incrementAge();

	// Then: 年齢が21になる
	EXPECT_EQ(agent.getAgeInt(), 21);
}

TEST(SettlementAgentUnitTest, IncrementAge_WithParameter) {
	// Given: 年齢20のエージェント
	paxs::SettlementAgent agent(1, 20, 50, paxs::Genome(), 100, 0, 5);

	// When: 年齢を5増加
	agent.incrementAge(5);

	// Then: 年齢が25になる
	EXPECT_EQ(agent.getAgeInt(), 25);
}

TEST(SettlementAgentUnitTest, IncrementAge_DoesNotOverflow) {
	// Given: 年齢が最大値のエージェント
	paxs::AgeType max_age = std::numeric_limits<paxs::AgeType>::max();
	paxs::SettlementAgent agent(1, max_age, max_age, paxs::Genome(), 100, 0, 5);

	// When: 年齢をインクリメント
	agent.incrementAge();

	// Then: オーバーフローせず最大値のまま
	EXPECT_EQ(agent.getAgeInt(), max_age);
}

TEST(SettlementAgentUnitTest, IsDead_WhenAgeExceedsLifespan) {
	// Given: 年齢が寿命を超えたエージェント
	paxs::SettlementAgent agent(1, 61, 60, paxs::Genome(), 100, 0, 5);

	// Then: 死亡と判定される
	EXPECT_TRUE(agent.isDead());
}

TEST(SettlementAgentUnitTest, IsDead_WhenAgeEqualsLifespan) {
	// Given: 年齢が寿命と等しいエージェント
	paxs::SettlementAgent agent(1, 60, 60, paxs::Genome(), 100, 0, 5);

	// Then: まだ生存（isDead()はage > life_span）
	EXPECT_FALSE(agent.isDead());
}

TEST(SettlementAgentUnitTest, IsDead_WhenAgeWithinLifespan) {
	// Given: 年齢が寿命以下のエージェント
	paxs::SettlementAgent agent(1, 30, 60, paxs::Genome(), 100, 0, 5);

	// Then: 生存
	EXPECT_FALSE(agent.isDead());
}

TEST(SettlementAgentUnitTest, SetAge) {
	// Given: エージェント
	paxs::SettlementAgent agent(1, 20, 60, paxs::Genome(), 100, 0, 5);

	// When: 年齢を設定
	agent.setAge(35);

	// Then: 年齢が更新される
	EXPECT_EQ(agent.getAgeInt(), 35);
}

TEST(SettlementAgentUnitTest, SetLifeSpan) {
	// Given: エージェント
	paxs::SettlementAgent agent(1, 20, 60, paxs::Genome(), 100, 0, 5);

	// When: 寿命を設定
	agent.setLifeSpan(80);

	// Then: 寿命が更新される
	EXPECT_EQ(agent.getLifeSpan(), 80);
}

// ========================================
// Marriage Status Tests
// ========================================

TEST(SettlementAgentUnitTest, InitialMarriageStatus_IsUnmarried) {
	// Given: 新しいエージェント
	paxs::SettlementAgent agent(1, 20, 50, paxs::Genome(), 100, 0, 5);

	// Then: 未婚
	EXPECT_FALSE(agent.isMarried());
	EXPECT_EQ(agent.getPartnerId(), 0);
}

TEST(SettlementAgentUnitTest, Marry_SetsMarriedStatus) {
	// Given: 未婚のエージェント
	paxs::SettlementAgent agent(1, 25, 60, paxs::Genome(), 100, 0, 5);

	// And: 配偶者情報
	paxs::Genome partner_genome;
	partner_genome.setYDNA(5);

	// When: 結婚
	agent.marry(2, partner_genome, 80, 20, 8);

	// Then: 婚姻状態になる
	EXPECT_TRUE(agent.isMarried());
}

TEST(SettlementAgentUnitTest, Marry_StoresPartnerInfo) {
	// Given: 未婚の女性エージェント
	paxs::Genome mother_genome;
	mother_genome.setMtDNA(10);
	mother_genome.setYDNA(0);
	paxs::SettlementAgent bride(1, 20, 50, mother_genome, 100, 0, 5);

	// And: 配偶者情報
	paxs::Genome father_genome;
	father_genome.setMtDNA(20);
	father_genome.setYDNA(5);
	paxs::HumanIndexType groom_id = 2;
	std::uint8_t groom_farming = 80;
	std::uint8_t groom_hunter = 10;
	std::uint8_t groom_language = 8;

	// When: 結婚
	bride.marry(groom_id, father_genome, groom_farming, groom_hunter, groom_language);

	// Then: 配偶者情報が保存される
	EXPECT_EQ(bride.getPartnerId(), groom_id);
	EXPECT_EQ(bride.cgetPartnerGenome().getMtDNA(), 20);
	EXPECT_EQ(bride.cgetPartnerGenome().getYDNA(), 5);
	EXPECT_EQ(bride.cgetPartnerFarming(), groom_farming);
	EXPECT_EQ(bride.cgetPartnerHunterGatherer(), groom_hunter);
	EXPECT_EQ(bride.cgetPartnerLanguage(), groom_language);
}

TEST(SettlementAgentUnitTest, Divorce_ClearsMarriedStatus) {
	// Given: 結婚しているエージェント
	paxs::SettlementAgent agent(1, 25, 60, paxs::Genome(), 100, 0, 5);
	agent.marry(2, paxs::Genome(), 80, 20, 8);
	ASSERT_TRUE(agent.isMarried());

	// When: 離婚
	agent.divorce();

	// Then: 未婚状態になる
	EXPECT_FALSE(agent.isMarried());
}

TEST(SettlementAgentUnitTest, Divorce_ClearsPartnerId) {
	// Given: 結婚しているエージェント
	paxs::SettlementAgent agent(1, 25, 60, paxs::Genome(), 100, 0, 5);
	agent.marry(2, paxs::Genome(), 80, 20, 8);
	ASSERT_EQ(agent.getPartnerId(), 2);

	// When: 離婚
	agent.divorce();

	// Then: 配偶者IDがクリアされる
	EXPECT_EQ(agent.getPartnerId(), 0);
}

// ========================================
// Marriage Eligibility Tests
// ========================================

TEST(SettlementAgentUnitTest, IsAbleToMarriage_Female_WithinAgeRange) {
	// Given: SimulationConstantsの設定を確認
	auto& config = paxs::SimulationConstants::getInstance();
	// 女性婚姻可能年齢を仮定（通常16-40など）

	// And: 婚姻可能年齢の女性エージェント（25歳 = 25 * steps_per_year）
	paxs::Genome genome;
	genome.setYDNA(0);  // Female
	paxs::AgeType age_in_steps = static_cast<paxs::AgeType>(25 * config.steps_per_year);
	paxs::SettlementAgent agent(1, age_in_steps, 1000, genome, 100, 0, 5);

	// Then: 婚姻可能
	EXPECT_TRUE(agent.isAbleToMarriage());
}

TEST(SettlementAgentUnitTest, IsAbleToMarriage_Female_TooYoung) {
	// Given: SimulationConstantsの設定
	auto& config = paxs::SimulationConstants::getInstance();

	// And: 婚姻可能最小年齢未満の女性（10歳）
	paxs::Genome genome;
	genome.setYDNA(0);  // Female
	paxs::AgeType age_in_steps = static_cast<paxs::AgeType>(10 * config.steps_per_year);
	paxs::SettlementAgent agent(1, age_in_steps, 1000, genome, 100, 0, 5);

	// Then: 婚姻不可
	EXPECT_FALSE(agent.isAbleToMarriage());
}

TEST(SettlementAgentUnitTest, IsAbleToMarriage_Female_TooOld) {
	// Given: SimulationConstantsの設定
	auto& config = paxs::SimulationConstants::getInstance();

	// And: 婚姻可能最大年齢を超える女性（60歳）
	paxs::Genome genome;
	genome.setYDNA(0);  // Female
	paxs::AgeType age_in_steps = static_cast<paxs::AgeType>(60 * config.steps_per_year);
	paxs::SettlementAgent agent(1, age_in_steps, 1000, genome, 100, 0, 5);

	// Then: 婚姻不可
	EXPECT_FALSE(agent.isAbleToMarriage());
}

TEST(SettlementAgentUnitTest, IsAbleToMarriage_Male_WithinAgeRange) {
	// Given: SimulationConstantsの設定
	auto& config = paxs::SimulationConstants::getInstance();

	// And: 婚姻可能年齢の男性エージェント（28歳）
	paxs::Genome genome;
	genome.setYDNA(5);  // Male
	paxs::AgeType age_in_steps = static_cast<paxs::AgeType>(28 * config.steps_per_year);
	paxs::SettlementAgent agent(1, age_in_steps, 1000, genome, 100, 0, 5);

	// Then: 婚姻可能
	EXPECT_TRUE(agent.isAbleToMarriage());
}

TEST(SettlementAgentUnitTest, IsAbleToMarriage_AlreadyMarried) {
	// Given: 婚姻可能年齢の女性エージェント（結婚済み）
	auto& config = paxs::SimulationConstants::getInstance();
	paxs::Genome genome;
	genome.setYDNA(0);  // Female
	paxs::AgeType age_in_steps = static_cast<paxs::AgeType>(25 * config.steps_per_year);
	paxs::SettlementAgent agent(1, age_in_steps, 1000, genome, 100, 0, 5);

	// When: 結婚
	agent.marry(2, paxs::Genome(), 80, 20, 8);

	// Then: 婚姻不可（既婚）
	EXPECT_FALSE(agent.isAbleToMarriage());
}

// ========================================
// Birth Eligibility Tests
// ========================================

TEST(SettlementAgentUnitTest, IsAbleToGiveBirth_Female_WithinAgeRange_Married) {
	// Given: SimulationConstantsの設定
	auto& config = paxs::SimulationConstants::getInstance();

	// And: 出産可能年齢、結婚済みの女性エージェント（28歳）
	paxs::Genome genome;
	genome.setYDNA(0);  // Female
	paxs::AgeType age_in_steps = static_cast<paxs::AgeType>(28 * config.steps_per_year);
	paxs::SettlementAgent agent(1, age_in_steps, 1000, genome, 100, 0, 5);
	agent.marry(2, paxs::Genome(), 80, 20, 8);

	// Then: 出産可能
	EXPECT_TRUE(agent.isAbleToGiveBirth());
}

TEST(SettlementAgentUnitTest, IsAbleToGiveBirth_Female_WithinAgeRange_Unmarried) {
	// Given: SimulationConstantsの設定
	auto& config = paxs::SimulationConstants::getInstance();

	// And: 出産可能年齢だが未婚の女性エージェント
	paxs::Genome genome;
	genome.setYDNA(0);  // Female
	paxs::AgeType age_in_steps = static_cast<paxs::AgeType>(28 * config.steps_per_year);
	paxs::SettlementAgent agent(1, age_in_steps, 1000, genome, 100, 0, 5);

	// Then: 出産不可（未婚）
	EXPECT_FALSE(agent.isAbleToGiveBirth());
}

TEST(SettlementAgentUnitTest, IsAbleToGiveBirth_Female_TooYoung) {
	// Given: SimulationConstantsの設定
	auto& config = paxs::SimulationConstants::getInstance();

	// And: 出産可能最小年齢未満の女性（結婚済み）
	paxs::Genome genome;
	genome.setYDNA(0);  // Female
	paxs::AgeType age_in_steps = static_cast<paxs::AgeType>(10 * config.steps_per_year);
	paxs::SettlementAgent agent(1, age_in_steps, 1000, genome, 100, 0, 5);
	agent.marry(2, paxs::Genome(), 80, 20, 8);

	// Then: 出産不可（若すぎる）
	EXPECT_FALSE(agent.isAbleToGiveBirth());
}

TEST(SettlementAgentUnitTest, IsAbleToGiveBirth_Female_TooOld) {
	// Given: SimulationConstantsの設定
	auto& config = paxs::SimulationConstants::getInstance();

	// And: 出産可能最大年齢を超える女性（結婚済み）
	paxs::Genome genome;
	genome.setYDNA(0);  // Female
	paxs::AgeType age_in_steps = static_cast<paxs::AgeType>(50 * config.steps_per_year);
	paxs::SettlementAgent agent(1, age_in_steps, 1000, genome, 100, 0, 5);
	agent.marry(2, paxs::Genome(), 80, 20, 8);

	// Then: 出産不可（高齢）
	EXPECT_FALSE(agent.isAbleToGiveBirth());
}

// ========================================
// Birth Interval Tests
// ========================================

TEST(SettlementAgentUnitTest, BirthIntervalCount_InitialValue) {
	// Given: 新しいエージェント
	paxs::SettlementAgent agent(1, 20, 50, paxs::Genome(), 100, 0, 5);

	// Then: 初期値は0
	EXPECT_EQ(agent.getBirthIntervalCount(), 0);
}

TEST(SettlementAgentUnitTest, SetBirthIntervalCount) {
	// Given: エージェント
	paxs::SettlementAgent agent(1, 20, 50, paxs::Genome(), 100, 0, 5);

	// When: 出産間隔カウントを設定
	agent.setBirthIntervalCount(10);

	// Then: 値が設定される
	EXPECT_EQ(agent.getBirthIntervalCount(), 10);
}

TEST(SettlementAgentUnitTest, DecrementBirthIntervalCount) {
	// Given: 出産間隔カウントが10のエージェント
	paxs::SettlementAgent agent(1, 20, 50, paxs::Genome(), 100, 0, 5);
	agent.setBirthIntervalCount(10);

	// When: デクリメント
	std::uint8_t result = agent.decrementBirthIntervalCount();

	// Then: 値が9に減る
	EXPECT_EQ(result, 9);
	EXPECT_EQ(agent.getBirthIntervalCount(), 9);
}

TEST(SettlementAgentUnitTest, DecrementBirthIntervalCount_MultipleDecrements) {
	// Given: 出産間隔カウントが5のエージェント
	paxs::SettlementAgent agent(1, 20, 50, paxs::Genome(), 100, 0, 5);
	agent.setBirthIntervalCount(5);

	// When: 5回デクリメント
	for (int i = 0; i < 5; ++i) {
		agent.decrementBirthIntervalCount();
	}

	// Then: 0になる
	EXPECT_EQ(agent.getBirthIntervalCount(), 0);
}

// ========================================
// Cultural Attributes Tests
// ========================================

TEST(SettlementAgentUnitTest, FarmingLevel) {
	// Given: 農耕レベル150のエージェント
	paxs::SettlementAgent agent(1, 20, 50, paxs::Genome(), 150, 0, 5);

	// Then: 農耕レベルが取得できる
	EXPECT_EQ(agent.cgetFarming(), 150);
}

TEST(SettlementAgentUnitTest, HunterGathererLevel) {
	// Given: 狩猟採集レベル200のエージェント
	paxs::SettlementAgent agent(1, 20, 50, paxs::Genome(), 0, 200, 5);

	// Then: 狩猟採集レベルが取得できる
	EXPECT_EQ(agent.cgetHunterGatherer(), 200);
}

TEST(SettlementAgentUnitTest, Language) {
	// Given: 言語IDが12のエージェント
	paxs::SettlementAgent agent(1, 20, 50, paxs::Genome(), 100, 0, 12);

	// Then: 言語IDが取得できる
	EXPECT_EQ(agent.cgetLanguage(), 12);
}

TEST(SettlementAgentUnitTest, PartnerCulturalAttributes) {
	// Given: エージェント
	paxs::SettlementAgent agent(1, 25, 60, paxs::Genome(), 100, 0, 5);

	// When: 文化的属性が異なる配偶者と結婚
	agent.marry(2, paxs::Genome(), 80, 120, 8);

	// Then: 配偶者の文化属性が取得できる
	EXPECT_EQ(agent.cgetPartnerFarming(), 80);
	EXPECT_EQ(agent.cgetPartnerHunterGatherer(), 120);
	EXPECT_EQ(agent.cgetPartnerLanguage(), 8);
}

// ========================================
// Genome Tests
// ========================================

TEST(SettlementAgentUnitTest, GetGenome) {
	// Given: 特定のゲノムを持つエージェント
	paxs::Genome genome;
	genome.setMtDNA(15);
	genome.setYDNA(3);

	paxs::SettlementAgent agent(1, 20, 50, genome, 100, 0, 5);

	// Then: ゲノムが取得できる
	EXPECT_EQ(agent.cgetGenome().getMtDNA(), 15);
	EXPECT_EQ(agent.cgetGenome().getYDNA(), 3);
}

TEST(SettlementAgentUnitTest, SetPartnerGenome) {
	// Given: エージェント
	paxs::SettlementAgent agent(1, 25, 60, paxs::Genome(), 100, 0, 5);

	// And: 配偶者のゲノム
	paxs::Genome partner_genome;
	partner_genome.setMtDNA(20);
	partner_genome.setYDNA(7);

	// When: 配偶者のゲノムを設定
	agent.setPartnerGenome(partner_genome);

	// Then: 配偶者のゲノムが取得できる
	EXPECT_EQ(agent.cgetPartnerGenome().getMtDNA(), 20);
	EXPECT_EQ(agent.cgetPartnerGenome().getYDNA(), 7);
}

// ========================================
// Equality Operator Tests
// ========================================

TEST(SettlementAgentUnitTest, EqualityOperator_SameAgents) {
	// Given: 同じパラメータの2つのエージェント
	paxs::Genome genome;
	genome.setMtDNA(10);
	genome.setYDNA(5);

	paxs::SettlementAgent agent1(1, 25, 60, genome, 100, 50, 5);
	paxs::SettlementAgent agent2(1, 25, 60, genome, 100, 50, 5);

	// Then: 等価
	EXPECT_TRUE(agent1 == agent2);
}

TEST(SettlementAgentUnitTest, EqualityOperator_DifferentId) {
	// Given: IDが異なる2つのエージェント
	paxs::Genome genome;
	paxs::SettlementAgent agent1(1, 25, 60, genome, 100, 50, 5);
	paxs::SettlementAgent agent2(2, 25, 60, genome, 100, 50, 5);

	// Then: 非等価
	EXPECT_FALSE(agent1 == agent2);
}

TEST(SettlementAgentUnitTest, EqualityOperator_DifferentAge) {
	// Given: 年齢が異なる2つのエージェント
	paxs::Genome genome;
	paxs::SettlementAgent agent1(1, 25, 60, genome, 100, 50, 5);
	paxs::SettlementAgent agent2(1, 30, 60, genome, 100, 50, 5);

	// Then: 非等価
	EXPECT_FALSE(agent1 == agent2);
}

TEST(SettlementAgentUnitTest, EqualityOperator_WithPartnerInfo) {
	// Given: 配偶者情報を持つ2つのエージェント
	paxs::Genome genome;
	paxs::Genome partner_genome;
	partner_genome.setMtDNA(20);

	paxs::SettlementAgent agent1(1, 25, 60, genome, 100, 50, 5);
	agent1.marry(2, partner_genome, 80, 20, 8);

	paxs::SettlementAgent agent2(1, 25, 60, genome, 100, 50, 5);
	agent2.marry(2, partner_genome, 80, 20, 8);

	// Then: 等価
	EXPECT_TRUE(agent1 == agent2);
}

TEST(SettlementAgentUnitTest, EqualityOperator_DifferentPartner) {
	// Given: 配偶者IDが異なる2つのエージェント
	paxs::Genome genome;
	paxs::SettlementAgent agent1(1, 25, 60, genome, 100, 50, 5);
	agent1.marry(2, paxs::Genome(), 80, 20, 8);

	paxs::SettlementAgent agent2(1, 25, 60, genome, 100, 50, 5);
	agent2.marry(3, paxs::Genome(), 80, 20, 8);

	// Then: 非等価
	EXPECT_FALSE(agent1 == agent2);
}

// ========================================
// Setter Tests
// ========================================

TEST(SettlementAgentUnitTest, SetPartnerId) {
	// Given: エージェント
	paxs::SettlementAgent agent(1, 25, 60, paxs::Genome(), 100, 0, 5);

	// When: 配偶者IDを設定
	agent.setPartnerId(123);

	// Then: 配偶者IDが更新される
	EXPECT_EQ(agent.getPartnerId(), 123);
}
