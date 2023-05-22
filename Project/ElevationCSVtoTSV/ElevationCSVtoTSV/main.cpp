#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <cstddef>
#include <memory>
#include <new>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include <PAX_SAPIENTICA/Elevation/Slope.hpp>

// 画像保存用
struct RGBA {
	unsigned char r, g, b, a; //赤, 緑, 青, 透過
	RGBA() = default;
	constexpr RGBA(const unsigned char r_, const unsigned char g_, const unsigned char b_, const unsigned char a_) :r(r_), g(g_), b(b_), a(a_) {}
};

struct TextXYZBD {
	bool sea{};
	double tile{};
};

class TextXYZTiles {
public:
	TextXYZBD sea_tile[256][256]{};
	
	std::vector<std::vector<std::string>> text{};

	void clearTiles() {
		constexpr std::size_t width{ 256 }, height{ 256 }; //幅と高さ
		for (std::size_t row{}; row < height; ++row) {
			for (std::size_t col{}; col < width; ++col) {
				sea_tile[row][col].sea = true;
				sea_tile[row][col].tile = 0.0;
			}
		}
	}
	bool setTiles() {
		clearTiles();
		constexpr std::size_t width{ 256 }, height{ 256 }; //幅と高さ
		for (std::size_t row{}; row < height && row < text.size(); ++row) {
			for (std::size_t col{}; col < width && col < text[row].size(); ++col) {
				if (text[row][col].size() == 0) {
					sea_tile[row][col].sea = true;
					sea_tile[row][col].tile = 0.0;
				}
				else {
					sea_tile[row][col].sea = false;
					sea_tile[row][col].tile = std::stod(text[row][col]);
				}
			}
		}
		return true;
	}

	void split(const std::string& input, const char delimiter) {
		std::istringstream stream(input);
		std::string field;
		std::vector<std::string> result;
		while (std::getline(stream, field, delimiter)) {
			result.emplace_back(field);
		}
		text.emplace_back(result);
	}
	bool add(const std::string& str_, const char delimiter) {
		text.clear();
		std::ifstream ifs(str_);
		if (!ifs) return true;
		std::string line;
		while (std::getline(ifs, line)) {
			split(line, delimiter);
		}
		return false;
	}

	bool getIsAllE() {
		constexpr std::size_t width{ 256 }, height{ 256 }; //幅と高さ
		if (text.size() == 0) return true; // 空っぽ
		for (std::size_t row{}; row < height && row < text.size(); ++row) {
			for (std::size_t col{}; col < width && col < text[row].size(); ++col) {
				if (text[row][col] != "e") {
					return false;
				}
			}
		}
		return true;
	}

	void output(const std::string& png_str) {
		if (getIsAllE()) return; // 全てデータなしだったら出力しない
		constexpr std::size_t width{ 256 }, height{ 256 }; //幅と高さ

		std::ofstream ofs(png_str);
		for (std::size_t row{}; row < height; ++row) {
			for (std::size_t col{}; col < width; ++col) {
				if (row < text.size() && col < text[row].size()) {
					if (text[row][col] == "e") {}
					else {
						ofs << text[row][col];
					}
				}
				if(col + 1 < width) ofs << '\t';
			}
			if (row + 1 < width) ofs << '\n';
		}
	}


	void inputOnly(
		int xyz_tile_z = 8,
		int xyz_tile_x = 0,
		int xyz_tile_y = 0,
		std::string map_save_name = "./elevation/8/",
		std::string map_name = "elevation"
	) {
		// マイナス値は消去
		if (xyz_tile_x < 0 || xyz_tile_y < 0) {
			text.clear();
			return;
		}

		int i = xyz_tile_y;
		int j = xyz_tile_x;
		const std::string str = {
			std::string(map_save_name) + map_name
			+ std::string("_") + std::to_string(xyz_tile_z)
			+ std::string("_") + std::to_string(j)
			+ std::string("_") + std::to_string(i)
			+ std::string(".tsv")
		};
		if (add(str, '\t')) return; // 失敗
	}

