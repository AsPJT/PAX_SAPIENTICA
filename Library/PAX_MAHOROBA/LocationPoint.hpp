/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_LOCATION_POINT_HPP
#define PAX_MAHOROBA_LOCATION_POINT_HPP

#include <cstdint>
#include <algorithm>
#include <string>
#include <unordered_map>

#include <PAX_MAHOROBA/Init.hpp>
#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/GraphicVisualizationList.hpp> // 可視化一覧
#include <PAX_SAPIENTICA/InputFile.hpp>
#ifdef PAXS_USING_SIMULATOR
#include <PAX_SAPIENTICA/Simulation/Agent.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementAgent.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementGrid.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#endif
#include <PAX_SAPIENTICA/StringExtensions.hpp>
#include <PAX_SAPIENTICA/MapProjection.hpp> // 地図投影法
#include <PAX_SAPIENTICA/MurMur3.hpp>

#include <PAX_GRAPHICA/Circle.hpp>
#include <PAX_GRAPHICA/Key.hpp> // キーボード入力
#include <PAX_GRAPHICA/Line.hpp>
#include <PAX_GRAPHICA/String.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Window.hpp>

/*##########################################################################################

##########################################################################################*/

namespace paxs {

    constexpr double days_in_a_year = 365.2425;
    constexpr double julian_day_on_m1_1_1 = 1721060.0;

    // 地名
    struct LocationPoint {
        constexpr explicit LocationPoint() = default;
        explicit LocationPoint(
            const std::unordered_map<std::uint_least32_t, std::string>& place_name_,  // 地名
            const paxs::MercatorDeg& coordinate_,  // 経緯度
            std::uint_least16_t x_size_,  // 重ね枚数
            std::uint_least16_t y_size_,  // 重ね枚数
            const double overall_length_,  // 全長
            const double min_view_,  // 可視化する地図の最小範囲
            const double max_view_,  // 可視化する地図の最大範囲
            const int min_year_,  // 可視化する時代（古い年～）
            const int max_year_,  // 可視化する時代（～新しい年）
            const std::uint_least32_t lpe_,  // 対象となる地物の種別
            const std::uint_least32_t place_texture_, // 出典
            const double zoom_ // 拡大率
        ) noexcept
            : place_name(place_name_), coordinate(coordinate_), x_size(x_size_), y_size(y_size_), overall_length(overall_length_),
            min_view(min_view_), max_view(max_view_), min_year(min_year_), max_year(max_year_), lpe(lpe_), place_texture(place_texture_), zoom(zoom_){}

        std::unordered_map<std::uint_least32_t, std::string> place_name{}; // 地名
        paxs::MercatorDeg coordinate{}; // 経緯度
        std::uint_least16_t x_size = 1;
        std::uint_least16_t y_size = 1;
        double overall_length = 10; // 全長
        double min_view = 0.0, max_view = 9999.0; // 可視化する地図の拡大縮小範囲
        int min_year = -99999999, max_year = 99999999; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t lpe = MurMur3::calcHash("place_name"); // 対象となる地物の種別
        std::uint_least32_t place_texture = 0; // 地物の画像
        double zoom = 1.0; // 拡大率
    };

    // 地物の一覧
    struct LocationPointList {
        std::vector<LocationPoint> location_point_list{}; // 地物の一覧

        paxs::MercatorDeg start_coordinate{}; // 経緯度
        paxs::MercatorDeg end_coordinate{}; // 経緯度
        double min_view = 0.0, max_view = 9999.0; // 可視化する地図の拡大縮小範囲
        int min_year = -99999999, max_year = 99999999; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t lpe = MurMur3::calcHash("place_name"); // 対象となる地物の種別
        std::uint_least32_t place_texture = 0; // 地物の画像

        constexpr explicit LocationPointList() = default;
        explicit LocationPointList(
            const std::vector<LocationPoint>& location_point_list_,  // 地物
            paxs::MercatorDeg start_coordinate_, // 経緯度
            paxs::MercatorDeg end_coordinate_, // 経緯度
            const double min_view_,  // 可視化する地図の最小範囲
            const double max_view_,  // 可視化する地図の最大範囲
            const int min_year_,  // 可視化する時代（古い年～）
            const int max_year_,  // 可視化する時代（～新しい年）
            const std::uint_least32_t lpe_,  // 対象となる地物の種別
            const std::uint_least32_t place_texture_ // 出典
        ) noexcept
            : location_point_list(location_point_list_),
            start_coordinate(start_coordinate_), end_coordinate(end_coordinate_),
            min_view(min_view_), max_view(max_view_), min_year(min_year_), max_year(max_year_), lpe(lpe_), place_texture(place_texture_) {}

    };

    // GUI に描画する地物の情報を管理・描画するクラス
    class PlaceNameLocation {
    private:
        std::vector<LocationPointList> location_point_list_list{}; // 地物の一覧
        // アイコンのテクスチャ
        paxs::KeyValueTSV<paxg::Texture> key_value_tsv;
    public:
        PlaceNameLocation() = default;
#ifdef PAXS_USING_SIMULATOR
        void update(const std::vector<paxs::Agent>& agents, const paxs::Vector2<int>& start_position) {
            // エージェントの設定を更新
            location_point_list_list.resize(0);
            std::vector<LocationPoint> location_point_list{}; // 地物の一覧
            for (std::size_t i = 0; i < agents.size(); ++i) {
                location_point_list.emplace_back(
                    std::unordered_map < std::uint_least32_t, std::string>(),
                    paxs::MercatorDeg(agents[i].getLocation(start_position, 10)),
                    1, 1, 10, 100, 0, 0, 99999999,
                    (agents[i].getGender()) ?
                    MurMur3::calcHash("agent1") :
                    MurMur3::calcHash("agent2")
                    , 0 /*出典無し*/
                    , 1.0 // 拡大率
                );
            }
            location_point_list_list.emplace_back(location_point_list,
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(-180/* 経度 */, -90/* 緯度 */)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(180/* 経度 */, 90/* 緯度 */)).toMercatorDeg(),
                0, 99999999, -99999999, 99999999, 0, 0);
        }
#endif
        // 地物を追加
        void add() {
            std::string str = "";
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("PlaceNames"),
                [&](const std::string& path_) {str = path_; });
            if (str.size() == 0) return;

