/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_LOCATION_POINT_HPP
#define PAX_SAPIENTICA_SIV3D_LOCATION_POINT_HPP

#include <string>

#include <PAX_SAPIENTICA/Siv3D/Init.hpp>
#include <PAX_SAPIENTICA/Simulation/Agent.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>
#include <PAX_SAPIENTICA/MapProjection.hpp> // 地図投影法

/*##########################################################################################

##########################################################################################*/

namespace paxs {
    // ユリウス日
    //int jdn = 1600407;
    //int jdn = 1808020;

    // ロケーションポイントの種類
    enum class LocationPointEnum {
        location_point_place_name, // 地名
        location_point_pit_dwelling, // 集落遺跡
        location_point_agent, // エージェント
        location_point_agent2, // エージェント
        location_point_zempo_koen_fun, // 前方後円墳
        location_point_zempo_koho_fun, // 前方後方墳
        location_point_hotategai_gata_kofun // 帆立貝型古墳
    };

    // 地名
    struct LocationPoint {
        constexpr explicit LocationPoint() = default;
        explicit LocationPoint(
            const std::string& name_,  // 日本語名
            const std::string& en_name_,  // 英語名
            const paxs::MercatorDeg& coordinate_,  // 経緯度
            const double min_view_,  // 可視化する地図の最小範囲
            const double max_view_,  // 可視化する地図の最大範囲
            const int min_year_,  // 可視化する時代（古い年～）
            const int max_year_,  // 可視化する時代（～新しい年）
            const LocationPointEnum lpe_,  // 対象となる地物の種別
            const std::string& source_ // 出典
        ) noexcept
            : name(name_), en_name(en_name_), coordinate(coordinate_), min_view(min_view_), max_view(max_view_), min_year(min_year_), max_year(max_year_), lpe(lpe_), source(source_) {}
        std::string name{}; // 日本語名
        std::string en_name{}; // 英語名
        paxs::MercatorDeg coordinate{}; // 経緯度
        double min_view{}, max_view{}; // 可視化する地図の拡大縮小範囲
        int min_year{}, max_year{}; // 可視化する時代（古い年～新しい年）
        LocationPointEnum lpe{}; // 対象となる地物の種別
        std::string source{}; // 出典
    };

    // GUI に描画する地物の情報を管理・描画するクラス
    class PlaceNameLocation {
    public:
        void update(const std::vector<paxs::Agent<int>>& agents, const paxs::Vector2<int>& start_position) {
            // エージェントの設定を更新
            location_point_list.resize(0);
            for (std::size_t i = 0; i < agents.size(); ++i) {
                location_point_list.emplace_back(
                    LocationPoint{
                        "","",
                        paxs::MercatorDeg(agents[i].getLocation(start_position, 10)),
                        100,0,0,99999999,
                        (agents[i].getGender()) ?
                        LocationPointEnum::location_point_agent :
                        LocationPointEnum::location_point_agent2
                        ,""

                    }
                );
            }
            //for (std::size_t i = 0; i < location_point_list.size(); ++i) {
            //	auto& lli = location_point_list[i];
            //	lli.coordinate.x += s3d::Random(-0.001,0.001);
            //	lli.coordinate.y += s3d::Random(-0.001, 0.001);
            //}
        }
        // 古墳を追加（実験用）
        void addKofun() {
            for (int i = 0; i < 10; ++i)
                inputPlace("./../../../../../Data/PlaceName/TestMap/Kofun.tsv", LocationPointEnum::location_point_agent);
        }
        void add() {
            // 古墳
            inputPlace("./../../../../../Data/PlaceName/TestMap/Hokkaido.tsv", LocationPointEnum::location_point_zempo_koen_fun);
            // 古墳
            inputPlace("./../../../../../Data/PlaceName/TestMap/Kofun.tsv", LocationPointEnum::location_point_zempo_koen_fun);

            // 古事記の地名
            inputPlace("./../../../../../Data/PlaceName/KojikiPlaceName.tsv");
            // 汎用的な地名
            inputPlace("./../../../../../Data/PlaceName/PlaceName.tsv");
            // 倭名類聚抄の地名
            inputPlace("./../../../../../Data/PlaceName/WamyoRuijushoPlaceName.tsv");
            // おもろさうしの地名
            inputPlace("./../../../../../Data/PlaceName/OmoroSoshiPlaceName.tsv");
        }

