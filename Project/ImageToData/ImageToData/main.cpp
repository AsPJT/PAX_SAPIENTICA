/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

/*##########################################################################################
	土壌温度の画像からデータを作成
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

bool output(const std::string& in_, const std::string& out_, int bpp = 4) {
	{
		std::ifstream ifs(in_);
		if (!ifs) return false;
	}
	constexpr std::size_t width{ 256 }, height{ 256 }; //幅と高さ
	int width2{ width }, height2{ height }; //幅と高さ
	std::unique_ptr<paxs::RGBA[][width]> rgba(new(std::nothrow) paxs::RGBA[height][width]);
	//std::unique_ptr<int[][width]> rgba_int(new(std::nothrow) int[height][width]);
	if (!rgba) return false;

	std::ofstream ofs(out_);
	paxs::SoilTemp st;


	//unsigned char* pixels = &(rgba[0][0].r);
	unsigned char* pixels = nullptr;

	// ファイルを読み込み、画像データを取り出す
	//   最後の引数でピクセルあたりのバイト数を強制できる
	pixels = stbi_load(in_.c_str(), &width2, &height2, &bpp, 0);
	if (pixels == nullptr) return false;
	for (std::size_t row{}, count = 0; row < height; ++row) {
		for (std::size_t col{}; col < width; ++col, count += std::size_t(bpp)) {

			{
				rgba[row][col].r = pixels[count]; //赤
				rgba[row][col].g = pixels[count + 1];
				rgba[row][col].b = pixels[count + 2];
				if (bpp == 4) {
					rgba[row][col].a = pixels[count + 3]; //不透過
				}
				else rgba[row][col].a = 255;
			}

			ofs << std::to_string(st.getIndex(rgba[row][col]));
			if (col + 1 < width) ofs << '\t';
			//rgba_int[row][col]
		}
		if (row + 1 < height) ofs << '\n';
	}
	// メモリ上の画像データを破棄
	stbi_image_free(pixels);
	//stbi_write_png("test06.png", static_cast<int>(width), static_cast<int>(height), static_cast<int>(sizeof(paxs::RGBA)), rgba.get(), 0);	
	return true;
}

int main() {

	const int z = 10;
	const int max_xy = std::pow(2, z);
	for (int y = 0; y < max_xy; ++y) {
		for (int x = 0; x < max_xy; ++x) {
			output(
				"../../../Data/Map/XYZTile/Temperature/Image/SoilTemperature/2023/"
				+ std::to_string(z)
				+ "/soil-tem_"
				+ std::to_string(z)
				+ "_" + std::to_string(x) + "_" + std::to_string(y) + ".png"
				,
				"../../../Data/Map/XYZTile/Temperature/Data/SoilTemperature/2023/"
				+ std::to_string(z)
				+ "/soil-tem_"
				+ std::to_string(z)
				+ "_" + std::to_string(x) + "_" + std::to_string(y) + ".tsv"
			);
		}
	}

}