            paxs::InputFile pifs(str);
            if (pifs.fail()) return;
            // 1 行目を読み込む
            if (!(pifs.getLine())) {
                return; // 何もない場合
            }
            // BOM を削除
            pifs.deleteBOM();
            // 1 行目を分割する
            std::unordered_map<std::uint_least32_t, std::size_t> menu = pifs.splitHashMapMurMur3('\t');

            const std::size_t file_path = getMenuIndex(menu, MurMur3::calcHash("file_path"));
            if (file_path == SIZE_MAX) return; // パスがないのはデータにならない
            const std::size_t place_type = getMenuIndex(menu, MurMur3::calcHash("place_type"));

            const std::size_t minimum_size = getMenuIndex(menu, MurMur3::calcHash("min_size"));
            const std::size_t maximum_size = getMenuIndex(menu, MurMur3::calcHash("max_size"));
            const std::size_t first_julian_day = getMenuIndex(menu, MurMur3::calcHash("first_julian_day"));
            const std::size_t last_julian_day = getMenuIndex(menu, MurMur3::calcHash("last_julian_day"));
            const std::size_t place_texture = getMenuIndex(menu, MurMur3::calcHash("texture"));
            const std::size_t zoom_size = getMenuIndex(menu, MurMur3::calcHash("zoom"));

            const std::size_t first_year = getMenuIndex(menu, MurMur3::calcHash("first_year"));
            const std::size_t last_year = getMenuIndex(menu, MurMur3::calcHash("last_year"));


