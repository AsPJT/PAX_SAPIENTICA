/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TYPE_FILE_READ_CONVERT_HPP
#define PAX_SAPIENTICA_TYPE_FILE_READ_CONVERT_HPP

/*##########################################################################################

##########################################################################################*/

namespace paxs {
    std::variant<double, std::string> convertString(const std::string& str) {
        try {
            return std::stod(str);
        } catch (const std::invalid_argument& ia) {
            // str is not convertible to double
            return str;
        } catch (const std::out_of_range& oor) {
            // str is out of range for a double
            return str;
        }
    }
}

#endif // !PAX_SAPIENTICA_TYPE_FILE_READ_CONVERT_HPP