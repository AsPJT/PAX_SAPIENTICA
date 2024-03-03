/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_VIEWER_HPP
#define PAX_MAHOROBA_MAP_VIEWER_HPP

/*##########################################################################################

##########################################################################################*/

#include <memory>
#include <new>

#include <PAX_MAHOROBA/LocationPoint.hpp>
#include <PAX_MAHOROBA/LocationRange.hpp> // TextureLocation
#include <PAX_MAHOROBA/PersonLocation.hpp> // PersonLocation 人物を描画
#include <PAX_MAHOROBA/StringViewer.hpp> // 文字

#include <PAX_GRAPHICA/Key.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#ifdef PAXS_USING_SIMULATOR
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>
#endif

namespace paxs {

    class MapViewerSiv3D {
    public:


        std::unique_ptr<TextureLocation> texture_location; // 地図上に描画する画像の一覧
#ifdef PAXS_USING_SIV3D
        s3d::Array<s3d::Vec2> route1, route2; // 線の情報を格納
#endif
        PlaceNameLocation place_name_location{}; // 地名
        PersonNameLocation person_name_location{}; // 人名

    private:
#ifdef PAXS_USING_SIMULATOR
        std::unique_ptr<AgentLocation> agent_location; // エージェント
#endif
    public:

        MapViewerSiv3D()
            :texture_location(std::unique_ptr<TextureLocation>(new(std::nothrow) TextureLocation))
#ifdef PAXS_USING_SIMULATOR
            ,agent_location(std::unique_ptr<AgentLocation>(new(std::nothrow) AgentLocation))
#endif
        {}

        void init() {
#ifdef PAXS_USING_SIV3D
            //// 航路を読み込み
            //std::ifstream rifs(AppConfig::getInstance()->getRootPath() + "Data/Route/Yamatai.tsv");
            //if (rifs.fail()) return;
            //std::string rline;
            //while (std::getline(rifs, rline)) { // ファイルを 1 行ずつ読み込む
            //    const std::vector<std::string> strvec = paxs::StringExtensions::split(rline, '\t');
            //    const MercatorDeg coordinate = EquirectangularDeg(
            //        paxs::Vector2(std::stod(strvec[0]), std::stod(strvec[1])) // 経緯度を格納
            //    );
            //    route1.emplace_back(coordinate.x, coordinate.y);
            //}
            //for (std::size_t i = 0; i < route1.size(); ++i) {
            //    // 経路を格納
            //    route2.emplace_back(
            //        (route1[i].x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(paxg::Window::width()),
            //        double(paxg::Window::height()) - ((route1[i].y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(paxg::Window::height()))
            //    );
            //}
#endif

            // 地名
            place_name_location.init();
            place_name_location.add();
            person_name_location.init();
            person_name_location.add();

        }

        void update(
            MapView& map_view,
            const SelectLanguage& select_language,
            const paxs::KoyomiSiv3D& koyomi_siv,
            paxs::StringViewerSiv3D& string_siv,
#ifdef PAXS_USING_SIMULATOR
            std::unique_ptr<paxs::SettlementSimulator>& simulator,
            std::size_t& pop_num, // 人口数
            std::size_t& sat_num, // 集落数
#endif
            paxs::GraphicVisualizationList& visible
            ) {
            if (visible[MurMur3::calcHash("Map")]) { // 地図が「可視」の場合は描画する
                // 地図上に画像を描画する
                texture_location->update(map_view.getCenterX(), map_view.getCenterY(), map_view.getWidth(), map_view.getHeight());
            }
#ifdef PAXS_USING_SIMULATOR
            if (visible[MurMur3::calcHash("Simulation")]) {
                if (agent_location.get() != nullptr && simulator.get() != nullptr) {
                    agent_location->draw(koyomi_siv.jdn.cgetDay(), simulator->getSettlementGrids(), map_view.getWidth(), map_view.getHeight(), map_view.getCenterX(), map_view.getCenterY(),
                        pop_num,
                        sat_num
                    );
                }
            }
#endif
            if (visible[MurMur3::calcHash("Map")]) { // 地図が「可視」の場合は描画する
                // 地図上に画像を描画する
                texture_location->update(map_view.getCenterX(), map_view.getCenterY(), map_view.getWidth(), map_view.getHeight());
#ifdef PAXS_USING_SIV3D
                //// 線の描画
                //for (std::size_t i = 0; i < route2.size(); ++i) {
                //    route2[i] = s3d::Vec2(
                //        (route1[i].x - (map_view->getCenterX() - map_view->getWidth() / 2)) / map_view->getWidth() * double(paxg::Window::width()),
                //        double(paxg::Window::height()) - ((route1[i].y - (map_view->getCenterY() - map_view->getHeight() / 2)) / map_view->getHeight() * double(paxg::Window::height()))
                //    );
                //}
                //// 航路を描画
                //s3d::Spline2D{ route2 }.draw(2, s3d::Color{ 85, 145, 245 });
#endif

                // フォントを指定
                paxg::Font* one_font = string_siv.language_fonts.getAndAdd(select_language.cgetKey(), static_cast<std::uint_least8_t>(string_siv.koyomi_font_size), static_cast<std::uint_least8_t>(string_siv.koyomi_font_buffer_thickness_size));

                place_name_location.draw(
                    koyomi_siv.jdn.cgetDay(),
                    map_view.getWidth(),
                    map_view.getHeight(),
                    map_view.getCenterX(),
                    map_view.getCenterY(),
                    (one_font == nullptr) ? string_siv.pin_font : (*one_font),
                    string_siv.en_font,
                    string_siv.pin_font);
                person_name_location.draw(
                    koyomi_siv.jdn.cgetDay(),
                    map_view.getWidth(),
                    map_view.getHeight(),
                    map_view.getCenterX(),
                    map_view.getCenterY(),
                    (one_font == nullptr) ? string_siv.pin_font : (*one_font),
                    string_siv.en_font,
                    string_siv.pin_font);

            }

        }


    };

}

#endif // !PAX_MAHOROBA_MAP_VIEWER_HPP
