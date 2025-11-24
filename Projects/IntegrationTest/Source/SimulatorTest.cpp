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

#include <PAX_SAPIENTICA/Simulation/Manager/Simulator.hpp>

int main() {
	const std::string setting_file_path = "Data/Simulations/Sample/MapList.tsv";
	const unsigned int seed = 10;

	std::cout << "[統合テスト] Simulator初期化開始\n";
	std::unique_ptr<paxs::Simulator> simulator = std::make_unique<paxs::Simulator>(setting_file_path, seed);
	simulator->init();
	std::cout << "[統合テスト] Simulator初期化完了\n";

	// シミュレーション実行
	const int steps = 50;
	std::cout << "[統合テスト] " << steps << "ステップのシミュレーション開始\n";
	simulator->run(steps);
	std::cout << "[統合テスト] シミュレーション完了\n";

	// エージェント取得と検証
	std::vector<paxs::Agent> agents = simulator->getAgents();
	std::cout << "[統合テスト] エージェント数: " << agents.size() << "\n";

	// 固定シード(seed=10)による決定的な結果の検証
	assert(agents.size() == 6352 && "エージェント数は6352であること");

	// 最初のエージェントの検証
	std::cout << "[統合テスト] 最初のエージェントID: " << agents[0].getId() << "\n";
	assert(agents[0].getId() == 7 && "最初のエージェントIDは7であること");

	std::cout << "[統合テスト] ✅ 全ての検証が成功しました\n";
	return 0;
}
