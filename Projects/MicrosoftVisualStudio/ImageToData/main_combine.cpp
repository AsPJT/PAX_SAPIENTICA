/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

/*##########################################################################################
	令制国の画像からデータを作成
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
#include <PAX_SAPIENTICA/Color/Ryoseikoku.hpp>

//853, 973,
//326, 448,

constexpr int min_x = 853;
constexpr int min_y = 326;
constexpr int max_x = 974;
constexpr int max_y = 385;

constexpr int width = 256 * (max_x - min_x);
constexpr int height = 256 * (max_y - min_y);

constexpr std::size_t width0{ 256 }, height0{ 256 }; //幅と高さ

bool ou(const std::string in_, paxs::RGBA all_rgba[][width], int row0, int col0) {
	{
		std::ifstream ifs(in_);
		if (!ifs) return false;
	}
    int bpp = 4;
	int width2{ width0 }, height2{ height0 }; //幅と高さ

	unsigned char* pixels = nullptr;
	// ファイルを読み込み、画像データを取り出す
	//   最後の引数でピクセルあたりのバイト数を強制できる
	pixels = stbi_load(in_.c_str(), &width2, &height2, &bpp, 0);
    if (pixels == nullptr) {
        std::cout << "null: " << in_ << std::endl;
        return false;
    }
    for (std::size_t row{}, count = 0; row < height0; ++row) {
        for (std::size_t col{}; col < width0; ++col, count += std::size_t(bpp)) {
            all_rgba[row0 * 256 + row][col0 * 256 + col].r = pixels[count];
            all_rgba[row0 * 256 + row][col0 * 256 + col].g = pixels[count + 1];
            all_rgba[row0 * 256 + row][col0 * 256 + col].b = pixels[count + 2];

            if (bpp == 4) {
                all_rgba[row0 * 256 + row][col0 * 256 + col].a = pixels[count + 3];
            }
            else {
                all_rgba[row0 * 256 + row][col0 * 256 + col].a = 255;
            }
        }
    }
	// メモリ上の画像データを破棄
	stbi_image_free(pixels);
    return true;
}

int main() {

    const std::string output = "../../../Data/XYZTiles/zxy_10.png";
    int index = 0;

    std::unique_ptr<paxs::RGBA[][width]> rgba(new(std::nothrow) paxs::RGBA[height][width]);

    for (int y = min_y, row0 = 0; y < max_y; ++y, ++row0) {
        for (int x = min_x, col0 = 0; x < max_x; ++x, ++col0) {
            ou(
                "../../../Data/XYZTiles/east_asia/10/zxy_10_" + std::to_string(x) + "_" + std::to_string(y) + ".png",
                rgba.get(), row0, col0);
        }
    }
    stbi_write_png(output.c_str(), static_cast<int>(width), static_cast<int>(height), static_cast<int>(sizeof(paxs::RGBA)), rgba.get(), 0);
}
