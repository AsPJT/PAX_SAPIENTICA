/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <memory>

#include <opencv2/opencv.hpp>

#include <PAX_SAPIENTICA/Simulation/SpaceGrid.hpp>
#include "ImageScaler.hpp"

const std::string simulation_folder_path = "../../../Data/Simulation/";

int main(){
    std::unique_ptr<paxs::ImageScaler> image_scaler(new(std::nothrow) paxs::ImageScaler); 
    
    // 画素値の取得
	//cv::Vec3b pix = img.at<cv::Vec3b>(cv::Point(0, 0));
    //std:: cout << "img.cols:" << size << "\nz_square:" << z_square << "\npix:" << (int)pix[0] << "\nhorizontal_pixels:" << horizontal_pixels << std::endl;

    // paxs::Helper::createFolder("../../../Data/Simulation/" + settings["source_folder_name"]);

    // std::string content = "1\t2\t3";
    // paxs::Helper::writeFile(simulation_folder_path + "sample.tsv", content);
}