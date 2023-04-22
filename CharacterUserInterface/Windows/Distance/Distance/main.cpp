/*##########################################################################################
	PAX SAPIENTICA Library 💀🌿🌏
	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/
##########################################################################################*/

#include <cmath>
#include <iostream>
#include <string>

struct StringXY {
	std::string x{};
	std::string y{};
};

struct DMS {
	int d = 0;
	int m = 0;
	double s = 0.0;

	void set(const double d_) {
		d = int(std::floor(d_) - (d_ >= 0 ? 0 : (d_ == std::floor(d_) ? 0 : -1)));
		const double mf = std::abs(d_ - double(d)) * 60;
		m = int(std::floor(mf));
		s = std::abs(mf - double(m)) * 60;
	}
	DMS() = default;
	DMS(const double d_) { set(d_); }
};

void dmsToString(std::string& str, const DMS& dms) {
	str += std::to_string(dms.d);
	if (dms.m < 10) str += "0";
	str += std::to_string(dms.m);
	if (dms.s < 10.0) str += "0";
	str += std::to_string(dms.s);
}

// 緯度・経度から距離文字列を算出する
StringXY llToDistanceString(
	const double s_lon, const double c_lon, const double e_lon,
	const double s_lat, const double c_lat, const double e_lat) {
	// 経度
	DMS s_longitude(s_lon);
	DMS c_longitude(c_lon);
	DMS e_longitude(e_lon);
	// 緯度
	DMS s_latitude(s_lat);
	DMS c_latitude(c_lat);
	DMS e_latitude(e_lat);

	StringXY str{};
	// 出発点緯度(dms)
	dmsToString(str.x, c_latitude);
	str.x.push_back(' ');
	// 出発点経度(dms)
	dmsToString(str.x, s_longitude);
	str.x.push_back(' ');
	// 到着点緯度(dms)
	dmsToString(str.x, c_latitude);
	str.x.push_back(' ');
	// 到着点経度(dms)
	dmsToString(str.x, e_longitude);

	// 出発点緯度(dms)
	dmsToString(str.y, s_latitude);
	str.y.push_back(' ');
	// 出発点経度(dms)
	dmsToString(str.y, c_longitude);
	str.y.push_back(' ');
	// 到着点緯度(dms)
	dmsToString(str.y, e_latitude);
	str.y.push_back(' ');
	// 到着点経度(dms)
	dmsToString(str.y, c_longitude);

	return str;
}

// 指定した XYZ タイルの XYZ の距離文字列を算出する
StringXY xyzToDistanceString(const int x, const int y, const int z) {
	const double pi = 3.1415926535897932384626433832795028841971;

	// 2^z
	const int z_square_of_2 = int(std::pow(2, z));

	const double start_x = 0.0;
	const double center_x = 0.5;
	const double end_x = 1.0;
	const double start_y = 0.0;
	const double center_y = 0.5;
	const double end_y = 1.0;

	double s_lon = (360.0 * (x + start_x) / z_square_of_2) - 180.0;
	double c_lon = (360.0 * (x + center_x) / z_square_of_2) - 180.0;
	double e_lon = (360.0 * (x + end_x) / z_square_of_2) - 180.0;

	double sy_len = (360.0 * (y + start_y) / z_square_of_2) - 180.0;
	double cy_len = (360.0 * (y + center_y) / z_square_of_2) - 180.0;
	double ey_len = (360.0 * (y + end_y) / z_square_of_2) - 180.0;

	double s_lat = std::asin(std::tanh(sy_len / 180.0 * pi)) / (pi) * 180.0;
	double c_lat = std::asin(std::tanh(cy_len / 180.0 * pi)) / (pi) * 180.0;
	double e_lat = std::asin(std::tanh(ey_len / 180.0 * pi)) / (pi) * 180.0;

	return llToDistanceString(s_lon, c_lon, e_lon, s_lat, c_lat, e_lat);
}

// 指定した XYZ タイルの XYZ の距離文字列を算出する
StringXY yzToDistanceString(const double lon, const int y, const int z) {
	const double pi = 3.1415926535897932384626433832795028841971;

	// 2^z
	const int z_square_of_2 = int(std::pow(2, z));

	const double start_y = 0.0;
	const double center_y = 0.5;
	const double end_y = 1.0;

	double sy_len = (360.0 * (y + start_y) / z_square_of_2) - 180.0;
	double cy_len = (360.0 * (y + center_y) / z_square_of_2) - 180.0;
	double ey_len = (360.0 * (y + end_y) / z_square_of_2) - 180.0;

	double s_lat = std::asin(std::tanh(sy_len / 180.0 * pi)) / (pi) * 180.0;
	double c_lat = std::asin(std::tanh(cy_len / 180.0 * pi)) / (pi) * 180.0;
	double e_lat = std::asin(std::tanh(ey_len / 180.0 * pi)) / (pi) * 180.0;

	return llToDistanceString(lon, lon, lon, s_lat, c_lat, e_lat);
}
#include <fstream>

int main() {

	double sx = (215.0 - 2.0) / 256;
	double ex = (233.0 + 3.0) / 256;

	double ey = 1.0 - (91.0 - 2.0) / 256;
	double sy = 1.0 - (110.0 + 3.0) / 256;


	for (int z = 16; z < 17; ++z) {
		int z_square_of_2 = int(std::pow(2, z));

		const int zsx = int(sx * z_square_of_2);
		const int zex = int(ex * z_square_of_2);
		const int zsy = int(sy * z_square_of_2);
		const int zey = int(ey * z_square_of_2);

		std::ofstream ofs_x("in_line/x" + std::to_string(z) + ".in");
		//std::ofstream ofs_y("in_line/y" + std::to_string(z) + ".in");
		std::ofstream ofs_x_txt("in_line/x" + std::to_string(z) + ".txt");
		//std::ofstream ofs_y_txt("in_line/y" + std::to_string(z) + ".txt");

		//int x = 127;
		//int y = 127;

		ofs_x_txt << "Z\tX\tY\tString\n";
		//ofs_y_txt << "Z\tX\tY\tString\n";

		//for (int x = zsx; x < z_square_of_2 && x < zex; ++x) {
		int x = int((135.0 + 180.0)/360.0 * double(z_square_of_2));
			//for (int y = 0; y < z_square_of_2; ++y) {
			for (int y = z_square_of_2 / 2; y < z_square_of_2 * 9 /10; ++y) {
			//for (int y = zsy; y < z_square_of_2 && y < zey; ++y) {
				StringXY str = xyzToDistanceString(x, y, z);

				const int ry = (z_square_of_2 - 1) - y;

				//StringXY str = xyzToDistanceString(x, y, z);
				ofs_x << str.x << '\n';
				//ofs_y << str.y << '\n';
				ofs_x_txt << z << '\t' << x << '\t' << ry << '\t' << str.x << '\n';
				//ofs_y_txt << z << '\t' << x << '\t' << ry << '\t' << str.x << '\n';
			}
		//}
	}


}