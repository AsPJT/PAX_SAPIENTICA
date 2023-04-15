/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_IMAGE_SEGMENTER_HPP
#define PAX_SAPIENTICA_IMAGE_SEGMENTER_HPP

/*##########################################################################################

##########################################################################################*/

#include <filesystem>

#include <Helper.hpp>

namespace paxs{
    constexpr int img_size = 256;

    class ImageSegmenter{
     public:
        bool init(){
            std::string setting_path = paxs::Helper::getSettingFileName() + ".txt";
            if(!std::filesystem::exists(setting_path)){
                std::ofstream writing_file;
                writing_file.open(setting_path, std::ios::out);
                std::string writing_text = "name = \nstart_x = \nstart_y = \nx_size = \ny_size = \nz = \nextension = \ninput_path = \noutput_path = ";
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

            input_path = settings["input_path"] + name + '_' + settings["z"] + '_' + settings["start_x"] + '_' + settings["start_y"] + '_' + settings["x_size"] + '_' + settings["y_size"] + '.' + extension;
            output_path = settings["output_path"];
            path_prefix = output_path + name + '_' + settings["z"] + '_';
            return true;
        }
        void segment(){
            cv::Mat img = cv::imread(input_path);
            if(img.empty()){
                std::cout << "image not found" << std::endl;
                return;
            }
            std::filesystem::create_directory(output_path);
            for(int x=0;x<x_size;x++){
                for(int y=0;y<y_size;y++){
                    cv::Mat seg = img(cv::Rect(img_size * x, img_size * y, img_size, img_size));
                    cv::imwrite(getFilePath(start_x + x, start_y + y), seg);
                }
            }
        }
    private:
        int start_x;
        int start_y;
        int x_size;
        int y_size;
        std::string input_path;
        std::string output_path;
        std::string path_prefix;
        std::string extension;

        std::string getFilePath(int x, int y){
            return path_prefix + std::to_string(x) + '_' + std::to_string(y) + '.' + extension;
        }
    };
}

#endif // !PAX_SAPIENTICA_IMAGE_SEGMENTER_HPP