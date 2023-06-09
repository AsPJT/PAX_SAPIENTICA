/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_MAIN_HPP
#define PAX_SAPIENTICA_SIV3D_MAIN_HPP

/*##########################################################################################

##########################################################################################*/


enum MenuBarType {
	file,edit,view,calendar,map
};
enum MapType {
	base,
	landand_sea,land_and_water,
	soil,soil_temperature,
	ryosei_country,ryosei_line,
	slope,
	river1,river2,line1,line2
};

#include <PAX_SAPIENTICA/Siv3D/Init.hpp>
#include <PAX_SAPIENTICA/Siv3D/InitLogo.hpp>
#include <PAX_SAPIENTICA/Siv3D/LocatipnRange.hpp>
#include <PAX_SAPIENTICA/Siv3D/LocationPoint.hpp>
#include <PAX_SAPIENTICA/Siv3D/3DModel.hpp>
#include <PAX_SAPIENTICA/Siv3D/Pulldown.hpp>
#include <PAX_SAPIENTICA/Siv3D/Language.hpp>

#include <PAX_SAPIENTICA/Simulation/Simulator.hpp>
#include <PAX_SAPIENTICA/Siv3D/XYZTiles.hpp>
#include <PAX_SAPIENTICA/Siv3D/XYZTilesList.hpp>

// シミュレータを使用する
//#define PAXS_USING_SIMULATOR
#include <PAX_SAPIENTICA/Siv3D/Calendar.hpp>

namespace paxs {

	void startMain() {
		// 初期化とロゴの表示
		paxs::PaxSapienticaInitSiv3D init{};
		init.firstInit();

		// フォルダ階層
		const s3d::String path = U"./../../../../../";
		const std::string path8 = "./../../../../../";

		paxs::Language language_text(path8 + "Data/Language/Text.txt");

		// マップ関連変数
		const std::unique_ptr<MapView> map_view(new(std::nothrow) MapView);

//#ifdef PAXS_USING_SIMULATOR
		paxs::Simulator<int> simlator;
		// 対馬のみ
		//paxs::Vector2<int> start_position = paxs::Vector2<int>{ 879, 406 };
		//paxs::Vector2<int> end_position = paxs::Vector2<int>{ 881, 409 };
		// 本州
		paxs::Vector2<int> start_position = paxs::Vector2<int>{ 877, 381 };
		paxs::Vector2<int> end_position = paxs::Vector2<int>{ 917, 422 };
		//simlator.init();
//#endif
		int old_width = s3d::Scene::Width();
		int old_height = s3d::Scene::Height();

		int size_change_count = 0;
		paxs::KoyomiSiv3D koyomi_siv{};
		koyomi_siv.init(language_text, path, path8, map_view);
/*##########################################################################################

	ループ開始

##########################################################################################*/

		while (s3d::System::Update()) {

			// 画像の拡大縮小の方式を設定
			const s3d::ScopedRenderStates2D sampler{ s3d::SamplerState::ClampNearest };
			/*##########################################################################################
				更新処理関連
			##########################################################################################*/

			// 画面サイズの変更に合わせて地図の幅を変える
			if (old_width != s3d::Scene::Width()) {
				map_view->setWidth(s3d::Scene::Width() * map_view->getWidth() / old_width);
				map_view->setHeight(map_view->getWidth() / double(s3d::Scene::Width()) * double(s3d::Scene::Height()));
			}
			if (old_height != s3d::Scene::Height()) {
				map_view->setHeight(map_view->getWidth() / double(s3d::Scene::Width()) * double(s3d::Scene::Height()));
			}
			if (old_width != s3d::Scene::Width() ||
				old_height != s3d::Scene::Height()) {
				// 影
				if (size_change_count < 1) {
					koyomi_siv.shadow_texture = s3d::RenderTexture{ s3d::Scene::Size(), s3d::ColorF{ 1.0, 0.0 } };
					koyomi_siv.internal_texture = s3d::RenderTexture{ koyomi_siv.shadow_texture.size() };
				}
				if (size_change_count >= 100) size_change_count = 100;
				++size_change_count;
			}
			else size_change_count = 0;

			old_width = s3d::Scene::Width();
			old_height = s3d::Scene::Height();

			koyomi_siv.koyomi_font_x = s3d::Scene::Width() - 220;
			koyomi_siv.koyomi_font_en_x = s3d::Scene::Width() - 220;

			// キーボード入力を更新
			map_view->update();

			const double map_view_width = map_view->getWidth();
			const double map_view_height = map_view->getHeight();
			const double map_view_center_x = map_view->getCenterX();
			const double map_view_center_y = map_view->getCenterY();
			const double map_view_center_lat = std::asin(std::tanh(map_view_center_y / 180.0 * paxs::pi)) / (paxs::pi) * 180.0;

			// プルダウンを更新
			koyomi_siv.pulldown.setPos(s3d::Point{ s3d::Scene::Width() - koyomi_siv.pulldown.getRect().w, 0 });
			koyomi_siv.pulldown.update(0);
			const std::size_t language = koyomi_siv.pulldown.getIndex();
			koyomi_siv.menu_bar.update(language);

			mapMapUpdate(koyomi_siv.xyz_tile_list, koyomi_siv.menu_bar, map_view.get());

			if (koyomi_siv.menu_bar.getPulldown(MenuBarType::view).getIsItems(1)) {
				// 地図上に画像を描画する
				koyomi_siv.texture_location->update(map_view_center_x, map_view_center_y, map_view_width, map_view_height);

#ifdef PAXS_USING_SIMULATOR
				koyomi_siv.agent_location->draw(simlator.getAgents(), start_position, map_view_width, map_view_height, map_view_center_x, map_view_center_y
					//,font[language], font[language]/*en_font*/, pin_font
				);
#endif

				// 線の描画
				for (int i = 0; i < koyomi_siv.route2.size(); ++i) {
					koyomi_siv.route2[i] = s3d::Vec2(
						(koyomi_siv.route1[i].x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
						double(s3d::Scene::Height()) - ((koyomi_siv.route1[i].y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height()))
					);
				}
				// 航路を描画
				s3d::Spline2D{ koyomi_siv.route2 }.draw(2, s3d::Color{ 85,145,245 });

				// 地名を描画
				koyomi_siv.place_name_location->draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y,
					koyomi_siv.font[language], koyomi_siv.font[language]/*en_font*/, koyomi_siv.pin_font);

				//#ifdef PAXS_USING_SIMULATOR
				//			// エージェント機能テスト
				//			if (is_agent_update) {
				//				static int step = 0;
				//				if (step == 0) {
				//					//s3d::Print(U"Agent Size:", simlator.getAgents().size());
				//					//for (int i = 0; i < 10 && i < simlator.getAgents().size(); ++i) {
				//					//	s3d::Print(U"Agent[",i,U"]:X" ,simlator.getAgents()[i].getLocation(10,256).x, U", Y", simlator.getAgents()[i].getLocation(10,256).y);
				//					//}
				//				}
				//				++step;
				//				if (step >= 0) {
				//					//if (step >= 30) {
				//					step = 0;
				//					simlator.step();
				//				}
				//			}
				//			//agent_location->update(simlator.getAgents());
				//			;				agent_location->draw(simlator.getAgents(), start_position, map_view_width, map_view_height, map_view_center_x, map_view_center_y,
				//				font[language], font[language]/*en_font*/, pin_font);
				//#endif
			}
			koyomi_siv.update(map_view, language, language_text, 
				simlator,
			start_position,
			end_position,
				path8
			);
			init.secondInit();
		}
	}
}

#endif // !PAX_SAPIENTICA_SIV3D_MAIN_HPP