/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>
#include <PAX_SAPIENTICA/Core/Type/Range.hpp>

// Range型の基本動作テスト
TEST(RangeTest, DefaultConstruction) {
	constexpr paxs::Range<int> range;
	EXPECT_EQ(range.minimum, 0);
	EXPECT_EQ(range.maximum, 0);
}

TEST(RangeTest, ValueConstruction) {
	constexpr paxs::Range<int> range(10, 20);
	EXPECT_EQ(range.minimum, 10);
	EXPECT_EQ(range.maximum, 20);
}

TEST(RangeTest, DoubleConstruction) {
	constexpr paxs::Range<double> range(1.5, 3.7);
	EXPECT_DOUBLE_EQ(range.minimum, 1.5);
	EXPECT_DOUBLE_EQ(range.maximum, 3.7);
}

// contains() のテスト
TEST(RangeTest, ContainsInside) {
	constexpr paxs::Range<int> range(10, 20);
	EXPECT_TRUE(range.contains(15));
	EXPECT_TRUE(range.contains(10));  // 境界値min
	EXPECT_TRUE(range.contains(20));  // 境界値max
}

TEST(RangeTest, ContainsOutside) {
	constexpr paxs::Range<int> range(10, 20);
	EXPECT_FALSE(range.contains(9));
	EXPECT_FALSE(range.contains(21));
	EXPECT_FALSE(range.contains(0));
	EXPECT_FALSE(range.contains(100));
}

TEST(RangeTest, ContainsDouble) {
	constexpr paxs::Range<double> range(1.0, 10.0);
	EXPECT_TRUE(range.contains(5.5));
	EXPECT_TRUE(range.contains(1.0));
	EXPECT_TRUE(range.contains(10.0));
	EXPECT_FALSE(range.contains(0.9));
	EXPECT_FALSE(range.contains(10.1));
}

// excludes() のテスト
TEST(RangeTest, ExcludesOutside) {
	constexpr paxs::Range<int> range(10, 20);
	EXPECT_TRUE(range.excludes(9));
	EXPECT_TRUE(range.excludes(21));
	EXPECT_TRUE(range.excludes(0));
	EXPECT_TRUE(range.excludes(100));
}

TEST(RangeTest, ExcludesInside) {
	constexpr paxs::Range<int> range(10, 20);
	EXPECT_FALSE(range.excludes(15));
	EXPECT_FALSE(range.excludes(10));  // 境界値min
	EXPECT_FALSE(range.excludes(20));  // 境界値max
}

// length() のテスト
TEST(RangeTest, Length) {
	constexpr paxs::Range<int> range1(10, 20);
	EXPECT_EQ(range1.length(), 10);

	constexpr paxs::Range<int> range2(0, 100);
	EXPECT_EQ(range2.length(), 100);

	constexpr paxs::Range<double> range3(1.5, 3.5);
	EXPECT_DOUBLE_EQ(range3.length(), 2.0);
}

// center() のテスト
TEST(RangeTest, Center) {
	constexpr paxs::Range<int> range1(10, 20);
	EXPECT_EQ(range1.center(), 15);

	constexpr paxs::Range<int> range2(0, 100);
	EXPECT_EQ(range2.center(), 50);

	constexpr paxs::Range<double> range3(1.0, 3.0);
	EXPECT_DOUBLE_EQ(range3.center(), 2.0);
}

// clamp() のテスト
TEST(RangeTest, ClampInside) {
	constexpr paxs::Range<int> range(10, 20);
	EXPECT_EQ(range.clamp(15), 15);
	EXPECT_EQ(range.clamp(10), 10);
	EXPECT_EQ(range.clamp(20), 20);
}

TEST(RangeTest, ClampOutside) {
	constexpr paxs::Range<int> range(10, 20);
	EXPECT_EQ(range.clamp(5), 10);   // 範囲未満
	EXPECT_EQ(range.clamp(25), 20);  // 範囲超過
	EXPECT_EQ(range.clamp(0), 10);
	EXPECT_EQ(range.clamp(100), 20);
}

