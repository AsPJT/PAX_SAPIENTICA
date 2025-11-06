/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_GEOGRAPHIC_INFORMATION_PLACE_NAME_REPOSITORY_HPP
#define PAX_SAPIENTICA_GEOGRAPHIC_INFORMATION_PLACE_NAME_REPOSITORY_HPP

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/InputFile.hpp>
#include <PAX_SAPIENTICA/MapProjection.hpp>
#include <PAX_SAPIENTICA/Map/LocationPoint.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Type/UnorderedMap.hpp>

namespace paxs {

	/// @brief 地名のデータ読み込みを担当するクラス (Infrastructure Layer)
	class PlaceNameRepository {
	public:
		PlaceNameRepository() = default;

		/// @brief 地名データファイルのリストを読み込み、各ファイルをinputPlace経由で処理
		void loadPlaceNameList(
			const std::function<void(const std::string&, double, double, int, int, std::uint_least32_t, std::uint_least32_t, double)>& inputPlaceFunc
		) const {
			std::string str = "";
			AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("PlaceNames"),
				[&](const std::string& path_) {str = path_; });
			if (str.size() == 0) return;

			paxs::InputFile pifs(str);
			if (pifs.fail()) return;
			// 1 行目を読み込む
			if (!(pifs.getLine())) {
				return; // 何もない場合
			}
			// BOM を削除
			pifs.deleteBOM();
			// 1 行目を分割する
			paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu = pifs.splitHashMapMurMur3('\t');

			const std::size_t file_path = getMenuIndex(menu, MurMur3::calcHash("file_path"));
			if (file_path == SIZE_MAX) return; // パスがないのはデータにならない
			const std::size_t place_type = getMenuIndex(menu, MurMur3::calcHash("place_type"));

			const std::size_t minimum_size = getMenuIndex(menu, MurMur3::calcHash("min_size"));
			const std::size_t maximum_size = getMenuIndex(menu, MurMur3::calcHash("max_size"));
			const std::size_t first_julian_day = getMenuIndex(menu, MurMur3::calcHash("first_julian_day"));
			const std::size_t last_julian_day = getMenuIndex(menu, MurMur3::calcHash("last_julian_day"));
			const std::size_t place_texture = getMenuIndex(menu, MurMur3::calcHash("texture"));
			const std::size_t zoom_size = getMenuIndex(menu, MurMur3::calcHash("zoom"));

			const std::size_t first_year = getMenuIndex(menu, MurMur3::calcHash("first_year"));
			const std::size_t last_year = getMenuIndex(menu, MurMur3::calcHash("last_year"));

			// 1 行ずつ読み込み（区切りはタブ）
			while (pifs.getLine()) {
				std::vector<std::string> strvec = pifs.split('\t');

				// パスが空の場合は読み込まない
				if (strvec[file_path].size() == 0) continue;

				// 対象となる地物の種別
				const std::uint_least32_t type = (place_type == SIZE_MAX) ?
					MurMur3::calcHash("place_name") :
					((strvec[place_type].size() == 0) ?
						MurMur3::calcHash("place_name") :
						MurMur3::calcHash(strvec[place_type].size(), strvec[place_type].c_str()));

				// 可視化する地図の最小範囲
				const double min_view = (minimum_size >= strvec.size()) ?
					0 : ((strvec[minimum_size].size() == 0) ?
						0 : std::stod(strvec[minimum_size]));
				// 可視化する地図の最大範囲
				const double max_view = (maximum_size >= strvec.size()) ?
					99999999.0 : ((strvec[maximum_size].size() == 0) ?
						99999999.0 : std::stod(strvec[maximum_size]));
				// 可視化する時代（古い年～）
				const int min_year = (first_julian_day >= strvec.size()) ?
					(first_year >= strvec.size()) ?
					-99999999 : ((strvec[first_year].size() == 0) ?
						-99999999 : int(std::stod(strvec[first_year]) * days_in_a_year + julian_day_on_m1_1_1))
					: ((strvec[first_julian_day].size() == 0) ?
						(first_year >= strvec.size()) ?
						-99999999 : ((strvec[first_year].size() == 0) ?
							-99999999 : int(std::stod(strvec[first_year]) * days_in_a_year + julian_day_on_m1_1_1))
						: std::stoi(strvec[first_julian_day]));
				// 可視化する時代（～新しい年）
				const int max_year = (last_julian_day >= strvec.size()) ?
					(last_year >= strvec.size()) ?
					99999999 : ((strvec[last_year].size() == 0) ?
						99999999 : int(std::stod(strvec[last_year]) * days_in_a_year + julian_day_on_m1_1_1))
					: ((strvec[last_julian_day].size() == 0) ?
						(last_year >= strvec.size()) ?
						99999999 : ((strvec[last_year].size() == 0) ?
							99999999 : int(std::stod(strvec[last_year]) * days_in_a_year + julian_day_on_m1_1_1))
						: std::stoi(strvec[last_julian_day]));

				// 画像
				const std::uint_least32_t place_texture_hash = (place_texture >= strvec.size()) ?
					0 : ((strvec[place_texture].size() == 0) ?
						0 : MurMur3::calcHash(strvec[place_texture].size(), strvec[place_texture].c_str()));

				// 対象となる地物の種別
				const double zoom = (zoom_size >= strvec.size()) ?
					1.0 : ((strvec[zoom_size].size() == 0) ?
						1.0 : std::stod(strvec[zoom_size]));

				// 地物を追加（コールバック経由）
				inputPlaceFunc(strvec[file_path], min_view, max_view, min_year, max_year, type, place_texture_hash, zoom);
			}
		}