	void inputXYZ(
		int xyz_tile_z = 8,
		int xyz_tile_min_x = 0, int xyz_max_x = 255,
		int xyz_tile_min_y = 0, int xyz_max_y = 255,
		std::string map_url_name = "https://cyberjapandata.gsi.go.jp/xyz/demgm",
		std::string map_save_name = "./elevation/8/",
		std::string map_save2_name = "./elevation_png/8/",
		std::string map_name = "elevation"
	) {
		for (int i = xyz_tile_min_y, k = 0; i <= xyz_max_y; ++i) {
			for (int j = xyz_tile_min_x; j <= xyz_max_x; ++j, ++k) {
				const std::string str = {
					std::string(map_save_name) + map_name
					+ std::string("_") + std::to_string(xyz_tile_z)
					+ std::string("_") + std::to_string(j)
					+ std::string("_") + std::to_string(i)
					+ std::string(".txt")
				};
				if(add(str, ',')) return; // 失敗

				const std::string output_str = {
					std::string(map_save2_name) + map_name
					+ std::string("_") + std::to_string(xyz_tile_z)
					+ std::string("_") + std::to_string(j)
					+ std::string("_") + std::to_string(i)
					+ std::string(".tsv")
				};
				output(output_str);
			}
		}
	}
};


class TextXYZTilesList {
public:
	enum class TextType : std::size_t {
		ul, uc, ur, cl, cc, cr, dl, dc, dr
	};
	//TextXYZTiles text_list[9]{};
	std::vector<TextXYZTiles> text_list{};
	TextXYZBD xyz[258][258]{};
	double slope[256][256]{};

	TextXYZTilesList() {
		text_list.resize(9);
	}

	std::vector<double> distance_x{};
	std::vector<double> distance_y{};

	bool add(const std::string& str_, std::vector<double>& distance) {
		distance.clear();
		std::ifstream ifs(str_);
		if (!ifs) return true;
		std::string line;
		while (std::getline(ifs, line)) {
			distance.emplace_back(std::stod(line));
		}
		return false;
	}


	bool getIsAllSea() {
		constexpr std::size_t width{ 256 }, height{ 256 }; //幅と高さ
		for (std::size_t row{}; row < height; ++row) {
			for (std::size_t col{}; col < width; ++col) {
				if (slope[row][col] != -1.0) {
					return false;
				}
			}
		}
		return true;
	}

	void output(const std::string& slope_str,
		const bool is_png = true // 画像保存かどうか
	) {
		if (getIsAllSea()) return; // 全てデータなしだったら出力しない

		constexpr std::size_t width{ 256 }, height{ 256 }; //幅と高さ

		if (is_png) {
			std::unique_ptr<RGBA[][width]> rgba(new(std::nothrow) RGBA[height][width]);
			if (!rgba) return;

			for (std::size_t row{}; row < height; ++row)
				for (std::size_t col{}; col < width; ++col) {
					const double num = slope[row][col];
					if (num == -1.0) { // 透明
						rgba[row][col].r = 0; // 赤
						rgba[row][col].g = 0;
						rgba[row][col].b = 0;
						rgba[row][col].a = 0; // 透明
					}
					else {
						if (num <= 0.0) {
							rgba[row][col].r = 194; //赤
							rgba[row][col].g = 123;
							rgba[row][col].b = 160;
							rgba[row][col].a = 255; //不透過
						}
						else if (num <= 1.0) {
							rgba[row][col].r = 142; //赤
							rgba[row][col].g = 124;
							rgba[row][col].b = 195;
							rgba[row][col].a = 255; //不透過
						}
						else if (num <= 2.0) {
							rgba[row][col].r = 111; //赤
							rgba[row][col].g = 168;
							rgba[row][col].b = 220;
							rgba[row][col].a = 255; //不透過
						}
						else if (num <= 3.0) {
							rgba[row][col].r = 109; //赤
							rgba[row][col].g = 158;
							rgba[row][col].b = 235;
							rgba[row][col].a = 255; //不透過
						}
						else if (num <= 4.0) {
							rgba[row][col].r = 118; //赤
							rgba[row][col].g = 165;
							rgba[row][col].b = 175;
							rgba[row][col].a = 255; //不透過
						}
						else if (num <= 6.0) {
							rgba[row][col].r = 147; //赤
							rgba[row][col].g = 196;
							rgba[row][col].b = 125;
							rgba[row][col].a = 255; //不透過
						}
						else if (num <= 8.0) {
							rgba[row][col].r = 255; //赤
							rgba[row][col].g = 217;
							rgba[row][col].b = 102;
							rgba[row][col].a = 255; //不透過
						}
						else if (num <= 12.0) {
							rgba[row][col].r = 246; //赤
							rgba[row][col].g = 178;
							rgba[row][col].b = 107;
							rgba[row][col].a = 255; //不透過
						}
						else if (num <= 16.0) {
							rgba[row][col].r = 224; //赤
							rgba[row][col].g = 102;
							rgba[row][col].b = 102;
							rgba[row][col].a = 255; //不透過
						}
						else if (num <= 32.0) {
							rgba[row][col].r = 204; //赤
							rgba[row][col].g = 65;
							rgba[row][col].b = 37;
							rgba[row][col].a = 255; //不透過
						}
						else if (num <= 48.0) {
							rgba[row][col].r = 166; //赤
							rgba[row][col].g = 28;
							rgba[row][col].b = 0;
							rgba[row][col].a = 255; //不透過
						}
						else if (num <= 64.0) {
							rgba[row][col].r = 133; //赤
							rgba[row][col].g = 32;
							rgba[row][col].b = 12;
							rgba[row][col].a = 255; //不透過
						}
						else {
							rgba[row][col].r = 0; //赤
							rgba[row][col].g = 0;
							rgba[row][col].b = 0;
							rgba[row][col].a = 255; //不透過
						}
					}
				}
			stbi_write_png(slope_str.c_str(), static_cast<int>(width), static_cast<int>(height), static_cast<int>(sizeof(RGBA)), rgba.get(), 0);
			return;
		}


		std::ofstream ofs(slope_str);
		for (std::size_t row{}; row < height; ++row) {
			for (std::size_t col{}; col < width; ++col) {
				if (slope[row][col] != -1.0) {
					ofs << slope[row][col];
				}
				if (col + 1 < width) ofs << '\t';
			}
			if (row + 1 < width) ofs << '\n';
		}
	}

