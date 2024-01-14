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
constexpr int min_y = 385;
constexpr int max_x = min_x + 64;
constexpr int max_y = min_y + 64;

constexpr int width = 256 * (max_x - min_x);
constexpr int height = 256 * (max_y - min_y);

constexpr std::size_t width0{ 256 }, height0{ 256 }; //幅と高さ

bool ou(const std::string in_, unsigned char* pixels, int row0, int col0, int bpp) {

    std::unique_ptr<paxs::RGBA[][width0]> rgba(new(std::nothrow) paxs::RGBA[height0][width0]);

    bool is_white_only = true; // 白のみ
    for (std::size_t row{}, count = 0; row < height0; ++row) {
        for (std::size_t col{}; col < width0; ++col, count += std::size_t(bpp)) {
            rgba[row][col].r = pixels[((row0 * 256 + row) * width + col0 * 256 + col) * bpp];
            rgba[row][col].g = pixels[((row0 * 256 + row) * width + col0 * 256 + col) * bpp + 1];
            rgba[row][col].b = pixels[((row0 * 256 + row) * width + col0 * 256 + col) * bpp + 2];

            if (bpp == 4) {
                rgba[row][col].a = pixels[((row0 * 256 + row) * width + col0 * 256 + col) * bpp + 3];
            }
            else {
                rgba[row][col].a = 255;
            }
            if (is_white_only) {
                if (rgba[row][col].r != 255 || rgba[row][col].g != 255 || rgba[row][col].b != 255) {
                    is_white_only = false;
                }
            }

        }
    }

    if (!is_white_only) stbi_write_png(in_.c_str(), static_cast<int>(width0), static_cast<int>(height0), static_cast<int>(sizeof(paxs::RGBA)), rgba.get(), 0);

    return true;
}

int main() {

    const std::string input = "../../../Data/Map/XYZTile/Union/ColorCodingByProvincesOfJapan/zxy_10_853_385_64_64.png";
    const std::string output = "../../../Data/XYZTiles/Ryoseikoku_20240114_Image/10/zxy_10_";

    {
        std::ifstream ifs(input);
        if (!ifs) return 0;
    }
    int bpp = 4;
    int width2{ width }, height2{ height }; //幅と高さ

    unsigned char* pixels = nullptr;
    // ファイルを読み込み、画像データを取り出す
    //   最後の引数でピクセルあたりのバイト数を強制できる
    pixels = stbi_load(input.c_str(), &width2, &height2, &bpp, 0);
    if (pixels == nullptr) {
        std::cout << "null: " << input << std::endl;
        return 0;
    }

    int index = 0;

    for (int y = min_y, row0 = 0; y < max_y; ++y, ++row0) {
        for (int x = min_x, col0 = 0; x < max_x; ++x, ++col0) {
            ou(output + std::to_string(x) + "_" + std::to_string(y) + ".png",
                pixels, row0, col0, bpp);
        }
    }
    // メモリ上の画像データを破棄
    stbi_image_free(pixels);
}
