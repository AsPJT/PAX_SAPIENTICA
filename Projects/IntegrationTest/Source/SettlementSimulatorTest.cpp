/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <cassert>
#include <iostream>
#include <memory>

#include <PAX_SAPIENTICA/Simulation/Manager/SettlementSimulator.hpp>

int main() {
    const std::string map_list_path = "Data/Simulations/Sample/MapList.tsv";
    const std::string& japan_provinces_path = "Data/Simulations/Sample";

    // 固定シードで決定的な動作を保証
    const unsigned int seed = 12345;
    std::unique_ptr<paxs::SettlementSimulator> simulator = std::make_unique<paxs::SettlementSimulator>(map_list_path, japan_provinces_path, seed);

    // 初期化検証
    simulator->init();
    std::cout << "[統合テスト] SettlementSimulator初期化完了\n";

    // 初期状態の検証
    const auto& initial_grids = simulator->cgetSettlementGrids();
    std::size_t initial_settlement_count = 0;
    std::size_t initial_population = 0;

    for (const auto& [key, grid] : initial_grids) {
        const auto& settlements = grid.cgetSettlements();
        initial_settlement_count += settlements.size();
        for (const auto& settlement : settlements) {
            initial_population += settlement.getPopulation();
        }
    }

    std::cout << "[統合テスト] 初期集落グリッド数: " << initial_grids.size() << "\n";
    std::cout << "[統合テスト] 初期集落数: " << initial_settlement_count << "\n";
    std::cout << "[統合テスト] 初期総人口: " << initial_population << "\n";

    // 固定シードによる決定的な初期状態の検証
    assert(initial_grids.size() == 18 && "初期集落グリッド数は18であること");
    assert(initial_settlement_count == 18 && "初期集落数は18であること");
    assert(initial_population == 1000 && "初期総人口は1000であること");

    // シミュレーション実行（50ステップ）
    const int steps = 50;
    std::cout << "[統合テスト] " << steps << "ステップのシミュレーション開始\n";
    simulator->run(steps);
    std::cout << "[統合テスト] シミュレーション完了\n";

    // シミュレーション後の状態検証
    const auto& final_grids = simulator->cgetSettlementGrids();
    std::size_t final_settlement_count = 0;
    std::size_t final_population = 0;

    for (const auto& [key, grid] : final_grids) {
        const auto& settlements = grid.cgetSettlements();
        final_settlement_count += settlements.size();
        for (const auto& settlement : settlements) {
            final_population += settlement.getPopulation();
        }
    }

    std::cout << "[統合テスト] 最終集落グリッド数: " << final_grids.size() << "\n";
    std::cout << "[統合テスト] 最終集落数: " << final_settlement_count << "\n";
    std::cout << "[統合テスト] 最終総人口: " << final_population << "\n";

    // 固定シードによる決定的な最終状態の検証
    assert(final_grids.size() == 115 && "最終集落グリッド数は115であること");
    assert(final_settlement_count == 18 && "最終集落数は18であること");
    assert(final_population == 645 && "最終総人口は645であること");

    // 人口変化のログ
    const int population_change = static_cast<int>(final_population) - static_cast<int>(initial_population);
    std::cout << "[統合テスト] 人口変化: " << (population_change >= 0 ? "+" : "") << population_change << "\n";
    assert(population_change == -355 && "人口変化は-355であること");

    // 全ての検証が成功
    std::cout << "[統合テスト] ✅ 全ての検証が成功しました\n";

    return 0;
}
