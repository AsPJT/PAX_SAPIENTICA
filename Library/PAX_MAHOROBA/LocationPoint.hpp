/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_LOCATION_POINT_HPP
#define PAX_MAHOROBA_LOCATION_POINT_HPP

#ifndef PAXS_PATH
#define PAXS_PATH ""
#endif

#include <cstdint>
#include <string>
#include <unordered_map>

#include <PAX_MAHOROBA/Init.hpp>
#include <PAX_SAPIENTICA/Simulation/Agent.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>
#include <PAX_SAPIENTICA/MapProjection.hpp> // 地図投影法
#include <PAX_SAPIENTICA/MurMur3.hpp>

#include <PAX_GRAPHICA/InputFile.hpp>
#include <PAX_GRAPHICA/String.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Window.hpp>

/*##########################################################################################

##########################################################################################*/

namespace paxs {
    // ユリウス日
    //int jdn = 1600407;
    //int jdn = 1808020;

    // ロケーションポイントの種類
    //    location_point_place_name, // 地名
    //    location_point_pit_dwelling, // 集落遺跡
    //    location_point_agent, // エージェント
    //    location_point_agent2, // エージェント
    //    location_point_zempo_koen_fun, // 前方後円墳
    //    location_point_zempo_koho_fun, // 前方後方墳
    //    location_point_hotategai_gata_kofun // 帆立貝型古墳

