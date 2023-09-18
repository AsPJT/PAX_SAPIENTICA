/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_XYZ_TILES_LIST_HPP
#define PAX_SAPIENTICA_SIV3D_XYZ_TILES_LIST_HPP

/*##########################################################################################

##########################################################################################*/

#include <string>
#include <unordered_map>

#include <PAX_SAPIENTICA/Siv3D/Key.hpp>
#include <PAX_SAPIENTICA/Siv3D/Pulldown.hpp>
#include <PAX_SAPIENTICA/Siv3D/XYZTiles.hpp>

namespace paxs {

    // 単体
    void mapMapInitOne(
        std::unordered_map<std::string, XYZTile>& xyz_tile_list,
        const std::string& path,
        const MapView* const map_view,
        const std::string& map_key,
        const std::string& map_url,
        const std::string& map_name,
        const std::string& map_file_path,
        const int min_z = -1,
        const int max_z = -1,
        const int draw_min_z = -1,
        const int draw_max_z = -1
    ) {
        const std::unique_ptr<XYZTile> xyz_tile_base(new(std::nothrow) XYZTile(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()));
        if (map_url.size() != 0) xyz_tile_base->setMapURL(map_url);
        if (map_name.size() != 0) xyz_tile_base->setMapName(map_name);
        if (min_z >= 0) xyz_tile_base->setMinZ(min_z);
        if (max_z >= 0) xyz_tile_base->setMaxZ(max_z);
        if (draw_min_z >= 0) xyz_tile_base->setDrawMinZ(draw_min_z);
        if (draw_max_z >= 0) xyz_tile_base->setDrawMaxZ(draw_max_z);
        xyz_tile_base->setMapFilePath(path + map_file_path);
        xyz_tile_list.emplace(map_key, *xyz_tile_base);
    }

