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
#include <PAX_SAPIENTICA/Siv3D/LocatinRange.hpp>

namespace paxs {

	void startMain() {
		// 画面サイズを変更
		s3d::Window::Resize(1280, 720);
		// 初期化とロゴの表示
		paxs::initLogo();

		// 暦の種類
		enum class KoyomiEnum {
			koyomi_japan, // 和暦
			koyomi_g, // グレゴリオ暦
			koyomi_j // ユリウス暦
		};

		// 各暦の日付情報を初期化
		std::array<OutputDate, 3> date_list = {
			OutputDate{U"",U"Japanese cal.",paxs::Date(),false},
			OutputDate{U"グレゴリオ暦",U"Gregorian cal.",paxs::Date(),false},
			OutputDate{U"ユリウス暦",U"Julian cal.",paxs::Date(),false}
		};

		// マップ関連変数
		const std::unique_ptr<MapView> map_view(new(std::nothrow) MapView);


/*##########################################################################################
	読み込む XYZ タイルの情報を記載
	map_name			地図名
	map_url_name		地図画像を取得する URL
	map_license_name	ライセンス情報
##########################################################################################*/

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

		const std::unique_ptr<XYZTile> xyz_tile(new(std::nothrow) XYZTile(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()));

		const s3d::String map_license_name = U"Maptiles by MIERUNE, under CC BY. Data by OpenStreetMap contributors, under ODbL.";



		// 地図上に描画する画像の一覧
		const std::unique_ptr<TextureLocation> texture_location(new(std::nothrow) TextureLocation);
		//location_range_list.emplace_back(U"./../../../../../Data/Map/TestMap/nara.png",
			//135.7104, getLatitudeToMercatorY(34.59451), 135.84725, getLatitudeToMercatorY(34.7072));

		//location_range_list.emplace_back(U"./../../../../../Data/Map/TestMap/YodoRiverAndYamatoRiverBasins.svg",
		//	/*135.291055*/135.45, getLatitudeToMercatorY(/*34.398726*/34.6), 135.853110, getLatitudeToMercatorY(35.010042));

		// 時代区分の文字列
		const s3d::Array<s3d::String> options = {
			U"旧石器時代", U"縄文時代", U"弥生時代", U"古墳時代 CE251-",
			U"飛鳥時代 CE592-", U"奈良時代 CE710-", U"平安時代 CE794-",
			U"鎌倉時代", U"室町時代", U"安土桃山時代", U"江戸時代", U"明治時代" };

		// 時代区分ごとのユリウス日の数値
		constexpr std::array<int, 12> period_jdn = {
			0,0,1538799,1812736,
			1937666,1980488,2011388,
			2154234,2209376,2295823,2306626,2403629
		};

		// 地名リスト
		std::vector<LocationPoint> location_point_list;

		// 古事記の地名
		inputPlace("./../../../../../Data/PlaceName/KojikiPlaceName.tsv", location_point_list);
		// 汎用的な地名
		inputPlace("./../../../../../Data/PlaceName/PlaceName.tsv", location_point_list);
		// 倭名類聚抄の地名
		inputPlace("./../../../../../Data/PlaceName/WamyoRuijushoPlaceName.tsv", location_point_list);
		// おもろさうしの地名
		inputPlace("./../../../../../Data/PlaceName/OmoroSoshiPlaceName.tsv", location_point_list);
		// 古墳名
		//inputPlace("./../../../../../Data/PlaceName/TestMap/Kofun.tsv", location_point_list, LocationPointEnum::location_point_zempo_koen_fun);

		// ピン
		//location_point_list.emplace_back("原の辻遺跡", "", 129.753222, getLatitudeToMercatorY(33.759444), 0.0, 2.0, 1721424, 1940208, LocationPointEnum::location_point_pit_dwelling, "");

		// 月の英語名
		s3d::String month_name_long[13] = { U"",U"January",U"February",U"March",U"April",U"May",U"June",
			U"July",U"August",U"September",U"October",U"November",U"December" };
		s3d::String month_name[13] = { U"",U"Jan.",U"Feb.",U"Mar.",U"Apr.",U"May",U"Jun.",
			U"Jul.",U"Aug.",U"Sep.",U"Oct.",U"Nov.",U"Dec." };

		// 線の情報を格納
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

		// 暦のフォントサイズ
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

		const s3d::Font en_font{ s3d::FontMethod::SDF, 16/*18*/ /*, Typeface::Bold*/
			, PAXS_S3D_RESOURCE(U"engine/font/noto-sans-jp/NotoSansJP-Regular.otf")
		};
		en_font.setBufferThickness(3);

		const s3d::Font font{ s3d::FontMethod::SDF, 20/*26*/ /*, Typeface::Bold*/
			, PAXS_S3D_RESOURCE(U"engine/font/noto-sans-jp/NotoSansJP-Regular.otf")
		};
		font.setBufferThickness(3);


		// 画像ファイルからテクスチャを作成 | Create a texture from an image file
		const s3d::Texture texture{ U"./../../../../../Image/Logo.svg" };
		const s3d::Texture texture_ko{ U"./../../../../../Data/OldDocumentIcon/JP-Kojiki.svg" };
		const s3d::Texture texture_wam{ U"./../../../../../Data/OldDocumentIcon/JP-WamyoRuijusho.svg" };
		const s3d::Texture texture_pin1{ U"./../../../../../Data/Pin/PitDwelling.svg" };
		const s3d::Texture texture_kofun1{ U"./../../../../../Data/MiniIcon/ZempoKoenFun.svg" };
		const s3d::Texture texture_kofun2{ U"./../../../../../Data/MiniIcon/ZempoKohoFun.svg" };
		const s3d::Texture texture_kofun3{ U"./../../../../../Data/MiniIcon/HotategaiGataKofun.svg" };
		const s3d::Texture texture_pn{ U"./../../../../../Data/MiniIcon/PlaceName.svg" };

		// ユリウス日
		//int jdn = 1600407;
		//int jdn = 1808020;
		int jdn = 1808286;

		// 画像の拡大縮小の方式を設定
		const s3d::ScopedRenderStates2D sampler{ s3d::SamplerState::ClampNearest };

		// 背景色を指定
		//Scene::SetBackground(Color{ int(184 * 0.7), int(212 * 0.9), int(238) });
		s3d::Scene::SetBackground(s3d::Color{ 184, 212, 238 });

/*##########################################################################################
	
	ループ開始

##########################################################################################*/

		while (s3d::System::Update()) {

/*##########################################################################################
	更新処理関連
##########################################################################################*/

			// キーボード入力を更新
			map_view->update();

			// タイルを更新
			xyz_tile->update(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY());
			

/*##########################################################################################
	地図描画関連
##########################################################################################*/


			// XYZ タイルの地図の描画
			{
				MapVec2 xyz_tile_start_cell = xyz_tile->getStartCell();
				MapVec2 xyz_tile_end_cell = xyz_tile->getEndCell();
				int xyz_tile_z_num = xyz_tile->getZNum();
				for (int i = xyz_tile_start_cell.y, k = 0; i <= xyz_tile_end_cell.y; ++i) {
					for (int j = xyz_tile_start_cell.x; j <= xyz_tile_end_cell.x; ++j, ++k) {
						if (xyz_tile->texture_list[k]) {
							xyz_tile->texture_list[k].resized(
								(360.0 / xyz_tile_z_num) / map_view->getWidth() * double(s3d::Scene::Width())
								, (360.0 / xyz_tile_z_num) / map_view->getHeight() * double(s3d::Scene::Height())
							).draw(
								(xyz_tile->pos_list[k].x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
								double(s3d::Scene::Height()) - ((xyz_tile->pos_list[k].y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height()))
							);
						}
					}
				}
			}

			// 地図上に描画する画像を描画する
			//texture.resized(150).drawAt(Scene::Center());
			//texture2.resized(750).drawAt(Scene::Center());
			texture_location->update(map_view->getCenterX(), map_view->getCenterY(), map_view->getWidth(), map_view->getHeight());

			// 線の描画
			for (int i = 0; i < route2.size(); ++i) {
				route2[i] = s3d::Vec2(
					(route1[i].x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
					double(s3d::Scene::Height()) - ((route1[i].y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height()))
				);
			}
			// 航路を描画
			s3d::Spline2D{ route2 }.draw(2, s3d::Color{ 85,145,245 });

			// 地名を描画
			for (int i = 0; i < location_point_list.size(); ++i) {
				auto& lli = location_point_list[i];
				// 範囲外を除去
				if (lli.x < (map_view->getCenterX() - map_view->getWidth() / 1.8)
					|| lli.x >(map_view->getCenterX() + map_view->getWidth() / 1.8)
					|| lli.y < (map_view->getCenterY() - map_view->getHeight() / 1.8)
					|| lli.y >(map_view->getCenterY() + map_view->getHeight() / 1.8)) continue;

				// 範囲内の場合
				if (lli.min_view > map_view->getWidth()
					|| lli.max_view < map_view->getWidth()
					|| lli.min_year > jdn
					|| lli.max_year < jdn) {

					// 地名
					if (lli.lpe == LocationPointEnum::location_point_place_name) {
						texture_pn.resized(14).drawAt(
							s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
						double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) });
						continue;
					}
					// 前方後円墳
					if (lli.lpe == LocationPointEnum::location_point_zempo_koen_fun) {
						texture_kofun1.resized(14).drawAt(
							s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
						double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) });
						continue;
					}
					// 前方後方墳
					if (lli.lpe == LocationPointEnum::location_point_zempo_koho_fun) {
						texture_kofun2.resized(14).drawAt(
							s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
						double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) });
						continue;
					}
					// 帆立貝型古墳
					if (lli.lpe == LocationPointEnum::location_point_hotategai_gata_kofun) {
						texture_kofun3.resized(14).drawAt(
							s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
						double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) });
						continue;
					}
				}
			}

			// 地名を描画
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

				// 集落遺跡ではない場合
				//if (lli.lpe == LocationPointEnum::location_point_place_name) {
				if (lli.lpe != LocationPointEnum::location_point_pit_dwelling) {

					if (lli.en_name.size() == 0) {
						// 名前を描画
						font(s3d::Unicode::FromUTF8(lli.name)).drawAt(
							s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White),
							(lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
							double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height()))// - 30
							, s3d::Palette::Black);
					}
					else {
						// 名前（英語）を描画
						en_font(s3d::Unicode::FromUTF8(lli.en_name)).draw(
							s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White),
							s3d::Arg::bottomCenter = s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
							double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height()))// - 30
							}
						, s3d::Palette::Black);
						// 名前を描画
						font(s3d::Unicode::FromUTF8(lli.name)).draw(
							s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White),
							s3d::Arg::topCenter = s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
							double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height()))// - 30
							}
						, s3d::Palette::Black);
					}
					// 古事記のアイコン
					if (lli.source == "JP-Kojiki") {
						texture_ko.resized(35).drawAt(s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
							double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) + 50
							});
					}
					// 倭名類聚抄のアイコン
					else if (lli.source == "JP-WamyoRuijusho") {
						texture_wam.resized(35).drawAt(s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
							double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) + 50
							});
					}
					// 前方後円墳のアイコン
					else if (lli.source == "ZempoKoenFun") {
						texture_kofun1.resized(35).drawAt(s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
							double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) + 50
							});
					}
					// 前方後方墳のアイコン
					else if (lli.source == "ZempoKohoFun") {
						texture_kofun2.resized(35).drawAt(s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
							double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) + 50
							});
					}
					// 帆立貝型古墳のアイコン
					else if (lli.source == "HotategaiGataKofun") {
						texture_kofun3.resized(35).drawAt(s3d::Vec2{ (lli.x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
							double(s3d::Scene::Height()) - ((lli.y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height())) + 50
							});
					}
				}
				// それ以外（集落遺跡）
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

