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
#include <unordered_map>

namespace paxs {

	class Language {
	private:
		std::vector<std::vector<s3d::String>> text{};
		std::unordered_map<s3d::String, std::size_t> text_map{};
		std::vector<s3d::String> empty{};

		void split(const std::string& input, const char delimiter) {
			std::istringstream stream(input);
			std::string field;
			std::vector<s3d::String> result;
			while (std::getline(stream, field, delimiter)) {
				result.emplace_back(s3d::Unicode::FromUTF8(field));
			}
			text_map.emplace(result.front(), text.size());
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
			empty.resize(text.front().size());
		}
		Language(const std::string& str_) {
			add(str_);
		}
		// 始点を探す
		std::size_t findStart(const std::string& str_) {
			if (text_map.find(s3d::Unicode::FromUTF8(str_)) != text_map.end()) {
				return text_map[s3d::Unicode::FromUTF8(str_)];
			}
			return 0;
		}
		std::vector<s3d::String>& getFindStart(const std::string& str_) {
			const std::size_t index = findStart(str_);
			if (index < text.size()) {
				return text[index];
			}
			return empty;
		}
		std::vector<std::vector<s3d::String>> getFindStartToVVS(const std::string& str_, const std::size_t start_index) {
			std::vector<std::vector<s3d::String>> tmp{};
			const std::vector<s3d::String>& lt = getFindStart(str_);
			for (std::size_t i = start_index; i < lt.size(); ++i) {
				tmp.emplace_back(std::vector<s3d::String>{ lt[i] });
			}
			return tmp;
		}
	};

}

#endif // !PAX_SAPIENTICA_SIV3D_LANGUAGE_HPP