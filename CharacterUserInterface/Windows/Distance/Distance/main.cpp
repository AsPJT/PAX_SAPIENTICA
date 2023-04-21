
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

// 指定した XYZ タイルの XYZ の距離文字列を算出する
StringXY xyzToDistanceString(int x, int y, int z) {
	const double pi = 3.1415926535897932384626433832795028841971;

	// 2^z
	int z_square_of_2 = int(std::pow(2, z));
	// (2^z)^2
	int z_square_of_2_squared = z_square_of_2 * z_square_of_2;

	double start_x = 0.0;
	double center_x = 0.5;
	double end_x = 1.0;
	double start_y = 0.0;
	double center_y = 0.5;
	double end_y = 1.0;

	double sx_len = (360.0 * (x + start_x) / z_square_of_2) - 180.0;
	double cx_len = (360.0 * (x + center_x) / z_square_of_2) - 180.0;
	double ex_len = (360.0 * (x + end_x) / z_square_of_2) - 180.0;

	double sy_len = (360.0 * (y + start_y) / z_square_of_2) - 180.0;
	double cy_len = (360.0 * (y + center_y) / z_square_of_2) - 180.0;
	double ey_len = (360.0 * (y + end_y) / z_square_of_2) - 180.0;

	double s_latitude_len = std::asin(std::tanh(sy_len / 180.0 * pi)) / (pi) * 180.0;
	double c_latitude_len = std::asin(std::tanh(cy_len / 180.0 * pi)) / (pi) * 180.0;
	double e_latitude_len = std::asin(std::tanh(ey_len / 180.0 * pi)) / (pi) * 180.0;

	// 確認用
	//std::cout << sx_len << std::endl;
	//std::cout << cx_len << std::endl;
	//std::cout << ex_len << std::endl;
	//std::cout << sy_len << std::endl;
	//std::cout << cy_len << std::endl;
	//std::cout << ey_len << std::endl;
	//std::cout << s_latitude_len << std::endl;
	//std::cout << c_latitude_len << std::endl;
	//std::cout << e_latitude_len << std::endl;

	// 経度
	DMS s_longitude(sx_len);
	DMS c_longitude(cx_len);
	DMS e_longitude(ex_len);
	// 緯度
	DMS s_latitude(s_latitude_len);
	DMS c_latitude(c_latitude_len);
	DMS e_latitude(e_latitude_len);

	StringXY str;
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

#include <fstream>

int main() {


	for (int z = 9; z < 10; ++z) {
		int z_square_of_2 = int(std::pow(2, z));

		std::ofstream ofs_x("in/x" + std::to_string(z) + ".in");
		std::ofstream ofs_y("in/y" + std::to_string(z) + ".in");
		std::ofstream ofs_x_txt("in/x" + std::to_string(z) + ".txt");
		std::ofstream ofs_y_txt("in/y" + std::to_string(z) + ".txt");

		//int x = 127;
		//int y = 127;

		ofs_x_txt << "Z\tX\tY\tString\n";
		ofs_y_txt << "Z\tX\tY\tString\n";

		for (int x = 0; x < z_square_of_2; ++x) {
			for (int y = 0; y < z_square_of_2; ++y) {
				StringXY str = xyzToDistanceString(x, y, z);
				ofs_x << str.x << '\n';
				ofs_y << str.y << '\n';
				ofs_x_txt << z << '\t' << x << '\t' << y << '\t' << str.x << '\n';
				ofs_y_txt << z << '\t' << x << '\t' << y << '\t' << str.x << '\n';
			}
		}
	}


}