/*##########################################################################################
	暦関連
##########################################################################################*/

			//Rect{ 0,0,50 }.drawFrame(2, ColorF{ 1, 0, 0, 0.5 });
			//Circle{ Cursor::Pos(), 40 }.drawFrame(2, ColorF{ 1, 0, 0, 0.5 });

			//Circle{ Scene::Center(), 10 }.draw(Palette::Red); // 真ん中の赤い点

			// 日付の構造体
			paxs::Date Date;
			// グレゴリオ暦を格納
			Date = paxs::getGregorianCalendarFromJulianDayNumber(jdn);
			date_list[std::size_t(KoyomiEnum::koyomi_g)].date = Date;
			// ユリウス暦を格納
			Date = paxs::getJulianCalendarFromJulianDayNumber(jdn);
			date_list[std::size_t(KoyomiEnum::koyomi_j)].date = Date;

			// 和暦を格納
			// ユリウス日が 1480407 以上（神武 1 年 1 月 1 日以降、グレゴリオ暦 紀元前 660 年 2 月 11 日以降）
			if (jdn >= 1480407) {
				bool is_break = false;
				// 元号一覧からその日に合った元号を取得
				for (int i = 0; i < japanese_era_list.size(); ++i) {
					auto& jeli = japanese_era_list[i];
					if (jdn >= jeli.start_jdn
						&& jdn < japanese_era_list[i + 1].start_jdn) {
						is_break = true; // 元号一覧からその日に合った元号が見つかったのでループを抜ける
						// 改元されている場合
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
					// 元号一覧からその日に合った元号が見つかった場合
					if (is_break) {
						int lm = ((jeli.leap_month == 0) ? 999 : jeli.leap_month - 1);

						int calc_day = jdn - jeli.start_jdn; // １月１日
						// 月と日の計算
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

			static int count = 0; // 暦を繰り上げるタイミングを決めるためのカウンタ
			++count;
			//jdn += 400;
			//if (count >= 0) {
			if (count >= 30) {
				count = 0;
				++jdn; // ユリウス日を繰り上げ（次の日にする）
			}

			// 暦表示の範囲に白背景を追加
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
			static size_t index1 = 2;
			if (s3d::SimpleGUI::RadioButtons(index1, options, s3d::Vec2{ s3d::Scene::Width() - 400, 400 })) {
				jdn = period_jdn[index1];
			}

			// その他のデバッグ用の変数情報の表示
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