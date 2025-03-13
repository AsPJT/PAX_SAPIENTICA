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
#include <filesystem>

#include <PAX_SAPIENTICA/Color/RGBA.hpp>
#include <PAX_SAPIENTICA/Color/Ryoseikoku.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>

#include <PAX_SAPIENTICA/GeographicInformation/Slope.hpp>

const std::string path = "../../../"; // Data 階層のパス

// ColorCodingByProvincesOfJapan_8_218_90_16_16.png
// 以下の変数を外部から受け取れるようにする
namespace xyz_tiles_conv {

    struct XYZTilesConv {
        XYZTilesConv(const int z_, const int min_x_, const int min_y_, const int len_x_, const int len_y_, const std::string& input_)
            : z(z_), min_x(min_x_), min_y(min_y_), len_x(len_x_), len_y(len_y_), width(256 * len_x_), height(256 * len_y_)
        {
            input = input_;
        };
        int z = 0;
        int min_x = 0;
        int min_y = 0;
        int len_x = 1;
        int len_y = 1;
        int width = 256;
        int height = 256;
        std::string input{};
    };
    //const std::string output = path + "Data/XYZTiles/Test20250313/{z}/{x}/{y}";
    const std::string output = path + "Data/XYZTiles/Test20250313/zxy_{z}_{x}_{y}";

    constexpr std::size_t width0{ 256 }, height0{ 256 }; //幅と高さ
}

bool output_png(const xyz_tiles_conv::XYZTilesConv& conv_data, const std::string& out_, unsigned char* pixels, const int row0, const int col0, const int bpp) {
    // 色の二次元配列
    std::unique_ptr<paxs::RGBA[][xyz_tiles_conv::width0]> rgba(new(std::nothrow) paxs::RGBA[xyz_tiles_conv::height0][xyz_tiles_conv::width0]);
    if (!rgba) return false;

    bool is_white_only = true; // 白のみ
    for (std::size_t row{}, count = 0; row < xyz_tiles_conv::height0; ++row) {
        for (std::size_t col{}; col < xyz_tiles_conv::width0; ++col, count += std::size_t(bpp)) {
            rgba[row][col].r = pixels[((row0 * 256 + row) * conv_data.width + col0 * 256 + col) * bpp];
            rgba[row][col].g = pixels[((row0 * 256 + row) * conv_data.width + col0 * 256 + col) * bpp + 1];
            rgba[row][col].b = pixels[((row0 * 256 + row) * conv_data.width + col0 * 256 + col) * bpp + 2];

            if (bpp == 4) {
                rgba[row][col].a = pixels[((row0 * 256 + row) * conv_data.width + col0 * 256 + col) * bpp + 3];
            }
            else rgba[row][col].a = 255;
            if (is_white_only) {
                if (rgba[row][col].r != 255 || rgba[row][col].g != 255 || rgba[row][col].b != 255) {
                    is_white_only = false;
                }
            }

        }
    }

    if (!is_white_only) stbi_write_png(std::string(out_ + ".png").c_str(), static_cast<int>(xyz_tiles_conv::width0), static_cast<int>(xyz_tiles_conv::height0), static_cast<int>(sizeof(paxs::RGBA)), rgba.get(), 0);

    return true;
}

