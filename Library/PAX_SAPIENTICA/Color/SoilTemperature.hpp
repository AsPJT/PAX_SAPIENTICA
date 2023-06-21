/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_COLOR_SOIL_TEMPERATURE_HPP
#define PAX_SAPIENTICA_COLOR_SOIL_TEMPERATURE_HPP

/*##########################################################################################

##########################################################################################*/

#include <vector>
#include <string>

#include <PAX_SAPIENTICA/Color/RGBA.hpp>

namespace paxs {

	struct SoilTemp {
		std::vector<paxs::RGBA> soil_temp =
		{
		std::string("#000000")
		,	std::string("#FFFFFF")
		,	std::string("#1A7DB7")
		,	std::string("#308ABE")
		,	std::string("#4697C5")
		,	std::string("#5BA4CC")
		,	std::string("#71B1D3")
		,	std::string("#87BEDA")
		,	std::string("#99C8DF")
		,	std::string("#A9D0E3")
		,	std::string("#B8D8E7")
		,	std::string("#C8DFEB")
		,	std::string("#DFEBF1")
		,	std::string("#EFF3F5")
		,	std::string("#F7F1EE")
		,	std::string("#F7E4DB")
		,	std::string("#F6D7C8")
		,	std::string("#FFC9B6")
		,	std::string("#F5BCA3")
		,	std::string("#F5AF90")
		,	std::string("#F39F7E")
		,	std::string("#F39F7E")
		,	std::string("#E56A5F")
		,	std::string("#DF4F4F")
		,	std::string("#D8353F")
		,	std::string("#D11A2F")
		,	std::string("#CA0020")
		};

		std::size_t getIndex(const RGBA& rgba) {
			std::size_t index = 0;
			int diff = 99999999;

			// 不透明度が255以外の時は0
			if (rgba.a != 255) {
				index = 0;
			}
			else if (rgba.r == 255 && rgba.g == 255 && rgba.b == 255) {
				index = 0;
			}
			else if (rgba.r == 0 && rgba.g == 0 && rgba.b == 0) {
				index = 0;
			}
			// 不透明度が255の時は0～25の中で最も近い値へ変換する（白、黒に1番近い場合は０となる）
			else
				for (std::size_t i = 0; i < soil_temp.size(); ++i) {
					const int new_diff = std::abs(int(rgba.r) - int(soil_temp[i].r))
						+ std::abs(int(rgba.g) - int(soil_temp[i].g))
						+ std::abs(int(rgba.b) - int(soil_temp[i].b));
					if (diff > new_diff) {
						diff = new_diff;
						index = i;
					}
				}
			return index;
		}
};

}

#endif // !PAX_SAPIENTICA_COLOR_SOIL_TEMPERATURE_HPP