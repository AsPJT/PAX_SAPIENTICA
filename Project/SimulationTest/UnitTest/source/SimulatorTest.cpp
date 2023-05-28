/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <memory>

#include <PAX_SAPIENTICA/Simulation/Simulator.hpp>

int main() {
	const std::string land_file_path = "../../../Data/Map/XYZTile/LandAndWater/Data/BlackAndWhiteBinary/1868/10/";
	const std::string slope_file_path = "../../../Data/Map/XYZTile/Slope/Data/SlopeData20230514/2010/8/";
	paxs::Vector2<int> start_position(861, 350);
	paxs::Vector2<int> end_position(950, 450);
    std::unique_ptr<paxs::Simulator<int>> simulator = std::make_unique<paxs::Simulator<int>>(land_file_path, slope_file_path, start_position, end_position, 10);
    simulator->init();
	simulator->run(50);
	std::vector<paxs::Agent<int>> agents = simulator->getAgents();
}