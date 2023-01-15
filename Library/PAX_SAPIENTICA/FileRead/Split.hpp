/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TYPE_FILE_READ_SPLIT_HPP
#define PAX_SAPIENTICA_TYPE_FILE_READ_SPLIT_HPP

/*##########################################################################################

##########################################################################################*/

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

namespace paxs {

	std::vector<std::string> split(const std::string& input_, const char delimiter_) {
		std::istringstream stream(input_);
		std::string field;
		std::vector<std::string> result;
		while (std::getline(stream, field, delimiter_)) {
			result.emplace_back(field);
		}
		return result;
	}

}

#endif // !PAX_SAPIENTICA_TYPE_FILE_READ_SPLIT_HPP