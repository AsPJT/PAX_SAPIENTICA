/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TYPE_STRING_REPLACE_HPP
#define PAX_SAPIENTICA_TYPE_STRING_REPLACE_HPP

/*##########################################################################################

##########################################################################################*/

#include <string>
#include <vector>

namespace paxs {

	void replace(std::string& str, const std::string& from, const std::string& to) {
		if (from.empty()) return;
		const std::size_t from_len = from.length();
		const std::size_t to_len = to.length();
		std::size_t pos = str.find(from);
		while ((pos = str.find(from, pos)) != std::string::npos) {
			str.replace(pos, from_len, to);
			pos += to_len;
		}
	}
	void replaceList(std::string& str, const std::vector<std::string>& from, const std::vector<std::string>& to) {
		for (std::size_t i = 0; i < from.size() && i < to.size(); ++i) {
			paxs::replace(str, from[i], to[i]);
		}
	}

}

#endif // !PAX_SAPIENTICA_TYPE_STRING_REPLACE_HPP