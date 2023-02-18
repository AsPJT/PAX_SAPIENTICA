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

#include <PAX_SAPIENTICA/Siv3D/Init.hpp>

namespace paxs {

	void startMain() {
		s3d::Window::Resize(1280, 720);
		paxs::initLogo();

		enum class KoyomiEnum {
			koyomi_japan,
			koyomi_g,
			koyomi_j
		};

		std::array<OutputDate, 3> date_list = {
			OutputDate{U"",U"Japanese cal.",paxs::Date(),false},
			OutputDate{U"グレゴリオ暦",U"Gregorian cal.",paxs::Date(),false},
			OutputDate{U"ユリウス暦",U"Julian cal.",paxs::Date(),false}
		};

		// マップ関連変数
		//double map_view_center_x = 135.0; // マップ座標の中央 X
		//double map_view_center_y = getLatitudeToMercatorY(35.0); // マップ座標の中央 Y
		//double map_view_width = 20.0; // マップの幅
		//double map_view_max_width = 160.0; // マップの最大幅
		//double map_view_min_width = 0.01; // マップの最小幅
		//double map_view_height =
		//	(map_view_width) / double(s3d::Scene::Width()) * double(s3d::Scene::Height()); // マップの高さ
		//double map_view_movement_size = 200.0; // マップの移動量
		//double map_view_expansion_size = 200.0; // マップの拡大量
		std::unique_ptr<MapView> map_view(new(std::nothrow) MapView);

		// 9 = 3.2
		// 19 は無い
		//std::vector<MapVec2D> xyz_tile_pos_list;
		//std::vector<s3d::Texture> xyz_tile_texture_list;

		//const s3d::String map_name = U"openstreetmap";
		//const s3d::String map_url_name = U"http://tiles.wmflabs.org/bw-mapnik";
		//const s3d::String map_url_name = U"http://tile.openstreetmap.org";
		//const s3d::String map_url_name = U"http://b.tile.openstreetmap.org";
		//const s3d::String map_name = U"hot";
		//const s3d::String map_url_name = U"http://a.tile.openstreetmap.fr/hot";
		//const s3d::String map_license_name = U"© OpenStreetMap contributors CC BY-SA 2.0";

		//const s3d::String map_name = U"gsi_en";
		//const s3d::String map_url_name = U"https://cyberjapandata.gsi.go.jp/xyz/english";
		//const s3d::String map_name = U"gsi";
		//const s3d::String map_url_name = U"https://cyberjapandata.gsi.go.jp/xyz/std";
		//const s3d::String map_license_name = U"国土地理院（https://maps.gsi.go.jp/development/ichiran.html）";

		//const s3d::String map_name = U"mierune";
		//const s3d::String map_url_name = U"https://tile.mierune.co.jp/mierune";
		const s3d::String map_license_name = U"Maptiles by MIERUNE, under CC BY. Data by OpenStreetMap contributors, under ODbL.";

		// タイルの Z
		std::unique_ptr<XYZTile>  xyz_tile(new(std::nothrow) XYZTile(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()));
		//int xyz_tile_z = 2;
		//int xyz_tile_z_num = int(std::pow(2, xyz_tile_z));

		//MapVec2 xyz_tile_start_cell = { int((((map_view->getCenterX() - map_view->getWidth() / 2) + 180.0) / 360.0) * xyz_tile_z_num),
		//	int(((360.0 - ((map_view->getCenterY() + map_view->getHeight() / 2) + 180.0)) / 360.0)* xyz_tile_z_num)};

		//MapVec2 xyz_tile_end_cell = { int((((map_view->getCenterX() + map_view->getWidth() / 2) + 180.0) / 360.0) * xyz_tile_z_num),
		//	int(((360.0 - ((map_view->getCenterY() - map_view->getHeight() / 2) + 180.0)) / 360.0)* xyz_tile_z_num)};

		//MapVec2 xyz_tile_cell_num = {
		//	(xyz_tile_end_cell.x - xyz_tile_start_cell.x),
		//	(xyz_tile_end_cell.y - xyz_tile_start_cell.y)
		//};
		//int xyz_tile_cell_all_num = (xyz_tile_cell_num.x + 1) * (xyz_tile_cell_num.y + 1);
		//xyz_tile_pos_list.resize(xyz_tile_cell_all_num);
		//xyz_tile_texture_list.resize(xyz_tile_cell_all_num);
		//for (int i = xyz_tile_start_cell.y, k = 0; i <= xyz_tile_end_cell.y; ++i) {
		//	for (int j = xyz_tile_start_cell.x; j <= xyz_tile_end_cell.x; ++j, ++k) {
		//		xyz_tile_pos_list[k] =
		//			//xyz_tile_pos_list[i * xyz_tile_cell_num.x + j] =
		//			MapVec2D{ j * 360.0 / xyz_tile_z_num - 180.0,
		//	(360.0 - i * 360.0 / xyz_tile_z_num) - 180.0 };
		//	}
		//}

		struct LocationRange {
			s3d::Texture texture;
			double msx, msy;
			double mex, mey;
			double mcx, mcy;
			double mx, my;

			LocationRange() = default;
			LocationRange(const s3d::String& str_, const double msx_, const double msy_, const double mex_, const double mey_)
				:texture(str_)
				, msx(msx_), msy(msy_), mex(mex_), mey(mey_)
				, mcx((mex_ - msx_) / 2 + msx_), mcy((mey_ - msy_) / 2 + msy_)
				, mx(mex_ - msx_), my(mey_ - msy_) {}
		};
		std::vector<LocationRange> location_range_list;
		location_range_list.emplace_back(U"./../../../../../Data/Map/Asia/Japan/North Japan.svg",
			128.343721, getLatitudeToMercatorY(30.990751), 148.90, getLatitudeToMercatorY(45.55));
		location_range_list.emplace_back(U"./../../../../../Data/Map/Asia/Japan/South Japan.svg",
			122.93, getLatitudeToMercatorY(20.42), 148.90, getLatitudeToMercatorY(30.990751));
		location_range_list.emplace_back(U"./../../../../../Data/Map/Asia/Japan/Tsushima.svg",
			129.166727, getLatitudeToMercatorY(34.083839), 129.497528, getLatitudeToMercatorY(34.727554));
		location_range_list.emplace_back(U"./../../../../../Data/Map/Asia/Japan/Okinoshima.svg",
			130.098193, getLatitudeToMercatorY(34.239688), 130.113008, getLatitudeToMercatorY(34.249891));
		location_range_list.emplace_back(U"./../../../../../Data/Map/Asia/Japan/Ikinoshima.svg",
			129.624166, getLatitudeToMercatorY(33.704071), 129.799450, getLatitudeToMercatorY(33.872968));
		location_range_list.emplace_back(U"./../../../../../Data/Map/Asia/Japan/Sakurajima.svg",
			130.589410, getLatitudeToMercatorY(31.541827), 130.729626, getLatitudeToMercatorY(31.637892));
		location_range_list.emplace_back(U"./../../../../../Data/Map/Asia/Japan/Kyushu.svg",
			128.343721, getLatitudeToMercatorY(30.990751), 132.101509, getLatitudeToMercatorY(33.969858));

		location_range_list.emplace_back(U"./../../../../../Data/Map/Asia/Karafuto.svg",
			141.208503, getLatitudeToMercatorY(45.893137), 144.746825, getLatitudeToMercatorY(54.425405));
		location_range_list.emplace_back(U"./../../../../../Data/Map/Asia/South Korea.svg",
			124.610143, getLatitudeToMercatorY(33.195652), 130.916883, getLatitudeToMercatorY(38.618049));
		location_range_list.emplace_back(U"./../../../../../Data/Map/Asia/North Korea.svg",
			124.187817, getLatitudeToMercatorY(/*37.6*/37.673457), 130.696288, getLatitudeToMercatorY(43.011539));
		location_range_list.emplace_back(U"./../../../../../Data/Map/Asia/China.svg",
			73.499485, getLatitudeToMercatorY(/*18.1*/18.157676), 134.775403, getLatitudeToMercatorY(/*53.5*/53.560947));
		location_range_list.emplace_back(U"./../../../../../Data/Map/Asia/Taiwan.svg",
			119.410639, getLatitudeToMercatorY(21.896833), 122.007173, getLatitudeToMercatorY(25.299653));
		location_range_list.emplace_back(U"./../../../../../Data/Map/Russia.svg",
			27.347498, getLatitudeToMercatorY(41.287923), 190, getLatitudeToMercatorY(77.709187));
		location_range_list.emplace_back(U"./../../../../../Data/Map/Africa.svg",
			-25.361057, getLatitudeToMercatorY(-34.833239), 54.534059, getLatitudeToMercatorY(37.348313));


		location_range_list.emplace_back(U"./../../../../../Data/Map/Asia/Vietnam.svg",
			102.144036, getLatitudeToMercatorY(8.563714), 109.458846, getLatitudeToMercatorY(23.392634));
		location_range_list.emplace_back(U"./../../../../../Data/Map/Asia/India.svg",
			68.198526, getLatitudeToMercatorY(8.077648), 97.163387, getLatitudeToMercatorY(35.987344));


		location_range_list.emplace_back(U"./../../../../../Data/Map/None.svg",
			30, getLatitudeToMercatorY(41), 129, getLatitudeToMercatorY(62));


		//location_range_list.emplace_back(U"./../../../../../Data/Map/TestMap/YodoRiverAndYamatoRiverBasins.svg",
		//	/*135.291055*/135.45, getLatitudeToMercatorY(/*34.398726*/34.6), 135.853110, getLatitudeToMercatorY(35.010042));

		const s3d::Array<s3d::String> options = {
			U"旧石器時代", U"縄文時代", U"弥生時代", U"古墳時代 CE251-",
			U"飛鳥時代 CE592-", U"奈良時代 CE710-", U"平安時代 CE794-",
			U"鎌倉時代", U"室町時代", U"安土桃山時代", U"江戸時代", U"明治時代" };

		constexpr std::array<int, 12> period_jdn = {
			0,0,1538799,1812736,
			1937666,1980488,2011388,
			2154234,2209376,2295823,2306626,2403629
		};
		size_t index1 = 2;

		// 地名リスト
		std::vector<LocationPoint> location_point_list;

		inputPlace("./../../../../../Data/PlaceName/KojikiPlaceName.tsv", location_point_list);
		inputPlace("./../../../../../Data/PlaceName/PlaceName.tsv", location_point_list);
		inputPlace("./../../../../../Data/PlaceName/WamyoRuijushoPlaceName.tsv", location_point_list);
		inputPlace("./../../../../../Data/PlaceName/OmoroSoshiPlaceName.tsv", location_point_list);
		//inputPlace("./../../../../../Data/PlaceName/TestMap/Kofun.tsv", location_point_list, LocationPointEnum::location_point_zempo_koen_fun);

		// ピン
		//location_point_list.emplace_back("原の辻遺跡", "", 129.753222, getLatitudeToMercatorY(33.759444), 0.0, 2.0, 1721424, 1940208, LocationPointEnum::location_point_pit_dwelling, "");

		s3d::String month_name_long[13] = { U"",U"January",U"February",U"March",U"April",U"May",U"June",
			U"July",U"August",U"September",U"October",U"November",U"December" };
		s3d::String month_name[13] = { U"",U"Jan.",U"Feb.",U"Mar.",U"Apr.",U"May",U"Jun.",
			U"Jul.",U"Aug.",U"Sep.",U"Oct.",U"Nov.",U"Dec." };

		s3d::Array<s3d::Vec2> route1;
		s3d::Array<s3d::Vec2> route2;

		// 航路を読み込み
		std::ifstream rifs("./../../../../../Data/Route/Yamatai.tsv");
		if (rifs.fail()) return;
		std::string rline;
		while (std::getline(rifs, rline)) {
			std::vector<std::string> strvec = paxs::split(rline, '\t');

			route1.emplace_back(std::stod(strvec[0]), getLatitudeToMercatorY(std::stod(strvec[1])));
		}
		for (int i = 0; i < route1.size(); ++i) {
			route2.emplace_back(
				(route1[i].x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
				double(s3d::Scene::Height()) - ((route1[i].y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height()))
			);
		}
		// 暦を読み込み
		std::vector<paxs::JapaneseEra> japanese_era_list;
		paxs::inputJapaneseEra(japanese_era_list, "./../../../../../Data/Calendar/JapaneseEraName.tsv");

		constexpr int koyomi_font_size = 20;
		const int koyomi_font_x = s3d::Scene::Width() - 240;//220;
		constexpr int koyomi_font_y = 20;
		const int koyomi_font_en_x = s3d::Scene::Width() - 240;//820;
		constexpr int koyomi_font_en_y = 220;

		// 通常のフォントを作成 | Create a new font
		const s3d::Font koyomi_font{ s3d::FontMethod::SDF, koyomi_font_size /*, Typeface::Bold*/
			, PAXS_S3D_RESOURCE(U"engine/font/noto-sans-jp/NotoSansJP-Regular.otf")
		};
		koyomi_font.setBufferThickness(3);

		const s3d::Font pin_font{ s3d::FontMethod::SDF, 20 /*, Typeface::Bold*/
			, PAXS_S3D_RESOURCE(U"engine/font/noto-sans-jp/NotoSansJP-Regular.otf")
		};
		pin_font.setBufferThickness(3);

		const s3d::Font en_font{ s3d::FontMethod::SDF, 18 /*, Typeface::Bold*/
			, PAXS_S3D_RESOURCE(U"engine/font/noto-sans-jp/NotoSansJP-Regular.otf")
		};
		en_font.setBufferThickness(3);

		const s3d::Font font{ s3d::FontMethod::SDF, 26 /*, Typeface::Bold*/
			, PAXS_S3D_RESOURCE(U"engine/font/noto-sans-jp/NotoSansJP-Regular.otf")
		};
		font.setBufferThickness(3);

		constexpr s3d::Vec2 pos{ 800,500 };

		// 画像ファイルからテクスチャを作成 | Create a texture from an image file
		const s3d::Texture texture{ U"./../../../../../Image/Logo.svg" };
		const s3d::Texture texture_ko{ U"./../../../../../Data/OldDocumentIcon/JP-Kojiki.svg" };
		const s3d::Texture texture_wam{ U"./../../../../../Data/OldDocumentIcon/JP-WamyoRuijusho.svg" };
		const s3d::Texture texture_pin1{ U"./../../../../../Data/Pin/PitDwelling.svg" };
		const s3d::Texture texture_kofun1{ U"./../../../../../Data/MiniIcon/ZempoKoenFun.svg" };
		const s3d::Texture texture_kofun2{ U"./../../../../../Data/MiniIcon/ZempoKohoFun.svg" };
		const s3d::Texture texture_kofun3{ U"./../../../../../Data/MiniIcon/HotategaiGataKofun.svg" };
		const s3d::Texture texture_pn{ U"./../../../../../Data/MiniIcon/PlaceName.svg" };

		//int jdn = 1600407;
		//int jdn = 1808020;
		int jdn = 1808286;
		s3d::String str;
		int count = 0;

		const s3d::ScopedRenderStates2D sampler{ s3d::SamplerState::ClampNearest };

		//Scene::SetBackground(Color{ int(184 * 0.7), int(212 * 0.9), int(238) });
		s3d::Scene::SetBackground(s3d::Color{ 184, 212, 238 });

		/*##########################################################################################
			ループ開始
		##########################################################################################*/

		while (s3d::System::Update()) {

			// キーボード入力を更新
			map_view->update();
			//paxs::updateKey(
			//	map_view_center_x, map_view_center_y,
			//	map_view_width, map_view_max_width, map_view_min_width,
			//	map_view_height, map_view_movement_size, map_view_expansion_size);

			// タイルを更新
			//paxs::updateXYZTiles(
			//	xyz_tile_z,
			//	xyz_tile_z_num,
			//	map_view->getWidth(),
			//	map_view->getHeight(),
			//	map_view->getCenterX(),
			//	map_view->getCenterY(),
			//	xyz_tile_start_cell,
			//	xyz_tile_end_cell,
			//	xyz_tile_cell_num,
			//	xyz_tile_cell_all_num,
			//	xyz_tile_pos_list,
			//	xyz_tile_texture_list,
			//	map_url_name,
			//	map_name
			//);

			// テクスチャを描く | Draw a texture
			//texture.resized(150).drawAt(Scene::Center());
			//texture2.resized(750).drawAt(Scene::Center());
			for (int i = 0; i < location_range_list.size(); ++i) {
				auto& jj = location_range_list[i];
				if (jj.mex > map_view->getCenterX() - map_view->getWidth() / 2 &&
					jj.msx < map_view->getCenterX() + map_view->getWidth() / 2 &&
					jj.mey > map_view->getCenterX() - map_view->getHeight() / 2 &&
					jj.msy < map_view->getCenterY() + map_view->getHeight() / 2) {
					jj.texture.resized(
						jj.mx / map_view->getWidth() * double(s3d::Scene::Width())
						, jj.my / map_view->getHeight() * double(s3d::Scene::Height())// * 1.3
					).drawAt(
						(jj.mcx - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()) - 0,
						double(s3d::Scene::Height()) - ((jj.mcy - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height()))// + 270
					);
				}
			}

			// 地図の描画
			{
				MapVec2 xyz_tile_start_cell = xyz_tile->getStartCell();
				MapVec2 xyz_tile_end_cell = xyz_tile->getEndCell();
				int xyz_tile_z_num = xyz_tile->getZNum();
				
				for (int i = xyz_tile_start_cell.y, k = 0; i <= xyz_tile_end_cell.y; ++i) {
					for (int j = xyz_tile_start_cell.x; j <= xyz_tile_end_cell.x; ++j, ++k) {
						if (xyz_tile->xyz_tile_texture_list[k]) {
							xyz_tile->xyz_tile_texture_list[k].resized(
								(360.0 / xyz_tile_z_num) / map_view->getWidth() * double(s3d::Scene::Width())
								, (360.0 / xyz_tile_z_num) / map_view->getHeight() * double(s3d::Scene::Height())
							).draw(
								(xyz_tile->xyz_tile_pos_list[k].x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
								double(s3d::Scene::Height()) - ((xyz_tile->xyz_tile_pos_list[k].y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height()))
							);
						}
					}
				}
			}



			for (int i = 0; i < route2.size(); ++i) {
				route2[i] = s3d::Vec2(
					(route1[i].x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
					double(s3d::Scene::Height()) - ((route1[i].y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height()))
				);
			}
			// 航路を描画
			s3d::Spline2D{ route2 }.draw(2, s3d::Color{ 85,145,245 });


			for (int i = 0; i < location_point_list.size(); ++i) {
				auto& lli = location_point_list[i];
				// 範囲外を除去
				if (lli.x < (map_view->getCenterX() - map_view->getWidth() / 1.8)
					|| lli.x >(map_view->getCenterX() + map_view->getWidth() / 1.8)
					|| lli.y < (map_view->getCenterY() - map_view->getHeight() / 1.8)
					|| lli.y >(map_view->getCenterY() + map_view->getHeight() / 1.8)) continue;

				if (lli.min_view > map_view->getWidth()
					|| lli.max_view < map_view->getWidth()
					|| lli.min_year > jdn
					|| lli.max_year < jdn) {

					if (lli.lpe == LocationPointEnum::location_point_place_name) {
						texture_pn.resized(14).drawAt(
							s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
						double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) });
						continue;
					}
					if (lli.lpe == LocationPointEnum::location_point_zempo_koen_fun) {
						texture_kofun1.resized(14).drawAt(
							s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
						double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) });
						continue;
					}
					if (lli.lpe == LocationPointEnum::location_point_zempo_koho_fun) {
						texture_kofun2.resized(14).drawAt(
							s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
						double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) });
						continue;
					}
					if (lli.lpe == LocationPointEnum::location_point_hotategai_gata_kofun) {
						texture_kofun3.resized(14).drawAt(
							s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
						double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) });
						continue;
					}
				}
			}

			for (int i = 0; i < location_point_list.size(); ++i) {
				auto& lli = location_point_list[i];

				// 範囲外を除去
				if (lli.x < (map_view->getCenterX() - map_view->getWidth() / 1.8)
					|| lli.x >(map_view->getCenterX() + map_view->getWidth() / 1.8)
					|| lli.y < (map_view->getCenterY() - map_view->getHeight() / 1.8)
					|| lli.y >(map_view->getCenterY() + map_view->getHeight() / 1.8)) continue;

				if (lli.min_view > map_view->getWidth()) continue;
				if (lli.max_view < map_view->getWidth()) continue;
				if (lli.min_year > jdn) continue;
				if (lli.max_year < jdn) continue;

				//if (lli.lpe == LocationPointEnum::location_point_place_name) {
				if (lli.lpe != LocationPointEnum::location_point_pit_dwelling) {

					if (lli.en_name.size() == 0) {
						//
						font(s3d::Unicode::FromUTF8(lli.name)).drawAt(
							s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White),
							(lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
							double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height()))// - 30
							, s3d::Palette::Black);
					}
					else {
						en_font(s3d::Unicode::FromUTF8(lli.en_name)).draw(
							s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White),
							s3d::Arg::bottomCenter = s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
							double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height()))// - 30
							}
						, s3d::Palette::Black);
						font(s3d::Unicode::FromUTF8(lli.name)).draw(
							s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White),
							s3d::Arg::topCenter = s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
							double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height()))// - 30
							}
						, s3d::Palette::Black);
					}
					if (lli.source == "JP-Kojiki") {
						texture_ko.resized(35).drawAt(s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
							double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) + 50
							});
					}
					else if (lli.source == "JP-WamyoRuijusho") {
						texture_wam.resized(35).drawAt(s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
							double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) + 50
							});
					}
					else if (lli.source == "ZempoKoenFun") {
						texture_kofun1.resized(35).drawAt(s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
							double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) + 50
							});
					}
					else if (lli.source == "ZempoKohoFun") {
						texture_kofun2.resized(35).drawAt(s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
							double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) + 50
							});
					}
					else if (lli.source == "HotategaiGataKofun") {
						texture_kofun3.resized(35).drawAt(s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
							double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) + 50
							});
					}
				}
				else {
					pin_font(s3d::Unicode::FromUTF8(lli.name)).drawAt(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White),
						(lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
						double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) - 70
						, s3d::Palette::Black);
					texture_pin1.resized(50).draw(
						s3d::Arg::bottomCenter = s3d::Vec2((lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
							double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())))
					);
				}
				//Circle{{(lli.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(Scene::Width()),
				//double(Scene::Height()) - ((lli.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(Scene::Height()))},5 }.draw(Palette::Black);

			}


			//Rect{ 0,0,50 }.drawFrame(2, ColorF{ 1, 0, 0, 0.5 });
			//Circle{ Cursor::Pos(), 40 }.drawFrame(2, ColorF{ 1, 0, 0, 0.5 });

			//Circle{ Scene::Center(), 10 }.draw(Palette::Red); // 真ん中の赤い点


			paxs::Date Date;
			Date = paxs::getGregorianCalendarFromJulianDayNumber(jdn);
			date_list[std::size_t(KoyomiEnum::koyomi_g)].date = Date;

			Date = paxs::getJulianCalendarFromJulianDayNumber(jdn);
			date_list[std::size_t(KoyomiEnum::koyomi_j)].date = Date;

			if (jdn >= 1480407) {
				bool is_break = false;
				for (int i = 0; i < japanese_era_list.size(); ++i) {
					auto& jeli = japanese_era_list[i];
					if (jdn >= jeli.start_jdn
						&& jdn < japanese_era_list[i + 1].start_jdn) {
						is_break = true;
						if (jeli.kaigen_jdn[0] != 0 &&
							jdn >= jeli.kaigen_jdn[0]) {
							date_list[std::size_t(KoyomiEnum::koyomi_japan)].calendar_name = s3d::Unicode::FromUTF8(jeli.gengo[1]);
							date_list[std::size_t(KoyomiEnum::koyomi_japan)].date.setYear(jeli.gengo_num[1]);
						}
						else {
							date_list[std::size_t(KoyomiEnum::koyomi_japan)].calendar_name = s3d::Unicode::FromUTF8(jeli.gengo[0]);
							date_list[std::size_t(KoyomiEnum::koyomi_japan)].date.setYear(jeli.gengo_num[0]);
						}
					}
					if (is_break) {
						int lm = ((jeli.leap_month == 0) ? 999 : jeli.leap_month - 1);

						int calc_day = jdn - jeli.start_jdn; // １月１日
						for (int j = 0; j < 12; ++j) {
							if (calc_day < jeli.number_of_days[j]) {
								date_list[std::size_t(KoyomiEnum::koyomi_japan)].date.setMonth(j + 1);
								date_list[std::size_t(KoyomiEnum::koyomi_japan)].date.setDay(calc_day + 1);
								date_list[std::size_t(KoyomiEnum::koyomi_japan)].is_leap_month = false;
								break;
							}
							calc_day -= jeli.number_of_days[j];
							if (j == lm) {
								if (calc_day < jeli.number_of_days_of_leap_month) {
									date_list[std::size_t(KoyomiEnum::koyomi_japan)].date.setMonth(j + 1);
									date_list[std::size_t(KoyomiEnum::koyomi_japan)].date.setDay(calc_day + 1);
									date_list[std::size_t(KoyomiEnum::koyomi_japan)].is_leap_month = true;
									break;
								}
								calc_day -= jeli.number_of_days_of_leap_month;
							}
						}
					}
					if (is_break) break;
				}
			}

			++count;
			//jdn += 400;
			//if (count >= 0) {
			if (count >= 30) {
				count = 0;
				++jdn;
			}

			s3d::Rect{ s3d::Scene::Width() - 400,0,400,s3d::Scene::Height() }.draw(s3d::Palette::White);
			//Rect{ 0,0,Scene::Width(),150}.draw(Palette::White);

			// 暦の表示（日本語）
			for (int i = 0; i < date_list.size(); ++i) {
				koyomi_font(date_list[i].calendar_name).draw(s3d::Arg::topRight = s3d::Vec2(koyomi_font_x, koyomi_font_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
				koyomi_font(U"年").draw(s3d::Arg::topRight = s3d::Vec2(int(120 * koyomi_font_size / 30.0) + koyomi_font_x, koyomi_font_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
				koyomi_font(U"月").draw(s3d::Arg::topRight = s3d::Vec2(int(220 * koyomi_font_size / 30.0) + koyomi_font_x, koyomi_font_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
				koyomi_font(U"日").draw(s3d::Arg::topRight = s3d::Vec2(int(300 * koyomi_font_size / 30.0) + koyomi_font_x, koyomi_font_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);

				koyomi_font(s3d::ToString(date_list[i].date.year)).draw(s3d::Arg::topRight = s3d::Vec2(int(85 * koyomi_font_size / 30.0) + koyomi_font_x, koyomi_font_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
				koyomi_font(s3d::ToString(date_list[i].date.month)).draw(s3d::Arg::topRight = s3d::Vec2(int(190 * koyomi_font_size / 30.0) + koyomi_font_x, koyomi_font_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
				koyomi_font(s3d::ToString(date_list[i].date.day)).draw(s3d::Arg::topRight = s3d::Vec2(int(270 * koyomi_font_size / 30.0) + koyomi_font_x, koyomi_font_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
				if (!date_list[i].is_leap_month) continue;
				koyomi_font(U"閏").draw(s3d::Arg::topRight = s3d::Vec2((
					(date_list[i].date.month < 10) ? int(167 * koyomi_font_size / 30.0) + koyomi_font_x : int(152 * koyomi_font_size / 30.0) + koyomi_font_x
					), koyomi_font_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
			}
			// 暦の表示（英語）
			for (int i = 0; i < date_list.size(); ++i) {
				koyomi_font(date_list[i].calendar_en_name).draw(s3d::Arg::topRight = s3d::Vec2(koyomi_font_en_x, koyomi_font_en_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
				koyomi_font(U",").draw(s3d::Arg::topRight = s3d::Vec2(int(95 * koyomi_font_size / 30.0) + koyomi_font_en_x, koyomi_font_en_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
				koyomi_font(U",").draw(s3d::Arg::topRight = s3d::Vec2(int(235 * koyomi_font_size / 30.0) + koyomi_font_en_x, koyomi_font_en_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
				koyomi_font(U"th").draw(s3d::Arg::topRight = s3d::Vec2(int(315 * koyomi_font_size / 30.0) + koyomi_font_en_x, koyomi_font_en_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);

				koyomi_font(s3d::ToString(date_list[i].date.year)).draw(s3d::Arg::topRight = s3d::Vec2(int(85 * koyomi_font_size / 30.0) + koyomi_font_en_x, koyomi_font_en_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
				koyomi_font(month_name[date_list[i].date.month]).draw(s3d::Arg::topRight = s3d::Vec2(int(220 * koyomi_font_size / 30.0) + koyomi_font_en_x, koyomi_font_en_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
				koyomi_font(s3d::ToString(date_list[i].date.day)).draw(s3d::Arg::topRight = s3d::Vec2(int(280 * koyomi_font_size / 30.0) + koyomi_font_en_x, koyomi_font_en_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
				if (!date_list[i].is_leap_month) continue;
				koyomi_font(U"int.").draw(s3d::Arg::topRight = s3d::Vec2((
					int(152 * koyomi_font_size / 30.0) + koyomi_font_en_x
					), koyomi_font_en_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
			}
			// 時代区分を選択するラジオボタン
			if (s3d::SimpleGUI::RadioButtons(index1, options, s3d::Vec2{ s3d::Scene::Width() - 400, 400 })) {
				jdn = period_jdn[index1];
			}

			font(s3d::String{ U"拡大率" } + s3d::ToString(map_view->getWidth())).draw(s3d::Arg::topRight = s3d::Vec2(s3d::Scene::Width() - 10, 600), s3d::Palette::Black);
			font(s3d::String{ U"メルカトル座標" } + s3d::ToString(map_view->getCenterX()) + s3d::String{ U":" } + s3d::ToString(map_view->getCenterY())).draw(s3d::Arg::topRight = s3d::Vec2(s3d::Scene::Width() - 10, 300), s3d::Palette::Black);
			font(s3d::String{ U"タイル" } + s3d::ToString(xyz_tile->getZ()) + s3d::String{ U":" } + s3d::ToString(xyz_tile->getZNum())).draw(s3d::Arg::topRight = s3d::Vec2(s3d::Scene::Width() - 10, 500), s3d::Palette::Black);
			//font(s3d::String{ U"A" } + s3d::ToString(xyz_tile_cell.x) + s3d::String{ U":" } + s3d::ToString(xyz_tile_cell.y)).draw(s3d::Arg::topRight = s3d::Vec2(s3d::Scene::Width() - 10, 400), s3d::Palette::Black);
			//font(s3d::String{ U"B" } + s3d::ToString(xyz_tile_pos.x) + s3d::String{ U":" } + s3d::ToString(xyz_tile_pos.y)).draw(s3d::Arg::topRight = s3d::Vec2(s3d::Scene::Width() - 10, 450), s3d::Palette::Black);
			pin_font(map_license_name).draw(
				s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White),
				s3d::Arg::topLeft = s3d::Vec2(10, 10),
				s3d::Palette::Black);
			//pin_font(s3d::String{ U"国土地理院（https://maps.gsi.go.jp/development/ichiran.html）" }).draw(s3d::Arg::topLeft = s3d::Vec2(10, 10), s3d::Palette::Black);

			//if (SimpleGUI::Button(U"GitHub", Vec2{ 20, 20 })) {
			//	// Web ページをブラウザで開く
			//	System::LaunchBrowser(U"https://github.com/AsPJT/PAX_SAPIENTICA");
			//}
		}

	}
}

#endif // !PAX_SAPIENTICA_SIV3D_MAIN_HPP