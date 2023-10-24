/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_XYZ_TILES_LIST_HPP
#define PAX_MAHOROBA_XYZ_TILES_LIST_HPP

/*##########################################################################################

##########################################################################################*/

#include <string>
#include <unordered_map>

#include <PAX_MAHOROBA/Pulldown.hpp>
#include <PAX_MAHOROBA/XYZTiles.hpp>

#include <PAX_GRAPHICA/Key.hpp>

namespace paxs {

    // 単体
    void mapMapInitOne(
        std::unordered_map<std::uint_least32_t, XYZTile>& xyz_tile_list,
        const std::string& path,
        const MapView* const map_view,
        const std::uint_least32_t map_key,
        const std::string& map_url,
        const std::string& map_name,
        const std::string& map_file_path,
        const int min_z = -1,
        const int max_z = -1,
        const int draw_min_z = -1,
        const int draw_max_z = -1
    ) {
        XYZTile xyz_tile(
            map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
            MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}"));
        if (map_url.size() != 0) xyz_tile.setMapURL(map_url);
        if (map_name.size() != 0) xyz_tile.setMapName(map_name);
        if (min_z >= 0) xyz_tile.setMinZ(min_z);
        if (max_z >= 0) xyz_tile.setMaxZ(max_z);
        if (draw_min_z >= 0) xyz_tile.setDrawMinZ(draw_min_z);
        if (draw_max_z >= 0) xyz_tile.setDrawMaxZ(draw_max_z);
        xyz_tile.setMapFilePath(path + map_file_path);
        xyz_tile_list.emplace(map_key, xyz_tile);
    }