		/// @brief 個別の地名ファイルを読み込んでLocationPointListを返す
		LocationPointList loadPlaceFromFile(
			const std::string& str_,
			const double min_view_,
			const double max_view_,
			const int min_year_,
			const int max_year_,
			const std::uint_least32_t lpe_,
			const std::uint_least32_t place_texture_,
			const double zoom_
		) const {
			std::vector<LocationPoint> location_point_list{}; // 地物の一覧

			paxs::InputFile pifs(str_, AppConfig::getInstance()->getRootPath());
			if (pifs.fail()) return LocationPointList{};
			// 1 行目を読み込む
			if (!(pifs.getLine())) {
				return LocationPointList{}; // 何もない場合
			}
			// BOM を削除
			pifs.deleteBOM();
			// 1 行目を分割する
			paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu = pifs.splitHashMapMurMur3('\t');

			const std::size_t longitude = getMenuIndex(menu, MurMur3::calcHash("longitude"));
			if (longitude == SIZE_MAX) return LocationPointList{}; // 経度がないのはデータにならない
			const std::size_t latitude = getMenuIndex(menu, MurMur3::calcHash("latitude"));
			if (latitude == SIZE_MAX) return LocationPointList{}; // 緯度がないのはデータにならない

			double start_longitude = 180.0; // 始点の経度
			double end_longitude = -180.0; // 終点の経度
			double start_latitude = 90.0; // 始点の緯度
			double end_latitude = -90.0; // 終点の緯度

			// 配列の添え字番号
			const std::size_t overall_length = getMenuIndex(menu, MurMur3::calcHash("overall_length"));
			const std::size_t x_size = getMenuIndex(menu, MurMur3::calcHash("x_size"));
			const std::size_t y_size = getMenuIndex(menu, MurMur3::calcHash("y_size"));
			const std::size_t minimum_size = getMenuIndex(menu, MurMur3::calcHash("min_size"));
			const std::size_t maximum_size = getMenuIndex(menu, MurMur3::calcHash("max_size"));
			const std::size_t first_julian_day = getMenuIndex(menu, MurMur3::calcHash("first_julian_day"));
			const std::size_t last_julian_day = getMenuIndex(menu, MurMur3::calcHash("last_julian_day"));
			const std::size_t place_texture = getMenuIndex(menu, MurMur3::calcHash("texture"));

			const std::size_t first_year = getMenuIndex(menu, MurMur3::calcHash("first_year"));
			const std::size_t last_year = getMenuIndex(menu, MurMur3::calcHash("last_year"));

			// 地名
			const std::size_t language_ja_jp = getMenuIndex(menu, MurMur3::calcHash("ja-JP"));
			const std::size_t language_en_us = getMenuIndex(menu, MurMur3::calcHash("en-US"));

			// 1 行ずつ読み込み（区切りはタブ）
			while (pifs.getLine()) {
				std::vector<std::string> strvec = pifs.split('\t');

				// 経度の文字列が空の場合は読み込まない
				if (longitude >= strvec.size()) continue;
				else if (strvec[longitude].size() == 0) continue;
				// 緯度の文字列が空の場合は読み込まない
				if (latitude >= strvec.size()) continue;
				else if (strvec[latitude].size() == 0) continue;

				// 地名
				paxs::UnorderedMap<std::uint_least32_t, std::string> place_name{};
				if (language_ja_jp < strvec.size() && strvec[language_ja_jp].size() != 0) {
					place_name.emplace(MurMur3::calcHash("ja-JP"), strvec[language_ja_jp]);
				}
				if (language_en_us < strvec.size() && strvec[language_en_us].size() != 0) {
					place_name.emplace(MurMur3::calcHash("en-US"), strvec[language_en_us]);
				}
				bool is_overall_length = false;
				if (overall_length < strvec.size()) {
					if (strvec[overall_length].size() != 0) is_overall_length = true;
				}
				bool is_x_size = false;
				if (x_size < strvec.size()) {
					if (strvec[x_size].size() != 0) is_x_size = true;
				}
				bool is_y_size = false;
				if (y_size < strvec.size()) {
					if (strvec[y_size].size() != 0) is_y_size = true;
				}

				double point_longitude = std::stod(strvec[longitude]); // 経度
				double point_latitude = std::stod(strvec[latitude]); // 緯度
				// 経緯度の範囲を求める
				start_longitude = (std::min)(start_longitude, point_longitude);
				start_latitude = (std::min)(start_latitude, point_latitude);
				end_longitude = (std::max)(end_longitude, point_longitude);
				end_latitude = (std::max)(end_latitude, point_latitude);

				auto first_jd = int((first_julian_day >= strvec.size()) ? min_year_ : (strvec[first_julian_day].size() == 0) ? min_year_ : std::stod(strvec[first_julian_day]));
				if (first_jd == min_year_) first_jd = int((first_year >= strvec.size()) ? min_year_ : (strvec[first_year].size() == 0) ? min_year_ : std::stod(strvec[first_year]) * days_in_a_year + julian_day_on_m1_1_1);

				auto last_jd = int((last_julian_day >= strvec.size()) ? max_year_ : (strvec[last_julian_day].size() == 0) ? max_year_ : std::stod(strvec[last_julian_day]));
				if (last_jd == max_year_) last_jd = int((last_year >= strvec.size()) ? max_year_ : (strvec[last_year].size() == 0) ? max_year_ : std::stod(strvec[last_year]) * days_in_a_year + julian_day_on_m1_1_1);

				// 格納
				location_point_list.emplace_back(
					place_name,
					paxs::EquirectangularDeg(
						paxs::Vector2<double>(
							point_longitude, // 経度
							point_latitude)).toMercatorDeg(), // 緯度
					(!is_x_size) ? 1 : (strvec[x_size].size() == 0) ? 1 : static_cast<std::uint_least16_t>(std::stod(strvec[x_size])), // 横の枚数
					(!is_y_size) ? 1 : (strvec[y_size].size() == 0) ? 1 : static_cast<std::uint_least16_t>(std::stod(strvec[y_size])), // 縦の枚数
					(!is_overall_length) ? 10.0 : (strvec[overall_length].size() == 0) ? 10.0 : std::stod(strvec[overall_length]), // 全長
					(minimum_size >= strvec.size()) ? min_view_ : (strvec[minimum_size].size() == 0) ? min_view_ : std::stod(strvec[minimum_size]), // 最小サイズ
					(maximum_size >= strvec.size()) ? max_view_ : (strvec[maximum_size].size() == 0) ? max_view_ : std::stod(strvec[maximum_size]), // 最大サイズ
					first_jd, // 最小時代
					last_jd, // 最大時代
					lpe_,
					(place_texture >= strvec.size()) ? place_texture_ : MurMur3::calcHash(strvec[place_texture].size(), strvec[place_texture].c_str()), // テクスチャの Key
					zoom_
				);
			}
			// 地物を何も読み込んでいない場合は空のLocationPointListを返す
			if (location_point_list.size() == 0) return LocationPointList{};

			// LocationPointListを構築して返す
			return LocationPointList(location_point_list,
				paxs::EquirectangularDeg(
					paxs::Vector2<double>(start_longitude/* 経度 */, start_latitude/* 緯度 */)).toMercatorDeg(),
				paxs::EquirectangularDeg(
					paxs::Vector2<double>(end_longitude/* 経度 */, end_latitude/* 緯度 */)).toMercatorDeg(),
				min_view_, max_view_, min_year_, max_year_
				, lpe_, place_texture_);
		}

	private:
		// 項目の ID を返す
		static std::size_t getMenuIndex(const paxs::UnorderedMap<std::uint_least32_t, std::size_t>& menu, const std::uint_least32_t& str_) {
			return  (menu.find(str_) != menu.end()) ? menu.at(str_) : SIZE_MAX;
		}
	};

}

#endif // !PAX_SAPIENTICA_GEOGRAPHIC_INFORMATION_PLACE_NAME_REPOSITORY_HPP
