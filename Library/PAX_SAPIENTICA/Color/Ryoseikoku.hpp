﻿/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_COLOR_RYOSEIKOKU_HPP
#define PAX_SAPIENTICA_COLOR_RYOSEIKOKU_HPP

/*##########################################################################################

##########################################################################################*/

#include <vector>
#include <string>

#include <PAX_SAPIENTICA/Color/RGBA.hpp>

namespace paxs {

    struct RyoseikokuColor {
        std::vector<paxs::RGBA> soil_temp =
        {
            {0,0,0},
            {245,105,100},
            {240,110,100},
            {235,115,100},
            {230,120,100},
            {225,125,100},
            {220,130,100},
            {215,135,100},
            {210,140,100},
            {205,145,100},
            {200,150,100},
            {195,155,100},
            {190,160,100},
            {185,165,100},
            {180,170,100},
            {175,175,100},
            {170,180,100},
            {165,185,100},
            {160,190,100},
            {155,195,100},
            {150,200,100},
            {145,205,100},
            {140,210,100},
            {135,215,100},
            {130,220,100},
            {125,225,100},
            {120,230,100},
            {115,235,100},
            {110,240,100},
            {105,245,100},
            {100,250,100},
            {100,245,105},
            {100,240,110},
            {100,235,115},
            {100,230,120},
            {100,225,125},
            {100,220,130},
            {100,215,135},
            {100,210,140},
            {100,205,145},
            {100,200,150},
            {100,195,155},
            {100,190,160},
            {100,185,165},
            {100,180,170},
            {100,175,175},
            {100,170,180},
            {100,165,185},
            {100,160,190},
            {100,155,195},
            {100,150,200},
            {100,145,205},
            {100,140,210},
            {100,135,215},
            {100,130,220},
            {100,125,225},
            {100,120,230},
            {100,115,235},
            {100,110,240},
            {100,105,245},
            {100,100,250},
            {105,100,245},
            {110,100,240},
            {115,100,235},
            {120,100,230},
            {125,100,225},
            {130,100,220},
            {135,100,215},
            {140,100,210},
            {145,100,205},
            {150,100,200},
            {155,100,195},
            {160,100,190},
            {165,100,185},
            {170,100,180},
            {175,100,175},
            {180,100,170},
            {185,100,165},
            {190,100,160},
            {195,100,155},
            {200,100,150},
            {205,100,145},
            {210,100,140},
            {215,100,135},
            {220,100,130},
            {225,100,125},
            {230,100,120},
            {235,100,115},
            {240,100,110},
            {245,100,105},
            {250,100,100}
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
            // 不透明度が255の時は0～90の中で最も近い値へ変換する（白、黒に1番近い場合は０となる）
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

#endif // !PAX_SAPIENTICA_COLOR_RYOSEIKOKU_HPP