    // 複数
    void mapMapInit(
        std::unordered_map<std::uint_least32_t, XYZTile>& xyz_tile_list,
        const std::string& path,
        const MapView* const map_view
    ) {
        // 使用するマップの XYZ タイルの情報を定義

        // mapMapInitOne(xyz_tile_list, path, map_view,
        // "map_base", "","", "Data/Map/XYZTile/Standard/Image/Land/2023/", -1, 7, -1, 7);
        {
            XYZTile xyz_tile(
                map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
                MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}"));
            xyz_tile.setMapURL("");
            xyz_tile.setMapName("");
            xyz_tile.setMaxZ(7);
            xyz_tile.setDrawMaxZ(7);
            xyz_tile.setMapFilePath(path + "Data/Map/XYZTile/Standard/Image/Land/2023/");
            // xyz_tile_base->setMapFilePath(path + "Data/Map/XYZTile/LandAndSea/Image/GreenAndAlphaBinary/2023/");

            xyz_tile_list.emplace(MurMur3::calcHash("map_base"), xyz_tile);
        }
        {
            XYZTile xyz_tile(
                map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
                MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}"));
            xyz_tile.setMapURL("");
            xyz_tile.setMapName("gbank");
            xyz_tile.setMinZ(8);
            xyz_tile.setMaxZ(10);
            xyz_tile.setDrawMinZ(8);
            xyz_tile.setMapFilePath(path + "Data/Map/XYZTile/LandAndWater/Image/GreenAndAlphaBinary/1868/");
            // xyz_tile.setMapFilePath(path + "Data/Map/XYZTile/LandAndWater/Image/BlackAndWhiteBinary/1868/");
            // xyz_tile.setMapFilePath("./SavedMap/gbank_noblank_reclaimed_land/");
            xyz_tile_list.emplace(MurMur3::calcHash("map_land_and_water"), xyz_tile);
        }
        //{
        //     const std::unique_ptr<XYZTile> xyz_tile_gmaps(new(std::nothrow) XYZTile(
        //     map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
        //     MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}")));
        //     //xyz_tile_gmaps->setMapURL("https://mt1.google.com/vt/lyrs=m&x={x}&y={y}&z={z}");
        //     xyz_tile_gmaps->setMapURL("http://a.tile.openstreetmap.fr/hot/{z}/{x}/{y}.png");
        //     xyz_tile_gmaps->setMapName("map");
        //     // xyz_tile_gmaps->setDefaultZ(12);
        //     // xyz_tile_gmaps->setDrawMinZ(11);
        //     xyz_tile_gmaps->setMapFilePath(path + "Data/Map/XYZTile/GoogleMaps/");

        //     xyz_tile_list.emplace(MurMur3::calcHash("map_gmaps", *xyz_tile_gmaps);
        //}
        {
            // const std::unique_ptr<XYZTile> xyz_tile3(new(std::nothrow) XYZTile(
            // map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
            // MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}")));
            // xyz_tile3->setMapURL("");
            // // xyz_tile3->setMapURL("https://aginfo.cgk.affrc.go.jp/ws/tmc/1.0.0/KSJ_Water-900913-L");
            // xyz_tile3->setMapName("water");
            // xyz_tile3->setDefaultZ(12);
            // xyz_tile3->setDrawMinZ(11);
            // xyz_tile3->setMapFilePath("./SavedMap/BlueRiver/");
        }
        {
            // const std::unique_ptr<XYZTile> xyz_tile4(new(std::nothrow) XYZTile(
            // map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
            // MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}")));
            // xyz_tile4->setMapURL("");
            // xyz_tile4->setMapName("soil");
            // xyz_tile4->setDefaultZ(12);
            // xyz_tile4->setMapFilePath("./SavedMap/Soil/soil/");
        }
        //{
        //    const std::unique_ptr<XYZTile> xyz_tile_kuni(new(std::nothrow) XYZTile(
        //        map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
        //        MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}")));
        //    xyz_tile_kuni->setMapURL("");
        //    xyz_tile_kuni->setMapName("ColorCodingByProvincesOfJapanWithLakes");
        //    xyz_tile_kuni->setDefaultZ(10);
        //    xyz_tile_kuni->setDrawMinZ(9);
        //    xyz_tile_kuni->setMapFilePath(path + "Data/Map/XYZTile/ColorCodingByProvincesOfJapan/");

        //    xyz_tile_list.emplace(MurMur3::calcHash("map_ryosei_country", *xyz_tile_kuni);
        //}
        //{
        //    const std::unique_ptr<XYZTile> xyz_tile_kuni_line(new(std::nothrow) XYZTile(
        //        map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
        //        MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}")));
        //    xyz_tile_kuni_line->setMapURL("");
        //    xyz_tile_kuni_line->setMapName("Line");
        //    xyz_tile_kuni_line->setMinZ(5);
        //    xyz_tile_kuni_line->setMaxZ(9);
        //    xyz_tile_kuni_line->setDrawMinZ(5);
        //    xyz_tile_kuni_line->setMapFilePath(path + "Data/Map/XYZTile/RyoseikokuLine/");

        //    xyz_tile_list.emplace(MurMur3::calcHash("map_ryosei_line", *xyz_tile_kuni_line);
        //}
        //{
        //    const std::unique_ptr<XYZTile> xyz_tile_kuni_korean_line(new(std::nothrow) XYZTile(
        //        map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
        //        MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}")));
        //    xyz_tile_kuni_korean_line->setMapURL("");
        //    xyz_tile_kuni_korean_line->setMapName("Line");
        //    xyz_tile_kuni_korean_line->setMinZ(7);
        //    xyz_tile_kuni_korean_line->setMaxZ(8);
        //    xyz_tile_kuni_korean_line->setDrawMinZ(6);
        //    xyz_tile_kuni_korean_line->setMinDate(1872272); // J 414-01-01
        //    xyz_tile_kuni_korean_line->setMaxDate(1915370); // J 532-01-01 - 1
        //    xyz_tile_kuni_korean_line->setMapFilePath(path + "Data/Map/XYZTile/KoreanLine/");

        //    xyz_tile_list.emplace(MurMur3::calcHash("map_korean_line", *xyz_tile_kuni_korean_line);
        //}
        //{
        //    const std::unique_ptr<XYZTile> xyz_tile_kuni_balhae_line(new(std::nothrow) XYZTile(
        //        map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
        //        MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}")));
        //    xyz_tile_kuni_balhae_line->setMapURL("");
        //    xyz_tile_kuni_balhae_line->setMapName("balhae_line");
        //    xyz_tile_kuni_balhae_line->setMinZ(7);
        //    xyz_tile_kuni_balhae_line->setMaxZ(8);
        //    xyz_tile_kuni_balhae_line->setDrawMinZ(3);
        //    xyz_tile_kuni_balhae_line->setMinDate(1976003); // J 698-01-01
        //    xyz_tile_kuni_balhae_line->setMaxDate(2059279); // J 926-01-01 - 1
        //    xyz_tile_kuni_balhae_line->setMapFilePath(path + "Data/Map/XYZTile/BalhaeLine/");

        //    xyz_tile_list.emplace(MurMur3::calcHash("map_balhae_line", *xyz_tile_kuni_balhae_line);
        //}
        //{
        //    const std::unique_ptr<XYZTile> xyz_tile_kuni_silla_line(new(std::nothrow) XYZTile(
        //        map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
        //        MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}")));
        //    xyz_tile_kuni_silla_line->setMapURL("");
        //    xyz_tile_kuni_silla_line->setMapName("silla_line");
        //    xyz_tile_kuni_silla_line->setMinZ(7);
        //    xyz_tile_kuni_silla_line->setMaxZ(10);
        //    xyz_tile_kuni_silla_line->setDrawMinZ(3);
        //    xyz_tile_kuni_silla_line->setMinDate(1915371); // J 532-01-01
        //    xyz_tile_kuni_silla_line->setMaxDate(2059279); // J 926-01-01 - 1 // 暫定の値
        //    xyz_tile_kuni_silla_line->setMapFilePath(path + "Data/Map/XYZTile/SillaLine/");

        //    xyz_tile_list.emplace(MurMur3::calcHash("map_silla_line", *xyz_tile_kuni_silla_line);
        //}
        {
            XYZTile xyz_tile(
                map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
                MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}"));
            xyz_tile.setMapURL("");
            xyz_tile.setMapName("zxy");
            xyz_tile.setMinZ(8);
            xyz_tile.setMaxZ(8);
            xyz_tile.setDrawMinZ(7);
            xyz_tile.setMapFilePath(path + "Data/Map/XYZTile/Gokishichido/");

            xyz_tile_list.emplace(MurMur3::calcHash("map_east_asia_line"), xyz_tile);
        }
        {
            XYZTile xyz_tile(
                map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
                MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}"));
            xyz_tile.setMapURL("");
            xyz_tile.setMapName("zxy");
            xyz_tile.setMinZ(12);
            xyz_tile.setMaxZ(12);
            xyz_tile.setDrawMinZ(11);
            xyz_tile.setMapFilePath(path + "Data/Map/XYZTile/Kinai/");
            xyz_tile_list.emplace(MurMur3::calcHash("map_kinai"), xyz_tile);
        }
        {
            XYZTile xyz_tile(
                map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
                MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}"));
            xyz_tile.setMapURL("");
            xyz_tile.setMapName("zxy");
            xyz_tile.setMinZ(16);
            xyz_tile.setMaxZ(16);
            xyz_tile.setDrawMinZ(15);
            xyz_tile.setMapFilePath(path + "Data/Map/XYZTile/Heijokyo/");
            xyz_tile_list.emplace(MurMur3::calcHash("map_heijokyo"), xyz_tile);
        }
        {
            XYZTile xyz_tile(
                map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
                MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}"));
            xyz_tile.setMapURL("");
            xyz_tile.setMapName("soil");
            xyz_tile.setDefaultZ(12);
            xyz_tile.setDrawMinZ(12);
            xyz_tile.setMapFilePath(path + "Data/Map/XYZTile/Soil/Image/Soil/2023/");
            xyz_tile_list.emplace(MurMur3::calcHash("map_soil"), xyz_tile);
        }
        // {
        // const std::unique_ptr<XYZTile> xyz_tile_slope(new(std::nothrow) XYZTile(
        // map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
        // MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}")));
        // xyz_tile_slope->setMapURL("");
        // xyz_tile_slope->setMapName("slope");
        // // xyz_tile_slope->setMinZ(3);
        // // xyz_tile_slope->setMaxZ(12);
        // // xyz_tile_slope->setDefaultZ(8);
        // xyz_tile_slope->setMaxZ(8);
        // // xyz_tile_slope->setDrawMinZ(3);
        // // xyz_tile_slope->setMapFilePath(path + "Data/Map/XYZTile/Slope/Image/Slope/2023/");
        // xyz_tile_slope->setMapFilePath(path + "Data/Map/XYZTile/Slope/Image/Slope/2010/");

        // xyz_tile_list.emplace(MurMur3::calcHash("map_slope", *xyz_tile_slope);
        // }
        mapMapInitOne(xyz_tile_list, path, map_view,
            MurMur3::calcHash("map_slope"), "", "slope_pale", "Data/Map/XYZTile/Slope/Image/SlopePale20230920/2010/", -1, 8, -1, -1);
        {
            XYZTile xyz_tile(
                map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
                MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}"));
            xyz_tile.setMapURL("");
            xyz_tile.setMapName("water");
            xyz_tile.setDefaultZ(12);
            xyz_tile.setDrawMinZ(11);
            xyz_tile.setMapFilePath(path + "Data/Map/XYZTile/RiversAndLakes/Image/RiversAndLakes/2023/");

            xyz_tile_list.emplace(MurMur3::calcHash("map_river1"), xyz_tile);
        }
        {
            XYZTile xyz_tile(
                map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
                MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}"));
            xyz_tile.setMapURL("");
            xyz_tile.setMapName("water");
            xyz_tile.setDefaultZ(12);
            xyz_tile.setDrawMinZ(11);
            xyz_tile.setMapFilePath(path + "Data/Map/XYZTile/RiversAndLakes/Image/RiversAndLakesBW/2023/");

            xyz_tile_list.emplace(MurMur3::calcHash("map_river2"), xyz_tile);
        }
        {
            XYZTile xyz_tile(
                map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
                MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}"));
            xyz_tile.setMapURL("");
            xyz_tile.setMapName("soil-tem");
            xyz_tile.setDrawMinZ(8);
            xyz_tile.setDrawMaxZ(10);
            xyz_tile.setMapFilePath(path + "Data/Map/XYZTile/Temperature/Image/SoilTemperature/2023/");
            xyz_tile_list.emplace(MurMur3::calcHash("map_soil_temperature"), xyz_tile);
        }
        {
            XYZTile xyz_tile(
                map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
                MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}"));
            xyz_tile_list.emplace(MurMur3::calcHash("map_line2"), xyz_tile);
        }
        {
            XYZTile xyz_tile(
                map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
                MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}"));
            xyz_tile_list.emplace(MurMur3::calcHash("map_line3"), xyz_tile);
        }
        {
            XYZTile xyz_tile(
                map_view->getWidth(), map_view->getHeight(), map_view->getCenterX(), map_view->getCenterY(),
                MurMur3::calcHash("{z}/{n}_{z}_{x}_{y}"));
            xyz_tile.setDefaultZ(18);
            xyz_tile_list.emplace(MurMur3::calcHash("map_line4"), xyz_tile);
        }
    }

