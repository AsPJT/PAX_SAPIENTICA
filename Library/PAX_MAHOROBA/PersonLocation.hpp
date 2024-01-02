/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_PERSON_LOCATION_HPP
#define PAX_MAHOROBA_PERSON_LOCATION_HPP

#include <cstdint>
#include <algorithm>
#include <string>
#include <unordered_map>

#include <PAX_MAHOROBA/Init.hpp>
#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/InputFile.hpp>
#include <PAX_SAPIENTICA/Simulation/Agent.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementAgent.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementGrid.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>
#include <PAX_SAPIENTICA/MapProjection.hpp> // 地図投影法
#include <PAX_SAPIENTICA/MurMur3.hpp>

#include <PAX_GRAPHICA/Circle.hpp>
#include <PAX_GRAPHICA/String.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Window.hpp>

/*##########################################################################################

##########################################################################################*/

namespace paxs {
    // 地名
    struct PersonLocationPoint {
        constexpr explicit PersonLocationPoint() = default;
        explicit PersonLocationPoint(
            const std::unordered_map<std::uint_least32_t, std::string>& place_name_,  // 地名
            const paxs::MercatorDeg& start_coordinate_,  // 経緯度
            const paxs::MercatorDeg& end_coordinate_,  // 経緯度
            const double overall_length_,  // 全長
            const double min_view_,  // 可視化する地図の最小範囲
            const double max_view_,  // 可視化する地図の最大範囲
            const double min_year_,  // 可視化する時代（古い年～）
            const double max_year_,  // 可視化する時代（～新しい年）
            const std::uint_least32_t lpe_,  // 対象となる地物の種別
            const std::uint_least32_t place_texture_ // 出典
        ) noexcept
            : place_name(place_name_), start_coordinate(start_coordinate_), end_coordinate(end_coordinate_), overall_length(overall_length_),
            min_view(min_view_), max_view(max_view_), min_year(min_year_), max_year(max_year_), lpe(lpe_), place_texture(place_texture_) {}

        std::unordered_map<std::uint_least32_t, std::string> place_name{}; // 地名
        paxs::MercatorDeg start_coordinate{}; // 経緯度
        paxs::MercatorDeg end_coordinate{}; // 経緯度
        double overall_length = 10; // 全長
        double min_view = 0.0, max_view = 9999.0; // 可視化する地図の拡大縮小範囲
        double min_year = -99999999, max_year = 99999999; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t lpe = MurMur3::calcHash("place_name"); // 対象となる地物の種別
        std::uint_least32_t place_texture = 0; // 地物の画像
    };

    // 地物の一覧
    struct PersonLocationList {
        std::vector<PersonLocationPoint> person_location_list{}; // 地物の一覧

        paxs::MercatorDeg start_start_coordinate{}; // 経緯度
        paxs::MercatorDeg start_end_coordinate{}; // 経緯度
        paxs::MercatorDeg end_start_coordinate{}; // 経緯度
        paxs::MercatorDeg end_end_coordinate{}; // 経緯度
        double min_view = 0.0, max_view = 9999.0; // 可視化する地図の拡大縮小範囲
        double min_year = -99999999, max_year = 99999999; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t lpe = MurMur3::calcHash("place_name"); // 対象となる地物の種別
        std::uint_least32_t place_texture = 0; // 地物の画像

        constexpr explicit PersonLocationList() = default;
        explicit PersonLocationList(
            const std::vector<PersonLocationPoint>& location_point_list_,  // 地物
            paxs::MercatorDeg start_start_coordinate_, // 経緯度
            paxs::MercatorDeg start_end_coordinate_, // 経緯度
            paxs::MercatorDeg end_start_coordinate_, // 経緯度
            paxs::MercatorDeg end_end_coordinate_, // 経緯度
            const double min_view_,  // 可視化する地図の最小範囲
            const double max_view_,  // 可視化する地図の最大範囲
            const double min_year_,  // 可視化する時代（古い年～）
            const double max_year_,  // 可視化する時代（～新しい年）
            const std::uint_least32_t lpe_,  // 対象となる地物の種別
            const std::uint_least32_t place_texture_ // 出典
        ) noexcept
            : person_location_list(location_point_list_),
            start_start_coordinate(start_start_coordinate_), start_end_coordinate(start_end_coordinate_),
            end_start_coordinate(end_start_coordinate_), end_end_coordinate(end_end_coordinate_),
            min_view(min_view_), max_view(max_view_), min_year(min_year_), max_year(max_year_), lpe(lpe_), place_texture(place_texture_) {}

    };