    // 地名
    struct LocationPoint {
        constexpr explicit LocationPoint() = default;
        explicit LocationPoint(
            const std::unordered_map<std::uint_least32_t, std::string>& place_name_,  // 地名
            const paxs::MercatorDeg& coordinate_,  // 経緯度
            const double min_view_,  // 可視化する地図の最小範囲
            const double max_view_,  // 可視化する地図の最大範囲
            const int min_year_,  // 可視化する時代（古い年～）
            const int max_year_,  // 可視化する時代（～新しい年）
            const std::uint_least32_t lpe_,  // 対象となる地物の種別
            const std::uint_least32_t& source_ // 出典
        ) noexcept
            : place_name(place_name_), coordinate(coordinate_), min_view(min_view_), max_view(max_view_), min_year(min_year_), max_year(max_year_), lpe(lpe_), source(source_) {}
        std::unordered_map<std::uint_least32_t, std::string> place_name{}; // 地名
        paxs::MercatorDeg coordinate{}; // 経緯度
        double min_view{}, max_view{}; // 可視化する地図の拡大縮小範囲
        int min_year{}, max_year{}; // 可視化する時代（古い年～新しい年）
        std::uint_least32_t lpe = 0; // 対象となる地物の種別
        std::uint_least32_t source = 0; // 出典
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
                        std::unordered_map < std::uint_least32_t, std::string>(),
                        paxs::MercatorDeg(agents[i].getLocation(start_position, 10)),
                        100,0,0,99999999,
                        (agents[i].getGender()) ?
                        MurMur3::calcHash("location_point_agent") :
                        MurMur3::calcHash("location_point_agent2")
                        ,0 /*出典無し*/

                    }
                );
            }
        }
        void add() {
            // 古墳
            inputPlace(std::string("Data/PlaceName/TestMap/Hokkaido.tsv"), MurMur3::calcHash("location_point_zempo_koen_fun"));
            // 古墳
            inputPlace(std::string("Data/PlaceName/TestMap/Kofun.tsv"), MurMur3::calcHash("location_point_zempo_koen_fun"));

            // 古事記の地名
            inputPlace(std::string("Data/PlaceName/KojikiPlaceName.tsv"));
            // 汎用的な地名
            inputPlace(std::string("Data/PlaceName/PlaceName.tsv"));
            // 令制国名
            inputPlace(std::string("Data/PlaceName/Ryoseikoku.tsv"));
            // 東北アイヌ
            inputPlace(std::string("Data/PlaceName/TohokuAynu.tsv"));
            // 渤海
            inputPlace(std::string("Data/PlaceName/Balhae.tsv"));
            // 三国史記
            inputPlace(std::string("Data/PlaceName/SamgukSagi.tsv"));
            // 倭名類聚抄の地名
            inputPlace(std::string("Data/PlaceName/WamyoRuijushoPlaceName.tsv"));
            // 倭名類聚抄の地名
            inputPlace(std::string("Data/PlaceName/WamyoRuijushoKori.tsv"));
            // おもろさうしの地名
            inputPlace(std::string("Data/PlaceName/OmoroSoshiPlaceName.tsv"));
            // 中国大陸の地名
            inputPlace(std::string("Data/PlaceName/China.tsv"));
            // 『日本歴史地名大系』地名項目データセット（重いので必要な時だけ使う）
            // inputPlace(std::string("Data/PlaceName/Geoshape/NRCT.tsv"));
        }

        PlaceNameLocation() {

            // アイコンのテクスチャ
            texture_ko = paxg::Texture{ PAXS_PATH + std::string("Data/OldDocumentIcon/JP-Kojiki.svg") };
            texture_wam = paxg::Texture{ PAXS_PATH + std::string("Data/OldDocumentIcon/JP-WamyoRuijusho.svg") };
            texture_pin1 = paxg::Texture{ PAXS_PATH + std::string("Data/Pin/PitDwelling.svg") };
            texture_blue_circle = paxg::Texture{ PAXS_PATH + std::string("Data/MiniIcon/BlueCircle.svg") };
            texture_red_circle = paxg::Texture{ PAXS_PATH + std::string("Data/MiniIcon/RedCircle.svg") };
            texture_kofun1 = paxg::Texture{ PAXS_PATH + std::string("Data/MiniIcon/ZempoKoenFun.svg") };
            texture_kofun2 = paxg::Texture{ PAXS_PATH + std::string("Data/MiniIcon/ZempoKohoFun.svg")
            };
            texture_kofun3 = paxg::Texture{ PAXS_PATH + std::string("Data/MiniIcon/HotategaiGataKofun.svg") };
            texture_pn = paxg::Texture{ PAXS_PATH + std::string("Data/MiniIcon/PlaceName.svg") };
        }
        // 描画
        void draw(const double jdn,
            const double map_view_width, const double map_view_height, const double map_view_center_x, const double map_view_center_y,
            paxg::Font& font, paxg::Font& en_font, paxg::Font& pin_font)const {

            const std::uint_least32_t first_language = MurMur3::calcHash("language_ja_jp");
            const std::uint_least32_t second_language = MurMur3::calcHash("language_en_us");

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

                    // 描画位置
                    const paxg::Vec2i draw_pos = paxg::Vec2i{
static_cast<int>((lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
    static_cast<int>(double(paxg::Window::height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                    };

                    // エージェントを描画
                    if (lli.lpe == MurMur3::calcHash("location_point_agent")) {
                        texture_blue_circle.resizedDrawAt(15, draw_pos);
                        continue;
                    }
                    // エージェントを描画
                    if (lli.lpe == MurMur3::calcHash("location_point_agent2")) {
                        texture_red_circle.resizedDrawAt(15, draw_pos);
                        continue;
                    }

                    switch (lli.source) {
                    case MurMur3::calcHash("Iseki"): // 遺跡を描画
                        texture_blue_circle.resizedDrawAt(10, draw_pos);
                        break;
                    case MurMur3::calcHash("ZempoKoenFun"): // 前方後円墳を描画
#if defined(PAXS_USING_DXLIB)
                        // paxg::Rect(draw_pos, paxg::Vec2i{14, 14}).drawAt(paxg::Color{255, 255, 255});
                        // paxg::Rect(draw_pos, paxg::Vec2i{10, 10}).drawAt(paxg::Color{37, 158, 78});
#elif defined(PAXS_USING_SFML)
                        paxg::Rect(draw_pos, paxg::Vec2i{ 14, 14 }).drawAt(paxg::Color{ 255, 255, 255 });
                        paxg::Rect(draw_pos, paxg::Vec2i{ 10, 10 }).drawAt(paxg::Color{ 37, 158, 78 });
#endif
                        texture_kofun1.resizedDrawAt(14, draw_pos);
                        break;
                    case MurMur3::calcHash("ZempoKohoFun"): // 前方後方墳を描画
                        texture_kofun2.resizedDrawAt(14, draw_pos);
                        break;
                    case MurMur3::calcHash("HotategaiGataKofun"): // 帆立貝型古墳を描画
                        texture_kofun3.resizedDrawAt(14, draw_pos);
                        break;
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

                // 描画位置
                const paxg::Vec2i draw_pos = paxg::Vec2i{
static_cast<int>((lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
    static_cast<int>(double(paxg::Window::height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                };

                switch (lli.source) {
                case MurMur3::calcHash("JP-Kojiki"): // 古事記のアイコンを描画
                    texture_ko.resizedDrawAt(35, draw_pos);
                    break;
                case MurMur3::calcHash("JP-WamyoRuijusho"): // 倭名類聚抄のアイコンを描画
                    // texture_wam.resizedDrawAt(35, draw_pos);
                    break;
                case MurMur3::calcHash("Iseki"): // 遺跡のアイコンを描画
                    texture_blue_circle.resizedDrawAt(35, draw_pos);
                    break;
                case MurMur3::calcHash("ZempoKoenFun"): // 前方後円墳のアイコンを描画
                    texture_kofun1.resizedDrawAt(35, draw_pos);
                    break;
                case MurMur3::calcHash("ZempoKohoFun"): // 前方後方墳のアイコンを描画
                    texture_kofun2.resizedDrawAt(35, draw_pos);
                    break;
                case MurMur3::calcHash("HotategaiGataKofun"): // 帆立貝型古墳のアイコンを描画
                    texture_kofun3.resizedDrawAt(35, draw_pos);
                    break;
                }
                // 英語名がない場合
                if (lli.place_name.find(second_language) == lli.place_name.end()) {
                    // 名前を描画
                    if (lli.place_name.find(first_language) != lli.place_name.end()) {
                        font.setOutline(0, 0.6, paxg::Color(255, 255, 255));
                        font.drawAt(lli.place_name.at(first_language)
                            , draw_pos, paxg::Color(0, 0, 0));
                    }
                }
                // 英語名がある場合
                else {
                    // 名前（英語）を描画
                    en_font.setOutline(0, 0.6, paxg::Color(255, 255, 255));
                    en_font.drawBottomCenter(lli.place_name.at(second_language), draw_pos
                        , paxg::Color(0, 0, 0));
                    // 名前を描画
                    if (lli.place_name.find(first_language) != lli.place_name.end()) {
                        font.setOutline(0, 0.6, paxg::Color(255, 255, 255));
                        font.drawTopCenter(lli.place_name.at(first_language)
                            , draw_pos, paxg::Color(0, 0, 0));
                    }
                }
            }
        }
    private:
        std::vector<LocationPoint> location_point_list{}; // 地物の一覧
        // アイコンのテクスチャ
        paxg::Texture texture_ko{};
        paxg::Texture texture_wam{};
        paxg::Texture texture_pin1{};
        paxg::Texture texture_blue_circle{};
        paxg::Texture texture_red_circle{};
        paxg::Texture texture_kofun1{};
        paxg::Texture texture_kofun2{};
        paxg::Texture texture_kofun3{};
        paxg::Texture texture_pn{};

        // 項目の ID を返す
        std::size_t getMenuIndex(const std::unordered_map<std::uint_least32_t, std::size_t>& menu, const std::uint_least32_t& str_) {
            return  (menu.find(str_) != menu.end()) ? menu.at(str_) : SIZE_MAX;
        }

        // 地名を読み込み
        void inputPlace(const std::string& str_, const std::uint_least32_t lpe_ = MurMur3::calcHash("location_point_place_name")) {

            paxg::InputFile pifs(str_, PAXS_PATH);
            if (pifs.fail()) return;

            if (!(pifs.getLine())) {
                return; // 何もない場合
            }
            // 1 行目を読み込む
            std::unordered_map<std::uint_least32_t, std::size_t> menu = pifs.splitHashMapMurMur3('\t');

            const std::size_t longitude = getMenuIndex(menu, MurMur3::calcHash("longitude"));
            if (longitude == SIZE_MAX) return; // 経度がないのはデータにならない
            const std::size_t latitude = getMenuIndex(menu, MurMur3::calcHash("latitude"));
            if (latitude == SIZE_MAX) return; // 緯度がないのはデータにならない

            const std::size_t local_language = getMenuIndex(menu, MurMur3::calcHash("language_ja_jp"));
            const std::size_t english = getMenuIndex(menu, MurMur3::calcHash("language_en_us"));
            const std::size_t minimum_size = getMenuIndex(menu, MurMur3::calcHash("min_size"));
            const std::size_t maximum_size = getMenuIndex(menu, MurMur3::calcHash("max_size"));
            const std::size_t first_julian_day = getMenuIndex(menu, MurMur3::calcHash("first_julian_day"));
            const std::size_t last_julian_day = getMenuIndex(menu, MurMur3::calcHash("last_julian_day"));
            const std::size_t source = getMenuIndex(menu, MurMur3::calcHash("texture"));

            // 地名
            const std::size_t language_ja_jp = getMenuIndex(menu, MurMur3::calcHash("language_ja_jp"));
            const std::size_t language_en_us = getMenuIndex(menu, MurMur3::calcHash("language_en_us"));

            // 1 行ずつ読み込み（区切りはタブ）
            while (pifs.getLine()) {
                std::vector<std::string> strvec = pifs.split('\t');

                // 経度の文字列が空の場合は読み込まない
                if (strvec[longitude].size() == 0) continue;
                // 経度の文字列が空の場合は読み込まない
                if (strvec[latitude].size() == 0) continue;

                // 地名
                std::unordered_map<std::uint_least32_t, std::string> place_name{};
                if (language_ja_jp != SIZE_MAX && strvec[language_ja_jp].size() != 0) {
                    place_name.emplace(MurMur3::calcHash("language_ja_jp"), strvec[language_ja_jp]);
                }
                if (language_en_us != SIZE_MAX && strvec[language_en_us].size() != 0) {
                    place_name.emplace(MurMur3::calcHash("language_en_us"), strvec[language_en_us]);
                }
                // 格納
                location_point_list.emplace_back(
                    place_name,
                    paxs::EquirectangularDeg(
                        paxs::Vector2<double>(
                            std::stod(strvec[longitude]), // 経度
                            std::stod(strvec[latitude]))).toMercatorDeg(), // 緯度
                    (minimum_size == SIZE_MAX) ? 0.0 : std::stod(strvec[minimum_size]), // 最小サイズ
                    (maximum_size == SIZE_MAX) ? 9999.0 : std::stod(strvec[maximum_size]), // 最大サイズ
                    (first_julian_day == SIZE_MAX) ? -99999999 : std::stod(strvec[first_julian_day]), // 最小時代
                    (last_julian_day == SIZE_MAX) ? 99999999 : std::stod(strvec[last_julian_day]), // 最大時代
                    lpe_,
                    (source == SIZE_MAX) ? 0 : MurMur3::calcHash(strvec[source].size(), strvec[source].c_str()) // テクスチャの Key
                );
            }
        }
    };

    // エージェントの位置を管理
    class AgentLocation {
    public:
        // アイコンのテクスチャ
        const paxg::Texture texture_blue_circle{ PAXS_PATH + std::string("Data/MiniIcon/BlueCircle.svg") };
        const paxg::Texture texture_red_circle{ PAXS_PATH + std::string("Data/MiniIcon/RedCircle.svg") };

        void draw(const double jdn,
            const std::vector<paxs::Agent<int>>& agents, const paxs::Vector2<int>& start_position,
            const double map_view_width, const double map_view_height, const double map_view_center_x, const double map_view_center_y
        )const {

            // 地名を描画
            for (std::size_t i = 0; i < agents.size(); ++i) {
                // エージェントの初期設定を定義
                const auto lli = LocationPoint{
                    std::unordered_map < std::uint_least32_t, std::string>(),
                        paxs::MercatorDeg(agents[i].getLocation(start_position, 10)),
                        100,0,0,99999999,
                        (agents[i].getGender()) ?
                        MurMur3::calcHash("location_point_agent") :
                        MurMur3::calcHash("location_point_agent2")
                        ,0 /* 出典なし */
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

                    // 描画位置
                    const paxg::Vec2i draw_pos = paxg::Vec2i{
static_cast<int>((lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
    static_cast<int>(double(paxg::Window::height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                    };

                    // エージェント
                    if (lli.lpe == MurMur3::calcHash("location_point_agent")) {
                        texture_blue_circle.resizedDrawAt(15, draw_pos);
                        continue;
                    }
                    // エージェント
                    if (lli.lpe == MurMur3::calcHash("location_point_agent2")) {
                        texture_red_circle.resizedDrawAt(15, draw_pos);
                        continue;
                    }
                }
            }
        }

    };

}

#endif // !PAX_MAHOROBA_LOCATION_POINT_HPP
