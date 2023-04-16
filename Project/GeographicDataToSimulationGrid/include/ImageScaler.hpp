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

#include <array>
#include <cmath>
#include <iostream>

#include <PAX_SAPIENTICA/Type/Coordinate.hpp>
#include "Helper.hpp"

namespace paxs {
	const std::string xyz_tile_folder_path = "../../../Data/Map/XYZTile/";
	const cv::Vec3b sea_color(115, 87, 0);
	const cv::Vec3b land_color(254, 254, 254);

    class ImageScaler{
    public:
		ImageScaler(){
			settings = paxs::Helper::getSettings("file_name");
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
					result[y][x] = getColor<cv::Vec3b>(x_pixel / img_size, y_pixel / img_size, cv::Point(x_pixel % img_size, y_pixel % img_size));
				}
			}
			return result;
		}

		std::vector<std::vector<cv::Vec3b>> bilinear(){
			std::vector<std::vector<cv::Vec3b>> result(division_num, std::vector<cv::Vec3b>(division_num));
			double division_pixel = (double)pixel_sum / division_num;
			double pixel_deviation = division_pixel * 0.5;
			for(int y=0;y<division_num;y++){
				for(int x=0;x<division_num;x++){
					double x_pixel_d = division_pixel * x + pixel_deviation;
					double y_pixel_d = division_pixel * y + pixel_deviation;
					int x_pixel = std::floor(x_pixel_d);
					int y_pixel = std::floor(y_pixel_d);
					auto colors = getAroundColor<cv::Vec3b>(x_pixel / img_size, y_pixel / img_size, cv::Point(x_pixel % img_size, y_pixel % img_size));
					double p = x_pixel_d  -x_pixel, q = 1 - p, s = y_pixel_d - y_pixel, t = 1 - s;
					result[y][x] = s * (p * colors[0] + q * colors[1]) + t * (p * colors[2] + q * colors[3]);
				}
			}
			return result;
		} 

		void cvWriteFile(const std::vector<std::vector<cv::Vec3b>>& data, const std::string& path){
			cv::Mat image(data[0].size(), data.size(), CV_8UC3);
			for(int y=0;y<data.size();y++){
				for(int x=0;x<data[0].size();x++){
			 		image.at<cv::Vec3b>(y,x) = data[y][x];
				}
			}
			cv::imwrite(path, image);
		}

        // ファイルに書き込む
        void static landAndSeaWriteFile(const std::vector<std::vector<cv::Vec3b>>& data, const std::string& path){
			std::string content;
			for(int y=0;y<data.size();y++){
				for(int x=0;x<data[0].size();x++){
			 		content += data[y][x] == sea_color ? '1' : '0';
				}
				content += "\n";
			}

            std::ofstream ofs;
            ofs.open(path, std::ios_base::trunc);
            ofs << content << std::endl;
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
		template<typename T>
		T getColor(const int x, const int y, cv::Point point){
			std::string path = getFilePath(start.x + x, start.y + y);
			cv::Mat img = cv::imread(path);
			if(img.empty()){
				std::cout << "File not found." << std::endl;
				return T();
    		}
			return img.at<T>(point);
		}
		
		template<typename T>
		std::array<T, 4> getAroundColor(const int x, const int y, cv::Point point){
			std::string path = getFilePath(start.x + x, start.y + y);
			cv::Mat img1 = cv::imread(path);
			if(img1.empty()){
				std::cout << "File not found." << std::endl;
				return std::array<T, 4>();
    		}
			std::array<T, 4> color;
			color[0] = img1.at<T>(point);
			if(point.x == img_size - 1){
				if(point.y == img_size - 1){
					color[1] = getColor<T>(x + 1, y, cv::Point(0, point.y));
					color[2] = getColor<T>(x, y + 1, cv::Point(point.x, 0));
					color[3] = getColor<T>(x + 1, y + 1, cv::Point(0, 0));					
				}else{
					std::string path2 = getFilePath(start.x + x + 1, start.y + y);
					cv::Mat img2 = cv::imread(path);
					if(img2.empty()){
						std::cout << "File not found." << std::endl;
						return std::array<T, 4>();
					}
					color[1] = img2.at<T>(cv::Point(0, point.y));
					color[2] = img1.at<T>(cv::Point(point.x, point.y + 1));
					color[3] = img2.at<T>(cv::Point(0, point.y + 1));
				}
			}else{
				if(point.y == img_size - 1){
					std::string path2 = getFilePath(start.x + x, start.y + y + 1);
					cv::Mat img2 = cv::imread(path);
					if(img2.empty()){
						std::cout << "File not found." << std::endl;
						return std::array<T, 4>();
					}
					color[1] = img1.at<T>(cv::Point(point.x + 1, point.y));
					color[2] = img2.at<T>(cv::Point(point.x, 0));
					color[3] = img2.at<T>(cv::Point(point.x + 1, 0));
				}else{
					color[1] = img1.at<T>(cv::Point(point.x + 1, point.y));
					color[2] = img1.at<T>(cv::Point(point.x, point.y + 1));
					color[3] = img1.at<T>(cv::Point(point.x + 1, point.y + 1));
				}
			}
			return color;
		}
    };
}

#endif // !PAX_SAPIENTICA_IMAGE_SCALER_HPP