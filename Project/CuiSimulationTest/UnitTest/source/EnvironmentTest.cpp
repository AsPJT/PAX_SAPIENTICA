/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <string>

#include <PAX_SAPIENTICA/Simulation/Environment.hpp>

int main()
{
	const std::string file_name = "../../../Data/Map/XYZTile/LandAndWater/Data/BlackAndWhiteBinary/1868/10/";
	paxs::Vector2<int> start_position(861, 350);
	paxs::Vector2<int> end_position(950, 450);
    paxs::Environment<int> environment(file_name, start_position, end_position, 10);
	std::cout << "geographic_informations size: " << environment.geographic_informations.size() << std::endl;
}
