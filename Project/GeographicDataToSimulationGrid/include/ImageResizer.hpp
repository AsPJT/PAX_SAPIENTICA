/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_IMAGE_RESIZER_HPP
#define PAX_SAPIENTICA_IMAGE_RESIZER_HPP

/*##########################################################################################

##########################################################################################*/

#include <cmath>
#include <filesystem>
#include <fstream>

#include <Helper.hpp>

namespace paxs{
    constexpr int img_size = 256;

    class ImageResizer{
    public:
        bool init(){
            std::string setting_path = paxs::Helper::getSettingPath("ImageResize");
            if(!std::filesystem::exists(setting_path)){
                std::cout << "setting not found" << std::endl;
                std::ofstream writing_file;
                writing_file.open(setting_path, std::ios::out);
                std::string writing_text = "name = \ninput_path = \noutput_path = \ndeviation_z = \nstart_x = \nstart_y = \nx_size = \ny_size = \nz = ";
                writing_file << writing_text << std::endl;
                writing_file.close();
                return false;
            }
			paxs::Settings settings = paxs::Helper::readSettings(setting_path);

            name = settings["name"];
            start_x = std::stoi(settings["start_x"]);
            start_y = std::stoi(settings["start_y"]);
            x_size = std::stoi(settings["x_size"]);
            y_size = std::stoi(settings["y_size"]);

            input_path = settings["input_path"];
            output_path = settings["output_path"];
            z = std::stoi(settings["z"]);
            deviation_z = std::stoi(settings["deviation_z"]);
            scale = std::pow(2.0, deviation_z);
            create_directory_recursive(output_path);
            return true;
        }
        void convertTiles(){
            if(deviation_z >= 0) return; // TODO: 拡大処理
            int n = 1 / scale;
            if(start_x % n != 0) start_x -= start_x % n;
            if(start_y % n != 0) start_y -= start_y % n;

            for(int y = start_y; y < start_y + y_size; y+=n){
                for(int x = start_x; x < start_x + x_size; x+=n){
                    std::vector<std::string> file_paths = getFilePaths(x, y, n);
                    if(file_paths.empty()) continue;
                    
                    std::vector<cv::Mat> input_tiles;
                    for(const auto& file_path : file_paths){
                        cv::Mat input_image = cv::imread(file_path, cv::IMREAD_UNCHANGED);
                        if(input_image.empty()) input_tiles.push_back(cv::Mat(img_size, img_size, CV_8UC4, cv::Scalar(0, 0, 0, 0)));
                        else input_tiles.push_back(input_image);
                    }

                    cv::Mat output_image = resizeAndStitchTiles(input_tiles);
                    std::string output_file_path = output_path + name + '_' + std::to_string(z + deviation_z) + '_' + std::to_string((int)(x * scale)) + '_' + std::to_string((int)(y * scale)) + ".png";
                    cv::imwrite(output_file_path, output_image);
                }
            }
        }
    private:
        std::string name;
        int start_x;
        int start_y;
        int x_size;
        int y_size;
        std::string input_path;
        std::string output_path;
        int z;
        int deviation_z;
        double scale;

        std::vector<std::string> getFilePaths(const int x, const int y, const int n) {
            std::vector<std::string> file_paths;
            for (int j = 0; j < n; j++) {
                for (int i = 0; i < n; i++) {
                    std::string file_path = input_path + name + '_' + std::to_string(z) + '_' + std::to_string(x + i) + '_' + std::to_string(y + j) + ".png";
                    if (!std::filesystem::exists(file_path)) {
                        file_paths.clear();
                        return file_paths;
                    }
                    file_paths.push_back(file_path);
                }
            }
            return file_paths;
        }
        void create_directory_recursive(const std::filesystem::path &path) {
            if (!std::filesystem::exists(path)) {
                create_directory_recursive(path.parent_path());
                std::filesystem::create_directory(path);
            }
        }
        cv::Mat resizeTile(const cv::Mat& input_image) {
            cv::Mat output_image;
            cv::Size output_size(input_image.cols * scale, input_image.rows * scale);
            cv::resize(input_image, output_image, output_size, 0, 0, cv::INTER_NEAREST);
            return output_image;
        }
        cv::Mat resizeAndStitchTiles(const std::vector<cv::Mat>& input_tiles) {
            std::vector<cv::Mat> resized_tiles;

            for (const auto& tile : input_tiles) {
                cv::Mat resized_tile = resizeTile(tile);
                resized_tiles.push_back(resized_tile);
            }

            return stitchTiles(resized_tiles);
        }
        cv::Mat stitchTiles(const std::vector<cv::Mat>& tiles) {
            const int n = 1 / scale;
            int tile_height = tiles[0].rows;
            int tile_width = tiles[0].cols;

            cv::Mat stitched_image(tile_height * n, tile_width * n, CV_8UC4);
            for (int row = 0; row < n; ++row) {
                for (int col = 0; col < n; ++col) {
                    cv::Rect roi(col * tile_width, row * tile_height, tile_width, tile_height);
                    tiles[row * n + col].copyTo(stitched_image(roi));
                }
            }
            return stitched_image;
        }
    };
}

#endif // !PAX_SAPIENTICA_IMAGE_RESIZER_HPP