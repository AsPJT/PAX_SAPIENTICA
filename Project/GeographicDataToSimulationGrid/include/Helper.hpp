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

#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>

namespace paxs {
    
    const std::array<const char*, 2> setting_file_names = {"GeologicalFeature", "LandAndSea"};

    using Settings = std::map<std::string, std::string>;

    struct Rgb{
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    class Helper{
    public:
        // 設定を取得
        static Settings getSettings(std::string file_name){
            std::string setting_file_path =  paxs::Helper::getSettingPath(file_name);
            return paxs::Helper::readSettings(setting_file_path);
        }

        static Settings readSettings(const std::string& path){
            std::map<std::string, std::string> settings;
            std::ifstream ifs(path);
            if(ifs.fail()){
                std::cout<<"File not found."<<std::endl;
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

        std::string static getSettingPath(std::string file_name){
            return "../Settings/" + file_name + ".txt";;
        }

        // 文字列を分割する
        std::vector<std::string> static split(const std::string input,const char delimiter)
        {
            std::string str = input;
            str.insert(str.begin(), ' ');
            std::istringstream stream(str);
            std::string field;
            std::vector<std::string> result;
            while (std::getline(stream, field, delimiter)) {
                result.push_back(field);
            }
            return result;
        }

        // Hex→RGB
        paxs::Rgb static hexToRgb(const uint32_t rgb_int){
            paxs::Rgb rgb;
            rgb.red = (rgb_int >> 16) & 0xFF;
            rgb.green = (rgb_int >> 8) & 0xFF;
            rgb.blue = (rgb_int >> 0) & 0xFF;
            return rgb;
        }
        paxs::Rgb static hexToRgb(const std::string color){
            uint32_t rgb_int = (uint32_t)std::stoul(color, nullptr, 16);
            paxs::Rgb rgb;
            rgb.red = (rgb_int >> 16) & 0xFF;
            rgb.green = (rgb_int >> 8) & 0xFF;
            rgb.blue = (rgb_int >> 0) & 0xFF;
            return rgb;
        }

        // tsvを読み込む
        std::vector<std::vector<std::string>> static readTsv(const std::string file_path){
            std::vector<std::vector<std::string>> result;
            std::ifstream ifs(file_path);
            std::string line;
            while (std::getline(ifs, line)) {
                std::vector<std::string> row = split(line, '\t');
                result.push_back(row);
            }
            ifs.close();
            return result;
        }

        // csvを読み込む
        std::vector<std::vector<std::string>> static readCsv(const std::string file_path){
            std::vector<std::vector<std::string>> result;
            std::ifstream ifs(file_path);
            std::string line;
            while (std::getline(ifs, line)) {
                std::vector<std::string> row = split(line, ',');
                result.push_back(row);
            }
            ifs.close();
            return result;
        }

        // フォルダを作成
        static void createFolder(const std::string path){
            std::filesystem::create_directory(path);
        }

        // ファイルに書き込む
        void static writeFile(const std::string& filename, const std::string& content){
            std::ofstream ofs;
            ofs.open(filename, std::ios_base::app);
            ofs << content << std::endl;
        }

        // 2のn乗
        double static powOfTwo(const int n){
            return std::pow(2, n);
        }
    private:
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
    };
}

#endif // !PAX_SAPIENTICA_HELPER_HPP