/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_PLACE_NAME_MANAGER_HPP
#define PAX_MAHOROBA_PLACE_NAME_MANAGER_HPP

#include <cstdint>
#include <string>
#include <vector>

#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Texture.hpp>

#include <PAX_MAHOROBA/LocationPoint.hpp>
#include <PAX_MAHOROBA/PlaceNameRenderer.hpp>
#include <PAX_MAHOROBA/PlaceNameRepository.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/GraphicVisualizationList.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>

namespace paxs {

	// GUI に描画する地物の情報を管理するクラス (Application Layer)
	class PlaceNameManager {
	private:
		std::vector<LocationPointList> location_point_list_list{}; // 地物の一覧
		// アイコンのテクスチャ
		paxs::KeyValueTSV<paxg::Texture> key_value_tsv;
		// 描画処理を担当
		PlaceNameRenderer renderer_;
		// データ読み込みを担当
		PlaceNameRepository repository_;
	public:
		PlaceNameManager() = default;

		// 地物を追加
		void add() {
			// リポジトリに委譲してデータを読み込む
			repository_.loadPlaceNameList(
				[this](const std::string& file_path, double min_view, double max_view, int min_year, int max_year,
					   std::uint_least32_t lpe, std::uint_least32_t place_texture, double zoom) {
					inputPlace(file_path, min_view, max_view, min_year, max_year, lpe, place_texture, zoom);
				}
			);
		}

		 void init() {
			 std::string str = "";
			 AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("MiniIcons"),
				 [&](const std::string& path_) {str = path_; });
			 if (str.size() == 0) return;

			 const std::string path = (AppConfig::getInstance()->getRootPath());
			 key_value_tsv.input(str, [&](const std::string& value_) { return paxg::Texture{ path + value_ }; });
		}
		// 描画
		void draw(paxs::GraphicVisualizationList& visible,
			const double jdn,
			const double map_view_width, const double map_view_height, const double map_view_center_x, const double map_view_center_y,
			paxg::Font& font, paxg::Font& en_font, paxg::Font& pin_font) {

			// 描画処理をレンダラーに委譲
			renderer_.draw(
				location_point_list_list,
				key_value_tsv.get(),
				visible,
				jdn,
				map_view_width,
				map_view_height,
				map_view_center_x,
				map_view_center_y,
				font,
				en_font,
				pin_font
			);
		}
	private:

		// 地名を読み込み（リポジトリに委譲）
		void inputPlace(
			const std::string& str_,
			const double min_view_,
			const double max_view_,
			const int min_year_,
			const int max_year_,
			const std::uint_least32_t lpe_,
			const std::uint_least32_t place_texture_,
			const double zoom_
		) {
			// リポジトリからLocationPointListを取得して追加
			LocationPointList loaded = repository_.loadPlaceFromFile(
				str_, min_view_, max_view_, min_year_, max_year_, lpe_, place_texture_, zoom_
			);

			// 空でない場合のみ追加
			if (loaded.location_point_list.size() > 0) {
				location_point_list_list.emplace_back(loaded);
			}
		}
	};
}

#endif // !PAX_MAHOROBA_PLACE_NAME_MANAGER_HPP
