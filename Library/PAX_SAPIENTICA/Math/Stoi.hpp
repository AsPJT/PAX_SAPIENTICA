/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TYPE_MATH_STOI_HPP
#define PAX_SAPIENTICA_TYPE_MATH_STOI_HPP

/*##########################################################################################
	Converts a string to a number.
	文字列を数値へ変換する。
##########################################################################################*/

namespace paxs {

	// 0 以上の整数へ変換
	int getUintFromString(const std::string& str_) {
		int num = 0;
		for (int i = 0; i < str_.size(); ++i) {
			switch (str_[i]) {
			case '0':num *= 10; break;
			case '1':num *= 10; num += 1; break;
			case '2':num *= 10; num += 2; break;
			case '3':num *= 10; num += 3; break;
			case '4':num *= 10; num += 4; break;
			case '5':num *= 10; num += 5; break;
			case '6':num *= 10; num += 6; break;
			case '7':num *= 10; num += 7; break;
			case '8':num *= 10; num += 8; break;
			case '9':num *= 10; num += 9; break;
			}
		}
		return num;
	}

}

#endif // !PAX_SAPIENTICA_TYPE_MATH_STOI_HPP