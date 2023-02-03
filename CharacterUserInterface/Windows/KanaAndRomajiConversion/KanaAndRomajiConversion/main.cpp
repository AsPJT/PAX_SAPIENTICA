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

int main() {

	std::vector<std::string> to;
	std::vector<std::string> from;

	std::string str = "./../../../../Data/Character/KanaAndRomajiConversion.tsv";
	std::ifstream ifs(str);
	if (ifs.fail()) return 0;
	std::string line;
	while (std::getline(ifs, line)) {
		std::vector<std::string> strvec = paxs::split(line, '\t');
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

	std::ofstream ofs("output.txt");
	if (ofs.fail()) return 0;

	std::string str2 = "input.txt";
	std::ifstream ifs2(str2);
	if (ifs2.fail()) return 0;
	std::string line2;
	while (std::getline(ifs2, line2)) {
		paxs::replaceList(line2, to, from);
		ofs << line2 << '\n';
	}
	return 0;
}