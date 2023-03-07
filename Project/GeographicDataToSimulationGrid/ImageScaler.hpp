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

#include <cmath>
#include <iostream>

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
			division_num = std::stoi(settings["division_num"]);

			std::cout << "Init ImageScaler is success" << std::endl;
		}

		std::vector<std::vector<cv::Vec3b>> nearestNeighbor(){
			std::vector<std::vector<cv::Vec3b>> result(division_num, std::vector<cv::Vec3b>(division_num));
			double division_pixel = (double)pixel_sum / division_num;
			double pixel_deviation = division_pixel * 0.5;
			for(int y=0;y<division_num;y++){
				for(int x=0;x<division_num;x++){
					int x_pixel = std::round(division_pixel * x + pixel_deviation);
					int y_pixel = std::round(division_pixel * y + pixel_deviation);
					result[y][x] = getPix(x_pixel / img_size, y_pixel / img_size, cv::Point(x_pixel % img_size, y_pixel % img_size));
				}
			}
			return result;
		}
    private:
        paxs::Settings settings; // 設定
		std::string path_prefix; // 元データのPathの接頭辞
		type::Coordinate<int> start;
		type::Coordinate<int> end;
		int img_size; // 画像の1方向のピクセル数
		int z_square; // 画像の1方向の枚数
		int pixel_sum; // 全画像の1方向のピクセル数
		int division_num; // 1方向の分割数

		std::string getFilePath(const int x, const int y){
			return path_prefix + std::to_string(x) + "_" + std::to_string(y) + "." + settings["file_type"];
		}

    	// 画素値の取得
		cv::Vec3b getPix(const int x, const int y, cv::Point point){
			cv::Mat img = cv::imread(getFilePath(start.x, start.y));
			if(img.empty()){
				std::cout << "File not found." << std::endl;
				return cv::Vec3b();
    		}
			return img.at<cv::Vec3b>(point);
		}
    };
}

#endif // !PAX_SAPIENTICA_IMAGE_SCALER_HPP