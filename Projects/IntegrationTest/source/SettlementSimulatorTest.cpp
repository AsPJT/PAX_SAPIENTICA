/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <memory>

#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>

int main() {
    const std::string map_list_path = "../../../Data/Simulation/MapList.tsv";
    const std::string& japan_provinces_path = "../../../Data/Simulation/Japan200-725";
    paxs::Vector2<int> start_position(861, 350);
    paxs::Vector2<int> end_position(950, 450);
    std::random_device seed_gen;
    std::unique_ptr<paxs::SettlementSimulator<int>> simulator = std::make_unique<paxs::SettlementSimulator<int>>(map_list_path, japan_provinces_path, start_position, end_position, 10, seed_gen());
    simulator->init();
    simulator->run(50);
}
