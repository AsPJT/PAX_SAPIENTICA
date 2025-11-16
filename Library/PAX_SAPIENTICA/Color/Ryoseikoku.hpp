/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_COLOR_RYOSEIKOKU_HPP
#define PAX_SAPIENTICA_COLOR_RYOSEIKOKU_HPP

#include <array>

#include <PAX_SAPIENTICA/Color/RGBA.hpp>

namespace paxs {

    class RyoseikokuColor {
        static constexpr std::array<paxs::RGBA, 91> soil_temp =
        {
            paxs::RGBA{0,0,0}, // 0
            paxs::RGBA{245,105,100},
            paxs::RGBA{240,110,100},
            paxs::RGBA{235,115,100},
            paxs::RGBA{230,120,100},
            paxs::RGBA{225,125,100},
            paxs::RGBA{220,130,100},
            paxs::RGBA{215,135,100},
            paxs::RGBA{210,140,100},
            paxs::RGBA{205,145,100},
            paxs::RGBA{200,150,100}, // 10
            paxs::RGBA{195,155,100},
            paxs::RGBA{190,160,100},
            paxs::RGBA{185,165,100},
            paxs::RGBA{180,170,100},
            paxs::RGBA{175,175,100},
            paxs::RGBA{170,180,100},
            paxs::RGBA{165,185,100},
            paxs::RGBA{160,190,100},
            paxs::RGBA{155,195,100},
            paxs::RGBA{150,200,100}, // 20
            paxs::RGBA{145,205,100},
            paxs::RGBA{140,210,100},
            paxs::RGBA{135,215,100},
            paxs::RGBA{130,220,100},
            paxs::RGBA{125,225,100},
            paxs::RGBA{120,230,100},
            paxs::RGBA{115,235,100},
            paxs::RGBA{110,240,100},
            paxs::RGBA{105,245,100},
            paxs::RGBA{100,250,100}, // 30
            paxs::RGBA{100,245,105},
            paxs::RGBA{100,240,110},
            paxs::RGBA{100,235,115},
            paxs::RGBA{100,230,120},
            paxs::RGBA{100,225,125},
            paxs::RGBA{100,220,130},
            paxs::RGBA{100,215,135},
            paxs::RGBA{100,210,140},
            paxs::RGBA{100,205,145},
            paxs::RGBA{100,200,150}, // 40
            paxs::RGBA{100,195,155},
            paxs::RGBA{100,190,160},
            paxs::RGBA{100,185,165},
            paxs::RGBA{100,180,170},
            paxs::RGBA{100,175,175},
            paxs::RGBA{100,170,180},
            paxs::RGBA{100,165,185},
            paxs::RGBA{100,160,190},
            paxs::RGBA{100,155,195},
            paxs::RGBA{100,150,200}, // 50
            paxs::RGBA{100,145,205},
            paxs::RGBA{100,140,210},
            paxs::RGBA{100,135,215},
            paxs::RGBA{100,130,220},
            paxs::RGBA{100,125,225},
            paxs::RGBA{100,120,230},
            paxs::RGBA{100,115,235},
            paxs::RGBA{100,110,240},
            paxs::RGBA{100,105,245},
            paxs::RGBA{100,100,250}, // 60
            paxs::RGBA{105,100,245},
            paxs::RGBA{110,100,240},
            paxs::RGBA{115,100,235},
            paxs::RGBA{120,100,230},
            paxs::RGBA{125,100,225},
            paxs::RGBA{130,100,220},
            paxs::RGBA{135,100,215},
            paxs::RGBA{140,100,210},
            paxs::RGBA{145,100,205},
            paxs::RGBA{150,100,200}, // 70
            paxs::RGBA{155,100,195},
            paxs::RGBA{160,100,190},
            paxs::RGBA{165,100,185},
            paxs::RGBA{170,100,180},
            paxs::RGBA{175,100,175},
            paxs::RGBA{180,100,170},
            paxs::RGBA{185,100,165},
            paxs::RGBA{190,100,160},
            paxs::RGBA{195,100,155},
            paxs::RGBA{200,100,150}, // 80
            paxs::RGBA{205,100,145},
            paxs::RGBA{210,100,140},
            paxs::RGBA{215,100,135},
            paxs::RGBA{220,100,130},
            paxs::RGBA{225,100,125},
            paxs::RGBA{230,100,120},
            paxs::RGBA{235,100,115},
            paxs::RGBA{240,100,110},
            paxs::RGBA{245,100,105},
            paxs::RGBA{250,100,100} // 90
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