    // GUI に描画する地物の情報を管理・描画するクラス
    class PersonNameLocation {
    public:
        void update(const std::vector<paxs::Agent<int>>& agents, const paxs::Vector2<int>& start_position) {
            // エージェントの設定を更新
            location_point_list_list.resize(0);
            std::vector<PersonLocationPoint> person_location_list{}; // 地物の一覧
            for (std::size_t i = 0; i < agents.size(); ++i) {
                person_location_list.emplace_back(
                    std::unordered_map < std::uint_least32_t, std::string>(),
                    paxs::MercatorDeg(agents[i].getLocation(start_position, 10)),
                    paxs::MercatorDeg(agents[i].getLocation(start_position, 10)),
                    10, 100, 0, 0, 99999999,
                    (agents[i].getGender()) ?
                    MurMur3::calcHash("agent1") :
                    MurMur3::calcHash("agent2")
                    , 0 /*出典無し*/
                );
            }
            location_point_list_list.emplace_back(person_location_list,
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(-180/* 経度 */, -90/* 緯度 */)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(180/* 経度 */, 90/* 緯度 */)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(-180/* 経度 */, -90/* 緯度 */)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(180/* 経度 */, 90/* 緯度 */)).toMercatorDeg(),
                0, 99999999, -99999999, 99999999, 0, 0);
        }
        // 地物を追加
        void add() {
            std::string str = "Data/PersonName/List.tsv";
            paxs::InputFile pifs(str, AppConfig::getInstance()->getRootPath());
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
                    -99999999 : ((strvec[first_julian_day].size() == 0) ?
                        -99999999 : std::stoi(strvec[first_julian_day]));
                // 可視化する時代（～新しい年）
                const int max_year = (last_julian_day >= strvec.size()) ?
                    99999999 : ((strvec[last_julian_day].size() == 0) ?
                        99999999 : std::stoi(strvec[last_julian_day]));
                // 画像
                const std::uint_least32_t place_texture_hash = (place_texture >= strvec.size()) ?
                    0 : ((strvec[place_texture].size() == 0) ?
                        0 : MurMur3::calcHash(strvec[place_texture].size(), strvec[place_texture].c_str()));

                // 地物を追加
                inputPlace(strvec[file_path], min_view, max_view, min_year, max_year, type, place_texture_hash);
            }
        }

        PersonNameLocation() = default;
        void init() {
            std::string str = "Data/Portraits/List.tsv";
            paxs::InputFile pifs(str, AppConfig::getInstance()->getRootPath());
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
            const std::size_t place_texture = getMenuIndex(menu, MurMur3::calcHash("texture"));
            if (place_texture == SIZE_MAX) return; // テクスチャ名がないのはデータにならない


            // 1 行ずつ読み込み（区切りはタブ）
            while (pifs.getLine()) {
                std::vector<std::string> strvec = pifs.split('\t');

                // パスが空の場合は読み込まない
                if (strvec[file_path].size() == 0) continue;
                // テクスチャ名が空の場合は読み込まない
                if (strvec[place_texture].size() == 0) continue;

                // 画像
                const std::uint_least32_t place_texture_hash =
                    (place_texture >= strvec.size()) ? 0 : // テクスチャがない場合
                    ((strvec[place_texture].size() == 0) ? 0 : // テクスチャ名がない場合
                        MurMur3::calcHash(strvec[place_texture].size(), strvec[place_texture].c_str()));
                if (place_texture_hash == 0) continue; // ハッシュが 0 の場合は追加しない

                // テクスチャを追加
                texture.emplace(MurMur3::calcHash(strvec[place_texture].size(), strvec[place_texture].c_str()), paxg::Texture{ AppConfig::getInstance()->getRootPath() + strvec[file_path] });
            }
        }
        // 描画
        void draw(const double jdn,
            const double map_view_width, const double map_view_height, const double map_view_center_x, const double map_view_center_y,
            paxg::Font& font, paxg::Font& en_font, paxg::Font& /*pin_font*/)const {

            const std::uint_least32_t first_language = MurMur3::calcHash("ja-JP");
            const std::uint_least32_t second_language = MurMur3::calcHash("en-US");

            for (std::size_t h = 0; h < location_point_list_list.size(); ++h) {
                const auto& person_location_list = location_point_list_list[h].person_location_list;

                auto& lll = location_point_list_list[h];
                //// 空間の範囲外を除去
                //if (lll.start_end_coordinate.x < (map_view_center_x - map_view_width / 0.6)
                //    || lll.start_start_coordinate.x >(map_view_center_x + map_view_width / 0.6)
                //    || lll.start_end_coordinate.y < (map_view_center_y - map_view_height / 0.6)
                //    || lll.start_start_coordinate.y >(map_view_center_y + map_view_height / 0.6)) continue;
                //if (lll.end_end_coordinate.x < (map_view_center_x - map_view_width / 0.6)
                //    || lll.end_start_coordinate.x >(map_view_center_x + map_view_width / 0.6)
                //    || lll.end_end_coordinate.y < (map_view_center_y - map_view_height / 0.6)
                //    || lll.end_start_coordinate.y >(map_view_center_y + map_view_height / 0.6)) continue;
                // 時間の範囲外を除去
                if (lll.min_year > jdn) continue;
                if (lll.max_year < jdn) continue;
                // 拡大率の範囲外を除去
                // if (lll.min_view > map_view_width || lll.max_view < map_view_width) continue;

                // 地名を描画
                for (std::size_t i = 0; i < person_location_list.size(); ++i) {
                    auto& lli = person_location_list[i];
                    //// 空間の範囲外を除去
                    //if (lli.start_coordinate.x < (map_view_center_x - map_view_width / 1.6)
                    //    || lli.start_coordinate.x >(map_view_center_x + map_view_width / 1.6)
                    //    || lli.start_coordinate.y < (map_view_center_y - map_view_height / 1.6)
                    //    || lli.start_coordinate.y >(map_view_center_y + map_view_height / 1.6)) continue;
                    //if (lli.end_coordinate.x < (map_view_center_x - map_view_width / 1.6)
                    //    || lli.end_coordinate.x >(map_view_center_x + map_view_width / 1.6)
                    //    || lli.end_coordinate.y < (map_view_center_y - map_view_height / 1.6)
                    //    || lli.end_coordinate.y >(map_view_center_y + map_view_height / 1.6)) continue;
                    // 時間の範囲外を除去
                    if (lli.min_year > jdn) continue;
                    if (lli.max_year < jdn) continue;

                    // 描画する年月日の変位
                    double view_year_displacement = lli.max_year - lli.min_year;
                    // 今の年月日の変位
                    double jdn_displacement = jdn - lli.min_year;

                    // 年月日の正規化
                    double year_normalization = jdn_displacement / view_year_displacement; // 0.0 から 1.0 の値をとる

                    // 座標の変位
                    double coordinate_displacement_x = lli.end_coordinate.x - lli.start_coordinate.x;
                    double coordinate_displacement_y = lli.end_coordinate.y - lli.start_coordinate.y;

                    // 今の座標
                    double now_coordinate_x = lli.start_coordinate.x + coordinate_displacement_x * year_normalization;
                    double now_coordinate_y = lli.start_coordinate.y + coordinate_displacement_y * year_normalization;


                    // 範囲内の場合
                    if (lli.min_view > map_view_width
                        || lli.max_view < map_view_width) {

                        // -----------------------------------------------------------------------------
                        // -----------------------------------------------------------------------------
                        // -----------------------------------------------------------------------------
                        // -----------------------------------------------------------------------------

                        // 描画位置（後で変える）
                        const paxg::Vec2i draw_pos = paxg::Vec2i{
    static_cast<int>((now_coordinate_x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
        static_cast<int>(double(paxg::Window::height()) - ((now_coordinate_y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
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
                        const int len = int(lli.overall_length / 2);

                        const std::uint_least32_t place_tex = (lli.place_texture == 0) ? lll.place_texture : lli.place_texture;
                        // 描画
                        if (texture.find(place_tex) != texture.end()) {
                            texture.at(place_tex).resizedDrawAt(len, draw_pos);
                        }
                        continue;
                    }


                    // -----------------------------------------------------------------------------
                    // -----------------------------------------------------------------------------
                    // -----------------------------------------------------------------------------
                    // -----------------------------------------------------------------------------


                    // 描画位置
                    const paxg::Vec2i draw_pos = paxg::Vec2i{
    static_cast<int>((now_coordinate_x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
        static_cast<int>(double(paxg::Window::height()) - ((now_coordinate_y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                    };

                    const paxg::Vec2i draw_font_pos = paxg::Vec2i{
                        draw_pos.x(), draw_pos.y() - 80
                    };

                    const std::uint_least32_t place_tex = (lli.place_texture == 0) ? lll.place_texture : lli.place_texture;
                    // 描画
                    if (texture.find(place_tex) != texture.end()) {
                        texture.at(place_tex).resizedDrawAt(300, draw_pos);
                    }
                    // 英語名がない場合
                    if (lli.place_name.find(second_language) == lli.place_name.end()) {
                        // 名前を描画
                        if (lli.place_name.find(first_language) != lli.place_name.end()) {
                            font.setOutline(0, 0.6, paxg::Color(255, 255, 255));
                            font.drawAt(lli.place_name.at(first_language)
                                , draw_font_pos, paxg::Color(0, 0, 0));
                        }
                    }
                    // 英語名がある場合
                    else {
                        // 名前（英語）を描画
                        en_font.setOutline(0, 0.6, paxg::Color(255, 255, 255));
                        en_font.drawBottomCenter(lli.place_name.at(second_language), draw_font_pos
                            , paxg::Color(0, 0, 0));
                        // 名前を描画
                        if (lli.place_name.find(first_language) != lli.place_name.end()) {
                            font.setOutline(0, 0.6, paxg::Color(255, 255, 255));
                            font.drawTopCenter(lli.place_name.at(first_language)
                                , draw_font_pos, paxg::Color(0, 0, 0));
                        }
                    }
                }
            }
        }
    private:
        std::vector<PersonLocationList> location_point_list_list{}; // 地物の一覧
        // アイコンのテクスチャ
        std::unordered_map<std::uint_least32_t, paxg::Texture> texture{};

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
            const std::uint_least32_t place_texture_ // 出典
        ) {

            std::vector<PersonLocationPoint> person_location_list{}; // 地物の一覧

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

            const std::size_t start_longitude = getMenuIndex(menu, MurMur3::calcHash("start_longitude"));
            if (start_longitude == SIZE_MAX) return; // 経度がないのはデータにならない
            const std::size_t start_latitude = getMenuIndex(menu, MurMur3::calcHash("start_latitude"));
            if (start_latitude == SIZE_MAX) return; // 緯度がないのはデータにならない

            const std::size_t end_longitude = getMenuIndex(menu, MurMur3::calcHash("end_longitude"));
            if (end_longitude == SIZE_MAX) return; // 経度がないのはデータにならない
            const std::size_t end_latitude = getMenuIndex(menu, MurMur3::calcHash("end_latitude"));
            if (end_latitude == SIZE_MAX) return; // 緯度がないのはデータにならない

            double start_start_longitude = 180.0; // 始点の経度
            double start_end_longitude = -180.0; // 終点の経度
            double start_start_latitude = 90.0; // 始点の緯度
            double start_end_latitude = -90.0; // 終点の緯度

            double end_start_longitude = 180.0; // 始点の経度
            double end_end_longitude = -180.0; // 終点の経度
            double end_start_latitude = 90.0; // 始点の緯度
            double end_end_latitude = -90.0; // 終点の緯度

            // 配列の添え字番号
            const std::size_t overall_length = getMenuIndex(menu, MurMur3::calcHash("overall_length"));
            const std::size_t minimum_size = getMenuIndex(menu, MurMur3::calcHash("min_size"));
            const std::size_t maximum_size = getMenuIndex(menu, MurMur3::calcHash("max_size"));
            const std::size_t first_julian_day = getMenuIndex(menu, MurMur3::calcHash("first_julian_day"));
            const std::size_t last_julian_day = getMenuIndex(menu, MurMur3::calcHash("last_julian_day"));
            const std::size_t place_texture = getMenuIndex(menu, MurMur3::calcHash("texture"));

            // 地名
            const std::size_t language_ja_jp = getMenuIndex(menu, MurMur3::calcHash("ja-JP"));
            const std::size_t language_en_us = getMenuIndex(menu, MurMur3::calcHash("en-US"));

            // 1 行ずつ読み込み（区切りはタブ）
            while (pifs.getLine()) {
                std::vector<std::string> strvec = pifs.split('\t');

                // 経度の文字列が空の場合は読み込まない
                if (strvec[start_longitude].size() == 0) continue;
                // 経度の文字列が空の場合は読み込まない
                if (strvec[start_latitude].size() == 0) continue;
                // 経度の文字列が空の場合は読み込まない
                if (strvec[end_longitude].size() == 0) continue;
                // 経度の文字列が空の場合は読み込まない
                if (strvec[end_latitude].size() == 0) continue;

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

                double start_point_longitude = std::stod(strvec[start_longitude]); // 経度
                double start_point_latitude = std::stod(strvec[start_latitude]); // 緯度
                // 経緯度の範囲を求める
                start_start_longitude = (std::min)(start_start_longitude, start_point_longitude);
                start_start_latitude = (std::min)(start_start_latitude, start_point_latitude);
                start_end_longitude = (std::max)(start_end_longitude, start_point_longitude);
                start_end_latitude = (std::max)(start_end_latitude, start_point_latitude);

                double end_point_longitude = std::stod(strvec[end_longitude]); // 経度
                double end_point_latitude = std::stod(strvec[end_latitude]); // 緯度
                // 経緯度の範囲を求める
                end_start_longitude = (std::min)(end_start_longitude, end_point_longitude);
                end_start_latitude = (std::min)(end_start_latitude, end_point_latitude);
                end_end_longitude = (std::max)(end_end_longitude, end_point_longitude);
                end_end_latitude = (std::max)(end_end_latitude, end_point_latitude);

                // 格納
                person_location_list.emplace_back(
                    place_name,
                    paxs::EquirectangularDeg(
                        paxs::Vector2<double>(
                            start_point_longitude, // 経度
                            start_point_latitude)).toMercatorDeg(), // 緯度
                    paxs::EquirectangularDeg(
                    paxs::Vector2<double>(
                        end_point_longitude, // 経度
                        end_point_latitude)).toMercatorDeg(), // 緯度
                    (!is_overall_length) ? 1000.0 : std::stod(strvec[overall_length]), // 全長
                    (minimum_size >= strvec.size()) ? min_view_ : std::stod(strvec[minimum_size]), // 最小サイズ
                    (maximum_size >= strvec.size()) ? max_view_ : std::stod(strvec[maximum_size]), // 最大サイズ
                    (first_julian_day >= strvec.size()) ? min_year_ : std::stod(strvec[first_julian_day]), // 最小時代
                    (last_julian_day >= strvec.size()) ? max_year_ : std::stod(strvec[last_julian_day]), // 最大時代
                    lpe_,
                    (place_texture >= strvec.size()) ? place_texture_ : MurMur3::calcHash(strvec[place_texture].size(), strvec[place_texture].c_str()) // テクスチャの Key
                );
            }
            // 地物を何も読み込んでいない場合は何もしないで終わる
            if (person_location_list.size() == 0) return;

            // 読み込んだファイルを格納する
            location_point_list_list.emplace_back(person_location_list,
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(start_start_longitude/* 経度 */, start_start_latitude/* 緯度 */)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(start_end_longitude/* 経度 */, start_end_latitude/* 緯度 */)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(end_start_longitude/* 経度 */, end_start_latitude/* 緯度 */)).toMercatorDeg(),
                paxs::EquirectangularDeg(
                    paxs::Vector2<double>(end_end_longitude/* 経度 */, end_end_latitude/* 緯度 */)).toMercatorDeg(),
                min_view_, max_view_, min_year_, max_year_
                , lpe_, place_texture_);
        }
    };

}

#endif // !PAX_MAHOROBA_PERSON_LOCATION_HPP
