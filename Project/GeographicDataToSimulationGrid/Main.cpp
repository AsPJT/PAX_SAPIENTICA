/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <iostream>
#include <memory>

#include <opencv2/opencv.hpp>

const std::string simulation_folder_path = "../../../Data/Simulation/";

#ifdef LandAndSea

#include <PAX_SAPIENTICA/Simulation/SpaceGrid.hpp>
#include <ImageScaler.hpp>

int main(){
    std::unique_ptr<paxs::ImageScaler> image_scaler(new(std::nothrow) paxs::ImageScaler); 
    auto result = image_scaler->bilinear();

    image_scaler->cvWriteFile(result, simulation_folder_path + "sample.png");
    //paxs::ImageScaler::landAndSeaWriteFile(result, simulation_folder_path + "sample.tsv");

    std::cout << "Finish" << std::endl;

    // paxs::Helper::createFolder("../../../Data/Simulation/" + settings["source_folder_name"]);

    // std::string content = "1\t2\t3";
    // paxs::Helper::writeFile(simulation_folder_path + "sample.tsv", content);
}
#endif // !LandAndSea

#ifdef LandAndWaterConvert

#include <LandAndWaterConverter.hpp>

int main(){
    std::unique_ptr<paxs::LandAndWaterConverter> land_and_water_converter(new(std::nothrow) paxs::LandAndWaterConverter);
    if(land_and_water_converter->init()) land_and_water_converter->convert();
}

#endif // !LandAndWaterConvert

#ifdef ImageCombine

#include <ImageCombiner.hpp>

int main(){
    std::unique_ptr<paxs::ImageCombiner> image_combiner(new(std::nothrow) paxs::ImageCombiner);
    if(image_combiner->init()) image_combiner->combine();
}
#endif // !ImageCombine

#ifdef ImageSegment

#include <ImageSegmenter.hpp>

int main(){
    std::unique_ptr<paxs::ImageSegmenter> image_segmenter(new(std::nothrow) paxs::ImageSegmenter);
    if(image_segmenter->init()) image_segmenter->segment();
}

#endif // !ImageSegment

#ifdef ImageResize

#include <ImageResizer.hpp>

int main(){
    std::unique_ptr<paxs::ImageResizer> image_resizer(new(std::nothrow) paxs::ImageResizer);
    if(image_resizer->init()) image_resizer->convertTiles();
}

#endif // !ImageResize

#ifdef ColorCount

#include <ColorCounter.hpp>

int main(){
    std::unique_ptr<paxs::ColorCounter> color_counter(new(std::nothrow) paxs::ColorCounter);
    if(color_counter->init()) color_counter->countColor();
}

#endif // !ColorCount

#ifdef AreaCalculate

#include <AreaCalculater.hpp>

int main(){
    std::unique_ptr<paxs::AreaCalculater> area_calculater(new(std::nothrow) paxs::AreaCalculater);
    if(area_calculater->init()) area_calculater->calculateArea();
}

#endif // !AreaCalculate

#ifdef CreateSampleImages

#include <filesystem>

int main(){
    int num, size;
    std::string z;
    std::cout << "Number of images" << std::endl;
    std::cin >> num;
    std::cout << "Image size" << std::endl;
    std::cin >> size;
    std::cout << "z" << std::endl;
    std::cin >> z;
    const std::string path_prefix = "../../../Data/Sample/sample_" + z + "_";
    std::filesystem::create_directory("../../../Data/Sample");
    for(int x=0;x<num;x++){
        for(int y=0;y<num;y++){
            cv::Mat img(size, size, CV_8UC3);
			cv::imwrite(path_prefix + std::to_string(x) + '_' + std::to_string(y) + ".png", img);
        } 
    }
}

#endif // !CreateSampleImages