bool output_tsv(const xyz_tiles_conv::XYZTilesConv& conv_data, const std::string& out_, unsigned char* pixels, const int row0, const int col0, const int bpp) {
    // 色の二次元配列
    std::unique_ptr<paxs::RGBA[][xyz_tiles_conv::width0]> rgba(new(std::nothrow) paxs::RGBA[xyz_tiles_conv::height0][xyz_tiles_conv::width0]);
    if (!rgba) return false;

    bool is_white_only = true; // 白のみ
    for (std::size_t row{}, count = 0; row < xyz_tiles_conv::height0; ++row) {
        for (std::size_t col{}; col < xyz_tiles_conv::width0; ++col, count += std::size_t(bpp)) {
            rgba[row][col].r = pixels[((row0 * 256 + row) * conv_data.width + col0 * 256 + col) * bpp];
            rgba[row][col].g = pixels[((row0 * 256 + row) * conv_data.width + col0 * 256 + col) * bpp + 1];
            rgba[row][col].b = pixels[((row0 * 256 + row) * conv_data.width + col0 * 256 + col) * bpp + 2];
            if (bpp == 4) rgba[row][col].a = pixels[((row0 * 256 + row) * conv_data.width + col0 * 256 + col) * bpp + 3];
            else rgba[row][col].a = 255;
            if (is_white_only) {
                if (rgba[row][col].r != 255 || rgba[row][col].g != 255 || rgba[row][col].b != 255) is_white_only = false;
            }
        }
    }
    if (is_white_only) return true; // 白しかなかったら出力しない
    std::ofstream ofs(out_ + ".tsv");
    paxs::RyoseikokuColor rc;
    for (std::size_t row{}, count = 0; row < xyz_tiles_conv::height0; ++row) {
        for (std::size_t col{}; col < xyz_tiles_conv::width0; ++col, count += std::size_t(bpp)) {
            ofs << std::to_string(rc.getIndex(rgba[row][col]));
            if (col + 1 < conv_data.width) ofs << '\t';
        }
        if (row + 1 < conv_data.height) ofs << '\n';
    }
    return true;
}

bool output_bin(const xyz_tiles_conv::XYZTilesConv& conv_data, const std::string& out_, unsigned char* pixels, const int row0, const int col0, const int bpp) {
    // 色の二次元配列
    std::unique_ptr<paxs::RGBA[][xyz_tiles_conv::width0]> rgba(new(std::nothrow) paxs::RGBA[xyz_tiles_conv::height0][xyz_tiles_conv::width0]);
    if (!rgba) return false;

    bool is_white_only = true; // 白のみ
    for (std::size_t row{}, count = 0; row < xyz_tiles_conv::height0; ++row) {
        for (std::size_t col{}; col < xyz_tiles_conv::width0; ++col, count += std::size_t(bpp)) {
            rgba[row][col].r = pixels[((row0 * 256 + row) * conv_data.width + col0 * 256 + col) * bpp];
            rgba[row][col].g = pixels[((row0 * 256 + row) * conv_data.width + col0 * 256 + col) * bpp + 1];
            rgba[row][col].b = pixels[((row0 * 256 + row) * conv_data.width + col0 * 256 + col) * bpp + 2];
            if (bpp == 4) rgba[row][col].a = pixels[((row0 * 256 + row) * conv_data.width + col0 * 256 + col) * bpp + 3];
            else rgba[row][col].a = 255;
            if (is_white_only) {
                if (rgba[row][col].r != 255 || rgba[row][col].g != 255 || rgba[row][col].b != 255) is_white_only = false;
            }
        }
    }
    if (is_white_only) return true; // 白しかなかったら出力しない
    paxs::SlopeDegU0To250UnitOutput sd(out_ + ".bin");
    unsigned char pre_value = 252; // 1 つ前の値
    paxs::RyoseikokuColor rc;
    for (std::size_t row{}, count = 0; row < xyz_tiles_conv::height0; ++row) {
        for (std::size_t col{}; col < xyz_tiles_conv::width0; ++col, count += std::size_t(bpp)) {
            const unsigned char next_value = static_cast<unsigned char>(rc.getIndex(rgba[row][col]));
            sd.calc(pre_value, next_value);
            pre_value = next_value;
        }
    }
    sd.calc(pre_value, 252);

    return true;
}

