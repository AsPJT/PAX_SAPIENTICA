/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <opencv2/opencv.hpp>

#include <PAX_SAPIENTICA/Simulation/SpaceGrid.hpp>
#include "Helper.hpp"

const std::string xyz_tile_foleder_path = "../../../Data/Map/XYZTile/";

int main(){
    auto settings = paxs::Helper::readSettings("Settings.txt");
    std::string path = xyz_tile_foleder_path + settings["source_folder_name"] + "/" + settings["file_prefix"] + settings["z"] + "_" + settings["start_x"] + "_" + settings["start_y"] + "." + settings["type"];
    std::cout<<path<<std::endl;
    
    cv::Mat img = cv::imread(path);
    if(img.empty()){
        std::cout << settings["source_folder_name"] + " not found." << std::endl;
        return -1;
    }

    cv::namedWindow("hogehoge", cv::WINDOW_AUTOSIZE); //Windowのタイトルとサイズ設定
    cv::imshow("hogehoge", img);
    cv::waitKey(0);

    cv::destroyAllWindows();
    std::cout << "Finish" << std::endl;
}