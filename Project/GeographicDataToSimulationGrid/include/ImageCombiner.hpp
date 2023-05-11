/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_IMAGE_COMBINER_HPP
#define PAX_SAPIENTICA_IMAGE_COMBINER_HPP

/*##########################################################################################

##########################################################################################*/

#include <filesystem>
#include <fstream>
#include <iostream>

#include <Helper.hpp>

namespace paxs{
    constexpr int img_size = 256;

    class ImageCombiner{
    public:
        bool init(){
            std::string setting_path = paxs::Helper::getSettingPath("ImageCombine");
            if(!std::filesystem::exists(setting_path)){
                std::ofstream writing_file;
                writing_file.open(setting_path, std::ios::out);
                std::string writing_text = "name = \nstart_x = \nstart_y = \nx_size = \ny_size = \nz = \nextension = \ninput_path = \noutput_path = \nalpha = ";
                writing_file << writing_text << std::endl;
                writing_file.close();
                return false;
            }
			paxs::Settings settings = paxs::Helper::readSettings(setting_path);

            start_x = std::stoi(settings["start_x"]);
            start_y = std::stoi(settings["start_y"]);
            x_size = std::stoi(settings["x_size"]);
            y_size = std::stoi(settings["y_size"]);

            std::string name = settings["name"];
            extension = settings["extension"];

            path_prefix = settings["input_path"] + name + '_' + settings["z"] + '_';
            output_path = settings["output_path"] + name + '_' + settings["z"] + '_' + settings["start_x"] + '_' + settings["start_y"] + '_' + settings["x_size"] + '_' + settings["y_size"] + '.' + extension;
            std::filesystem::create_directory(settings["output_path"]);

            is_alpha = settings["alpha"] == "true";

            zero_img = cv::Mat::zeros(img_size, img_size, is_alpha ?  CV_8UC4 : CV_8UC3);
            return true;
        }
        void combine(){
            cv::Mat combined_img = vCombine(start_x);
            for(int x=1;x<x_size;x++){
                cv::Mat tmp_img = vCombine(start_x + x);
                cv::hconcat(combined_img, tmp_img, combined_img);
            }
			cv::imwrite(output_path, combined_img);
        }
    private:
        int start_x;
        int start_y;
        int x_size;
        int y_size;
        bool is_alpha;
        std::string path_prefix;
        std::string output_path;
        std::string extension;
        cv::Mat zero_img;

        cv::Mat vCombine(const int x){
            cv::Mat combined_img = zero_img.clone();
            std::string path = getFilePath(x, start_y);
            if(std::filesystem::exists(path)){
		        combined_img = cv::imread(path, cv::IMREAD_UNCHANGED);
            }
            for(int y=1;y<y_size;y++){
                std::string path = getFilePath(x, start_y + y);
                if(std::filesystem::exists(path)){
			        cv::Mat img = cv::imread(path, cv::IMREAD_UNCHANGED);
                    if(img.size() == zero_img.size() && img.type() == zero_img.type()){
                        cv::vconcat(combined_img, img, combined_img);
                    } else {
                        std::cerr << "Error: Input image size or type does not match at (" << x << ", " << y << "). Skipping this image." << std::endl;
                        std::cerr << "Expected size: " << zero_img.size() << ", type: " << zero_img.type() << std::endl;
                        std::cerr << "Actual size: " << img.size() << ", type: " << img.type() << std::endl;
                    }
                    continue;
                }
                cv::vconcat(combined_img, zero_img, combined_img);
            }
            return combined_img;
        }

        std::string getFilePath(int x, int y){
            return path_prefix + std::to_string(x) + '_' + std::to_string(y) + '.' + extension;
        }
    };
}

#endif // !PAX_SAPIENTICA_IMAGE_COMBINER_HPP