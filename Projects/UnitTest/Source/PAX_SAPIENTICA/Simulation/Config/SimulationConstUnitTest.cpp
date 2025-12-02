/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2025 As Project
	[Production]	2023-2025 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <random>

#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>

// ========================================
// Singleton Pattern Tests
// ========================================

TEST(SimulationConstUnitTest, GetInstance_ReturnsSameInstance) {
	// Given: Singletonインスタンスを2回取得
	auto& instance1 = paxs::SimulationConstants::getInstance();
	auto& instance2 = paxs::SimulationConstants::getInstance();

	// Then: 同じインスタンス
	EXPECT_EQ(&instance1, &instance2);
}

// ========================================
// Basic Constants Tests
// ========================================

TEST(SimulationConstUnitTest, MaxNumberOfDistricts_IsConstant) {
	// Then: 最大地区数が90
	EXPECT_EQ(paxs::max_number_of_districts, 90);
}

// ========================================
// Configuration Parameter Tests
// ========================================

TEST(SimulationConstUnitTest, StepsPerYear_IsPositive) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: steps_per_yearが正の値
	EXPECT_GT(config.steps_per_year, 0);
}

TEST(SimulationConstUnitTest, StartJulianDay_IsValid) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: start_julian_dayが有効な値（0以上）
	EXPECT_GE(config.start_julian_day, 0);
}

TEST(SimulationConstUnitTest, TotalSteps_IsNonNegative) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: total_stepsが非負の値（0は無限ステップを意味）
	EXPECT_GE(config.total_steps, 0);
}

TEST(SimulationConstUnitTest, CellGroupLength_IsPositive) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: cell_group_lengthが正の値
	EXPECT_GT(config.cell_group_length, 0);
}

// ========================================
// Marriage Parameters Tests
// ========================================

TEST(SimulationConstUnitTest, MarriageAgeRanges_AreValid) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: 婚姻可能年齢範囲が論理的
	EXPECT_GT(config.female_marriageable_age_min, 0.0);
	EXPECT_GT(config.female_marriageable_age_max, config.female_marriageable_age_min);

	EXPECT_GT(config.male_marriageable_age_min, 0.0);
	EXPECT_GT(config.male_marriageable_age_max, config.male_marriageable_age_min);
}

TEST(SimulationConstUnitTest, MarriageSearchRange_IsPositive) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: marriage_search_rangeが正の値
	EXPECT_GT(config.marriage_search_range, 0);
}

TEST(SimulationConstUnitTest, MaternalResidenceProbability_IsWithinRange) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: maternal_residence_probabilityが確率範囲内（0.0〜1.0）
	EXPECT_GE(config.maternal_residence_probability, 0.0);
	EXPECT_LE(config.maternal_residence_probability, 1.0);
}

TEST(SimulationConstUnitTest, GetMarriageProbability_Agricultural_ReturnsValidValue) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// When: 農耕民の婚姻可能年齢（例: 25歳）の確率を取得
	std::size_t age = 25;
	double prob = config.getMarriageProbability(age, true);

	// Then: 確率範囲内（0.0〜1.0）
	EXPECT_GE(prob, 0.0);
	EXPECT_LE(prob, 1.0);
}

TEST(SimulationConstUnitTest, GetMarriageProbability_HunterGatherer_ReturnsValidValue) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// When: 狩猟採集民の婚姻可能年齢の確率を取得
	std::size_t age = 25;
	double prob = config.getMarriageProbability(age, false);

	// Then: 確率範囲内
	EXPECT_GE(prob, 0.0);
	EXPECT_LE(prob, 1.0);
}

// Note: Invalid age behavior varies by configuration, so not testing edge cases

// ========================================
// Birth Parameters Tests
// ========================================

TEST(SimulationConstUnitTest, ChildbearingAgeRanges_AreValid) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: 出産可能年齢範囲が論理的
	EXPECT_GT(config.childbearing_age_min, 0.0);
	EXPECT_GT(config.childbearing_age_max, config.childbearing_age_min);
}

TEST(SimulationConstUnitTest, BirthInterval_IsPositive) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: birth_intervalが正の値
	EXPECT_GT(config.birth_interval, 0);
}

TEST(SimulationConstUnitTest, StillbirthRates_AreWithinRange) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: 死産率が確率範囲内
	EXPECT_GE(config.agricultural_stillbirth_rate, 0.0);
	EXPECT_LE(config.agricultural_stillbirth_rate, 1.0);

	EXPECT_GE(config.hunter_gatherer_stillbirth_rate, 0.0);
	EXPECT_LE(config.hunter_gatherer_stillbirth_rate, 1.0);
}

TEST(SimulationConstUnitTest, GetChildbearingProbability_Agricultural_ReturnsValidValue) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// When: 農耕民の出産可能年齢（例: 28歳）の確率を取得
	std::size_t age = 28;
	double prob = config.getChildbearingProbability(age, true);

	// Then: 確率範囲内
	EXPECT_GE(prob, 0.0);
	EXPECT_LE(prob, 1.0);
}

