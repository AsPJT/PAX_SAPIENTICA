/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <memory>

#include <PAX_SAPIENTICA/Simulation/Manager/Simulator.hpp>

int main() {
	const std::string setting_file_path = "../../../Data/Simulations/MapList.tsv";
    std::unique_ptr<paxs::Simulator> simulator = std::make_unique<paxs::Simulator>(setting_file_path, 10);
    simulator->init();
	simulator->run(50);
	std::vector<paxs::Agent> agents = simulator->getAgents();
}
