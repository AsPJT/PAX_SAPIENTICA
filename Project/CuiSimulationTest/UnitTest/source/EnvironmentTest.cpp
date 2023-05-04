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
    paxs::Environment<int> environment(file_name, 861, 350, 10);
	std::cout << "geographic_informations size: " << environment.geographic_informations.size() << std::endl;
}
