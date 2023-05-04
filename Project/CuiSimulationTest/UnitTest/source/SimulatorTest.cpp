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
	const std::string file_name = "../../../Data/Map/XYZTile/LandAndWater/Data/BlackAndWhiteBinary/1868/10/";
    std::unique_ptr<paxs::Simulator<int>> simulator = std::make_unique<paxs::Simulator<int>>(file_name, paxs::Vector2<int>(861, 350), 10);
    simulator->init();
	simulator->run(10);
}