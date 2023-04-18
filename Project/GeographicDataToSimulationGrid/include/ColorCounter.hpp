/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_COLOR_COUNTER_HPP
#define PAX_SAPIENTICA_COLOR_COUNTER_HPP

/*##########################################################################################

##########################################################################################*/

#include <filesystem>

#include <Helper.hpp>

namespace paxs{
    class ColorCounter{
    public:
        bool init(){
            std::string setting_path = paxs::Helper::getSettingPath("ColorCount");
            if(!std::filesystem::exists(setting_path)){
                std::ofstream writing_file;
                writing_file.open(setting_path, std::ios::out);
                std::string writing_text = "tsv_path = \ninput_path = \noutput_path = ";
                writing_file.close();
                return false;
            }
			paxs::Settings settings = paxs::Helper::readSettings(setting_path);

            input_path = settings["input_path"];
            output_path = settings["output_path"];

            tsv = paxs::Helper::readTsv(settings["tsv_path"]);
            return true;
        }
        void countColor(){
            cv::Mat img = cv::imread(input_path);
            if(img.empty()){
                std::cout << "image not found" << std::endl;
                return;
            }
            for(int r=0;r<img.rows;r++){
                for(int c=0;c<img.cols;c++){
                    
                }
            }
        }
    private:
        std::string input_path;
        std::string output_path;
        std::vector<std::vector<std::string>> tsv;
        std::map<int, std::string> color_map;

        bool setColor(){
            bool is_rgb;
            if(tsv[0].size() == 5) is_rgb = true; // rgb
            else if(tsv[0].size() == 3) return false; // Hexかどうか
            
            for(const auto& row : tsv){
                if(is_rgb) color_map[std::stoi(row[0])] = row[2] + ',' + row[3] + ',' + row[4];
                // else
            }
            return true;
        }
    };
}

#endif // !PAX_SAPIENTICA_COLOR_COUNTER_HPP