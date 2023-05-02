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

#include <Helper.hpp>

namespace paxs{
    constexpr int img_size = 256;

    class LandAndWaterConverter{
    public:
        bool init(){
            std::string setting_path = paxs::Helper::getSettingPath("LandAndWaterConvert");
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
            return true;
        }
        void convert(){
            std::string txt_extension = ".txt";
            for (const auto & file : std::filesystem::directory_iterator(input_path)){
                cv::Mat img = cv::imread(file.path());

                std::filesystem::path path = file.path();
                path.replace_extension(txt_extension);
                std::ofstream writing_file;
                writing_file.open(output_path + (std::string)path.filename(), std::ios::out);

                for(int y=0;y<img_size;y++){
                    for(int x=0;x<img_size;x++){
                        if(img.at<cv::Vec3b>(y, x)[0] == 255 && img.at<cv::Vec3b>(y, x)[1] == 255 && img.at<cv::Vec3b>(y, x)[2] == 255){
                            writing_file << "0";
                        }else{
                            writing_file << "1";
                        }
                    }
                    writing_file << std::endl;
                }
                writing_file.close();
            }
        }
    private:
        std::string input_path;
        std::string output_path;
        void create_directory_recursive(const std::filesystem::path &path) {
            if (!std::filesystem::exists(path)) {
                create_directory_recursive(path.parent_path());
                std::filesystem::create_directory(path);
            }
        }
    };
}

#endif // !PAX_SAPIENTICA_LAND_AND_WATER_TO_TXT_HPP