/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_MAP_VIEWER_HPP
#define PAX_SAPIENTICA_SIV3D_MAP_VIEWER_HPP

/*##########################################################################################

##########################################################################################*/

#include <memory>
#include <new>

#include <PAX_SAPIENTICA/Siv3D/Key.hpp> // MapView
#include <PAX_SAPIENTICA/Siv3D/LocationRange.hpp> // TextureLocation
#include <PAX_SAPIENTICA/Siv3D/StringViewer.hpp> // 文字

namespace paxs {

    class MapViewerSiv3D {
    public:

        std::unique_ptr<MapView> map_view;
        std::unique_ptr<TextureLocation> texture_location; // 地図上に描画する画像の一覧
        s3d::Array<s3d::Vec2> route1, route2; // 線の情報を格納
        std::unique_ptr<PlaceNameLocation> place_name_location{}; // 地名

        //#ifdef PAXS_USING_SIMULATOR
        std::unique_ptr<AgentLocation> agent_location; // エージェント
        //#endif

        /*##########################################################################################
            読み込む XYZ タイルの情報を記載
            map_name			地図名
            map_url_name		地図画像を取得する URL
            map_license_name	ライセンス情報
        ##########################################################################################*/
        std::unordered_map<std::string, XYZTile> xyz_tile_list;

    public:

        MapViewerSiv3D()
            :map_view(new(std::nothrow) MapView),
            texture_location(std::unique_ptr<TextureLocation>(new(std::nothrow) TextureLocation))
        {}

        void init(
            const std::string& path8
        ) {
            // 航路を読み込み
            std::ifstream rifs(path8 + "Data/Route/Yamatai.tsv");
            if (rifs.fail()) return;
            std::string rline;
            while (std::getline(rifs, rline)) { // ファイルを 1 行ずつ読み込む
                const std::vector<std::string> strvec = paxs::StringExtensions::split(rline, '\t');
                const MercatorDeg coordinate = EquirectangularDeg(
                    paxs::Vector2(std::stod(strvec[0]), std::stod(strvec[1])) // 経緯度を格納
                );
                route1.emplace_back(coordinate.x, coordinate.y);
            }
            for (std::size_t i = 0; i < route1.size(); ++i) {
                // 経路を格納
                route2.emplace_back(
                    (route1[i].x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
                    double(s3d::Scene::Height()) - ((route1[i].y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height()))
                );
            }

            /*##########################################################################################
                読み込む XYZ タイルの情報を記載
                map_name			地図名
                map_url_name		地図画像を取得する URL
                map_license_name	ライセンス情報
            ##########################################################################################*/
            mapMapInit(xyz_tile_list, path8, map_view.get());

            // 地名
            place_name_location = std::unique_ptr<PlaceNameLocation>(new(std::nothrow) PlaceNameLocation);
            if (place_name_location != nullptr) {
                place_name_location->add();
            }
            //#ifdef PAXS_USING_SIMULATOR
            agent_location = std::unique_ptr<AgentLocation>(new(std::nothrow) AgentLocation); // エージェント
            //agent_location->addKofun();
//#endif
        }

        void update(
            const SelectLanguage& select_language,
            const paxs::KoyomiSiv3D& koyomi_siv,
            const paxs::StringViewerSiv3D& string_siv,
            const paxs::Simulator<int>& simlator,
            const paxs::Vector2<int>& start_position,
            paxs::GraphicVisualizationList& visible
        ) {
            map_view->update(); // キーボード入力を更新
            mapMapUpdate(xyz_tile_list, string_siv.menu_bar, map_view.get()); // 地図の辞書を更新

            if (visible["Map"]) {
                // 地図上に画像を描画する
                texture_location->update(map_view->getCenterX(), map_view->getCenterY(), map_view->getWidth(), map_view->getHeight());
#ifdef PAXS_USING_SIMULATOR
                agent_location->draw(koyomi_siv.jdn.cgetDay(), simlator.cgetAgents(), start_position, map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()
                    //,font[language], font[language]/*en_font*/, pin_font
                );
#endif
                // 線の描画
                for (std::size_t i = 0; i < route2.size(); ++i) {
                    route2[i] = s3d::Vec2(
                        (route1[i].x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(s3d::Scene::Width()),
                        double(s3d::Scene::Height()) - ((route1[i].y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(s3d::Scene::Height()))
                    );
                }
                // 航路を描画
                s3d::Spline2D{ route2 }.draw(2, s3d::Color{ 85, 145, 245 });

                // 地名を描画
                place_name_location->draw(koyomi_siv.jdn.cgetDay(), map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
                    string_siv.font[select_language.cget()], string_siv.font[select_language.cget()]/*en_font*/, string_siv.pin_font);

                //#ifdef PAXS_USING_SIMULATOR
                //			// エージェント機能テスト
                //			if (is_agent_update) {
                //				static int step = 0;
                //				if (step == 0) {
                //					//s3d::Print(U"Agent Size:", simlator.getAgents().size());
                //					//for (std::size_t i = 0; i < 10 && i < simlator.getAgents().size(); ++i) {
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
                //			;				agent_location->draw(simlator.getAgents(), start_position, map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
                //				font[language], font[language]/*en_font*/, pin_font);
                //#endif
            }

        }


    };

}

#endif // !PAX_SAPIENTICA_SIV3D_MAP_VIEWER_HPP
