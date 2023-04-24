/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/
#include <iostream>
#include <vector>
#include <PAX_SAPIENTICA/String/Replace.hpp>
#include <PAX_SAPIENTICA/FileRead/Split.hpp>

#include <fstream>

// CSV/TSV ファイルを読み込み、左の文字列を右の文字列へ変換する
void convertLeftToRight(
	const std::string& str, 
	const char delimiter_, 
	const std::string& str2, 
	const std::string& str3) {

	std::vector<std::string> to;
	std::vector<std::string> from;

	std::ifstream ifs(str);
	if (ifs.fail()) return;
	std::string line;
	while (std::getline(ifs, line)) {
		std::vector<std::string> strvec = paxs::split(line, delimiter_);
		if (strvec.size() == 0) continue;
		else if (strvec.size() == 1) {
			to.emplace_back(strvec[0]);
			from.emplace_back("");
		}
		else if (strvec.size() == 2) {
			to.emplace_back(strvec[0]);
			from.emplace_back(strvec[1]);
		}
	}

	std::ofstream ofs(str3);
	if (ofs.fail()) return;

	std::ifstream ifs2(str2);
	if (ifs2.fail()) return;
	std::string line2;
	while (std::getline(ifs2, line2)) {
		paxs::replaceList(line2, to, from);
		ofs << line2 << '\n';
	}
}

int main() {
	convertLeftToRight(
		"./../../../../Data/Character/KanaAndRomajiConversion.tsv",
		'\t',
		"input.txt", "output.txt");
	return 0;
}