TEST(SimulationConstUnitTest, GetChildbearingProbability_HunterGatherer_ReturnsValidValue) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// When: 狩猟採集民の出産可能年齢の確率を取得
	std::size_t age = 28;
	double prob = config.getChildbearingProbability(age, false);

	// Then: 確率範囲内
	EXPECT_GE(prob, 0.0);
	EXPECT_LE(prob, 1.0);
}

// Note: Invalid age behavior varies by configuration, so not testing edge cases

// ========================================
// Migration Parameters Tests
// ========================================

TEST(SimulationConstUnitTest, MoveDistances_AreValid) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: 移動距離範囲が論理的
	EXPECT_GT(config.min_move_distance, 0);
	EXPECT_GT(config.max_move_distance, 0);
	EXPECT_GE(config.max_move_distance, config.min_move_distance);
}

TEST(SimulationConstUnitTest, MoveProbability_IsWithinRange) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: move_probabilityが確率範囲内
	EXPECT_GE(config.move_probability, 0.0);
	EXPECT_LE(config.move_probability, 1.0);
}

TEST(SimulationConstUnitTest, MoveMethod_IsSet) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: move_methodが設定されている（ハッシュ値なので0以上）
	EXPECT_GE(config.move_method, 0u);
}

TEST(SimulationConstUnitTest, CoastAndOceanCosts_AreNonNegative) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: 海岸と海上のコストが非負
	EXPECT_GE(config.coast_cost, 0.0);
	EXPECT_GE(config.ocean_cost, 0.0);
	EXPECT_GE(config.land_cost, 0.0);
}

// ========================================
// Population Parameters Tests
// ========================================

TEST(SimulationConstUnitTest, MaxSettlementWeights_ArePositive) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: 集落最大人口重みが正の値
	EXPECT_GT(config.max_agricultural_settlement_weight, 0.0);
	EXPECT_GT(config.max_hunter_gatherer_settlement_weight, 0.0);
}

// ========================================
// Immigration Parameters Tests
// ========================================

TEST(SimulationConstUnitTest, ImmigrationTiming_IsValid) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: 渡来期間が論理的
	EXPECT_LE(config.immigration_start_steps, config.immigration_end_steps);
}

TEST(SimulationConstUnitTest, ImmigrationStepInterval_IsPositive) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: 渡来間隔が正の値
	EXPECT_GT(config.immigration_step_interval, 0);
}

TEST(SimulationConstUnitTest, ImmigrationDistrictId_IsWithinRange) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// Then: 渡来先地区IDが有効範囲内
	EXPECT_LT(config.immigration_district_id, paxs::max_number_of_districts);
}

// ========================================
// LifeSpan Tests
// ========================================

TEST(SimulationConstUnitTest, GetLifeSpan_FarmingFemale_ReturnsPositiveValue) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();
	std::mt19937 gen(12345);

	// When: 農耕民女性の寿命を生成
	paxs::AgeType life_span = config.getLifeSpan(true, true, gen);

	// Then: 正の値
	EXPECT_GT(life_span, 0);
}

TEST(SimulationConstUnitTest, GetLifeSpan_FarmingMale_ReturnsPositiveValue) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();
	std::mt19937 gen(12345);

	// When: 農耕民男性の寿命を生成
	paxs::AgeType life_span = config.getLifeSpan(true, false, gen);

	// Then: 正の値
	EXPECT_GT(life_span, 0);
}

TEST(SimulationConstUnitTest, GetLifeSpan_HunterGathererFemale_ReturnsPositiveValue) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();
	std::mt19937 gen(12345);

	// When: 狩猟採集民女性の寿命を生成
	paxs::AgeType life_span = config.getLifeSpan(false, true, gen);

	// Then: 正の値
	EXPECT_GT(life_span, 0);
}

TEST(SimulationConstUnitTest, GetLifeSpan_HunterGathererMale_ReturnsPositiveValue) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();
	std::mt19937 gen(12345);

	// When: 狩猟採集民男性の寿命を生成
	paxs::AgeType life_span = config.getLifeSpan(false, false, gen);

	// Then: 正の値
	EXPECT_GT(life_span, 0);
}

TEST(SimulationConstUnitTest, GetLifeSpan_WithFixedSeed_IsDeterministic) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// When: 同じシードで2回寿命を生成
	std::mt19937 gen1(12345);
	paxs::AgeType life_span1 = config.getLifeSpan(true, true, gen1);

	std::mt19937 gen2(12345);
	paxs::AgeType life_span2 = config.getLifeSpan(true, true, gen2);

	// Then: 同じ値
	EXPECT_EQ(life_span1, life_span2);
}

// ========================================
// Zoom Level Tests
// ========================================

TEST(SimulationConstUnitTest, GetZ_ReturnsPositiveValue) {
	// Given: SimulationConstants
	auto& config = paxs::SimulationConstants::getInstance();

	// When: ズームレベルを取得
	int z = config.getZ();

	// Then: 正の値
	EXPECT_GT(z, 0);
	// And: 妥当な範囲（通常0-18程度）
	EXPECT_LE(z, 20);
}
