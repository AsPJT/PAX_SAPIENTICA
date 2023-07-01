/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_STRING_EXTENSIONS_HPP
#define PAX_SAPIENTICA_STRING_EXTENSIONS_HPP

/*##########################################################################################

##########################################################################################*/

#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace paxs {
    /// @brief Extensions for std::string
    class StringExtensions {
    public:

		/// @brief Split string by delimiter
		static std::vector<std::string> split(const std::string& input, const char delimiter) {
			std::istringstream stream(input);
			std::string field;
			std::vector<std::string> result;
			while (std::getline(stream, field, delimiter)) {
				result.emplace_back(field);
			}
			return result;
		}

		/// @brief Replace string
		static void replace(std::string& str, const std::string& from, const std::string& to) {
			if (from.empty()) return;
			const std::size_t from_len = from.length();
			const std::size_t to_len = to.length();
			std::size_t pos = 0;
			while ((pos = str.find(from, pos)) != std::string::npos) {
				str.replace(pos, from_len, to);
				pos += to_len;
			}
		}

		/// @brief Replace string list
		void replaceList(std::string& str, const std::vector<std::string>& from, const std::vector<std::string>& to) {
			for (std::size_t i = 0; i < from.size() && i < to.size(); ++i) {
				replace(str, from[i], to[i]);
			}
		}

		/// @brief Convert string to double
        static std::variant<double, std::string> tryToConvertStringToDouble(const std::string& str) {
			try {
				return std::stod(str);
			} catch (const std::invalid_argument&/*ia*/) {
				// str is not convertible to double
				return str;
			} catch (const std::out_of_range&/*oor*/) {
				// str is out of range for a double
				return str;
			}
		}

		/// @brief Convert string to int
		static std::variant<int, std::string> tryToConvertStringToInt(const std::string& str) {
			try {
				return std::stoi(str);
			} catch (const std::invalid_argument&/*ia*/) {
				// str is not convertible to int
				return str;
			} catch (const std::out_of_range&/*oor*/) {
				// str is out of range for a int
				return str;
			}
		}
    };
}

#endif // !PAX_SAPIENTICA_STRING_EXTENSIONS_HPP