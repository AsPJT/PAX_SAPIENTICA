/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <string>

#include <PAX_SAPIENTICA/Simulation/Environment.hpp>

int main()
{
	const std::string setting_file_path = "../../../Data/Simulations/MapList.tsv";
    paxs::Environment<int> environment(setting_file_path);
	std::cout << "data size: " << environment.data_map.size() << std::endl;
}
