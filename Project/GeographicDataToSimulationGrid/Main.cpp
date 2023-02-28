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

const std::string xyz_tile_folder_path = "../../../Data/Map/XYZTile/";
const std::string simulation_folder_path = "../../../Data/Simulation/";

int main(){
    paxs::Settings settings = paxs::Helper::getSettings();
    std::string prefix = xyz_tile_folder_path + settings["source_folder_name"] + "/" + settings["file_prefix"] + settings["z"] + "_";
    std::string path = prefix + settings["start_x"] + "_" + settings["start_y"] + "." + settings["file_type"];
    
    // cv::Mat img = cv::imread(path);
    // if(img.empty()){
    //     std::cout << settings["source_folder_name"] + " not found." << std::endl;
    //     return -1;
    // }

    // paxs::Helper::createFolder("../../../Data/Simulation/" + settings["source_folder_name"]);
    // cv::imwrite("../../../Data/Simulation/" + settings["source_folder_name"] + "/1.png", img);	

    std::string content = "1\t2\t3";
    paxs::Helper::writeFile(simulation_folder_path + "sample.tsv", content);
}