    // 地図の辞書を更新
    void mapMapUpdate(
        std::unordered_map<std::uint_least32_t, XYZTile>& xyz_tile_list,
        const paxs::MenuBar& menu_bar,
        const MapView* const map_view,
        cal::JDN_F64 jdn) {

        const double map_view_width = map_view->getWidth();
        const double map_view_height = map_view->getHeight();
        const double map_view_center_x = map_view->getCenterX();
        const double map_view_center_y = map_view->getCenterY();
        // 未使用
        // const double map_view_center_lat = std::asin(std::tanh(map_view_center_y / 180.0 * paxs::pi)) / (paxs::pi) * 180.0;

        // タイルを更新
        // xyz_tile1")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        if (!menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_base"))) {
            xyz_tile_list[MurMur3::calcHash("map_base")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        }
        if (!menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_soil"))) {
            xyz_tile_list[MurMur3::calcHash("map_soil")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        }
        if (!menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_soil_temperature"))) {
            xyz_tile_list[MurMur3::calcHash("map_soil_temperature")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        }
        if (!menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_land_and_water"))) {
            xyz_tile_list[MurMur3::calcHash("map_land_and_water")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        }
        // xyz_tile3")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        //if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_ryosei_country"))) {
        //    xyz_tile_list[MurMur3::calcHash("map_ryosei")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        //}
        //if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_ryosei_line"))) {
        //    xyz_tile_list[MurMur3::calcHash("map_ryosei_line")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        //}
        //if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_ryosei_line"))) {
        //    xyz_tile_list[MurMur3::calcHash("map_korean_line")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        //}
        //if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_ryosei_line"))) {
        //    xyz_tile_list[MurMur3::calcHash("map_balhae_line")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        //}
        //if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_ryosei_line"))) {
        //    xyz_tile_list[MurMur3::calcHash("map_silla_line")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        //}
        if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_ryosei_line"))) {
            xyz_tile_list[MurMur3::calcHash("map_east_asia_line")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        }
        if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_ryosei_line"))) {
            xyz_tile_list[MurMur3::calcHash("map_kinai")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        }
        if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_ryosei_line"))) {
            xyz_tile_list[MurMur3::calcHash("map_heijokyo")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        }
        if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_slope"))) {
            xyz_tile_list[MurMur3::calcHash("map_slope")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        }
        if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_slope"))) {
            xyz_tile_list[MurMur3::calcHash("map_gmaps")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        }
        if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_lakes_and_rivers1"))) {
            xyz_tile_list[MurMur3::calcHash("map_river")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        }
        if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_lakes_and_rivers2"))) {
            xyz_tile_list[MurMur3::calcHash("map_river_bw")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        }
        // xyz_tile4")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
    // if (menu_bar.getPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_line1"))) {
    // xyz_tile_list[MurMur3::calcHash("map_line")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
    // }
    // if (menu_bar.getPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_line2"))) {
    // xyz_tile_list[MurMur3::calcHash("map_line2")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
    // }
        if (!menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_line2"))) {
            xyz_tile_list[MurMur3::calcHash("map_line3")].update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
        }
        /*##########################################################################################
            地図描画関連
        ##########################################################################################*/

        // 地図の表示
        if (menu_bar.cgetPulldown(MurMur3::calcHash("view")).getIsItemsKey(MurMur3::calcHash("menu_bar_view_map"))) {

            const int date = static_cast<int>(jdn.cgetDay());

            // XYZ タイルの地図の描画
            // xyz_tile1"].draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            if (!menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_base"))) {
                if (xyz_tile_list.find(MurMur3::calcHash("map_base")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_base")).draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            }
            if (!menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_soil"))) {
                if (xyz_tile_list.find(MurMur3::calcHash("map_soil")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_soil")).draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            }
            if (!menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_land_and_water"))) {
                if (xyz_tile_list.find(MurMur3::calcHash("map_land_and_water")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_land_and_water")).draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            }
            if (!menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_soil_temperature"))) {
                if (xyz_tile_list.find(MurMur3::calcHash("map_soil_temperature")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_soil_temperature")).draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            }
            // xyz_tile2"].draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);

            if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_slope"))) {
                if (xyz_tile_list.find(MurMur3::calcHash("map_slope")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_slope")).draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            }
            if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_slope"))) {
                if (xyz_tile_list.find(MurMur3::calcHash("map_gmaps")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_gmaps")).draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            }
            if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_lakes_and_rivers1"))) {
                if (xyz_tile_list.find(MurMur3::calcHash("map_river")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_river")).draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            }
            if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_lakes_and_rivers2"))) {
                if (xyz_tile_list.find(MurMur3::calcHash("map_river_bw")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_river_bw")).draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            }
            //if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_ryosei_country"))) {
            //    if (xyz_tile_list.find(MurMur3::calcHash("map_ryosei")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_ryosei")).draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            //}
            //if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_ryosei_line"))) {
            //    if (xyz_tile_list.find(MurMur3::calcHash("map_ryosei_line")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_ryosei_line")).draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            //}
            //if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_ryosei_line"))) {
            //    if (xyz_tile_list.find(MurMur3::calcHash("map_korean_line")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_korean_line")).draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            //}
            //if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_ryosei_line"))) {
            //    if (xyz_tile_list.find(MurMur3::calcHash("map_balhae_line")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_balhae_line")).draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            //}
            //if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_ryosei_line"))) {
            //    if (xyz_tile_list.find(MurMur3::calcHash("map_silla_line")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_silla_line")).draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            //}
            if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_ryosei_line"))) {
                if (xyz_tile_list.find(MurMur3::calcHash("map_east_asia_line")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_east_asia_line")).draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            }
            if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_ryosei_line"))) {
                if (xyz_tile_list.find(MurMur3::calcHash("map_kinai")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_kinai")).draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            }
            if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_ryosei_line"))) {
                if (xyz_tile_list.find(MurMur3::calcHash("map_heijokyo")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_heijokyo")).draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            }
            // xyz_tile3"].draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            // xyz_tile4"].draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            // if (menu_bar.getPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_line1"))) {
            // xyz_tile_list.at(MurMur3::calcHash("map_line")).drawLine(map_view_width, map_view_height, map_view_center_x, map_view_center_y, 0.8, paxg::Color{ 95,99,104 });
            // }
            // if (menu_bar.getPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_line2"))) {
            if (xyz_tile_list.find(MurMur3::calcHash("map_line2")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_line2")).drawLine(map_view_width, map_view_height, map_view_center_x, map_view_center_y, 0.4, paxg::Color{ 95, 99, 104 });
            // }
            if (!menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_line2"))) {
                if (xyz_tile_list.find(MurMur3::calcHash("map_line3")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_line3")).drawLine(map_view_width, map_view_height, map_view_center_x, map_view_center_y, 0.8, paxg::Color{ 95, 99, 104 });
                if (xyz_tile_list.find(MurMur3::calcHash("map_line3")) != xyz_tile_list.end()) xyz_tile_list.at(MurMur3::calcHash("map_line3")).drawXYZ(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
            }
            if (!menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(MurMur3::calcHash("menu_bar_map_line2"))) {
                // xyz_tile_list.at(MurMur3::calcHash("map_line4")).drawLine(map_view_width, map_view_height, map_view_center_x, map_view_center_y, 0.8, paxg::Color{ 95, 99, 104 });
                // xyz_tile_list.at(MurMur3::calcHash("map_line4")).drawXYZ(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
            }
        }

    }
}

#endif // !PAX_MAHOROBA_XYZ_TILES_LIST_HPP