            // 1 行ずつ読み込み（区切りはタブ）
            while (pifs.getLine()) {
                std::vector<std::string> strvec = pifs.split('\t');

                // パスが空の場合は読み込まない
                if (strvec[file_path].size() == 0) continue;

                // 対象となる地物の種別
                const std::uint_least32_t type = (place_type == SIZE_MAX) ?
                    MurMur3::calcHash("place_name") :
                    ((strvec[place_type].size() == 0) ?
                        MurMur3::calcHash("place_name") :
                        MurMur3::calcHash(strvec[place_type].size(), strvec[place_type].c_str()));
                
                // 可視化する地図の最小範囲
                const double min_view = (minimum_size >= strvec.size()) ?
                    0 : ((strvec[minimum_size].size() == 0) ?
                        0 : std::stod(strvec[minimum_size]));
                // 可視化する地図の最大範囲
                const double max_view = (maximum_size >= strvec.size()) ?
                    99999999.0 : ((strvec[maximum_size].size() == 0) ?
                        99999999.0 : std::stod(strvec[maximum_size]));
                // 可視化する時代（古い年～）
                const int min_year = (first_julian_day >= strvec.size()) ?
                    (first_year >= strvec.size()) ?
                    -99999999 : ((strvec[first_year].size() == 0) ?
                        -99999999 : int(std::stod(strvec[first_year]) * days_in_a_year + julian_day_on_m1_1_1))
                    : ((strvec[first_julian_day].size() == 0) ?
                        (first_year >= strvec.size()) ?
                        -99999999 : ((strvec[first_year].size() == 0) ?
                            -99999999 : int(std::stod(strvec[first_year]) * days_in_a_year + julian_day_on_m1_1_1))
                        : std::stoi(strvec[first_julian_day]));
                // 可視化する時代（～新しい年）
                const int max_year = (last_julian_day >= strvec.size()) ?
                    (last_year >= strvec.size()) ?
                    99999999 : ((strvec[last_year].size() == 0) ?
                        99999999 : int(std::stod(strvec[last_year]) * days_in_a_year + julian_day_on_m1_1_1))
                    : ((strvec[last_julian_day].size() == 0) ?
                        (last_year >= strvec.size()) ?
                        99999999 : ((strvec[last_year].size() == 0) ?
                            99999999 : int(std::stod(strvec[last_year]) * days_in_a_year + julian_day_on_m1_1_1))
                        : std::stoi(strvec[last_julian_day]));

                // 画像
                const std::uint_least32_t place_texture_hash = (place_texture >= strvec.size()) ?
                    0 : ((strvec[place_texture].size() == 0) ?
                        0 : MurMur3::calcHash(strvec[place_texture].size(), strvec[place_texture].c_str()));

                // 対象となる地物の種別
                const double zoom = (zoom_size >= strvec.size()) ?
                    1.0 : ((strvec[zoom_size].size() == 0) ?
                        1.0 : std::stod(strvec[zoom_size]));

                // 地物を追加
                inputPlace(strvec[file_path], min_view, max_view, min_year, max_year, type, place_texture_hash, zoom);
            }
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
            paxg::Font& font, paxg::Font& en_font, paxg::Font& /*pin_font*/) {

            const std::uint_least32_t first_language = MurMur3::calcHash("ja-JP");
            const std::uint_least32_t second_language = MurMur3::calcHash("en-US");

            const std::unordered_map<std::uint_least32_t, paxg::Texture> texture = key_value_tsv.get();

            for (std::size_t h = 0; h < location_point_list_list.size(); ++h) {
                const auto& location_point_list = location_point_list_list[h].location_point_list;

                auto& lll = location_point_list_list[h];
                // 空間の範囲外を除去
                if (lll.end_coordinate.x < (map_view_center_x - map_view_width / 1.6)
                    || lll.start_coordinate.x >(map_view_center_x + map_view_width / 1.6)
                    || lll.end_coordinate.y < (map_view_center_y - map_view_height / 1.6)
                    || lll.start_coordinate.y >(map_view_center_y + map_view_height / 1.6)) continue;
                // 時間の範囲外を除去
                if (lll.min_year > jdn) continue;
                if (lll.max_year < jdn) continue;

                if (!visible[lll.lpe]) continue;

                // 拡大率の範囲外を除去
                // if (lll.min_view > map_view_width || lll.max_view < map_view_width) continue;
                
                // 地名を描画
                for (std::size_t i = 0; i < location_point_list.size(); ++i) {
                    auto& lli = location_point_list[i];
                    // 空間の範囲外を除去
                    if (lli.coordinate.x < (map_view_center_x - map_view_width / 1.6)
                        || lli.coordinate.x >(map_view_center_x + map_view_width / 1.6)
                        || lli.coordinate.y < (map_view_center_y - map_view_height / 1.6)
                        || lli.coordinate.y >(map_view_center_y + map_view_height / 1.6)) continue;
                    // 時間の範囲外を除去
                    if (lli.min_year > jdn) continue;
                    if (lli.max_year < jdn) continue;

                    // 範囲内の場合
                    if (lli.min_view > map_view_height
                        || lli.max_view < map_view_height) {

                        // 描画位置
                        const paxg::Vec2i draw_pos = paxg::Vec2i{
    static_cast<int>((lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
        static_cast<int>(double(paxg::Window::height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                        };

                        // エージェントを描画
                        if (lli.lpe == MurMur3::calcHash("agent1")) {
                            if (texture.find(MurMur3::calcHash("BlueCircle")) != texture.end()) {
                                texture.at(MurMur3::calcHash("BlueCircle")).resizedDrawAt(15, draw_pos);
                            }
                            continue;
                        }
                        // エージェントを描画
                        else if (lli.lpe == MurMur3::calcHash("agent2")) {
                            if (texture.find(MurMur3::calcHash("RedCircle")) != texture.end()) {
                                texture.at(MurMur3::calcHash("RedCircle")).resizedDrawAt(15, draw_pos);
                            }
                            continue;
                        }

                        const std::uint_least32_t place_tex = (lli.place_texture == 0) ? lll.place_texture : lli.place_texture;
                        // 描画
                        if (texture.find(place_tex) != texture.end()) {
                            const int len = int(lli.overall_length / 2 * lli.zoom);
                            if (lli.x_size <= 1) {
                                if (lli.y_size <= 1) {
                                    texture.at(place_tex).resizedDrawAt(len, draw_pos);
                                }
                                else {
                                    for (std::uint_least16_t iy = 0; iy < lli.y_size; ++iy) {
                                        texture.at(place_tex).resizedDrawAt(len, paxg::Vec2i{ draw_pos.x(), draw_pos.y() + iy * 4 });
                                    }
                                }
                            }
                            else {
                                const std::uint_least16_t split_count = 10;
                                if (lli.y_size <= 1) {
                                    for (std::uint_least16_t ix = 0, ixx = 0, iyy = 0; ix < lli.x_size; ++ix, ++ixx) {
                                        if (ix != 0 && ix % split_count == 0) {
                                            ixx = 0;
                                            ++iyy;
                                        }
                                        texture.at(place_tex).resizedDrawAt(len, paxg::Vec2i{ draw_pos.x() + ixx * 6, draw_pos.y() + iyy * 4 });

                                    }
                                }
                                else {
                                    for (std::uint_least16_t iy = 0; iy < lli.y_size; ++iy) {
                                        for (std::uint_least16_t ix = 0; ix < lli.x_size; ++ix) {
                                            texture.at(place_tex).resizedDrawAt(len, paxg::Vec2i{ draw_pos.x() + ix * 4, draw_pos.y() + iy * 4 });
                                        }
                                    }
                                }
                            }
                        }
                        continue;
                    }

                    // 描画位置
                    const paxg::Vec2i draw_pos = paxg::Vec2i{
    static_cast<int>((lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
        static_cast<int>(double(paxg::Window::height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                    };

                    const std::uint_least32_t place_tex = (lli.place_texture == 0) ? lll.place_texture : lli.place_texture;
                    // 描画
                    if (texture.find(place_tex) != texture.end()) {
                        const int len = int(lli.overall_length / 2);
                        if (lli.x_size <= 1) {
                            if (lli.y_size <= 1) {
                                texture.at(place_tex).resizedDrawAt(len, draw_pos);
                            }
                            else {
                                for (std::uint_least16_t iy = 0; iy < lli.y_size; ++iy) {
                                    texture.at(place_tex).resizedDrawAt(len, paxg::Vec2i{ draw_pos.x(), draw_pos.y() + iy * 4 });
                                }
                            }
                        }
                        else {
                            if (lli.y_size <= 1) {
                                for (std::uint_least16_t ix = 0; ix < lli.x_size; ++ix) {
                                    texture.at(place_tex).resizedDrawAt(len, paxg::Vec2i{ draw_pos.x() + ix * 4, draw_pos.y() });
                                }
                            }
                            else {
                                for (std::uint_least16_t iy = 0; iy < lli.y_size; ++iy) {
                                    for (std::uint_least16_t ix = 0; ix < lli.x_size; ++ix) {
                                        texture.at(place_tex).resizedDrawAt(len, paxg::Vec2i{ draw_pos.x() + ix * 4, draw_pos.y() + iy * 4 });
                                    }
                                }
                            }
                        }
                    }
                    // 英語名がない場合
                    if (lli.place_name.find(second_language) == lli.place_name.end()) {
                        // 名前を描画
                        if (lli.place_name.find(first_language) != lli.place_name.end()) {
                            font.setOutline(0, 0.6, paxg::Color(240, 245, 250));
                            font.drawAt(lli.place_name.at(first_language)
                                , draw_pos, paxg::Color(90, 90, 90));
                        }
                    }
                    // 日本語名がない場合
                    else if (lli.place_name.find(first_language) == lli.place_name.end()) {
                        // 名前を描画
                        if (lli.place_name.find(second_language) != lli.place_name.end()) {
                            en_font.setOutline(0, 0.6, paxg::Color(240, 245, 250));
                            en_font.drawAt(lli.place_name.at(second_language)
                                , draw_pos, paxg::Color(90, 90, 90));
                        }
                    }
                    // 英語名がある場合
                    else {
                        // 名前（英語）を描画
                        en_font.setOutline(0, 0.6, paxg::Color(240, 245, 250));
                        en_font.drawBottomCenter(lli.place_name.at(second_language), draw_pos
                            , paxg::Color(90, 90, 90));
                        // 名前を描画
                        if (lli.place_name.find(first_language) != lli.place_name.end()) {
                            font.setOutline(0, 0.6, paxg::Color(240, 245, 250));
                            font.drawTopCenter(lli.place_name.at(first_language)
                                , draw_pos, paxg::Color(90, 90, 90));
                        }
                    }
                }
            }
        }
    private:

        // 項目の ID を返す
        std::size_t getMenuIndex(const std::unordered_map<std::uint_least32_t, std::size_t>& menu, const std::uint_least32_t& str_) const {
            return  (menu.find(str_) != menu.end()) ? menu.at(str_) : SIZE_MAX;
        }

        // 地名を読み込み
        void inputPlace(
            const std::string& str_,
            const double min_view_,  // 可視化する地図の最小範囲
            const double max_view_,  // 可視化する地図の最大範囲
            const int min_year_,  // 可視化する時代（古い年～）
            const int max_year_,  // 可視化する時代（～新しい年）
            const std::uint_least32_t lpe_,  // 対象となる地物の種別
            const std::uint_least32_t place_texture_, // 出典
            const double zoom_ // 拡大率
        ) {

            std::vector<LocationPoint> location_point_list{}; // 地物の一覧

            paxs::InputFile pifs(str_, AppConfig::getInstance()->getRootPath());
            if (pifs.fail()) return;
            // 1 行目を読み込む
            if (!(pifs.getLine())) {
                return; // 何もない場合
            }
            // BOM を削除
            pifs.deleteBOM();
            // 1 行目を分割する
            std::unordered_map<std::uint_least32_t, std::size_t> menu = pifs.splitHashMapMurMur3('\t');

            const std::size_t longitude = getMenuIndex(menu, MurMur3::calcHash("longitude"));
            if (longitude == SIZE_MAX) return; // 経度がないのはデータにならない
            const std::size_t latitude = getMenuIndex(menu, MurMur3::calcHash("latitude"));
            if (latitude == SIZE_MAX) return; // 緯度がないのはデータにならない

            double start_longitude = 180.0; // 始点の経度
            double end_longitude = -180.0; // 終点の経度
            double start_latitude = 90.0; // 始点の緯度
            double end_latitude = -90.0; // 終点の緯度

            // 配列の添え字番号
            const std::size_t overall_length = getMenuIndex(menu, MurMur3::calcHash("overall_length"));
            const std::size_t x_size = getMenuIndex(menu, MurMur3::calcHash("x_size"));
            const std::size_t y_size = getMenuIndex(menu, MurMur3::calcHash("y_size"));
            const std::size_t minimum_size = getMenuIndex(menu, MurMur3::calcHash("min_size"));
            const std::size_t maximum_size = getMenuIndex(menu, MurMur3::calcHash("max_size"));
            const std::size_t first_julian_day = getMenuIndex(menu, MurMur3::calcHash("first_julian_day"));
            const std::size_t last_julian_day = getMenuIndex(menu, MurMur3::calcHash("last_julian_day"));
            const std::size_t place_texture = getMenuIndex(menu, MurMur3::calcHash("texture"));

            const std::size_t first_year = getMenuIndex(menu, MurMur3::calcHash("first_year"));
            const std::size_t last_year = getMenuIndex(menu, MurMur3::calcHash("last_year"));

            // 地名
            const std::size_t language_ja_jp = getMenuIndex(menu, MurMur3::calcHash("ja-JP"));
            const std::size_t language_en_us = getMenuIndex(menu, MurMur3::calcHash("en-US"));

            // 1 行ずつ読み込み（区切りはタブ）
            while (pifs.getLine()) {
                std::vector<std::string> strvec = pifs.split('\t');

                // 経度の文字列が空の場合は読み込まない
                if (longitude >= strvec.size()) continue;
                else if (strvec[longitude].size() == 0) continue;
                // 経度の文字列が空の場合は読み込まない
                if (latitude >= strvec.size()) continue;
                else if (strvec[latitude].size() == 0) continue;

                // 地名
                std::unordered_map<std::uint_least32_t, std::string> place_name{};
                if (language_ja_jp < strvec.size() && strvec[language_ja_jp].size() != 0) {
                    place_name.emplace(MurMur3::calcHash("ja-JP"), strvec[language_ja_jp]);
                }
                if (language_en_us < strvec.size() && strvec[language_en_us].size() != 0) {
                    place_name.emplace(MurMur3::calcHash("en-US"), strvec[language_en_us]);
                }
                bool is_overall_length = false;
                if (overall_length < strvec.size()) {
                    if (strvec[overall_length].size() != 0) is_overall_length = true;
                }
                bool is_x_size = false;
                if (x_size < strvec.size()) {
                    if (strvec[x_size].size() != 0) is_x_size = true;
                }
                bool is_y_size = false;
                if (y_size < strvec.size()) {
                    if (strvec[y_size].size() != 0) is_y_size = true;
                }

                double point_longitude = std::stod(strvec[longitude]); // 経度
                double point_latitude = std::stod(strvec[latitude]); // 緯度
                // 経緯度の範囲を求める
                start_longitude = (std::min)(start_longitude, point_longitude);
                start_latitude = (std::min)(start_latitude, point_latitude);
                end_longitude = (std::max)(end_longitude, point_longitude);
                end_latitude = (std::max)(end_latitude, point_latitude);

                auto first_jd = int((first_julian_day >= strvec.size()) ? min_year_ : (strvec[first_julian_day].size() == 0) ? min_year_ : std::stod(strvec[first_julian_day]));
                if(first_jd == min_year_) first_jd = int((first_year >= strvec.size()) ? min_year_ : (strvec[first_year].size() == 0) ? min_year_ : std::stod(strvec[first_year]) * days_in_a_year + julian_day_on_m1_1_1);

                auto last_jd = int((last_julian_day >= strvec.size()) ? max_year_ : (strvec[last_julian_day].size() == 0) ? max_year_ : std::stod(strvec[last_julian_day]));
                if (last_jd == max_year_) last_jd = int((last_year >= strvec.size()) ? max_year_ : (strvec[last_year].size() == 0) ? max_year_ : std::stod(strvec[last_year]) * days_in_a_year + julian_day_on_m1_1_1);

                // 格納
                location_point_list.emplace_back(
                    place_name,
                    paxs::EquirectangularDeg(
                        paxs::Vector2<double>(
                            point_longitude, // 経度
                            point_latitude)).toMercatorDeg(), // 緯度
                    (!is_x_size) ? 1 : (strvec[x_size].size() == 0) ? 1 : std::stod(strvec[x_size]), // 横の枚数
                    (!is_y_size) ? 1 : (strvec[y_size].size() == 0) ? 1 : std::stod(strvec[y_size]), // 縦の枚数
                    (!is_overall_length) ? 10.0 : (strvec[overall_length].size() == 0) ? 10.0 : std::stod(strvec[overall_length]), // 全長
                    (minimum_size >= strvec.size()) ? min_view_ : (strvec[minimum_size].size() == 0) ? min_view_ : std::stod(strvec[minimum_size]), // 最小サイズ
                    (maximum_size >= strvec.size()) ? max_view_ : (strvec[maximum_size].size() == 0) ? max_view_ : std::stod(strvec[maximum_size]), // 最大サイズ
                    first_jd, // 最小時代
                    last_jd, // 最大時代
                    lpe_,
                    (place_texture >= strvec.size()) ? place_texture_ : MurMur3::calcHash(strvec[place_texture].size(), strvec[place_texture].c_str()), // テクスチャの Key
                    zoom_
                );
            }
            // 地物を何も読み込んでいない場合は何もしないで終わる
            if (location_point_list.size() == 0) return;

            // 読み込んだファイルを格納する
            location_point_list_list.emplace_back(location_point_list,
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(start_longitude/* 経度 */, start_latitude/* 緯度 */)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(end_longitude/* 経度 */, end_latitude/* 緯度 */)).toMercatorDeg(),
                min_view_, max_view_, min_year_, max_year_
                , lpe_, place_texture_);
        }
    };

#ifdef PAXS_USING_SIMULATOR
    // エージェントの位置を管理
    class AgentLocation {
    private:
        std::size_t select_draw = 1;
        // 線を表示するか
        bool is_line = false;
        // 移動線（矢印）を表示するか
        bool is_arrow = true;
#ifdef PAXS_USING_SIV3D
        // 選択肢を表示するフォント
        const s3d::Font select_font{ 30, s3d::Typeface::Bold };
#endif
    public:

        /// @brief Get the mercator coordinate from the XYZTile coordinate.
        /// @brief 座標をメルカトル座標で取得
        paxs::Vector2<double> getLocation(
            const paxs::Vector2<int>& start_position,
            const paxs::Vector2<int>& position,
            const int z) const noexcept {
            return MapUtility::convertToMercatorCoordinate(start_position, position, z);
        }

    private:

        paxg::Color getColor(const std::uint_least8_t pop_) const {
            //// 白黒
            //if (pop_ >= 80) return paxg::Color(0, 0, 0);
            //const std::uint_least8_t color_bw = (80 - pop_) * 2;
            //return paxg::Color(color_bw, color_bw, color_bw);

            // 色付き
            switch (pop_) {
            case 0: return paxg::Color(45, 87, 154); // 青
            case 1: return paxg::Color(36, 92, 156);
            case 2: return paxg::Color(27, 96, 158);
            case 3: return paxg::Color(18, 101, 161);
            case 4: return paxg::Color(9, 105, 163);
            case 5: return paxg::Color(0, 110, 165);
            case 6: return paxg::Color(0, 111, 164);
            case 7: return paxg::Color(0, 112, 163);
            case 8: return paxg::Color(0, 112, 161);
            case 9: return paxg::Color(0, 113, 160);
            case 10: return paxg::Color(0, 114, 159);
            case 11: return paxg::Color(0, 115, 158);
            case 12: return paxg::Color(0, 117, 157);
            case 13: return paxg::Color(0, 118, 156);
            case 14: return paxg::Color(0, 120, 155);
            case 15: return paxg::Color(0, 121, 154);
            case 16: return paxg::Color(0, 125, 148);
            case 17: return paxg::Color(0, 130, 143);
            case 18: return paxg::Color(0, 134, 137);
            case 19: return paxg::Color(0, 139, 132);
            case 20: return paxg::Color(0, 143, 126);
            case 21: return paxg::Color(0, 141, 115);
            case 22: return paxg::Color(0, 138, 104);
            case 23: return paxg::Color(0, 136, 94);
            case 24: return paxg::Color(0, 133, 83);
            case 25: return paxg::Color(0, 131, 72); // 緑
            case 26: return paxg::Color(6, 136, 60);
            case 27: return paxg::Color(13, 141, 48);
            case 28: return paxg::Color(19, 145, 37);
            case 29: return paxg::Color(26, 150, 25);
            case 30: return paxg::Color(32, 155, 13);
            case 31: return paxg::Color(55, 159, 16);
            case 32: return paxg::Color(78, 164, 19);
            case 33: return paxg::Color(101, 168, 23);
            case 34: return paxg::Color(124, 173, 26);
            case 35: return paxg::Color(147, 177, 29);
            case 36: return paxg::Color(156, 182, 26);
            case 37: return paxg::Color(166, 187, 24);
            case 38: return paxg::Color(175, 193, 21);
            case 39: return paxg::Color(185, 198, 19);
            case 40: return paxg::Color(194, 203, 16);
            case 41: return paxg::Color(201, 204, 13);
            case 42: return paxg::Color(208, 205, 10);
            case 43: return paxg::Color(216, 205, 6);
            case 44: return paxg::Color(223, 206, 3);
            case 45: return paxg::Color(230, 207, 0); // 黄
            case 46: return paxg::Color(228, 202, 1);
            case 47: return paxg::Color(226, 197, 2);
            case 48: return paxg::Color(225, 191, 3);
            case 49: return paxg::Color(223, 186, 4);
            case 50: return paxg::Color(221, 181, 5);
            case 51: return paxg::Color(219, 176, 6);
            case 52: return paxg::Color(218, 171, 7);
            case 53: return paxg::Color(216, 165, 8);
            case 54: return paxg::Color(215, 160, 9);
            case 55: return paxg::Color(213, 155, 10);
            case 56: return paxg::Color(210, 138, 12);
            case 57: return paxg::Color(206, 122, 15);
            case 58: return paxg::Color(203, 105, 17);
            case 59: return paxg::Color(199, 89, 20);
            case 60: return paxg::Color(196, 72, 22);
            case 61: return paxg::Color(193, 64, 28);
            case 62: return paxg::Color(189, 56, 34);
            case 63: return paxg::Color(186, 48, 40);
            case 64: return paxg::Color(182, 40, 46); // 赤
            case 65: return paxg::Color(179, 32, 52);
            case 66: return paxg::Color(178, 31, 57);
            case 67: return paxg::Color(176, 30, 62);
            case 68: return paxg::Color(175, 29, 68);
            case 69: return paxg::Color(173, 28, 73);
            case 70: return paxg::Color(172, 27, 78);
            case 71: return paxg::Color(170, 26, 83);
            case 72: return paxg::Color(169, 25, 88);
            case 73: return paxg::Color(167, 24, 92);
            case 74: return paxg::Color(166, 23, 97);
            case 75: return paxg::Color(164, 22, 102);
            case 76: return paxg::Color(131, 18, 82);
            case 77: return paxg::Color(98, 13, 61);
            case 78: return paxg::Color(66, 9, 41);
            case 79: return paxg::Color(33, 4, 20);
            case 80: return paxg::Color(0, 0, 0);
            }
            return paxg::Color(255, 255, 255);
        }

    public:

        void drawText() {
#ifdef PAXS_USING_SIV3D
            constexpr int start_x = 40; // 背景端の左上の X 座標
            constexpr int start_y = 80; // 背景端の左上の Y 座標
            constexpr int font_space = 20; // 文字端から背景端までの幅
            s3d::String text = U""; // 表示するテキスト
            switch (select_draw)
            {
            case 1:
                text = U"1. 人口 Population";
                break;
            case 2:
                text = U"2. 農耕文化 Farming";
                break;
            case 3:
                text = U"3. mtDNA haplogroup";
                break;
            case 4:
                text = U"4. SNP / Genome";
                break;
            case 5:
                text = U"5. 言語 Language";
                break;
            case 6:
                text = U"6. 青銅 Bronze";
                break;
            };
            // 選択項目を描画
            const s3d::RectF rect = select_font(text).region();
            s3d::RoundRect{ start_x, start_y, rect.w + font_space * 2, rect.h + font_space * 2, 10 }.draw();
            select_font(text).draw(start_x + font_space, start_y + font_space, s3d::Palette::Black);
#endif
        }

        void draw(const double jdn,
            std::unordered_map<SettlementGridsType, paxs::SettlementGrid>& agents,
            const std::vector<GridType4>& marriage_pos_list/* SFML では使わない */,
            const double map_view_width, const double map_view_height, const double map_view_center_x, const double map_view_center_y
        )/*const Siv3D Key は非 const */ {
            if (Key(SIV3D_KEY_1).isPressed()) select_draw = 1;
            else if (Key(SIV3D_KEY_2).isPressed()) select_draw = 2;
            else if (Key(SIV3D_KEY_3).isPressed()) select_draw = 3;
            else if (Key(SIV3D_KEY_4).isPressed()) select_draw = 4;
            else if (Key(SIV3D_KEY_5).isPressed()) select_draw = 5;
            else if (Key(SIV3D_KEY_6).isPressed()) select_draw = 6;
            else if (Key(SIV3D_KEY_L).isPressed()) is_line = (!is_line);
            else if (Key(SIV3D_KEY_K).isPressed()) is_arrow = (!is_arrow);
//#ifdef PAXS_USING_SIV3D
//            if (s3d::Key1.pressed()) select_draw = 1;
//            else if (s3d::Key2.pressed()) select_draw = 2;
//            else if (s3d::Key3.pressed()) select_draw = 3;
//            else if (s3d::Key4.pressed()) select_draw = 4;
//            else if (s3d::Key5.pressed()) select_draw = 5;
//            else if (s3d::Key6.pressed()) select_draw = 6;
//            // グリッド線を描画する
//            else if (s3d::KeyL.up()) is_line = (!is_line);
//            else if (s3d::KeyK.up()) is_arrow = (!is_arrow);
//#endif

            // 地名を描画
            for (const auto& agent : agents) {
                for (const auto& settlement : agent.second.cgetSettlements()) {
                    // エージェントの初期設定を定義
                    const auto lli = LocationPoint{
                        std::unordered_map < std::uint_least32_t, std::string>(),
                            paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                            paxs::Vector2<int>(
                    settlement.getPosition().x,settlement.getPosition().y), 10)),
                            1, 1, 10, 100,0,0,99999999,
                        MurMur3::calcHash("agent1")
                        ,0 /* 出典なし */
                        ,1.0 // 拡大率
                    };

                    // 経緯度の範囲外を除去
                    if (lli.coordinate.x < (map_view_center_x - map_view_width / 1.6)
                        || lli.coordinate.x >(map_view_center_x + map_view_width / 1.6)
                        || lli.coordinate.y < (map_view_center_y - map_view_height / 1.6)
                        || lli.coordinate.y >(map_view_center_y + map_view_height / 1.6)) continue;

                    // 範囲内の場合
                    if (lli.min_view > map_view_height || lli.max_view < map_view_height || lli.min_year > jdn || lli.max_year < jdn) {
                        if (lli.min_year > jdn) continue;
                        if (lli.max_year < jdn) continue;

                        // 描画位置
                        const paxg::Vec2i draw_pos = paxg::Vec2i{
    static_cast<int>((lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
        static_cast<int>(double(paxg::Window::height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                        };

                        // エージェント
                        // if (lli.lpe == MurMur3::calcHash("agent1"))
                        if (select_draw != 5) {
                            double pop_original = 0.0;
                            switch (select_draw)
                            {
                            case 1:
                                // const std::size_t
                                // pop_original = settlement.getFarmingPopulation();
                                pop_original = static_cast<double>(settlement.getPopulation());
                                break;
                            case 2:
                                //const float
                                pop_original = settlement.getFarmingPopulation() / float(settlement.getPopulation()) * 75.0;
                                break;
                            case 3:
                                //const float
                                pop_original = settlement.getMostMtDNA() / 27.0 * 75.0;
                                break;
                            case 4:
                                //const double
                                pop_original = settlement.getSNP() * 75.0;
                                break;
                            case 6:
                                pop_original = static_cast<double>(settlement.getBronze());
                                break;
                            }

                            const std::uint_least8_t pop = (pop_original >= 75) ? 75 : static_cast<std::uint_least8_t>(pop_original);
                            paxg::Circle(draw_pos,
                                1.0f + (settlement.getPopulation() / 10.0f)//2.0f
                            ).draw(getColor(pop));
                        }
                        else {
                            paxg::Color language_color = paxg::Color(99, 99, 99); // 灰色
                            switch (settlement.getLanguage())
                            {
                            case 0:
                                language_color = paxg::Color(99, 99, 99); // 灰色
                                break;
                            case 1:
                                language_color = paxg::Color(0, 131, 72); // 緑色
                                break;
                            case 2:
                                language_color = paxg::Color(45, 87, 154); // 青色
                                break;
                            case 3:
                                language_color = paxg::Color(182, 40, 46); // 赤色
                                break;
                            case 4:
                                language_color = paxg::Color(230, 207, 0); // 黄色
                                break;
                            }
                            paxg::Circle(draw_pos,
                                1.0f + (settlement.getPopulation() / 10.0f)//2.0f
                            ).draw(language_color);
                        }

                    }
                }
            }

            // グリッド線を描画
            if (is_line) {
                const auto area_width = SimulationConstants::getInstance()->getEndArea().x - SimulationConstants::getInstance()->getStartArea().x;
                const auto area_height = SimulationConstants::getInstance()->getEndArea().y - SimulationConstants::getInstance()->getStartArea().y;

                const paxs::MercatorDeg start_coordinate = paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                    paxs::Vector2<int>(0, 0), 10));
                const paxg::Vec2f draw_start_pos = paxg::Vec2f{
    static_cast<float>((start_coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
        static_cast<float>(double(paxg::Window::height()) - ((start_coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                };
                const paxs::MercatorDeg end_coordinate = paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                    paxs::Vector2<int>(area_width * 256, area_height * 256), 10));
                const paxg::Vec2f draw_end_pos = paxg::Vec2f{
    static_cast<float>((end_coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
        static_cast<float>(double(paxg::Window::height()) - ((end_coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                };
                const paxs::MercatorDeg tile_coordinate = paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                    paxs::Vector2<int>(SimulationConstants::getInstance()->cell_group_length, SimulationConstants::getInstance()->cell_group_length), 10));
                const paxg::Vec2f tile_pos = paxg::Vec2f{
    static_cast<float>((tile_coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())) - draw_start_pos.x(),
        static_cast<float>(double(paxg::Window::height()) - ((tile_coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height()))) - draw_start_pos.y()
                };

                paxg::Line(
                    static_cast<float>(draw_start_pos.x()), static_cast<float>(draw_start_pos.y()),
                    static_cast<float>(draw_start_pos.x()), static_cast<float>(draw_end_pos.y())
                ).draw(5, paxg::Color(0, 0, 0));
                paxg::Line(
                    static_cast<float>(draw_start_pos.x()), static_cast<float>(draw_start_pos.y()),
                    static_cast<float>(draw_end_pos.x()), static_cast<float>(draw_start_pos.y())
                ).draw(5, paxg::Color(0, 0, 0));
                paxg::Line(
                    static_cast<float>(draw_end_pos.x()), static_cast<float>(draw_start_pos.y()),
                    static_cast<float>(draw_end_pos.x()), static_cast<float>(draw_end_pos.y())
                ).draw(5, paxg::Color(0, 0, 0));
                paxg::Line(
                    static_cast<float>(draw_start_pos.x()), static_cast<float>(draw_end_pos.y()),
                    static_cast<float>(draw_end_pos.x()), static_cast<float>(draw_end_pos.y())
                ).draw(5, paxg::Color(0, 0, 0));

                for (float i = draw_start_pos.x(); i < draw_end_pos.x(); i += tile_pos.x()) {
                    paxg::Line(
                        static_cast<float>(i), static_cast<float>(draw_start_pos.y()),
                        static_cast<float>(i), static_cast<float>(draw_end_pos.y())
                    ).draw(0.5, paxg::Color(0, 0, 0));
                }
                for (float i = draw_start_pos.y(); i < draw_end_pos.y(); i += tile_pos.y()) {
                    paxg::Line(
                        static_cast<float>(draw_start_pos.x()), static_cast<float>(i),
                        static_cast<float>(draw_end_pos.x()), static_cast<float>(i)
                    ).draw(0.5, paxg::Color(0, 0, 0));
                }
            }
#ifdef PAXS_USING_SIV3D
            // 移動線を描画
            if (is_arrow) {
                for (const auto& agent : agents) {
                    for (const auto& settlement : agent.second.cgetSettlements()) {
                        // エージェントの初期設定を定義
                        const auto lli = LocationPoint{
                            std::unordered_map < std::uint_least32_t, std::string>(),
                                paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                                paxs::Vector2<int>(
                        settlement.getPosition().x,settlement.getPosition().y), 10)),
                                1, 1, 10, 100,0,0,99999999,
                            MurMur3::calcHash("agent1")
                            ,0 /* 出典なし */
                            ,1.0 // 拡大率
                        };

                        // 経緯度の範囲外を除去
                        if (lli.coordinate.x < (map_view_center_x - map_view_width / 1.6)
                            || lli.coordinate.x >(map_view_center_x + map_view_width / 1.6)
                            || lli.coordinate.y < (map_view_center_y - map_view_height / 1.6)
                            || lli.coordinate.y >(map_view_center_y + map_view_height / 1.6)) continue;

                        // 範囲内の場合
                        if (lli.min_view > map_view_height || lli.max_view < map_view_height || lli.min_year > jdn || lli.max_year < jdn) {
                            if (lli.min_year > jdn) continue;
                            if (lli.max_year < jdn) continue;

                            // 描画位置
                            const paxg::Vec2i draw_pos = paxg::Vec2i{
        static_cast<int>((lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
            static_cast<int>(double(paxg::Window::height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                            };

                            if (settlement.getOldPosition().x != -1 && settlement.getOldPosition().x != 0) {
                                if (settlement.getPositions().size() >= 1) {

                                    // 過去の位置
                                    auto old_lli = lli;
                                    old_lli.coordinate = paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                                        paxs::Vector2<int>(
                                            settlement.getOldPosition().x,
                                            settlement.getOldPosition().y), 10));
                                    const paxg::Vec2i draw_old_pos = paxg::Vec2i{
            static_cast<int>((old_lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
                static_cast<int>(double(paxg::Window::height()) - ((old_lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                                    };

                                    s3d::Array<s3d::Vec2> va;
                                    va << s3d::Vec2{ draw_pos.x(), draw_pos.y() };
                                    for (auto&& p : settlement.getPositions()) {
                                        auto one_lli = lli;
                                        one_lli.coordinate = paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                                            paxs::Vector2<int>(p.x, p.y), 10));
                                        const paxg::Vec2i draw_one_pos = paxg::Vec2i{
        static_cast<int>((one_lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
            static_cast<int>(double(paxg::Window::height()) - ((one_lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                                        };
                                        va << s3d::Vec2{ draw_one_pos.x(), draw_one_pos.y() };
                                    }
                                    va << s3d::Vec2{ draw_old_pos.x(), draw_old_pos.y() };

                                    const s3d::Spline2D spline(va);
                                    spline.draw(2, Palette::Black);

                                    // 過去の位置
                                    auto one_lli = lli;
                                    one_lli.coordinate = paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                                        paxs::Vector2<int>(
                                            settlement.getPositions()[0].x,
                                            settlement.getPositions()[0].y), 10));
                                    const paxg::Vec2i draw_one_pos = paxg::Vec2i{
            static_cast<int>((one_lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
                static_cast<int>(double(paxg::Window::height()) - ((one_lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                                    };
                                    s3d::Line{ draw_one_pos.x(), draw_one_pos.y(), draw_pos.x(), draw_pos.y() }.drawArrow(0.1, s3d::Vec2{ 8, 16 }, s3d::Palette::Black);
                                }
                                else {
                                    // 過去の位置
                                    auto old_lli = lli;
                                    old_lli.coordinate = paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                                        paxs::Vector2<int>(
                                            settlement.getOldPosition().x,
                                            settlement.getOldPosition().y), 10));
                                    const paxg::Vec2i draw_old_pos = paxg::Vec2i{
            static_cast<int>((old_lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
                static_cast<int>(double(paxg::Window::height()) - ((old_lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                                    };
                                    s3d::Line{ draw_old_pos.x(), draw_old_pos.y(), draw_pos.x(), draw_pos.y() }.drawArrow(2, s3d::Vec2{ 8, 16 }, s3d::Palette::Black);
                                }
                            }

                        }

                    }
                }
                // 移動線を描画
                for (const auto& marriage_pos : marriage_pos_list) {
                    // エージェントの初期設定を定義
                    const auto lli = LocationPoint{
                        std::unordered_map < std::uint_least32_t, std::string>(),
                            paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                            paxs::Vector2<int>(
                    marriage_pos.ex,marriage_pos.ey), 10)),
                            1, 1, 10, 100,0,0,99999999,
                        MurMur3::calcHash("agent1")
                        ,0 /* 出典なし */
                        ,1.0 // 拡大率
                    };

                    // 経緯度の範囲外を除去
                    if (lli.coordinate.x < (map_view_center_x - map_view_width / 1.6)
                        || lli.coordinate.x >(map_view_center_x + map_view_width / 1.6)
                        || lli.coordinate.y < (map_view_center_y - map_view_height / 1.6)
                        || lli.coordinate.y >(map_view_center_y + map_view_height / 1.6)) continue;

                    // 範囲内の場合
                    if (lli.min_view > map_view_height || lli.max_view < map_view_height || lli.min_year > jdn || lli.max_year < jdn) {
                        if (lli.min_year > jdn) continue;
                        if (lli.max_year < jdn) continue;

                        // 描画位置
                        const paxg::Vec2i draw_pos = paxg::Vec2i{
    static_cast<int>((lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
        static_cast<int>(double(paxg::Window::height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                        };

                        if (marriage_pos.sx != -1 && marriage_pos.sx != 0) {
                            // 過去の位置
                            auto old_lli = lli;
                            old_lli.coordinate = paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                                paxs::Vector2<int>(
                                    marriage_pos.sx,
                                    marriage_pos.sy), 10));
                            const paxg::Vec2i draw_old_pos = paxg::Vec2i{
    static_cast<int>((old_lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
        static_cast<int>(double(paxg::Window::height()) - ((old_lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                            };
                            s3d::Line{ draw_old_pos.x(), draw_old_pos.y(), draw_pos.x(), draw_pos.y() }.drawArrow(2, s3d::Vec2{ 8, 16 }, (marriage_pos.is_matrilocality) ? s3d::Color(221, 67, 98) : s3d::Color(87, 66, 221));
                        }
                    }
                }
        }
#endif
        }

    };
#endif

}

#endif // !PAX_MAHOROBA_LOCATION_POINT_HPP
