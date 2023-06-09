/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_COLOR_RGBA_HPP
#define PAX_SAPIENTICA_COLOR_RGBA_HPP

/*##########################################################################################

##########################################################################################*/

#include <string>

namespace paxs {

	struct RGBA {
	public:
		unsigned char r = 0, g = 0, b = 0, a = 255; //赤, 緑, 青, 不透過
	private:
		unsigned char getHexToRGB(const char hex) {
			switch (hex) {
			case '0':return 0; break;
			case '1':return 1; break;
			case '2':return 2; break;
			case '3':return 3; break;
			case '4':return 4; break;
			case '5':return 5; break;
			case '6':return 6; break;
			case '7':return 7; break;
			case '8':return 8; break;
			case '9':return 9; break;
			case 'a':return 10; break;
			case 'b':return 11; break;
			case 'c':return 12; break;
			case 'd':return 13; break;
			case 'e':return 14; break;
			case 'f':return 15; break;
			case 'A':return 10; break;
			case 'B':return 11; break;
			case 'C':return 12; break;
			case 'D':return 13; break;
			case 'E':return 14; break;
			case 'F':return 15; break;
			}
			return 0;
		}
		unsigned char getHexToRGB(const char hex1, const char hex2) {
			return (getHexToRGB(hex1) * 16 + getHexToRGB(hex2));
		}
		void setHexToRGB(
			const char hex1, const char hex2,
			const char hex3, const char hex4,
			const char hex5, const char hex6
		) {
			r = getHexToRGB(hex1, hex2);
			g = getHexToRGB(hex3, hex4);
			b = getHexToRGB(hex5, hex6);
		}

	public:
		RGBA() = default;
		constexpr RGBA(const unsigned char r_, const unsigned char g_, const unsigned char b_, const unsigned char a_) :r(r_), g(g_), b(b_), a(a_) {}
		RGBA(const std::string& str_) {

			if (str_.size() == 7) {
				setHexToRGB(str_[1], str_[2], str_[3], str_[4], str_[5], str_[6]);
			}

		}

	};

}

#endif // !PAX_SAPIENTICA_COLOR_RGBA_HPP