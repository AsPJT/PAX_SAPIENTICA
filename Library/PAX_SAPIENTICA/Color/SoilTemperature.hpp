/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_COLOR_SOIL_TEMPERATURE_HPP
#define PAX_SAPIENTICA_COLOR_SOIL_TEMPERATURE_HPP

#include <array>
#include <string>

#include <PAX_SAPIENTICA/Color/RGBA.hpp>

namespace paxs {

    class SoilTemp {
        inline static constexpr std::array<paxs::RGBA, 91> soil_temp =
        {
              paxs::RGBA("#000000")
            , paxs::RGBA("#FFFFFF")
            , paxs::RGBA("#1A7DB7")
            , paxs::RGBA("#308ABE")
            , paxs::RGBA("#4697C5")
            , paxs::RGBA("#5BA4CC")
            , paxs::RGBA("#71B1D3")
            , paxs::RGBA("#87BEDA")
            , paxs::RGBA("#99C8DF")
            , paxs::RGBA("#A9D0E3")
            , paxs::RGBA("#B8D8E7")
            , paxs::RGBA("#C8DFEB")
            , paxs::RGBA("#DFEBF1")
            , paxs::RGBA("#EFF3F5")
            , paxs::RGBA("#F7F1EE")
            , paxs::RGBA("#F7E4DB")
            , paxs::RGBA("#F6D7C8")
            , paxs::RGBA("#FFC9B6")
            , paxs::RGBA("#F5BCA3")
            , paxs::RGBA("#F5AF90")
            , paxs::RGBA("#F39F7E")
            , paxs::RGBA("#F39F7E")
            , paxs::RGBA("#E56A5F")
            , paxs::RGBA("#DF4F4F")
            , paxs::RGBA("#D8353F")
            , paxs::RGBA("#D11A2F")
            , paxs::RGBA("#CA0020")
        };

    public:
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
            else {
                for (std::size_t i = 0; i < soil_temp.size(); ++i) {
                    const int new_diff = std::abs(int(rgba.r) - int(soil_temp[i].r))
                        + std::abs(int(rgba.g) - int(soil_temp[i].g))
                        + std::abs(int(rgba.b) - int(soil_temp[i].b));
                    if (diff > new_diff) {
                        diff = new_diff;
                        index = i;
                    }
                }
            }
            return index;
        }
};

}

#endif // !PAX_SAPIENTICA_COLOR_SOIL_TEMPERATURE_HPP
