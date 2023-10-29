/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/


#include <PAX_SAPIENTICA/GeographicInformation/Slope.hpp>

#include <iostream>

int main() {
    // 傾斜のバイナリデータを読み込む
    const std::string input_name = "Data/Map/XYZTile/Slope/Data/SlopeData20231029/2010/8/zxy_8_0_76.bin";
    paxs::Input8BitBinarySlope i8bbs(input_name, "./../../../");

    unsigned char xyz_tiles[256 * 256]{};

    for (std::size_t i = 0; i < 256 * 256; ++i) {
        xyz_tiles[i] = 255; // エラー値を入れ、エラー値が出ないか確認する
    }

    i8bbs.calc(xyz_tiles);

    for (std::size_t i = 0; i < 256 * 256; ++i) {
        std::cout << (int)xyz_tiles[i] << ",";
        if ((i & 255) == 255) std::cout << "\n";
    }
}

int  output_main() {

    const std::string input_name = "Data/Map/XYZTile/Slope/Data/SlopeData20230514/2010/{z}/slope_{z}_{x}_{y}.tsv";
    const std::string output_name = "test7/{z}/zxy_{z}_{x}_{y}.bin";
    for (std::size_t z = 0; z < 4; ++z) {
        const std::size_t z_num = std::size_t(std::pow(2, z));

        for (std::size_t x = 0; x < z_num; ++x) {
            for (std::size_t y = 0; y < z_num; ++y) {

                std::string input_name_zxy = input_name;
                paxs::StringExtensions::replace(input_name_zxy, "{z}", std::to_string(z));
                paxs::StringExtensions::replace(input_name_zxy, "{x}", std::to_string(x));
                paxs::StringExtensions::replace(input_name_zxy, "{y}", std::to_string(y));
                std::string output_name_zxy = output_name;
                paxs::StringExtensions::replace(output_name_zxy, "{z}", std::to_string(z));
                paxs::StringExtensions::replace(output_name_zxy, "{x}", std::to_string(x));
                paxs::StringExtensions::replace(output_name_zxy, "{y}", std::to_string(y));

                paxs::SlopeDegU0To250AllOutput sda(
                    input_name_zxy, "./../../../",
                    output_name_zxy, "./", '\t'
                );
                sda.calc();
            }
        }
        std::cout << z << ", ";
    }
    return 0;
}
