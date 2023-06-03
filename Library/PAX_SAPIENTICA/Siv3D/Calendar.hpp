/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_CALENDAR_HPP
#define PAX_SAPIENTICA_SIV3D_CALENDAR_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/Siv3D/Language.hpp>
#include <PAX_SAPIENTICA/Type/Date.hpp>

namespace paxs {

	std::vector<s3d::Font> setFont(const s3d::FontMethod& font_method, const int font_size, const s3d::String& path, const std::string& key, paxs::Language& language_text) {
		std::vector<s3d::Font> font{};
		const std::vector<s3d::String>& vs = language_text.getFindStart(key);
		for (std::size_t i = 1; i < vs.size(); ++i) {
			font.emplace_back(s3d::Font{ font_method , font_size , path + vs[i] });
		}
		return font;
	}

	// 暦の種類
	enum class KoyomiEnum {
		koyomi_japan, // 和暦
		koyomi_g, // グレゴリオ暦
		koyomi_j // ユリウス暦
	};

	class KoyomiSiv3D {
	public:
		std::array<OutputDate, 3> date_list{};

		std::vector<s3d::Font> font_pulldown;
		int font_size = 17;
		std::vector<s3d::Font> font;

		std::size_t sueki_nakamura_index;
		std::size_t sueki_tanabe_index;

		std::size_t map_view_width_str_index;
		std::size_t map_view_center_x_str_index;
		std::size_t map_view_center_y_str_index;
		std::size_t map_view_center_lat_str_index;
		std::size_t xyz_tile_z_str_index;

		std::vector<std::vector<s3d::String>> items_pulldown;
		paxs::Pulldown pulldown;

		paxs::MenuBar menu_bar;
		s3d::Texture texture_tlt;
		s3d::Texture texture_github;
		s3d::Texture texture_d_l;
		s3d::Texture texture_d_r;
		s3d::Texture texture_m_l;
		s3d::Texture texture_m_r;
		s3d::Texture texture_y_l;
		s3d::Texture texture_y_r;
		s3d::Texture texture_10y_l;
		s3d::Texture texture_10y_r;
		s3d::Texture texture_c_l;
		s3d::Texture texture_c_r;
		s3d::Texture texture_10c_l;
		s3d::Texture texture_10c_r;
		s3d::Texture texture_100c_l;
		s3d::Texture texture_100c_r;
		s3d::Texture texture_stop;
		s3d::Texture texture_playback;
		s3d::Texture texture_reverse_playback;


		/*##########################################################################################
	読み込む XYZ タイルの情報を記載
	map_name			地図名
	map_url_name		地図画像を取得する URL
	map_license_name	ライセンス情報
##########################################################################################*/

		std::unordered_map<std::string, XYZTile> xyz_tile_list;

		//const s3d::String map_license_name = U"Maptiles by\n淺野孝利 2023「古墳時代の『常総の内海』水域復原に関する一試論」\n研究代表者 荒井啓汰『埋葬施設からみた常総地域の地域構造』\n特別研究員奨励費報告書 筑波大学大学院 人文社会科学研究科";
		s3d::String map_license_name = U"Maptiles by\n農研機構農業環境研究部門, under CC BY 2.1 JP.\n20万分の1シームレス地質図V2.\nOpenStreetMap contributors, under ODbL.";
		//const s3d::String map_license_name = U"Maptiles by MIERUNE, under CC BY. Data by OpenStreetMap contributors, under ODbL.\nMaptiles by 農研機構農業環境研究部門, under CC BY 2.1 JP";

		// 地図上に描画する画像の一覧
		std::unique_ptr<TextureLocation> texture_location;
		// 時代区分の文字列
		std::vector<s3d::String> options = {
			U"旧石器時代", U"縄文時代", U"弥生時代", U"古墳時代 CE251-",
			U"飛鳥時代 CE592-", U"奈良時代 CE710-", U"平安時代 CE794-",
			U"鎌倉時代", U"室町時代", U"安土桃山時代", U"江戸時代", U"明治時代" };
		// 時代区分ごとのユリウス日の数値
		std::array<int, 12> period_jdn = {
			0,0,1538799,1812736,
			1937666,1980488,2011388,
			2154234,2209376,2295823,2306626,2403629
		};
		// 地名
		std::unique_ptr<PlaceNameLocation> place_name_location{};

//#ifdef PAXS_USING_SIMULATOR
		// エージェント
		std::unique_ptr<AgentLocation> agent_location;
		//agent_location->addKofun();
//#endif
		// 月の英語名
		s3d::String month_name_long[13] = { U"",U"January",U"February",U"March",U"April",U"May",U"June",
			U"July",U"August",U"September",U"October",U"November",U"December" };
		s3d::String month_name[13] = { U"",U"Jan.",U"Feb.",U"Mar.",U"Apr.",U"May",U"Jun.",
			U"Jul.",U"Aug.",U"Sep.",U"Oct.",U"Nov.",U"Dec." };

