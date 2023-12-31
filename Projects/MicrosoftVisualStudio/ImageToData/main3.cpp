/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

/*##########################################################################################
	棒状の画像から色情報を作成
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

	int bpp = 3;
	int width2{ 1 }; //幅と高さ
	int height2{ 489 }; //幅と高さ

	std::ofstream ofs("color.tsv");
	std::string in_ = "color.png";
	std::ifstream ifs(in_);
	if (!ifs) return false;

	unsigned char* pixels = nullptr;

	// ファイルを読み込み、画像データを取り出す
	//   最後の引数でピクセルあたりのバイト数を強制できる
	pixels = stbi_load(in_.c_str(), &width2, &height2, &bpp, 0);
	if (pixels == nullptr) return false;

	for (std::size_t row{}, count = 0; row < (std::size_t)height2; ++row) {
		for (std::size_t col{}; col < (std::size_t)width2; ++col, count += std::size_t(bpp)) {
			ofs << std::to_string(pixels[count]) << '\t'; // 赤
			ofs << std::to_string(pixels[count + 1]) << '\t'; // 緑
			ofs << std::to_string(pixels[count + 2]) << '\n'; // 青
		}
	}
	// メモリ上の画像データを破棄
	stbi_image_free(pixels);
	std::cout << "end";
	return true;
}

int main() {
	output();
}
