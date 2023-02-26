/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_HELPER_HPP
#define PAX_SAPIENTICA_HELPER_HPP

/*##########################################################################################

##########################################################################################*/

#include <fstream>
#include <iostream>
#include <map>

namespace paxs {
    struct Helper
    {
        static std::string trim(std::string string, const char* trim_character_list = " \t\v\r\n"){
            std::string::size_type left = string.find_first_not_of(trim_character_list);
            if (left != std::string::npos) {
                string = string.substr(left);
            }

            std::string::size_type right = string.find_last_not_of(trim_character_list);
            if (right != std::string::npos) {
                string = string.substr(0, right + 1);
            }
            
            return string;
        }

        static std::map<std::string, std::string> readSettings(const std::string& path){
            std::map<std::string, std::string> settings;
            std::ifstream ifs(path);
            if(ifs.fail()){
                std::cout<<"File not found.\n";
                return settings;
            }

            std::string line;
            while(getline(ifs, line)){
                std::string::size_type separator_index = line.find('=');
                if (separator_index == std::string::npos) {
                    continue;
                }

                std::string key = line.substr(0, separator_index);
                std::string value = line.substr(separator_index);

                if (value.size() > 0) {
                    value[0] = ' ';
                }

                settings.insert_or_assign(trim(key), trim(value));
            }
            ifs.close();
            return settings;
        }
    };
}

#endif // !PAX_SAPIENTICA_HELPER_HPP