TEST(RangeTest, ClampDouble) {
	constexpr paxs::Range<double> range(1.0, 10.0);
	EXPECT_DOUBLE_EQ(range.clamp(5.5), 5.5);
	EXPECT_DOUBLE_EQ(range.clamp(0.5), 1.0);
	EXPECT_DOUBLE_EQ(range.clamp(15.0), 10.0);
}

// overlaps() のテスト
TEST(RangeTest, OverlapsTrue) {
	constexpr paxs::Range<int> range1(10, 20);
	constexpr paxs::Range<int> range2(15, 25);
	EXPECT_TRUE(range1.overlaps(range2));
	EXPECT_TRUE(range2.overlaps(range1));
}

TEST(RangeTest, OverlapsFalse) {
	constexpr paxs::Range<int> range1(10, 20);
	constexpr paxs::Range<int> range2(21, 30);
	EXPECT_FALSE(range1.overlaps(range2));
	EXPECT_FALSE(range2.overlaps(range1));
}

TEST(RangeTest, OverlapsBoundary) {
	constexpr paxs::Range<int> range1(10, 20);
	constexpr paxs::Range<int> range2(20, 30);  // 境界が接している
	EXPECT_TRUE(range1.overlaps(range2));
	EXPECT_TRUE(range2.overlaps(range1));
}

TEST(RangeTest, OverlapsContained) {
	constexpr paxs::Range<int> range1(10, 30);
	constexpr paxs::Range<int> range2(15, 25);  // range2がrange1に完全に含まれる
	EXPECT_TRUE(range1.overlaps(range2));
	EXPECT_TRUE(range2.overlaps(range1));
}

// 比較演算子のテスト
TEST(RangeTest, Equality) {
	constexpr paxs::Range<int> range1(10, 20);
	constexpr paxs::Range<int> range2(10, 20);
	constexpr paxs::Range<int> range3(15, 25);

	EXPECT_EQ(range1, range2);
	EXPECT_NE(range1, range3);
}

TEST(RangeTest, Comparison) {
	constexpr paxs::Range<int> range1(10, 20);
	constexpr paxs::Range<int> range2(15, 25);
	constexpr paxs::Range<int> range3(10, 25);

	EXPECT_LT(range1, range2);
	EXPECT_LT(range1, range3);
	EXPECT_LE(range1, range2);
	EXPECT_GT(range2, range1);
	EXPECT_GE(range2, range1);
}

// constexpr テスト
TEST(RangeTest, ConstexprUsage) {
	constexpr paxs::Range<int> range(10, 20);
	constexpr bool contains_15 = range.contains(15);
	constexpr bool excludes_5 = range.excludes(5);
	constexpr int length = range.length();
	constexpr int center = range.center();
	constexpr int clamped = range.clamp(25);

	EXPECT_TRUE(contains_15);
	EXPECT_TRUE(excludes_5);
	EXPECT_EQ(length, 10);
	EXPECT_EQ(center, 15);
	EXPECT_EQ(clamped, 20);
}

// 実際の使用例：ズームレベルの範囲チェック
TEST(RangeTest, ZoomLevelUseCase) {
	constexpr paxs::Range<double> zoom_range(0.5, 10.0);
	constexpr double current_zoom = 5.0;

	// 範囲内チェック（GeographicFeature.hpp:98のようなケース）
	EXPECT_TRUE(zoom_range.contains(current_zoom));
	EXPECT_FALSE(zoom_range.excludes(current_zoom));
}

// 実際の使用例：時間範囲のチェック
TEST(RangeTest, TimeRangeUseCase) {
	constexpr paxs::Range<double> time_range(1000.0, 2000.0);  // ユリウス日
	constexpr double jdn = 1500.0;

	// PlaceNameFeature.hpp:118のようなケース
	EXPECT_TRUE(time_range.contains(jdn));

	// GenomeFeature.hpp:100のようなケース（範囲外チェック）
	EXPECT_FALSE(time_range.excludes(jdn));
}