        PlaceNameLocation() {
        }
        // 描画
        void draw(const double jdn,
            const double map_view_width, const double map_view_height, const double map_view_center_x, const double map_view_center_y,
            const s3d::Font& font, const s3d::Font& en_font, const s3d::Font& pin_font)const {

            // 地名を描画
            for (std::size_t i = 0; i < location_point_list.size(); ++i) {
                auto& lli = location_point_list[i];
                // 範囲外を除去
                if (lli.coordinate.x < (map_view_center_x - map_view_width / 1.8)
                    || lli.coordinate.x >(map_view_center_x + map_view_width / 1.8)
                    || lli.coordinate.y < (map_view_center_y - map_view_height / 1.8)
                    || lli.coordinate.y >(map_view_center_y + map_view_height / 1.8)) continue;

                // 範囲内の場合
                if (lli.min_view > map_view_width
                    || lli.max_view < map_view_width
                    || lli.min_year > jdn
                    || lli.max_year < jdn) {
                    if (lli.min_year > jdn) continue;
                    if (lli.max_year < jdn) continue;
                    // 地名を描画
                    //if (lli.lpe == LocationPointEnum::location_point_place_name) {
                    //	texture_pn.resized(14).drawAt(
                    //		s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                    //	double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height())) });
                    //	continue;
                    //}
                    // エージェントを描画
                    if (lli.lpe == LocationPointEnum::location_point_agent) {
                        //s3d::Circle(	s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                        //double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height())) }
                        //,10).draw();
                        texture_blue_circle.resized(15).drawAt(
                            s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                            double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height())) });
                        continue;
                    }
                    // エージェントを描画
                    if (lli.lpe == LocationPointEnum::location_point_agent2) {
                        //s3d::Circle(	s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                        //double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height())) }
                        //,10).draw();
                        texture_red_circle.resized(15).drawAt(
                            s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                            double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height())) });
                        continue;
                    }
                    // 遺跡を描画
                    if (lli.source == "Iseki") {
                        texture_blue_circle.resized(10).drawAt(
                            s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                            double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height())) });
                        continue;
                    }
                    // 前方後円墳を描画
                    //if (lli.lpe == LocationPointEnum::location_point_zempo_koen_fun) {
                    if (lli.source == "ZempoKoenFun") {
                        texture_kofun1.resized(14).drawAt(
                            s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                            double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height())) });
                        continue;
                    }
                    // 前方後方墳を描画
                    //if (lli.lpe == LocationPointEnum::location_point_zempo_koho_fun) {
                    if (lli.source == "ZempoKohoFun") {
                        texture_kofun2.resized(14).drawAt(
                            s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                            double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height())) });
                        continue;
                    }
                    // 帆立貝型古墳を描画
                    //if (lli.lpe == LocationPointEnum::location_point_hotategai_gata_kofun) {
                    if (lli.source == "HotategaiGataKofun") {
                        texture_kofun3.resized(14).drawAt(
                            s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                            double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height())) });
                        continue;
                    }
                }
            }

            // 地名を描画
            for (std::size_t i = 0; i < location_point_list.size(); ++i) {
                auto& lli = location_point_list[i];

                // 範囲外を除去
                if (lli.coordinate.x < (map_view_center_x - map_view_width / 1.8)
                    || lli.coordinate.x >(map_view_center_x + map_view_width / 1.8)
                    || lli.coordinate.y < (map_view_center_y - map_view_height / 1.8)
                    || lli.coordinate.y >(map_view_center_y + map_view_height / 1.8)) continue;

                if (lli.min_view > map_view_width) continue;
                if (lli.max_view < map_view_width) continue;
                if (lli.min_year > jdn) continue;
                if (lli.max_year < jdn) continue;

                // 集落遺跡ではない場合
                //if (lli.lpe == LocationPointEnum::location_point_place_name) {
                if (lli.lpe != LocationPointEnum::location_point_pit_dwelling) {

                    if (lli.en_name.size() == 0) {
                        // 名前を描画
                        font(s3d::Unicode::FromUTF8(lli.name)).drawAt(
                            s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White),
                            (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                            double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height()))// - 30
                            , s3d::Palette::Black);
                    }
                    else {
                        // 名前（英語）を描画
                        en_font(s3d::Unicode::FromUTF8(lli.en_name)).draw(
                            s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White),
                            s3d::Arg::bottomCenter = s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                            double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height()))// - 30
                            }
                        , s3d::Palette::Black);
                        // 名前を描画
                        font(s3d::Unicode::FromUTF8(lli.name)).draw(
                            s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White),
                            s3d::Arg::topCenter = s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                            double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height()))// - 30
                            }
                        , s3d::Palette::Black);
                    }
                    // 古事記のアイコンを描画
                    if (lli.source == "JP-Kojiki") {
                        texture_ko.resized(20).drawAt(s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                            double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height()))
                        });
                    }
                    // 倭名類聚抄のアイコンを描画
                    else if (lli.source == "JP-WamyoRuijusho") {
                        //texture_wam.resized(20).drawAt(s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                        //	double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height()))
                        //	});
                    }
                    // 前方後円墳のアイコンを描画
                    else if (lli.source == "Iseki") {
                        texture_blue_circle.resized(35).drawAt(s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                            double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height()))
                        });
                    }
                    // 前方後円墳のアイコンを描画
                    else if (lli.source == "ZempoKoenFun") {
                        texture_kofun1.resized(35).drawAt(s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                            double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height()))
                        });
                    }
                    // 前方後方墳のアイコンを描画
                    else if (lli.source == "ZempoKohoFun") {
                        texture_kofun2.resized(35).drawAt(s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                            double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height()))
                        });
                    }
                    // 帆立貝型古墳のアイコンを描画
                    else if (lli.source == "HotategaiGataKofun") {
                        texture_kofun3.resized(35).drawAt(s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                            double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height()))
                        });
                    }
                }
                // それ以外（集落遺跡）を描画
                else {
                    pin_font(s3d::Unicode::FromUTF8(lli.name)).drawAt(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White),
                        (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                        double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height())) - 70
                        , s3d::Palette::Black);
                    texture_pin1.resized(50).draw(
                        s3d::Arg::bottomCenter = s3d::Vec2((lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                            double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height())))
                    );
                }
                //Circle{{(lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(Scene::Width()),
                //double(Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(Scene::Height()))},5 }.draw(Palette::Black);

            }
        }
    private:
        std::vector<LocationPoint> location_point_list{}; // 地物の一覧
        // アイコンのテクスチャ
        const s3d::Texture texture_ko{ U"./../../../../../Data/OldDocumentIcon/JP-Kojiki.svg" };
        const s3d::Texture texture_wam{ U"./../../../../../Data/OldDocumentIcon/JP-WamyoRuijusho.svg" };
        const s3d::Texture texture_pin1{ U"./../../../../../Data/Pin/PitDwelling.svg" };
        const s3d::Texture texture_blue_circle{ U"./../../../../../Data/MiniIcon/BlueCircle.svg" };
        const s3d::Texture texture_red_circle{ U"./../../../../../Data/MiniIcon/RedCircle.svg" };
        const s3d::Texture texture_kofun1{ U"./../../../../../Data/MiniIcon/ZempoKoenFun.svg" };
        const s3d::Texture texture_kofun2{ U"./../../../../../Data/MiniIcon/ZempoKohoFun.svg" };
        const s3d::Texture texture_kofun3{ U"./../../../../../Data/MiniIcon/HotategaiGataKofun.svg" };
        const s3d::Texture texture_pn{ U"./../../../../../Data/MiniIcon/PlaceName.svg" };

        // 地名を読み込み
        void inputPlace(const std::string& str_, const LocationPointEnum lpe_ = LocationPointEnum::location_point_place_name) {

            std::ifstream pifs(str_); // 地名を読み込む
            if (pifs.fail()) return;
            std::string pline;
            // 1 行ずつ読み込み（区切りはタブ）
            while (std::getline(pifs, pline)) {
                std::vector<std::string> strvec = paxs::StringExtensions::split(pline, '\t');
                // 格納
                location_point_list.emplace_back(
                    strvec[0], // 漢字
                    strvec[1], // ローマ字
                    paxs::EquirectangularDeg(
                        paxs::Vector2<double>(
                            std::stod(strvec[2]), // 経度
                            std::stod(strvec[3]))).toMercatorDeg(), // 緯度
                    std::stod(strvec[4]), // 最小サイズ
                    std::stod(strvec[5]), // 最大サイズ
                    std::stod(strvec[6]), // 最小時代
                    std::stod(strvec[7]), // 出典
                    lpe_,
                    strvec[8] // 最大時代
                );
            }
        }
    };

    // エージェントの位置を管理
    class AgentLocation {
    public:
        // アイコンのテクスチャ
        const s3d::Texture texture_blue_circle{ U"./../../../../../Data/MiniIcon/BlueCircle.svg" };
        const s3d::Texture texture_red_circle{ U"./../../../../../Data/MiniIcon/RedCircle.svg" };

        void draw(const double jdn,
            const std::vector<paxs::Agent<int>>& agents, const paxs::Vector2<int>& start_position,
            const double map_view_width, const double map_view_height, const double map_view_center_x, const double map_view_center_y
            /*, const s3d::Font& font*/
            /*, const s3d::Font& en_font*/
        /*, const s3d::Font& pin_font*/)const {

            // 地名を描画
            for (std::size_t i = 0; i < agents.size(); ++i) {
                // エージェントの初期設定を定義
                const auto lli = LocationPoint{
                        "","",
                        paxs::MercatorDeg(agents[i].getLocation(start_position, 10)),
                        100,0,0,99999999,
                        (agents[i].getGender()) ?
                        LocationPointEnum::location_point_agent :
                        LocationPointEnum::location_point_agent2
                        ,""
                };
                // 範囲外を除去
                if (lli.coordinate.x < (map_view_center_x - map_view_width / 1.8)
                    || lli.coordinate.x >(map_view_center_x + map_view_width / 1.8)
                    || lli.coordinate.y < (map_view_center_y - map_view_height / 1.8)
                    || lli.coordinate.y >(map_view_center_y + map_view_height / 1.8)) continue;

                // 範囲内の場合
                if (lli.min_view > map_view_width
                    || lli.max_view < map_view_width
                    || lli.min_year > jdn
                    || lli.max_year < jdn) {
                    if (lli.min_year > jdn) continue;
                    if (lli.max_year < jdn) continue;

                    // エージェント
                    if (lli.lpe == LocationPointEnum::location_point_agent) {
                        texture_blue_circle.resized(15).drawAt(
                            s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                            double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height())) });
                        continue;
                    }
                    // エージェント
                    if (lli.lpe == LocationPointEnum::location_point_agent2) {
                        texture_red_circle.resized(15).drawAt(
                            s3d::Vec2{ (lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(s3d::Scene::Width()),
                            double(s3d::Scene::Height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(s3d::Scene::Height())) });
                        continue;
                    }
                }
            }
        }

    };

}

#endif // !PAX_SAPIENTICA_SIV3D_LOCATION_POINT_HPP