    // 複数
    void mapMapInit(
        std::unordered_map<std::string, XYZTile>& xyz_tile_list,
        const std::string& path,
        const MapView* const map_view
    ) {
        // 使用するマップの XYZ タイルの情報を定義

        //mapMapInitOne(xyz_tile_list, path, map_view,
        //	"map_base", "","", "Data/Map/XYZTile/Standard/Image/Land/2023/", -1, 7, -1, 7);
        {
            const std::unique_ptr<XYZTile> xyz_tile_base(new(std::nothrow) XYZTile(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()));
            xyz_tile_base->setMapURL("");
            xyz_tile_base->setMapName("");
            xyz_tile_base->setMaxZ(7);
            xyz_tile_base->setDrawMaxZ(7);
            xyz_tile_base->setMapFilePath(path + "Data/Map/XYZTile/Standard/Image/Land/2023/");
            //xyz_tile_base->setMapFilePath(path + "Data/Map/XYZTile/LandAndSea/Image/GreenAndAlphaBinary/2023/");

            xyz_tile_list.emplace("map_base", *xyz_tile_base);
        }
        {
            const std::unique_ptr<XYZTile> xyz_tile2(new(std::nothrow) XYZTile(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()));
            xyz_tile2->setMapURL("");
            xyz_tile2->setMapName("gbank");
            xyz_tile2->setMinZ(8);
            xyz_tile2->setMaxZ(10);
            xyz_tile2->setDrawMinZ(8);
            xyz_tile2->setMapFilePath(path + "Data/Map/XYZTile/LandAndWater/Image/GreenAndAlphaBinary/1868/");
            //xyz_tile2->setMapFilePath(path + "Data/Map/XYZTile/LandAndWater/Image/BlackAndWhiteBinary/1868/");
            //xyz_tile2->setMapFilePath("./SavedMap/gbank_noblank_reclaimed_land/");

            xyz_tile_list.emplace("map_land_and_water", *xyz_tile2);
        }
        {
            //const std::unique_ptr<XYZTile> xyz_tile3(new(std::nothrow) XYZTile(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()));
            //xyz_tile3->setMapURL("");
            ////xyz_tile3->setMapURL("https://aginfo.cgk.affrc.go.jp/ws/tmc/1.0.0/KSJ_Water-900913-L");
            //xyz_tile3->setMapName("water");
            //xyz_tile3->setDefaultZ(12);
            //xyz_tile3->setDrawMinZ(11);
            //xyz_tile3->setMapFilePath("./SavedMap/BlueRiver/");
        }
        {
            //const std::unique_ptr<XYZTile> xyz_tile4(new(std::nothrow) XYZTile(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()));
            //xyz_tile4->setMapURL("");
            //xyz_tile4->setMapName("soil");
            //xyz_tile4->setDefaultZ(12);
            //xyz_tile4->setMapFilePath("./SavedMap/Soil/soil/");
        }
        {
            const std::unique_ptr<XYZTile> xyz_tile_kuni(new(std::nothrow) XYZTile(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()));
            xyz_tile_kuni->setMapURL("");
            xyz_tile_kuni->setMapName("ColorCodingByProvincesOfJapanWithLakes");
            xyz_tile_kuni->setDefaultZ(10);
            xyz_tile_kuni->setDrawMinZ(9);
            xyz_tile_kuni->setMapFilePath(path + "Data/Map/XYZTile/ColorCodingByProvincesOfJapan/");

            xyz_tile_list.emplace("map_ryosei_country", *xyz_tile_kuni);
        }
        {
            const std::unique_ptr<XYZTile> xyz_tile_kuni_line(new(std::nothrow) XYZTile(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()));
            xyz_tile_kuni_line->setMapURL("");
            xyz_tile_kuni_line->setMapName("Ryoseikoku");
            xyz_tile_kuni_line->setDefaultZ(10);
            xyz_tile_kuni_line->setDrawMinZ(11);
            xyz_tile_kuni_line->setMapFilePath(path + "Data/Map/XYZTile/Ryoseikoku/");

            xyz_tile_list.emplace("map_ryosei_line", *xyz_tile_kuni_line);
        }
        {
            const std::unique_ptr<XYZTile> xyz_tile_soil(new(std::nothrow) XYZTile(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()));
            xyz_tile_soil->setMapURL("");
            xyz_tile_soil->setMapName("soil");
            xyz_tile_soil->setDefaultZ(12);
            xyz_tile_soil->setDrawMinZ(12);
            xyz_tile_soil->setMapFilePath(path + "Data/Map/XYZTile/Soil/Image/Soil/2023/");

            xyz_tile_list.emplace("map_soil", *xyz_tile_soil);
        }
        //{
        //	const std::unique_ptr<XYZTile> xyz_tile_slope(new(std::nothrow) XYZTile(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()));
        //	xyz_tile_slope->setMapURL("");
        //	xyz_tile_slope->setMapName("slope");
        //	//xyz_tile_slope->setMinZ(3);
        //	//xyz_tile_slope->setMaxZ(12);
        //	//xyz_tile_slope->setDefaultZ(8);
        //	xyz_tile_slope->setMaxZ(8);
        //	//xyz_tile_slope->setDrawMinZ(3);
        //	//xyz_tile_slope->setMapFilePath(path + "Data/Map/XYZTile/Slope/Image/Slope/2023/");
        //	xyz_tile_slope->setMapFilePath(path + "Data/Map/XYZTile/Slope/Image/Slope/2010/");

        //	xyz_tile_list.emplace("map_slope", *xyz_tile_slope);
        //}
        mapMapInitOne(xyz_tile_list, path, map_view,
            "map_slope", "", "slope", "Data/Map/XYZTile/Slope/Image/Slope/2010/", -1, 8, -1, -1);
        {
            const std::unique_ptr<XYZTile> xyz_tile_river(new(std::nothrow) XYZTile(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()));
            xyz_tile_river->setMapURL("");
            xyz_tile_river->setMapName("water");
            xyz_tile_river->setDefaultZ(12);
            xyz_tile_river->setDrawMinZ(11);
            xyz_tile_river->setMapFilePath(path + "Data/Map/XYZTile/RiversAndLakes/Image/RiversAndLakes/2023/");

            xyz_tile_list.emplace("map_river1", *xyz_tile_river);
        }
        {
            const std::unique_ptr<XYZTile> xyz_tile_river_bw(new(std::nothrow) XYZTile(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()));
            xyz_tile_river_bw->setMapURL("");
            xyz_tile_river_bw->setMapName("water");
            xyz_tile_river_bw->setDefaultZ(12);
            xyz_tile_river_bw->setDrawMinZ(11);
            xyz_tile_river_bw->setMapFilePath(path + "Data/Map/XYZTile/RiversAndLakes/Image/RiversAndLakesBW/2023/");

            xyz_tile_list.emplace("map_river2", *xyz_tile_river_bw);
        }
        {
            const std::unique_ptr<XYZTile> xyz_tile_soil_temperature(new(std::nothrow) XYZTile(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()));
            xyz_tile_soil_temperature->setMapURL("");
            xyz_tile_soil_temperature->setMapName("soil-tem");
            xyz_tile_soil_temperature->setDrawMinZ(8);
            xyz_tile_soil_temperature->setDrawMaxZ(10);
            xyz_tile_soil_temperature->setMapFilePath(path + "Data/Map/XYZTile/Temperature/Image/SoilTemperature/2023/");

            xyz_tile_list.emplace("map_soil_temperature", *xyz_tile_soil_temperature);
        }
        {
            //const std::unique_ptr<XYZTile> xyz_tile_line(new(std::nothrow) XYZTile(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()));
            //xyz_tile_line->setDefaultZ(8);
            //xyz_tile_line->setDrawMinZ(5);
        }
        {
            //const std::unique_ptr<XYZTile> xyz_tile_line2(new(std::nothrow) XYZTile(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()));
            //xyz_tile_line2->setDefaultZ(10);
            //xyz_tile_line2->setDrawMinZ(7);
        }
        {
            const std::unique_ptr<XYZTile> xyz_tile_line3(new(std::nothrow) XYZTile(map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY()));
            //xyz_tile_line3->setDefaultZ(12);
            //xyz_tile_line3->setDrawMinZ(12);
            //xyz_tile_line3->setDrawMaxZ(16);

            xyz_tile_list.emplace("map_line3", *xyz_tile_line3);
        }
    }

