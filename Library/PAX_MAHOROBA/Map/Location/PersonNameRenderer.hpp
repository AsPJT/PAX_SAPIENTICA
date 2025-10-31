/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_PERSON_NAME_RENDERER_HPP
#define PAX_MAHOROBA_PERSON_NAME_RENDERER_HPP

#include <cstdint>
#include <vector>

#include <PAX_GRAPHICA/Circle.hpp>
#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Map/Location/PersonNameRepository.hpp>

#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/UnorderedMap.hpp>

namespace paxs {

    /// @brief 人物名の描画を担当するクラス (Presentation Layer)
    class PersonNameRenderer {
    public:
        PersonNameRenderer() = default;

        /// @brief 人物名を描画
        void draw(
            const std::vector<PersonLocationList>& location_point_list_list,
            const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>& texture,
            const double jdn,
            const double map_view_width,
            const double map_view_height,
            const double map_view_center_x,
            const double map_view_center_y,
            paxg::Font& font,
            paxg::Font& en_font,
            paxg::Font& /*pin_font*/
        ) const {
            const std::uint_least32_t ja_jp_language = MurMur3::calcHash("ja-JP");
            const std::uint_least32_t en_us_language = MurMur3::calcHash("en-US");

            for (std::size_t h = 0; h < location_point_list_list.size(); ++h) {
                const auto& person_location_list = location_point_list_list[h].person_location_list;

                auto& lll = location_point_list_list[h];
                // 時間の範囲外を除去
                if (lll.min_year > jdn) continue;
                if (lll.max_year < jdn) continue;

                // 人物名を描画
                for (std::size_t i = 0; i < person_location_list.size(); ++i) {
                    auto& lli = person_location_list[i];
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
                    if (lli.min_view > map_view_width || lli.max_view < map_view_width) {
                        drawPersonIconOnly(
                            texture, lll, lli,
                            now_coordinate_x, now_coordinate_y,
                            map_view_width, map_view_height,
                            map_view_center_x, map_view_center_y
                        );
                        continue;
                    }

                    // アイコンとテキストを描画
                    drawPersonIconAndText(
                        texture, lll, lli,
                        now_coordinate_x, now_coordinate_y,
                        map_view_width, map_view_height,
                        map_view_center_x, map_view_center_y,
                        font, en_font,
                        ja_jp_language, en_us_language
                    );
                }
            }
        }

    private:
        /// @brief アイコンのみを描画
        void drawPersonIconOnly(
            const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>& texture,
            const PersonLocationList& lll,
            const PersonLocationPoint& lli,
            double now_coordinate_x,
            double now_coordinate_y,
            double map_view_width,
            double map_view_height,
            double map_view_center_x,
            double map_view_center_y
        ) const {
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
                return;
            }
            // エージェントを描画
            else if (lli.lpe == MurMur3::calcHash("agent2")) {
                if (texture.find(MurMur3::calcHash("RedCircle")) != texture.end()) {
                    texture.at(MurMur3::calcHash("RedCircle")).resizedDrawAt(15, draw_pos);
                }
                return;
            }
            const int len = int(lli.overall_length / 2);

            const std::uint_least32_t place_tex = (lli.place_texture == 0) ? lll.place_texture : lli.place_texture;
            // 描画
            if (texture.find(place_tex) != texture.end()) {
                texture.at(place_tex).resizedDrawAt(len, draw_pos);
            }
        }

        /// @brief アイコンとテキストを描画
        void drawPersonIconAndText(
            const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>& texture,
            const PersonLocationList& lll,
            const PersonLocationPoint& lli,
            double now_coordinate_x,
            double now_coordinate_y,
            double map_view_width,
            double map_view_height,
            double map_view_center_x,
            double map_view_center_y,
            paxg::Font& font,
            paxg::Font& en_font,
            std::uint_least32_t ja_jp_language,
            std::uint_least32_t en_us_language
        ) const {
            // 描画位置
            const paxg::Vec2i draw_pos = paxg::Vec2i{
                static_cast<int>((now_coordinate_x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
                static_cast<int>(double(paxg::Window::height()) - ((now_coordinate_y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
            };

            const paxg::Vec2i draw_font_pos = paxg::Vec2i{
                draw_pos.x(), draw_pos.y() - 60
            };

            const std::uint_least32_t place_tex = (lli.place_texture == 0) ? lll.place_texture : lli.place_texture;
            // 描画
            if (texture.find(place_tex) != texture.end()) {
                texture.at(place_tex).resizedDrawAt(120, draw_pos);
            }

            // テキストを描画
            drawPersonNameText(lli, font, en_font, draw_font_pos, ja_jp_language, en_us_language);
        }

        /// @brief 人物名のテキストを描画
        void drawPersonNameText(
            const PersonLocationPoint& lli,
            paxg::Font& font,
            paxg::Font& en_font,
            const paxg::Vec2i& draw_font_pos,
            std::uint_least32_t ja_jp_language,
            std::uint_least32_t en_us_language
        ) const {
            // 英語名がない場合
            if (lli.place_name.find(en_us_language) == lli.place_name.end()) {
                // 日本語名を描画
                if (lli.place_name.find(ja_jp_language) != lli.place_name.end()) {
                    font.setOutline(0, 0.6, paxg::Color(255, 255, 255));
                    font.drawTopCenter(lli.place_name.at(ja_jp_language), draw_font_pos, paxg::Color(0, 0, 0));
                }
            }
            // 英語名がある場合
            else {
                // 日本語名がある場合
                if (lli.place_name.find(ja_jp_language) != lli.place_name.end()) {
                    // 名前（英語）を描画
                    en_font.setOutline(0, 0.6, paxg::Color(255, 255, 255));
                    en_font.drawBottomCenter(lli.place_name.at(en_us_language), draw_font_pos, paxg::Color(0, 0, 0));
                    // 日本語名を描画
                    font.setOutline(0, 0.6, paxg::Color(255, 255, 255));
                    font.drawTopCenter(lli.place_name.at(ja_jp_language), draw_font_pos, paxg::Color(0, 0, 0));
                }
                else {
                    // 名前（英語）を描画
                    en_font.setOutline(0, 0.6, paxg::Color(255, 255, 255));
                    en_font.drawTopCenter(lli.place_name.at(en_us_language), draw_font_pos, paxg::Color(0, 0, 0));
                }
            }
        }
    };

}

#endif // !PAX_MAHOROBA_PERSON_NAME_RENDERER_HPP
