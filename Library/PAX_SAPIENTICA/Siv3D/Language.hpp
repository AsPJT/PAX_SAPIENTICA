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

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

#include <PAX_SAPIENTICA/Siv3D/Init.hpp>

namespace paxs {

	class Language {
	private:
		std::vector<std::vector<std::string>> text{};
		std::unordered_map<std::string, std::size_t> text_map{};
		std::vector<std::string> empty{};

		void split(const std::string& input, const char delimiter) {
			std::istringstream stream(input);
			std::string field;
			std::vector<std::string> result;
			while (std::getline(stream, field, delimiter)) {
				result.emplace_back(field);
			}
			text_map.emplace(result.front(), text.size());
			text.emplace_back(result);
		}

	public:
		std::vector<std::vector<std::string>>& get() {
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
		std::size_t findStart(const std::string& str_) const {
			if (text_map.find(str_) != text_map.end()) {
				return text_map.at(str_);
			}
			return 0;
		}
		std::vector<std::string>& getFindStart(const std::string& str_) {
			const std::size_t index = findStart(str_);
			if (index < text.size()) {
				return text[index];
			}
			return empty;
		}
		const std::vector<std::string>& cgetFindStart(const std::string& str_) const {
			const std::size_t index = findStart(str_);
			if (index < text.size()) {
				return text[index];
			}
			return empty;
		}
		std::vector<std::vector<std::string>> getFindStartToVVS(const std::string& str_, const std::size_t start_index) {
			std::vector<std::vector<std::string>> tmp{};
			const std::vector<std::string>& lt = getFindStart(str_);
			for (std::size_t i = start_index; i < lt.size(); ++i) {
				tmp.emplace_back(std::vector<std::string>{ lt[i] });
			}
			return tmp;
		}
	};

}

#endif // !PAX_SAPIENTICA_SIV3D_LANGUAGE_HPP