/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

/*##########################################################################################
	XYZ Tiles を合体して１枚の画像にする
##########################################################################################*/

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include <iostream>
#include <cstddef>
#include <memory>
#include <new>
#include <vector>
#include <string>
#include <fstream>

#include <PAX_SAPIENTICA/Color/RGBA.hpp>
#include <PAX_SAPIENTICA/Color/SoilTemperature.hpp>

bool output() {

	int bpp = 4;

	constexpr std::size_t z = 6;
	constexpr std::size_t max_xy = 64;//std::pow(2, z);

	constexpr std::size_t width_all{ 256 * max_xy };
	constexpr std::size_t height_all{ 256 * max_xy }; //幅と高さ
	int width2{ 256 }, height2{ 256 }; //幅と高さ
	std::unique_ptr<paxs::RGBA[][width_all]> rgba(new(std::nothrow) paxs::RGBA[height_all][width_all]);
	//std::unique_ptr<int[][width]> rgba_int(new(std::nothrow) int[height][width]);
	if (!rgba) return false;

	for (std::size_t y = 0; y < height_all; ++y) {
		for (std::size_t x = 0; x < width_all; ++x) {
			rgba[y][x].r = 0;
			rgba[y][x].g = 0;
			rgba[y][x].b = 0;
			rgba[y][x].a = 0;
		}
	}
	for (std::size_t y = 0; y < max_xy; ++y) {
		for (std::size_t x = 0; x < max_xy; ++x) {
			std::string in_ = "../../../Data/Map/XYZTile/Slope/Image/Slope/2010/"
				+ std::to_string(z)
				+ "/slope_"
				+ std::to_string(z)
				+ "_" + std::to_string(x) + "_" + std::to_string(y) + ".png";

			{
				std::ifstream ifs(in_);
				if (!ifs) {
					continue;
				}
			}

			//unsigned char* pixels = &(rgba[0][0].r);
			unsigned char* pixels = nullptr;

			// ファイルを読み込み、画像データを取り出す
			//   最後の引数でピクセルあたりのバイト数を強制できる
			pixels = stbi_load(in_.c_str(), &width2, &height2, &bpp, 0);
			if (pixels == nullptr) {
				continue;
			}
			for (std::size_t row{}, count = 0; row < (std::size_t)height2; ++row) {
				for (std::size_t col{}; col < (std::size_t)width2; ++col, count += std::size_t(bpp)) {
					rgba[row + y * 256][col + x * 256].r = pixels[count]; //赤
					rgba[row + y * 256][col + x * 256].g = pixels[count + 1];
					rgba[row + y * 256][col + x * 256].b = pixels[count + 2];
					//std::cout << pixels[count] << " ";
					if (bpp == 4) {
						rgba[row + y * 256][col + x * 256].a = pixels[count + 3]; //不透過
					}
					else rgba[row + y * 256][col + x * 256].a = 255;
				}
			}
			// メモリ上の画像データを破棄
			stbi_image_free(pixels);

		}
	}


	stbi_write_png("z6.png", static_cast<int>(width_all), static_cast<int>(height_all), static_cast<int>(sizeof(paxs::RGBA)), rgba.get(), 0);
	std::cout << "end";
	return true;
}

int main() {

	output();

}