    // 地図の辞書を更新
    void mapMapUpdate(
        std::unordered_map<std::string, XYZTile>& xyz_tile_list,
        const paxs::MenuBar& menu_bar,
        const MapView* const map_view) {

        const double map_view_width = map_view->getWidth();
        const double map_view_height = map_view->getHeight();
        const double map_view_center_x = map_view->getCenterX();
        const double map_view_center_y = map_view->getCenterY();
        // 未使用
        //const double map_view_center_lat = std::asin(std::tanh(map_view_center_y / 180.0 * paxs::pi)) / (paxs::pi) * 180.0;

        // タイルを更新
        //xyz_tile1"].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::base)) {
            xyz_tile_list["map_base"].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y, paxs::XYZTile::XYZTileFileName::Default);
        }
        if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::soil)) {
            xyz_tile_list["map_soil"].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y, paxs::XYZTile::XYZTileFileName::Z_Original);
        }
        if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::soil_temperature)) {
            xyz_tile_list["map_soil_temperature"].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y, paxs::XYZTile::XYZTileFileName::Z_Original);
        }
        if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::land_and_water)) {
            xyz_tile_list["map_land_and_water"].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y, paxs::XYZTile::XYZTileFileName::Z_Original);
        }
        //xyz_tile3"].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::ryosei_country)) {
            xyz_tile_list["map_kuni"].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y, paxs::XYZTile::XYZTileFileName::Z_Original);
        }
        if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::ryosei_line)) {
            xyz_tile_list["map_kuni_line"].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y, paxs::XYZTile::XYZTileFileName::Z_Original);
        }
        if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::slope)) {
            xyz_tile_list["map_slope"].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y, paxs::XYZTile::XYZTileFileName::Z_Original);
        }
        if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::river1)) {
            xyz_tile_list["map_river"].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y, paxs::XYZTile::XYZTileFileName::Z_Original);
        }
        if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::river2)) {
            xyz_tile_list["map_river_bw"].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y, paxs::XYZTile::XYZTileFileName::Z_Original);
        }
        //xyz_tile4"].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y, paxs::XYZTileFileName::Original);
    //if (menu_bar.getPulldown(MenuBarType::map).getIsItems(MapType::line1)) {
    //	xyz_tile_list["map_line"].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
    //}
    //if (menu_bar.getPulldown(MenuBarType::map).getIsItems(MapType::line2)) {
    //	xyz_tile_list["map_line2"].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
    //}
        if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::line2)) {
            xyz_tile_list["map_line3"].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        }
        /*##########################################################################################
            地図描画関連
        ##########################################################################################*/


        //if (menu_bar_view.getIsItems(1)) {
        if (menu_bar.cgetPulldown(MenuBarType::view).getIsItems(1)) {
            // XYZ タイルの地図の描画
            //xyz_tile1"].draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
            if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::base)) {
                xyz_tile_list.at("map_base").draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
            }
            if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::soil)) {
                xyz_tile_list.at("map_soil").draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
            }
            if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::land_and_water)) {
                xyz_tile_list.at("map_land_and_water").draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
            }
            if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::soil_temperature)) {
                xyz_tile_list.at("map_soil_temperature").draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
            }
            //xyz_tile2"].draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y);

            if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::ryosei_country)) {
                xyz_tile_list.at("map_kuni").draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
            }
            if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::ryosei_line)) {
                xyz_tile_list.at("map_kuni_line").draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
            }
            if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::slope)) {
                xyz_tile_list.at("map_slope").draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
            }
            if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::river1)) {
                xyz_tile_list.at("map_river").draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
            }
            if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::river2)) {
                xyz_tile_list.at("map_river_bw").draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
            }
            //xyz_tile3"].draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
            //xyz_tile4"].draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
            //if (menu_bar.getPulldown(MenuBarType::map).getIsItems(MapType::line1)) {
            //	xyz_tile_list.at("map_line").drawLine(map_view_width, map_view_height, map_view_center_x, map_view_center_y, 0.8, s3d::Color{ 95,99,104 }/*s3d::Palette::Black*/);
            //}
            //if (menu_bar.getPulldown(MenuBarType::map).getIsItems(MapType::line2)) {
            //	xyz_tile_list.at("map_line2").drawLine(map_view_width, map_view_height, map_view_center_x, map_view_center_y, 0.4, s3d::Color{ 95,99,104 }/*s3d::Palette::Black*/);
            //}
            if (menu_bar.cgetPulldown(MenuBarType::map).getIsItems(MapType::line2)) {
                xyz_tile_list.at("map_line3").drawLine(map_view_width, map_view_height, map_view_center_x, map_view_center_y, 0.8, s3d::Color{ 95, 99, 104 }/*s3d::Palette::Black*/);
                xyz_tile_list.at("map_line3").drawXYZ(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
            }
        }

    }
}

#endif // !PAX_SAPIENTICA_SIV3D_XYZ_TILES_LIST_HPP
