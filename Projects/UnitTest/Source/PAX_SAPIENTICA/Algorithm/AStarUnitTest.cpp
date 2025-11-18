/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include <PAX_SAPIENTICA/Simulation/Algorithm/AStar.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/Environment.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>

// A* 経路探索アルゴリズムのユニットテスト
// リファクタリング前後で同じ経路が得られることを検証

class AStarTest : public ::testing::Test {
protected:
	std::shared_ptr<paxs::Environment> environment;

	void SetUp() override {
		environment = std::make_shared<paxs::Environment>();
	}
};

// 基本的な経路探索のテスト
TEST_F(AStarTest, BasicPathfinding) {
	using AStarVec2 = paxs::Vector2<paxs::GridType>;

	// 開始点と終了点
	AStarVec2 start(0, 0);
	AStarVec2 end(100, 100);
	paxs::GridType z = 1;

	// A*インスタンス作成
	paxs::AStar astar(start, end, z);

	// 経路探索実行
	bool found = astar.search(environment);

	// 経路が見つかるはず
	EXPECT_TRUE(found);

	// コストが正の値
	double cost = astar.getCost();
	EXPECT_GT(cost, 0.0);
}

// 経路の一貫性テスト（同じ入力で同じ経路が得られる）
TEST_F(AStarTest, PathConsistency) {
	using AStarVec2 = paxs::Vector2<paxs::GridType>;

	AStarVec2 start(0, 0);
	AStarVec2 end(100, 100);
	paxs::GridType z = 1;

	// 1回目の探索
	paxs::AStar astar1(start, end, z);
	bool found1 = astar1.search(environment);
	double cost1 = astar1.getCost();
	std::vector<AStarVec2> path1;
	astar1.setPath(path1);

	// 2回目の探索（同じ条件）
	paxs::AStar astar2(start, end, z);
	bool found2 = astar2.search(environment);
	double cost2 = astar2.getCost();
	std::vector<AStarVec2> path2;
	astar2.setPath(path2);

	// 結果が同じであることを確認
	EXPECT_EQ(found1, found2);
	EXPECT_DOUBLE_EQ(cost1, cost2);
	EXPECT_EQ(path1.size(), path2.size());

	// 経路の各点が同じ
	for (size_t i = 0; i < path1.size(); ++i) {
		EXPECT_EQ(path1[i], path2[i]);
	}
}

// 距離計算のテスト
TEST_F(AStarTest, DistanceCalculation) {
	using AStarVec2 = paxs::Vector2<paxs::GridType>;

	AStarVec2 start(0, 0);
	AStarVec2 end(3, 4);  // 距離5の直角三角形
	paxs::GridType z = 1;

	paxs::AStar astar(start, end, z);

	// 終点までの距離は 3^2 + 4^2 = 25 (二乗距離)
	paxs::GridType distance = astar.calculateDistance(start);
	EXPECT_EQ(distance, 25);

	// 終点での距離は0
	paxs::GridType distance_at_end = astar.calculateDistance(end);
	EXPECT_EQ(distance_at_end, 0);
}

// AStarNode の等価演算子テスト
TEST(AStarNodeTest, EqualityOperator) {
	using AStarVec2 = paxs::Vector2<paxs::GridType>;

	paxs::AStarNode node1(AStarVec2(10, 20), AStarVec2(0, 0), 100, 5.0);
	paxs::AStarNode node2(AStarVec2(10, 20), AStarVec2(5, 5), 200, 10.0);
	paxs::AStarNode node3(AStarVec2(30, 40), AStarVec2(0, 0), 100, 5.0);

	// 位置が同じならノードは等しい
	EXPECT_TRUE(node1 == node2);

	// 位置が異なれば等しくない
	EXPECT_FALSE(node1 == node3);

	// Vector2との比較
	EXPECT_TRUE(node1 == AStarVec2(10, 20));
	EXPECT_FALSE(node1 == AStarVec2(30, 40));
}

// 短距離経路のテスト
TEST_F(AStarTest, ShortPath) {
	using AStarVec2 = paxs::Vector2<paxs::GridType>;

	// 隣接点への経路
	AStarVec2 start(0, 0);
	AStarVec2 end(1, 1);
	paxs::GridType z = 1;

	paxs::AStar astar(start, end, z);
	bool found = astar.search(environment);

	EXPECT_TRUE(found);

	std::vector<AStarVec2> path;
	astar.setPath(path);

	// 経路が存在する
	EXPECT_GT(path.size(), 0);
}

// 同一点への経路（開始点=終了点）
TEST_F(AStarTest, SameStartEnd) {
	using AStarVec2 = paxs::Vector2<paxs::GridType>;

	AStarVec2 start(50, 50);
	AStarVec2 end(50, 50);
	paxs::GridType z = 1;

	paxs::AStar astar(start, end, z);

	// 距離は0
	paxs::GridType distance = astar.calculateDistance(start);
	EXPECT_EQ(distance, 0);
}

// 異なるzレベルでのテスト
TEST_F(AStarTest, DifferentZLevels) {
	using AStarVec2 = paxs::Vector2<paxs::GridType>;

	AStarVec2 start(0, 0);
	AStarVec2 end(256, 256);

	// z=1とz=2で経路を比較
	paxs::AStar astar1(start, end, 1);
	bool found1 = astar1.search(environment);
	double cost1 = astar1.getCost();

	paxs::AStar astar2(start, end, 2);
	bool found2 = astar2.search(environment);
	double cost2 = astar2.getCost();

	// どちらも経路が見つかる
	EXPECT_TRUE(found1);
	EXPECT_TRUE(found2);

	// コストは異なる可能性がある（探索範囲が違うため）
	EXPECT_GT(cost1, 0.0);
	EXPECT_GT(cost2, 0.0);
}
