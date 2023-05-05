/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_LANGUAGE_HPP
#define PAX_SAPIENTICA_SIV3D_LANGUAGE_HPP

/*##########################################################################################

##########################################################################################*/

#include <Siv3D.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

namespace paxs {

	class Language {
	private:
		std::vector<std::vector<s3d::String>> text{};

		void split(const std::string& input, const char delimiter) {
			std::istringstream stream(input);
			std::string field;
			std::vector<s3d::String> result;
			while (std::getline(stream, field, delimiter)) {
				result.emplace_back(s3d::Unicode::FromUTF8(field));
			}
			text.emplace_back(result);
		}

	public:
		std::vector<std::vector<s3d::String>>& get() {
			return text;
		}
		void add(const std::string& str_) {
			std::ifstream ifs(str_);
			std::string line;
			while (std::getline(ifs, line)) {
				split(line, '\t');
			}
		}
		Language(const std::string& str_) {
			add(str_);
		}
		// 始点を探す
		std::size_t findStart(const std::string& str_) {
			for (std::size_t i = 0; i < text.size(); ++i) {
				if (text[i].front() == s3d::Unicode::FromUTF8(str_)) return i;
			}
			return 0;
		}
	};

}

#endif // !PAX_SAPIENTICA_SIV3D_LANGUAGE_HPP