		// 線の情報を格納
		s3d::Array<s3d::Vec2> route1;
		s3d::Array<s3d::Vec2> route2;

		// 暦を読み込み
		std::vector<paxs::JapaneseEra> japanese_era_list;

		// 暦のフォントサイズ
		int koyomi_font_size = 17;
		int koyomi_font_x = 0;//220;
		int koyomi_font_y = 50;
		int koyomi_font_en_x = 0;//820;
		int koyomi_font_en_y = 50;
		std::vector<s3d::Font> koyomi_font;
		s3d::Font license_font;

		s3d::Font pin_font{};

		// 影
		s3d::RenderTexture shadow_texture{};
		s3d::RenderTexture internal_texture{};

		paxs::Graphics3DModel g3d_model;
		bool move_forward_in_time = false;
		bool go_back_in_time = false;

		bool is_agent_update = true; // エージェントの更新をするか
	public:
		void init(paxs::Language & language_text
		, const s3d::String& path,
		const std::string& path8,
			const std::unique_ptr<MapView>& map_view
		) {

		texture_location= std::unique_ptr<TextureLocation>(new(std::nothrow) TextureLocation);



			// 各暦の日付情報を初期化
			date_list = std::array<OutputDate, 3>{
				OutputDate{
					language_text.getFindStart("calendar_japan")
			,paxs::Date(),false },
					OutputDate{
					language_text.getFindStart("calendar_gregorian")
				,paxs::Date(),false },
					OutputDate{
					language_text.getFindStart("calendar_julian")
				, paxs::Date(), false
				}
			};

			font_pulldown = setFont(s3d::FontMethod::SDF, 16, path, "font_path", language_text);
			font = setFont(s3d::FontMethod::SDF, font_size, path, "font_path", language_text);

			sueki_nakamura_index = language_text.findStart("sueki_nakamura");
			sueki_tanabe_index = language_text.findStart("sueki_tanabe");

			map_view_width_str_index = language_text.findStart("debug_magnification_power");
			map_view_center_x_str_index = language_text.findStart("debug_mercator_longitude");
			map_view_center_y_str_index = language_text.findStart("debug_mercator_latitude");
			map_view_center_lat_str_index = language_text.findStart("debug_latitude");
			xyz_tile_z_str_index = language_text.findStart("debug_xyz_tiles_z");

			items_pulldown = language_text.getFindStartToVVS("language", 1);
			pulldown= paxs::Pulldown(items_pulldown, 0, 0, font_pulldown, s3d::Point{ 3000, 0 }, PulldownType::Zero, true);
			pulldown.setPos(s3d::Point{ s3d::Scene::Width() - pulldown.getRect().w, 0 });

			const std::vector<s3d::Font>& font_menu_bar = font_pulldown;

			menu_bar.add(language_text.get(), language_text.findStart("> menu_bar_file") + 1, 1, font_menu_bar);
			menu_bar.add(language_text.get(), language_text.findStart("> menu_bar_edit") + 1, 1, font_menu_bar);
			menu_bar.add(language_text.get(), language_text.findStart("> menu_bar_view") + 1, 1, font_menu_bar);
			menu_bar.add(language_text.get(), language_text.findStart("> menu_bar_calendar") + 1, 1, font_menu_bar);
			menu_bar.add(language_text.get(), language_text.findStart("> menu_bar_map") + 1, 1, font_menu_bar);

			texture_tlt = s3d::Texture{ path + U"Image/Logo/TitleLogoText2.svg" };
			texture_github = s3d::Texture{ path + U"Data/MenuIcon/github.svg" };
			texture_d_l = s3d::Texture{ path + U"Data/MenuIcon/DayL.svg" };
			texture_d_r = s3d::Texture{ path + U"Data/MenuIcon/DayR.svg" };
			texture_m_l = s3d::Texture{ path + U"Data/MenuIcon/MonthL.svg" };
			texture_m_r = s3d::Texture{ path + U"Data/MenuIcon/MonthR.svg" };
			texture_y_l = s3d::Texture{ path + U"Data/MenuIcon/YearL.svg" };
			texture_y_r = s3d::Texture{ path + U"Data/MenuIcon/YearR.svg" };
			texture_10y_l = s3d::Texture{ path + U"Data/MenuIcon/10YearL.svg" };
			texture_10y_r = s3d::Texture{ path + U"Data/MenuIcon/10YearR.svg" };
			texture_c_l = s3d::Texture{ path + U"Data/MenuIcon/100YearL.svg" };
			texture_c_r = s3d::Texture{ path + U"Data/MenuIcon/100YearR.svg" };
			texture_10c_l = s3d::Texture{ path + U"Data/MenuIcon/1kYearL.svg" };
			texture_10c_r = s3d::Texture{ path + U"Data/MenuIcon/1kYearR.svg" };
			texture_100c_l = s3d::Texture{ path + U"Data/MenuIcon/10kYearL.svg" };
			texture_100c_r = s3d::Texture{ path + U"Data/MenuIcon/10kYearR.svg" };
			texture_stop = s3d::Texture{ path + U"Data/MenuIcon/stop.svg" };
			texture_playback = s3d::Texture{ path + U"Data/MenuIcon/playback.svg" };
			texture_reverse_playback = s3d::Texture{ path + U"Data/MenuIcon/reverse-playback.svg" };

			/*##########################################################################################
		読み込む XYZ タイルの情報を記載
		map_name			地図名
		map_url_name		地図画像を取得する URL
		map_license_name	ライセンス情報
	##########################################################################################*/

			mapMapInit(xyz_tile_list, path, map_view.get());

			// 地名
			place_name_location= std::unique_ptr<PlaceNameLocation>(new(std::nothrow) PlaceNameLocation);
			if (place_name_location != nullptr) {
				place_name_location->add();
			}
//#ifdef PAXS_USING_SIMULATOR
			// エージェント
			agent_location= std::unique_ptr<AgentLocation>(new(std::nothrow) AgentLocation);
			//agent_location->addKofun();
//#endif
			// 航路を読み込み
			std::ifstream rifs(path8 + "Data/Route/Yamatai.tsv");
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
			paxs::inputJapaneseEra(japanese_era_list, path8 + "Data/Calendar/JapaneseEraName.tsv");

			koyomi_font = setFont(s3d::FontMethod::SDF, koyomi_font_size, path, "font_path", language_text);

			license_font = s3d::Font{ s3d::FontMethod::SDF, 14 /*, Typeface::Bold*/
				, (path + U"Data/Font/noto-sans-jp/NotoSansJP-Regular.otf")
			};
			license_font.setBufferThickness(3);

			pin_font= s3d::Font{ s3d::FontMethod::SDF, 18 /*, Typeface::Bold*/
				, (path + U"Data/Font/noto-sans-jp/NotoSansJP-Regular.otf")
			};
			pin_font.setBufferThickness(3);

			// 影
			shadow_texture= s3d::RenderTexture{ s3d::Scene::Size(), s3d::ColorF{ 1.0, 0.0 } };
			internal_texture= s3d::RenderTexture{ shadow_texture.size() };


		}
		void update(const std::unique_ptr<MapView>& map_view, const std::size_t language
		, paxs::Language& language_text,
			paxs::Simulator<int>& simulator,
			const paxs::Vector2<int>& start_position,
			const paxs::Vector2<int>& end_position,
			const std::string& path8
			) {
			// 画像の拡大縮小の方式を設定
			const s3d::ScopedRenderStates2D sampler{ s3d::SamplerState::ClampLinear };

			const double map_view_width = map_view->getWidth();
			const double map_view_height = map_view->getHeight();
			const double map_view_center_x = map_view->getCenterX();
			const double map_view_center_y = map_view->getCenterY();
			const double map_view_center_lat = std::asin(std::tanh(map_view_center_y / 180.0 * paxs::pi)) / (paxs::pi) * 180.0;

			/*##########################################################################################
	暦関連
##########################################################################################*/

//s3d::Rect{ 0,0,50 }.drawFrame(2, ColorF{ 1, 0, 0, 0.5 });
//Circle{ Cursor::Pos(), 40 }.drawFrame(2, ColorF{ 1, 0, 0, 0.5 });
//Circle{ Scene::Center(), 10 }.draw(s3d::Palette::Red); // 真ん中の赤い点

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
				for (int i = 0; i < japanese_era_list.size() - 1; ++i) {
					auto& jeli = japanese_era_list[i];
					if (jdn >= jeli.start_jdn
						&& jdn < japanese_era_list[i + 1].start_jdn) {
						is_break = true; // 元号一覧からその日に合った元号が見つかったのでループを抜ける
						// 改元されている場合
						if (jeli.kaigen_jdn[0] != 0 &&
							jdn >= jeli.kaigen_jdn[0]) {
							date_list[std::size_t(KoyomiEnum::koyomi_japan)].calendar_name[language + 1 /* 言語位置調整 */] = language_text.getFindStart(jeli.gengo[1])[language + 1 /* 言語位置調整 */];
							date_list[std::size_t(KoyomiEnum::koyomi_japan)].date.setYear(jeli.gengo_num[1]);
						}
						else {
							date_list[std::size_t(KoyomiEnum::koyomi_japan)].calendar_name[language + 1 /* 言語位置調整 */] = language_text.getFindStart(jeli.gengo[0])[language + 1 /* 言語位置調整 */];
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
								date_list[std::size_t(KoyomiEnum::koyomi_japan)].date.setMonth(DateMonth(j + 1));
								date_list[std::size_t(KoyomiEnum::koyomi_japan)].date.setDay(DateDay(calc_day + 1));
								date_list[std::size_t(KoyomiEnum::koyomi_japan)].is_leap_month = false;
								break;
							}
							calc_day -= jeli.number_of_days[j];
							if (j == lm) {
								if (calc_day < jeli.number_of_days_of_leap_month) {
									date_list[std::size_t(KoyomiEnum::koyomi_japan)].date.setMonth(DateMonth(j + 1));
									date_list[std::size_t(KoyomiEnum::koyomi_japan)].date.setDay(DateDay(calc_day + 1));
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
			if(move_forward_in_time) jdn += 1000;
			else if(go_back_in_time) jdn -= 1000;
		//if (count >= 0) {
			if (count >= 30) {
				count = 0;
				if (move_forward_in_time) {
					//++jdn; // ユリウス日を繰り上げ（次の日にする）
					jdn += 365; // ユリウス日を繰り上げ（次の日にする）
#ifdef PAXS_USING_SIMULATOR
					// エージェント機能テスト
					if (is_agent_update) {
						simulator.step();
					}
#endif
				}
				else if (go_back_in_time) {
					++jdn; // ユリウス日を繰り上げ（次の日にする）
				}
			}

			if (menu_bar.getPulldown(MenuBarType::view).getIsItems(0)) {

				// 影を作る図形を shadow_texture に描く
				{
					const s3d::ScopedRenderTarget2D target{ shadow_texture.clear(s3d::ColorF{ 1.0, 0.0 }) };
					const s3d::ScopedRenderStates2D blend{ s3d::BlendState::MaxAlpha };
					const s3d::Transformer2D transform{ s3d::Mat3x2::Translate(3, 3) };

					s3d::Rect{ 0,0,s3d::Scene::Width(),30 }.draw(); // メニューバー
					s3d::RoundRect{ s3d::Scene::Width() - 375,koyomi_font_y - 5,360,210, 10 }.draw();
					s3d::RoundRect{ s3d::Scene::Width() - 375,koyomi_font_y + 235,360,380, 10 }.draw();
				}

				// shadow_texture を 2 回ガウスぼかし
				{
					s3d::Shader::GaussianBlur(shadow_texture, internal_texture, shadow_texture);
					s3d::Shader::GaussianBlur(shadow_texture, internal_texture, shadow_texture);
				}

				shadow_texture.draw(s3d::ColorF{ 0.0, 0.5 });

				// 暦表示の範囲に白背景を追加
				s3d::RoundRect{ s3d::Scene::Width() - 375,koyomi_font_y - 5,360,210, 10 }.draw(s3d::ColorF{ 1,1,1 }/*s3d::Palette::White*/);
				s3d::RoundRect{ s3d::Scene::Width() - 375,koyomi_font_y + 235,360,380, 10 }.draw(s3d::ColorF{ 1,1,1 }/*s3d::Palette::White*/);

				// 暦の表示（日本語）
				if (
					language == 1
					|| language == 2
					|| language == 3
					) {
					for (int i = 0; i < date_list.size(); ++i) {
						koyomi_font[language](date_list[i].calendar_name[language + 1 /* 言語位置調整 */]).draw(s3d::Arg::topRight = s3d::Vec2(koyomi_font_x, koyomi_font_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
						koyomi_font[language](U"年").draw(s3d::Arg::topRight = s3d::Vec2(int(120 * koyomi_font_size / 30.0) + koyomi_font_x, koyomi_font_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
						koyomi_font[language](U"月").draw(s3d::Arg::topRight = s3d::Vec2(int(220 * koyomi_font_size / 30.0) + koyomi_font_x, koyomi_font_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
						koyomi_font[language](U"日").draw(s3d::Arg::topRight = s3d::Vec2(int(300 * koyomi_font_size / 30.0) + koyomi_font_x, koyomi_font_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);

						koyomi_font[language](s3d::ToString(date_list[i].date.year)).draw(s3d::Arg::topRight = s3d::Vec2(int(85 * koyomi_font_size / 30.0) + koyomi_font_x, koyomi_font_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
						koyomi_font[language](s3d::ToString(date_list[i].date.month)).draw(s3d::Arg::topRight = s3d::Vec2(int(190 * koyomi_font_size / 30.0) + koyomi_font_x, koyomi_font_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
						koyomi_font[language](s3d::ToString(date_list[i].date.day)).draw(s3d::Arg::topRight = s3d::Vec2(int(270 * koyomi_font_size / 30.0) + koyomi_font_x, koyomi_font_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
						if (!date_list[i].is_leap_month) continue;
						koyomi_font[language](U"閏").draw(s3d::Arg::topRight = s3d::Vec2((
							(date_list[i].date.month < 10) ? int(167 * koyomi_font_size / 30.0) + koyomi_font_x : int(152 * koyomi_font_size / 30.0) + koyomi_font_x
							), koyomi_font_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
					}
				}
				else {
					// 暦の表示（英語）
					for (int i = 0; i < date_list.size(); ++i) {
						koyomi_font[language](date_list[i].calendar_name[language + 1 /* 言語位置調整 */]).draw(s3d::Arg::topRight = s3d::Vec2(koyomi_font_en_x, koyomi_font_en_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
						koyomi_font[language](U",").draw(s3d::Arg::topRight = s3d::Vec2(int(95 * koyomi_font_size / 30.0) + koyomi_font_en_x, koyomi_font_en_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
						koyomi_font[language](U",").draw(s3d::Arg::topRight = s3d::Vec2(int(235 * koyomi_font_size / 30.0) + koyomi_font_en_x, koyomi_font_en_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
						koyomi_font[language](U"th").draw(s3d::Arg::topRight = s3d::Vec2(int(315 * koyomi_font_size / 30.0) + koyomi_font_en_x, koyomi_font_en_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);

						koyomi_font[language](s3d::ToString(date_list[i].date.year)).draw(s3d::Arg::topRight = s3d::Vec2(int(85 * koyomi_font_size / 30.0) + koyomi_font_en_x, koyomi_font_en_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
						koyomi_font[language](month_name[date_list[i].date.month]).draw(s3d::Arg::topRight = s3d::Vec2(int(220 * koyomi_font_size / 30.0) + koyomi_font_en_x, koyomi_font_en_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
						koyomi_font[language](s3d::ToString(date_list[i].date.day)).draw(s3d::Arg::topRight = s3d::Vec2(int(280 * koyomi_font_size / 30.0) + koyomi_font_en_x, koyomi_font_en_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
						if (!date_list[i].is_leap_month) continue;
						koyomi_font[language](U"int.").draw(s3d::Arg::topRight = s3d::Vec2((
							int(152 * koyomi_font_size / 30.0) + koyomi_font_en_x
							), koyomi_font_en_y + i * (koyomi_font_size * 4 / 3)), s3d::Palette::Black);
					}
				}
				const int arrow_time_icon_size = 24; // 時間操作アイコンの大きさ
				const int time_icon_size = 40; // 時間操作アイコンの大きさ
				const int icon_const_start_x = 360;
				int icon_start_x = icon_const_start_x;
				int icon_start_y = 80;
				const int arrow_icon_move_x = int(arrow_time_icon_size * 1.2);// int(time_icon_size * 1.4);
				const int icon_move_x = int(time_icon_size * 1.1);// int(time_icon_size * 1.4);
				const int arrow_icon_move_y = 30;
				const int icon_move_y = 44;

				texture_reverse_playback.resized(arrow_time_icon_size).draw(s3d::Scene::Width() - icon_start_x, koyomi_font_y + icon_start_y);
				if (s3d::Rect{ s3d::Scene::Width() - icon_start_x,koyomi_font_y + icon_start_y , time_icon_size,time_icon_size }.leftClicked()) {
					move_forward_in_time = false;
					go_back_in_time = true; // 逆再生
				}
				icon_start_x -= arrow_icon_move_x;
				texture_stop.resized(arrow_time_icon_size).draw(s3d::Scene::Width() - icon_start_x, koyomi_font_y + icon_start_y);
				if (s3d::Rect{ s3d::Scene::Width() - icon_start_x,koyomi_font_y + icon_start_y , time_icon_size,time_icon_size }.leftClicked()) {
					move_forward_in_time = false; // 一時停止
					go_back_in_time = false;
				}
				icon_start_x -= arrow_icon_move_x;
				texture_playback.resized(arrow_time_icon_size).draw(s3d::Scene::Width() - icon_start_x, koyomi_font_y + icon_start_y);
				if (s3d::Rect{ s3d::Scene::Width() - icon_start_x,koyomi_font_y + icon_start_y , time_icon_size,time_icon_size }.leftClicked()) {
					move_forward_in_time = true; // 再生
					go_back_in_time = false;
				}
				icon_start_y += arrow_icon_move_y;
				icon_start_x = icon_const_start_x;

				texture_d_l.resized(time_icon_size).draw(s3d::Scene::Width() - icon_start_x, koyomi_font_y + icon_start_y);
				if (s3d::Rect{ s3d::Scene::Width() - icon_start_x,koyomi_font_y + icon_start_y , time_icon_size,time_icon_size }.leftClicked()) {
					jdn -= 1;
				}
				icon_start_x -= icon_move_x;
				texture_m_l.resized(time_icon_size).draw(s3d::Scene::Width() - icon_start_x, koyomi_font_y + icon_start_y);
				if (s3d::Rect{ s3d::Scene::Width() - icon_start_x,koyomi_font_y + icon_start_y , time_icon_size,time_icon_size }.leftClicked()) {
					jdn -= 30;
				}
				icon_start_x -= icon_move_x;
				texture_y_l.resized(time_icon_size).draw(s3d::Scene::Width() - icon_start_x, koyomi_font_y + icon_start_y);
				if (s3d::Rect{ s3d::Scene::Width() - icon_start_x,koyomi_font_y + icon_start_y , time_icon_size,time_icon_size }.leftClicked()) {
					jdn -= 365;
				}
				icon_start_x -= icon_move_x;
				texture_10y_l.resized(time_icon_size).draw(s3d::Scene::Width() - icon_start_x, koyomi_font_y + icon_start_y);
				if (s3d::Rect{ s3d::Scene::Width() - icon_start_x,koyomi_font_y + icon_start_y , time_icon_size,time_icon_size }.leftClicked()) {
					jdn -= 3650;
				}
				icon_start_x -= icon_move_x;
				texture_c_l.resized(time_icon_size).draw(s3d::Scene::Width() - icon_start_x, koyomi_font_y + icon_start_y);
				if (s3d::Rect{ s3d::Scene::Width() - icon_start_x,koyomi_font_y + icon_start_y , time_icon_size,time_icon_size }.leftClicked()) {
					jdn -= (365 * 100);
				}
				icon_start_x -= icon_move_x;
				texture_10c_l.resized(time_icon_size).draw(s3d::Scene::Width() - icon_start_x, koyomi_font_y + icon_start_y);
				if (s3d::Rect{ s3d::Scene::Width() - icon_start_x,koyomi_font_y + icon_start_y , time_icon_size,time_icon_size }.leftClicked()) {
					jdn -= (365 * 1000);
				}
				icon_start_x -= icon_move_x;
				texture_100c_l.resized(time_icon_size).draw(s3d::Scene::Width() - icon_start_x, koyomi_font_y + icon_start_y);
				if (s3d::Rect{ s3d::Scene::Width() - icon_start_x,koyomi_font_y + icon_start_y , time_icon_size,time_icon_size }.leftClicked()) {
					jdn -= (365 * 10000);
				}
				icon_start_y += icon_move_y;
				icon_start_x = icon_const_start_x;

				texture_d_r.resized(time_icon_size).draw(s3d::Scene::Width() - icon_start_x, koyomi_font_y + icon_start_y);
				if (s3d::Rect{ s3d::Scene::Width() - icon_start_x,koyomi_font_y + icon_start_y , time_icon_size,time_icon_size }.leftClicked()) {
					jdn += 1;
				}
				icon_start_x -= icon_move_x;
				texture_m_r.resized(time_icon_size).draw(s3d::Scene::Width() - icon_start_x, koyomi_font_y + icon_start_y);
				if (s3d::Rect{ s3d::Scene::Width() - icon_start_x,koyomi_font_y + icon_start_y , time_icon_size,time_icon_size }.leftClicked()) {
					jdn += 30;
				}
				icon_start_x -= icon_move_x;
				texture_y_r.resized(time_icon_size).draw(s3d::Scene::Width() - icon_start_x, koyomi_font_y + icon_start_y);
				if (s3d::Rect{ s3d::Scene::Width() - icon_start_x,koyomi_font_y + icon_start_y , time_icon_size,time_icon_size }.leftClicked()) {
					jdn += 365;
				}
				icon_start_x -= icon_move_x;
				texture_10y_r.resized(time_icon_size).draw(s3d::Scene::Width() - icon_start_x, koyomi_font_y + icon_start_y);
				if (s3d::Rect{ s3d::Scene::Width() - icon_start_x,koyomi_font_y + icon_start_y , time_icon_size,time_icon_size }.leftClicked()) {
					jdn += 3650;
				}
				icon_start_x -= icon_move_x;
				texture_c_r.resized(time_icon_size).draw(s3d::Scene::Width() - icon_start_x, koyomi_font_y + icon_start_y);
				if (s3d::Rect{ s3d::Scene::Width() - icon_start_x,koyomi_font_y + icon_start_y , time_icon_size,time_icon_size }.leftClicked()) {
					jdn += (365 * 100);
				}
				icon_start_x -= icon_move_x;
				texture_10c_r.resized(time_icon_size).draw(s3d::Scene::Width() - icon_start_x, koyomi_font_y + icon_start_y);
				if (s3d::Rect{ s3d::Scene::Width() - icon_start_x,koyomi_font_y + icon_start_y , time_icon_size,time_icon_size }.leftClicked()) {
					jdn += (365 * 1000);
				}
				icon_start_x -= icon_move_x;
				texture_100c_r.resized(time_icon_size).draw(s3d::Scene::Width() - icon_start_x, koyomi_font_y + icon_start_y);
				if (s3d::Rect{ s3d::Scene::Width() - icon_start_x,koyomi_font_y + icon_start_y , time_icon_size,time_icon_size }.leftClicked()) {
					jdn += (365 * 10000);
				}

			}

			//時代区分を選択するラジオボタン
		   //static size_t index1 = 2;
		   //if (s3d::SimpleGUI::RadioButtons(index1, options, s3d::Vec2{ s3d::Scene::Width() - 400, 400 })) {
		   //	jdn = period_jdn[index1];
		   //}

			int debug_start_y = 300;
			int debug_move_y = 25;
			// その他のデバッグ用の変数情報の表示
			if (menu_bar.getPulldown(MenuBarType::view).getIsItems(3)) {
				font[language](language_text.get()[map_view_center_x_str_index][language + 1 /* 言語位置調整 */]
					).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Arg::topRight = s3d::Vec2(s3d::Scene::Width() - 160, debug_start_y), s3d::Palette::Black);
				font[language](s3d::ToString(map_view_center_x)
					).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Vec2(s3d::Scene::Width() - 110, debug_start_y), s3d::Palette::Black);
				debug_start_y += debug_move_y;
				// マップ中心座標 Y
				font[language](language_text.get()[map_view_center_y_str_index][language + 1 /* 言語位置調整 */]
					).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Arg::topRight = s3d::Vec2(s3d::Scene::Width() - 160, debug_start_y), s3d::Palette::Black);
				font[language](s3d::ToString(map_view_center_y)
					).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Vec2(s3d::Scene::Width() - 110, debug_start_y), s3d::Palette::Black);
				debug_start_y += debug_move_y;
				font[language](language_text.get()[map_view_center_lat_str_index][language + 1 /* 言語位置調整 */]
					).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Arg::topRight = s3d::Vec2(s3d::Scene::Width() - 160, debug_start_y), s3d::Palette::Black);
				font[language](s3d::ToString(map_view_center_lat)
					).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Vec2(s3d::Scene::Width() - 110, debug_start_y), s3d::Palette::Black);
				debug_start_y += debug_move_y;
				font[language](language_text.get()[map_view_width_str_index][language + 1 /* 言語位置調整 */]
					).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Arg::topRight = s3d::Vec2(s3d::Scene::Width() - 160, debug_start_y), s3d::Palette::Black);
				font[language](s3d::ToString(map_view_width)
					).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Vec2(s3d::Scene::Width() - 110, debug_start_y), s3d::Palette::Black);
				debug_start_y += debug_move_y;
				font[language](language_text.get()[xyz_tile_z_str_index][language + 1 /* 言語位置調整 */]
					).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Arg::topRight = s3d::Vec2(s3d::Scene::Width() - 160, debug_start_y), s3d::Palette::Black);
				//font[language](s3d::ToString(xyz_tile2->getZ())
				//).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Vec2(s3d::Scene::Width() - 110, debug_start_y), s3d::Palette::Black);
				//debug_start_y += debug_move_y;
				//font[language](s3d::ToString(xyz_tile2->getZNum())
				//).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Vec2(s3d::Scene::Width() - 110, debug_start_y), s3d::Palette::Black);

				// ユリウス通日
				font[language](s3d::ToString(jdn)
					).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Vec2(s3d::Scene::Width() - 110, 120), s3d::Palette::Black);

			}
			if (menu_bar.getPulldown(MenuBarType::view).getIsItems(2)) {
				//font(s3d::String{ U"A" } + s3d::ToString(xyz_tile_cell.x) + s3d::String{ U":" } + s3d::ToString(xyz_tile_cell.y)).draw(s3d::Arg::topRight = s3d::Vec2(s3d::Scene::Width() - 10, 400), s3d::Palette::Black);
				//font(s3d::String{ U"B" } + s3d::ToString(xyz_tile_pos.x) + s3d::String{ U":" } + s3d::ToString(xyz_tile_pos.y)).draw(s3d::Arg::topRight = s3d::Vec2(s3d::Scene::Width() - 10, 450), s3d::Palette::Black);
				license_font(map_license_name).draw(
					s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White),
					s3d::Arg::bottomLeft = s3d::Vec2(10, s3d::Scene::Height() - 10),
					s3d::Palette::White);
				license_font(map_license_name).draw(
					s3d::Arg::bottomLeft = s3d::Vec2(10, s3d::Scene::Height() - 10),
					s3d::Palette::Black);
				//pin_font(s3d::String{ U"国土地理院（https://maps.gsi.go.jp/development/ichiran.html）" }).draw(s3d::Arg::topLeft = s3d::Vec2(10, 10), s3d::Palette::Black);
				//texture_tlt.resized(180).draw(s3d::Arg::bottomRight = s3d::Vec2(s3d::Scene::Width() - 10, s3d::Scene::Height() - 10));
			}

			if (!menu_bar.getPulldown(MenuBarType::view).getIsItems(4)) {
				g3d_model.updateRotation(); // 3D モデルを回転させる
			}

			// メニューバー
			s3d::Rect{ 0,0,s3d::Scene::Width(),30 }.draw(s3d::Color{ 243,243,243 });
			texture_github.resized(24).draw(s3d::Vec2{ s3d::Scene::Width() - 280, 3 });
			pulldown.draw(); // 言語選択
			menu_bar.draw(); // 左上メニューバー

			if (s3d::Rect(s3d::Scene::Width() - 280, 3, 28).leftClicked()) {
				// Web ページをブラウザで開く
				s3d::System::LaunchBrowser(U"https://github.com/AsPJT/PAX_SAPIENTICA");
			}

			if (menu_bar.getPulldown(MenuBarType::view).getIsItems(2)) {

				font[language](language_text.get()[sueki_nakamura_index][language + 1 /* 言語位置調整 */]
					).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White),
						s3d::Arg::topRight = s3d::Vec2(s3d::Scene::Width() - 160, 590), s3d::Palette::Black);
				font[language](language_text.get()[sueki_tanabe_index][language + 1 /* 言語位置調整 */]
					).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White),
						s3d::Arg::topRight = s3d::Vec2(s3d::Scene::Width() - 160, 620), s3d::Palette::Black);
				{
					const int y = int(date_list[1].date.year);
					s3d::String sueki_nakamura = U"";
					s3d::String sueki_tanabe = U"";

					static std::array<int, 18> sueki_year = { {
					380,390,410,430,440,450,460,470,490,500,520,530,550,560,590,620,645,670
					} };
					static std::array<s3d::String, 18> sueki_name = { {
		U"",U"TG232",U"TK73",U"TK216",U"ON46",U"ON46/TK208",U"TK208",U"TK23",U"TK23/TK47",U"TK47",
		U"MT15",U"TK10",U"TK10/MT85",U"MT85",U"TK43",U"TK209",U"TK217古",U"TK217新"
								} };
					static std::array<s3d::String, 18> sueki_nakamura_name = { {
		U"",U"I-1前",U"I-1後",U"I-2",U"I-3",U"I-3",U"I-3",U"I-4",U"I-4/I-5",U"I-5",
		U"II-1",U"II-2",U"II-2/II-3",U"II-3",U"II-4",U"II-5",U"II-6",U"III-1"
								} };
					for (std::size_t i = 0; i < sueki_year.size(); ++i) {
						if (y < sueki_year[i]) {
							sueki_tanabe = sueki_name[i];
							sueki_nakamura = sueki_nakamura_name[i];
							break;
						}
					}
					font[language](sueki_tanabe
						).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White),
							s3d::Arg::topRight = s3d::Vec2(s3d::Scene::Width() - 60, 620), s3d::Palette::Black);
					font[language](sueki_nakamura
						).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White),
							s3d::Arg::topRight = s3d::Vec2(s3d::Scene::Width() - 60, 590), s3d::Palette::Black);
				}
#ifdef PAXS_USING_SIMULATOR
				if (s3d::SimpleGUI::Button(U"Init", s3d::Vec2{ 10,60 })) {
					simulator = paxs::Simulator<int>(
						path8 + "Data/Map/XYZTile/LandAndWater/Data/BlackAndWhiteBinary/1868/10",
						//paxs::Vector2<int>{861, 350},
						//paxs::Vector2<int>{950, 450}, 10);
						start_position,
						end_position, 10);
					simulator.init();
				}
				if (s3d::SimpleGUI::Button(U"Start", s3d::Vec2{ 110,60 })) {
					is_agent_update = true;
				}
				if (s3d::SimpleGUI::Button(U"Stop", s3d::Vec2{ 210,60 })) {
					is_agent_update = false;
				}
#endif


			}
		}

	};

}

#endif // !PAX_SAPIENTICA_SIV3D_CALENDAR_HPP