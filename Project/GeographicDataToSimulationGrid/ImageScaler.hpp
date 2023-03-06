/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_IMAGE_SCALER_HPP
#define PAX_SAPIENTICA_IMAGE_SCALER_HPP

/*##########################################################################################

##########################################################################################*/

#include <iostream>
#include <string>

#include <PAX_SAPIENTICA/Type/Coordinate.hpp>
#include "Helper.hpp"

namespace paxs {
	const std::string xyz_tile_folder_path = "../../../Data/Map/XYZTile/";

    class ImageScaler{
    public:
		ImageScaler(){
			settings = paxs::Helper::getSettings();
			path_prefix = xyz_tile_folder_path + settings["source_folder_name"] + "/" + settings["file_prefix"] + settings["z"] + "_";
			
			start.x = std::stoi(settings["start_x"]);
			start.y = std::stoi(settings["start_y"]);
			end.x = std::stoi(settings["end_x"]);
			end.y = std::stoi(settings["end_y"]);

			cv::Mat img = cv::imread(getFilePath(start.x, start.y));
			if(img.empty()){
				std::cout << "File not found." << std::endl;
				return;
    		}
			img_size = img.cols;
			z_square = paxs::Helper::powOfTwo(std::stoi(settings["z"]));
			pixel_sum = img_size * z_square;

			std::cout << "Init ImageScaler is success" << std::endl;
		}
		static void nearestNeighbor(){

		}
    private:
        paxs::Settings settings; // 設定
		std::string path_prefix; // 元データのPathの接頭辞
		type::Coordinate<int> start;
		type::Coordinate<int> end;
		int img_size; // 画像の1方向のピクセル数
		int z_square; // 画像の1方向の枚数
		int pixel_sum; // 全画像の1方向のピクセル数

		std::string getFilePath(const int x, const int y){
			return path_prefix + std::to_string(x) + "_" + std::to_string(y) + "." + settings["file_type"];
		}
    };
}

#endif // !PAX_SAPIENTICA_IMAGE_SCALER_HPP