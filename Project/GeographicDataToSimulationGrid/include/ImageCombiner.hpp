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

#include <iostream>
#include <memory>

#include <Helper.hpp>

namespace paxs{
    constexpr int img_size = 256;
    const std::string simulation_folder_path = "../../../Data/Simulation/";

    class ImageCombiner{
    public:
        ImageCombiner(){
			paxs::Settings settings = paxs::Helper::getSettings();
            img_num = std::stoi(settings["imgnum"]);

            std::string name = settings["name"];
            extension = settings["extension"];

            path_prefix = simulation_folder_path + name + '/' + name + '-' + settings["z"] + '-';
            output_path = simulation_folder_path + name + '.' + extension;
        }
        void combine(){
            cv::Mat combined_img = vCombine(0);
            for(int x=1;x<img_num;x++){
                cv::Mat tmp_img = vCombine(x);
                cv::hconcat(combined_img, tmp_img, combined_img);
            }
			cv::imwrite(output_path, combined_img);
        }
    private:
        std::string path_prefix;
        std::string output_path;
        int img_num;
        std::string extension;

        cv::Mat vCombine(const int x){
            cv::Mat combined_img = cv::imread(getFilePath(x, 0));
            for(int y=1;y<img_num;y++){
			    cv::Mat img = cv::imread(getFilePath(x, y));
                cv::vconcat(combined_img, img, combined_img);
            }
            return combined_img;
        }

        std::string getFilePath(int x, int y){
            return path_prefix + std::to_string(x) + '-' + std::to_string(y) + '.' + extension;
        }
    };
}

#endif // !PAX_SAPIENTICA_IMAGE_COMBINER_HPP