	void setSlope(const int z, const int y) {
		constexpr std::size_t width{ 256 }, height{ 256 }; //幅と高さ
		for (std::size_t row{}; row < height; ++row) {
			for (std::size_t col{}; col < width; ++col) {
				// 海だったらスルー
				if (xyz[row + 1][col + 1].sea) {
					slope[row][col] = -1.0;
					continue;
				}
				const double* const grid[9] = {
				&(xyz[row][col].tile),
				&(xyz[row][col + 1].tile),
				&(xyz[row][col + 2].tile),
				&(xyz[row + 1][col].tile),
				&(xyz[row + 1][col + 1].tile),
				&(xyz[row + 1][col + 2].tile),
				&(xyz[row + 2][col].tile),
				&(xyz[row + 2][col + 1].tile),
				&(xyz[row + 2][col + 2].tile),
				};

				double x_cellsize = 0.0;
				double y_cellsize = 0.0;
				if (y * 8 + row < distance_x.size()) {
					x_cellsize = distance_x[y * 8 + row];
				}
				else std::cout << "DX over" << std::endl;
				if (y * 8 + row < distance_y.size()) {
					y_cellsize = distance_y[y * 8 + row];
				}
				else std::cout << "DY over" << std::endl;

				slope[row][col] = paxs::getSlopeRad(grid, x_cellsize, y_cellsize) * (180.0 / 3.141592653589793);
			}
		}
	}

	void clearXYZ() {
		constexpr std::size_t width{ 256 }, height{ 256 }; //幅と高さ
		for (std::size_t row{}; row < height; ++row) {
			for (std::size_t col{}; col < width; ++col) {
				slope[row][col] = 0.0;
			}
		}
		for (std::size_t row{}; row < 258; ++row) {
			for (std::size_t col{}; col < 258; ++col) {
				xyz[row][col].tile = 0.0;
				xyz[row][col].sea = true;
			}
		}
	}

