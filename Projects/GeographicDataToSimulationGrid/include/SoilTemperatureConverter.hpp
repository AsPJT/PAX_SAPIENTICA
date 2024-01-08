/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_LAND_AND_WATER_TO_TXT_HPP
#define PAX_SAPIENTICA_LAND_AND_WATER_TO_TXT_HPP

/*##########################################################################################

##########################################################################################*/

#include <filesystem>
#include <fstream>
#include <map>

#include <Helper.hpp>

namespace paxs{
    constexpr int img_size = 256;

    class SoilTemperatureConverter{
    public:
        bool init(){
            std::string setting_path = paxs::Helper::getSettingPath("SoilTemperatureConvert");
            if(!std::filesystem::exists(setting_path)){
                std::cout << "setting not found" << std::endl;
                std::ofstream writing_file;
                writing_file.open(setting_path, std::ios::out);
                std::string writing_text = "input_path = \noutput_path = ";
                writing_file << writing_text << std::endl;
                writing_file.close();
                return false;
            }
			paxs::Settings settings = paxs::Helper::readSettings(setting_path);

            input_path = settings["input_path"];
            output_path = settings["output_path"];
            create_directory_recursive(output_path);

            int idx = 1;
            while(settings.count(std::to_string(idx)) == 0){
                std::string color = settings[std::to_string(idx)];
                paxs::Rgb rgb = paxs::Helper::hexToRgb(color);
                color_map[idx] = (rgb);
                idx++;
            }

            return true;
        }
        void convert(){
            std::string txt_extension = ".txt";
            for (const auto & file : std::filesystem::directory_iterator(input_path)){
                cv::Mat img = cv::imread(file.path(), cv::IMREAD_UNCHANGED);
                if (img.empty()) {
                    std::cerr << "Error: Could not open or find the image." << std::endl;
                    exit(-1);
                }
                if (img.channels() != 4) {
                    std::cerr << "Error: The image must have 4 channels." << std::endl;
                    exit(-1);
                }

                std::filesystem::path path = file.path();
                path.replace_extension(txt_extension);
                std::ofstream writing_file;
                writing_file.open(output_path + (std::string)path.filename(), std::ios::out);

                for(int y=0;y<img_size;y++){
                    for(int x=0;x<img_size;x++){
                        cv::Vec4b vec_color = img.at<cv::Vec4b>(y, x);
                        // 不透明度が255以外の時は0
                        if(vec_color[3] != 255){
                            writing_file << "0";
                            if(x != img_size - 1) writing_file << "\t";
                            continue;
                        }

                        int closestKey = findClosestColorKey(paxs::Rgb(vec_color[2], vec_color[1], vec_color[0]));
                        writing_file << closestKey;
                        if(x != img_size - 1) writing_file << "\t";
                    }
                    writing_file << std::endl;
                }
                writing_file.close();
            }
        }
    private:
        std::string input_path;
        std::string output_path;
        std::map<int, paxs::Rgb> color_map;

        void create_directory_recursive(const std::filesystem::path &path) {
            if (!std::filesystem::exists(path)) {
                create_directory_recursive(path.parent_path());
                std::filesystem::create_directory(path);
            }
        }
        int findClosestColorKey(const Rgb& color) {
            int closestKey = 0;
            double minDistance = std::numeric_limits<double>::max();

            for (const auto& kv : color_map) {
                double distance = color.colorDistance(kv.second);
                if (distance < minDistance) {
                    minDistance = distance;
                    closestKey = kv.first;
                }
            }

            return closestKey;
        }
    };
}

#endif // !PAX_SAPIENTICA_LAND_AND_WATER_TO_TXT_HPP