bool ToTSV(const xyz_tiles_conv::XYZTilesConv& conv_data) {
    std::string input_path = conv_data.input;
    paxs::StringExtensions::replace(input_path, "{z}", std::to_string(conv_data.z));
    paxs::StringExtensions::replace(input_path, "{min_x}", std::to_string(conv_data.min_x));
    paxs::StringExtensions::replace(input_path, "{min_y}", std::to_string(conv_data.min_y));
    paxs::StringExtensions::replace(input_path, "{len_x}", std::to_string(conv_data.len_x));
    paxs::StringExtensions::replace(input_path, "{len_y}", std::to_string(conv_data.len_y));
    {
        std::ifstream ifs(input_path);
        if (!ifs) return false; // 入力ファイルが見つからなかった
    }
    int bpp = 4; // bit
    int width2{ conv_data.width }, height2{ conv_data.height }; //幅と高さ

    // Z 数値変換済みの出力パス
    std::string output_path_z = xyz_tiles_conv::output;
    paxs::StringExtensions::replace(output_path_z, "{z}", std::to_string(conv_data.z));

    unsigned char* pixels = nullptr;

    // ファイルを読み込み、画像データを取り出す
    //   最後の引数でピクセルあたりのバイト数を強制できる
    pixels = stbi_load(input_path.c_str(), &width2, &height2, &bpp, 0);
    if (pixels == nullptr) {
        std::cout << "null: " << input_path << std::endl;
        return false;
    }

    const int max_x = conv_data.min_x + conv_data.len_x, max_y = conv_data.min_y + conv_data.len_y;
    for (int y = conv_data.min_y, row0 = 0; y < max_y; ++y, ++row0) {
        std::string output_path_zy = output_path_z;
        paxs::StringExtensions::replace(output_path_zy, "{y}", std::to_string(y));
        for (int x = conv_data.min_x, col0 = 0; x < max_x; ++x, ++col0) {
            std::string output_path = output_path_zy;
            paxs::StringExtensions::replace(output_path, "{x}", std::to_string(x));

            int slash_num = 0;
            for (int i = 0; i < output_path.size(); ++i) {
                if (output_path[i] == '/') slash_num = i;
            }
            if (slash_num != 0 && slash_num < output_path.size()) {
                std::string dir_path = output_path;
                dir_path.resize(slash_num);
                std::filesystem::create_directories(dir_path);
                std::cout << x << "," << y << "," << dir_path << std::endl;
            }
            //output_png(conv_data, output_path, pixels, row0, col0, bpp);
            //output_tsv(conv_data, output_path, pixels, row0, col0, bpp);
            output_bin(conv_data, output_path, pixels, row0, col0, bpp);
        }
    }
    // メモリ上の画像データを破棄
    stbi_image_free(pixels);
    return true;
}

int main() {

    std::vector<xyz_tiles_conv::XYZTilesConv> conv_data_list;
    conv_data_list.emplace_back(
        /*z*/ 10,/*min_x*/ 908,/*min_y*/ 326,/*len_x*/ 50,/*len_y*/ 59,
        /*std::string*/ path + "Data/Map/XYZTile/Union/ColorCodingByProvincesOfJapan/zxy3_{z}_{min_x}_{min_y}_{len_x}_{len_y}.png"
    );
    conv_data_list.emplace_back(
        /*z*/ 10,/*min_x*/ 853,/*min_y*/ 385,/*len_x*/ 64,/*len_y*/ 64,
        /*std::string*/ path + "Data/Map/XYZTile/Union/ColorCodingByProvincesOfJapan/zxy3_{z}_{min_x}_{min_y}_{len_x}_{len_y}.png"
    );

    //conv_data_list.emplace_back(
    //    /*z*/ 10,/*min_x*/ 872,/*min_y*/ 360,/*len_x*/ 64,/*len_y*/ 64,
    //    /*std::string*/ path + "Data/Map/XYZTile/Union/ColorCodingByProvincesOfJapan/ColorCodingByProvincesOfJapan_{z}_{min_x}_{min_y}_{len_x}_{len_y}.png"
    //);

    ToTSV(conv_data_list[0]);
    ToTSV(conv_data_list[1]);

}