	void setXYZ() {
		clearXYZ();
		xyz[0][0] = (text_list[std::size_t(TextType::ul)].sea_tile[255][255]);
		xyz[257][0] = (text_list[std::size_t(TextType::dl)].sea_tile[0][255]);
		xyz[0][257] = (text_list[std::size_t(TextType::ur)].sea_tile[255][0]);
		xyz[257][257] = (text_list[std::size_t(TextType::dr)].sea_tile[0][0]);

		for (std::size_t i = 0; i < 256; ++i) {
			xyz[0][i + 1] = (text_list[std::size_t(TextType::uc)].sea_tile[255][i]);
			xyz[257][i + 1] = (text_list[std::size_t(TextType::dc)].sea_tile[0][i]);
			xyz[i + 1][0] = (text_list[std::size_t(TextType::cl)].sea_tile[i][255]);
			xyz[i + 1][257] = (text_list[std::size_t(TextType::cr)].sea_tile[i][0]);
		}

		for (std::size_t i = 0; i < 256; ++i) {
			for (std::size_t j = 0; j < 256; ++j) {
				xyz[i + 1][j + 1] = (text_list[std::size_t(TextType::cc)].sea_tile[i][j]);
			}
		}

	}

	void setAll(const int z,
		std::string map_d_name = "./elevation_d/8/",
		std::string map_load_name = "./elevation_png/8/",
		std::string map_save_name = "./elevation_png/8/",
		std::string map_name = "elevation",
		std::string map_output_name = "slope",
		const bool is_png = true // 画像保存かどうか
	) {
		const std::string dx_str = { std::string(map_d_name) + std::string("x") + std::to_string(z + 8) + std::string(".txt") };
		const std::string dy_str = { std::string(map_d_name) + std::string("y") + std::to_string(z + 8) + std::string(".txt") };
		if (add(dx_str, distance_x)) std::cout << "No DX." << std::endl;
		if (add(dy_str, distance_y)) std::cout << "No DY." << std::endl;

		const int min_x = 0;
		const int min_y = 0;
		const int max_x = int(std::pow(2, z)) - 1;
		const int max_y = int(std::pow(2, z)) - 1;
		for (int y = min_y; y <= max_y; ++y) {
			for (int x = min_x; x <= max_x; ++x) {

				const int lx = ((x <= min_x) ? max_x : (x - 1));
				const int rx = ((x >= max_x) ? min_x : (x + 1));
				const int uy = ((y <= min_y) ? -1 : (y - 1));
				const int dy = ((y >= max_y) ? -1 : (y + 1));

				const int ulx = lx;
				const int uly = uy;
				const int ucx = x;
				const int ucy = uy;
				const int urx = rx;
				const int ury = uy;
				const int clx = lx;
				const int cly = y;
				const int ccx = x;
				const int ccy = y;
				const int crx = rx;
				const int cry = y;
				const int dlx = lx;
				const int dly = dy;
				const int dcx = x;
				const int dcy = dy;
				const int drx = rx;
				const int dry = dy;
				text_list[std::size_t(TextType::ul)].inputOnly(
					z, ulx, uly, map_load_name, map_name);
				text_list[std::size_t(TextType::uc)].inputOnly(
					z, ucx, ucy, map_load_name, map_name);
				text_list[std::size_t(TextType::ur)].inputOnly(
					z, urx, ury, map_load_name, map_name);
				text_list[std::size_t(TextType::cl)].inputOnly(
					z, clx, cly, map_load_name, map_name);
				text_list[std::size_t(TextType::cc)].inputOnly(
					z, ccx, ccy, map_load_name, map_name);
				text_list[std::size_t(TextType::cr)].inputOnly(
					z, crx, cry, map_load_name, map_name);
				text_list[std::size_t(TextType::dl)].inputOnly(
					z, dlx, dly, map_load_name, map_name);
				text_list[std::size_t(TextType::dc)].inputOnly(
					z, dcx, dcy, map_load_name, map_name);
				text_list[std::size_t(TextType::dr)].inputOnly(
					z, drx, dry, map_load_name, map_name);

				for (int layer = 0; layer < 9; ++layer) {
					text_list[layer].setTiles();
				}
				setXYZ();
				setSlope(z, y);

				const std::string output_str = {
					std::string(map_save_name) + map_output_name
					+ std::string("_") + std::to_string(z)
					+ std::string("_") + std::to_string(x)
					+ std::string("_") + std::to_string(y)
					+ (is_png ? std::string(".png") : std::string(".tsv"))
				};
				output(output_str, is_png);
			}
		}
	}
};

