/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <iostream>

#include <PAX_SAPIENTICA/File.hpp>

int main()
{
	try{
		paxs::File::readTSV("test.tsv");
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
