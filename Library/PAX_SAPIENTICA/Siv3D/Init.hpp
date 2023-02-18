/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_INIT_HPP
#define PAX_SAPIENTICA_SIV3D_INIT_HPP

/*##########################################################################################

##########################################################################################*/

namespace paxs {

	struct MapVec2 {
		int x{}, y{};
	};
	struct MapVec2D {
		double x{}, y{};
	};
}

#include <Siv3D.hpp> // OpenSiv3D v0.6.6

#include <PAX_SAPIENTICA/Calendar/JulianDayNumber.hpp>
#include <PAX_SAPIENTICA/FileRead/Split.hpp>
#include <PAX_SAPIENTICA/Math/Stoi.hpp>

#include <array>
#include <cmath>

#include <string>

#include <PAX_SAPIENTICA/Calendar/JapaneseEra.hpp> // 日本の元号

#include <PAX_SAPIENTICA/Constant/Math.hpp> // 数学定数

namespace paxs {

	// 緯度をメルカトル座標 RadY へ変換する
	double getLatitudeToMercatorRadY(const double value_) {
		return (value_ >= 0 ? 1 : -1) * std::abs(std::log(std::abs(std::tan(paxs::pi / 4.0 - (value_ * paxs::deg_to_rad) / 2.0))));
	}

	// 緯度をメルカトル座標 Y へ変換する
	double getLatitudeToMercatorY(const double value_) {
		return paxs::rad_to_deg * getLatitudeToMercatorRadY(value_);
	}

	struct OutputDate {
		s3d::String calendar_name{};
		s3d::String calendar_en_name{};
		paxs::Date date{};
		bool is_leap_month = false;
	};


	enum class LocationPointEnum {
		location_point_place_name, // 地名
		location_point_pit_dwelling, // 集落遺跡
		location_point_zempo_koen_fun, // 前方後円墳
		location_point_zempo_koho_fun, // 前方後方墳
		location_point_hotategai_gata_kofun // 帆立貝型古墳
	};

	// 地名
	struct LocationPoint {
		std::string name;
		std::string en_name;
		double x, y;
		double min_view, max_view;
		int min_year, max_year;
		LocationPointEnum lpe;
		std::string source{};
	};

	void inputPlace(const std::string& str_, std::vector<LocationPoint>& location_point_list_, const LocationPointEnum lpe_ = LocationPointEnum::location_point_place_name) {
		// 地名を読み込み
		std::ifstream pifs(str_);
		if (pifs.fail()) return;
		std::string pline;
		while (std::getline(pifs, pline)) {
			std::vector<std::string> strvec = paxs::split(pline, '\t');

			location_point_list_.emplace_back(
				strvec[0], // 漢字
				strvec[1], // ローマ字
				std::stod(strvec[2]), // 経度
				getLatitudeToMercatorY(std::stod(strvec[3])), // 緯度
				std::stod(strvec[4]), // 最小サイズ
				std::stod(strvec[5]), // 最大サイズ
				std::stod(strvec[6]), // 最小時代
				std::stod(strvec[7]), // 出典
				lpe_,
				strvec[8] // 最大時代
			);
		}
	}
}

#include <PAX_SAPIENTICA/Siv3D/Key.hpp> // キーボードの入力
#include <PAX_SAPIENTICA/Siv3D/XYZTiles.hpp> // タイルの更新
#include <PAX_SAPIENTICA/Siv3D/InitLogo.hpp> // ロゴを描画

#ifdef SIV3D_PLATFORM(WINDOWS)
#ifndef PAXS_S3D_RESOURCE
#define PAXS_S3D_RESOURCE s3d::Resource
#endif
#else
#ifndef PAXS_S3D_RESOURCE
#define PAXS_S3D_RESOURCE
#endif
#endif

#endif // !PAX_SAPIENTICA_SIV3D_INIT_HPP