#include <memory>

int main(void) {

	const std::string path = "./../../../";

	std::unique_ptr<TextXYZTilesList> tiles_list(new TextXYZTilesList);
//	{
//	tiles_list->setAll(0,
//		path + "Data/Distance/East135/",
//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/0/",
//		path + "Data/Map/XYZTile/Slope/Image/Slope/2010/0/",
//		"elevation", "slope");
//}
//{
//	tiles_list->setAll(1,
//		path + "Data/Distance/East135/",
//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/1/",
//		path + "Data/Map/XYZTile/Slope/Image/Slope/2010/1/",
//		"elevation", "slope");
//}
//{
//	tiles_list->setAll(2,
//		path + "Data/Distance/East135/",
//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/2/",
//		path + "Data/Map/XYZTile/Slope/Image/Slope/2010/2/",
//		"elevation", "slope");
//}
//{
//	tiles_list->setAll(3,
//		path + "Data/Distance/East135/",
//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/3/",
//		path + "Data/Map/XYZTile/Slope/Image/Slope/2010/3/",
//		"elevation", "slope");
//}
//{
//	tiles_list->setAll(4,
//		path + "Data/Distance/East135/",
//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/4/",
//		path + "Data/Map/XYZTile/Slope/Image/Slope/2010/4/",
//		"elevation", "slope");
//}
{
	tiles_list->setAll(5,
		path + "Data/Distance/East135/",
		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/5/",
		path + "Data/Map/XYZTile/Slope/Image/Slope/2010/5/",
		"elevation", "slope");
}
std::cout << 5 << std::endl;
{
	tiles_list->setAll(6,
		path + "Data/Distance/East135/",
		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/6/",
		path + "Data/Map/XYZTile/Slope/Image/Slope/2010/6/",
		"elevation", "slope");
}
std::cout << 6 << std::endl;
{
	tiles_list->setAll(7,
		path + "Data/Distance/East135/",
		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/7/",
		path + "Data/Map/XYZTile/Slope/Image/Slope/2010/7/",
		"elevation", "slope");
}
std::cout << 7 << std::endl;
{
	tiles_list->setAll(8,
		path + "Data/Distance/East135/",
		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/8/",
		path + "Data/Map/XYZTile/Slope/Image/Slope/2010/8/",
		"elevation", "slope");
}
std::cout << 8 << std::endl;



	//{
	//	tiles_list->setAll(0,
	//		path + "Data/Distance/East135/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/0/",
	//		path + "Data/Map/XYZTile/Slope/Data/Slope/2010/0/",
	//		"elevation", "slope");
	//}
	//{
	//	tiles_list->setAll(1,
	//		path + "Data/Distance/East135/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/1/",
	//		path + "Data/Map/XYZTile/Slope/Data/Slope/2010/1/",
	//		"elevation", "slope");
	//}
	//{
	//	tiles_list->setAll(2,
	//		path + "Data/Distance/East135/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/2/",
	//		path + "Data/Map/XYZTile/Slope/Data/Slope/2010/2/",
	//		"elevation", "slope");
	//}
	//{
	//	tiles_list->setAll(3,
	//		path + "Data/Distance/East135/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/3/",
	//		path + "Data/Map/XYZTile/Slope/Data/Slope/2010/3/",
	//		"elevation", "slope");
	//}
	//{
	//	tiles_list->setAll(4,
	//		path + "Data/Distance/East135/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/4/",
	//		path + "Data/Map/XYZTile/Slope/Data/Slope/2010/4/",
	//		"elevation", "slope");
	//}
	//{
	//	tiles_list->setAll(5,
	//		path + "Data/Distance/East135/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/5/",
	//		path + "Data/Map/XYZTile/Slope/Data/Slope/2010/5/",
	//		"elevation", "slope");
	//}
	//{
	//	tiles_list->setAll(6,
	//		path + "Data/Distance/East135/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/6/",
	//		path + "Data/Map/XYZTile/Slope/Data/Slope/2010/6/",
	//		"elevation", "slope");
	//}
	//{
	//	tiles_list->setAll(7,
	//		path + "Data/Distance/East135/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/7/",
	//		path + "Data/Map/XYZTile/Slope/Data/Slope/2010/7/",
	//		"elevation", "slope");
	//}
	//{
	//	tiles_list->setAll(8,
	//		path + "Data/Distance/East135/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/8/",
	//		path + "Data/Map/XYZTile/Slope/Data/Slope/2010/8/",
	//		"elevation", "slope");
	//}



	//{
	//	std::unique_ptr <TextXYZTiles> io_text(new TextXYZTiles);
	//	io_text->inputXYZ(0, 0, 0, 0, 0, "https://cyberjapandata.gsi.go.jp/xyz/demgm",
	//		"C:/Users/wafuu/source/repos/HTTPDownload/HTTPDownload/App/elevation/0/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/0/",
	//		"elevation");
	//}
	//{
	//	std::unique_ptr <TextXYZTiles> io_text(new TextXYZTiles);
	//	io_text->inputXYZ(1, 0, 1, 0, 1, "https://cyberjapandata.gsi.go.jp/xyz/demgm",
	//		"C:/Users/wafuu/source/repos/HTTPDownload/HTTPDownload/App/elevation/1/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/1/",
	//		"elevation");
	//}
	//{
	//	std::unique_ptr <TextXYZTiles> io_text(new TextXYZTiles);
	//	io_text->inputXYZ(2, 0, 3, 0, 3, "https://cyberjapandata.gsi.go.jp/xyz/demgm",
	//		"C:/Users/wafuu/source/repos/HTTPDownload/HTTPDownload/App/elevation/2/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/2/",
	//		"elevation");
	//}
	//{
	//	std::unique_ptr <TextXYZTiles> io_text(new TextXYZTiles);
	//	io_text->inputXYZ(3, 0, 7, 0, 7, "https://cyberjapandata.gsi.go.jp/xyz/demgm",
	//		"C:/Users/wafuu/source/repos/HTTPDownload/HTTPDownload/App/elevation/3/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/3/",
	//		"elevation");
	//}
	//{
	//	std::unique_ptr <TextXYZTiles> io_text(new TextXYZTiles);
	//	io_text->inputXYZ(4, 0, 15, 0, 15, "https://cyberjapandata.gsi.go.jp/xyz/demgm",
	//		"C:/Users/wafuu/source/repos/HTTPDownload/HTTPDownload/App/elevation/4/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/4/",
	//		"elevation");
	//}
	//std::cout << 4 << std::endl;
	//{
	//	std::unique_ptr <TextXYZTiles> io_text(new TextXYZTiles);
	//	io_text->inputXYZ(5, 0, 31, 0, 31, "https://cyberjapandata.gsi.go.jp/xyz/demgm",
	//		"C:/Users/wafuu/source/repos/HTTPDownload/HTTPDownload/App/elevation/5/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/5/",
	//		"elevation");
	//}
	//std::cout << 5 << std::endl;
	//{
	//	std::unique_ptr <TextXYZTiles> io_text(new TextXYZTiles);
	//	io_text->inputXYZ(6, 0, 63, 0, 63, "https://cyberjapandata.gsi.go.jp/xyz/demgm",
	//		"C:/Users/wafuu/source/repos/HTTPDownload/HTTPDownload/App/elevation/6/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/6/",
	//		"elevation");
	//}
	//std::cout << 6 << std::endl;
	//{
	//	std::unique_ptr <TextXYZTiles> io_text(new TextXYZTiles);
	//	io_text->inputXYZ(7, 0, 127, 0, 127, "https://cyberjapandata.gsi.go.jp/xyz/demgm",
	//		"C:/Users/wafuu/source/repos/HTTPDownload/HTTPDownload/App/elevation/7/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/7/",
	//		"elevation");
	//}
	//std::cout << 7 << std::endl;
	//{
	//	std::unique_ptr <TextXYZTiles> io_text(new TextXYZTiles);
	//	io_text->inputXYZ(8, 0, 255, 0, 255, "https://cyberjapandata.gsi.go.jp/xyz/demgm",
	//		"C:/Users/wafuu/source/repos/HTTPDownload/HTTPDownload/App/elevation/8/",
	//		path + "Data/Map/XYZTile/Elevation/Data/Elevation/2010/8/",
	//		"elevation");
	//}
	//std::cout << 8 << std::endl;
}
