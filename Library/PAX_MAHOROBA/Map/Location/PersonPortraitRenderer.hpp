/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0.  https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_PERSON_PORTRAIT_RENDERER_HPP
#define PAX_MAHOROBA_PERSON_PORTRAIT_RENDERER_HPP

#include <cstdint>
#include <vector>

#include <PAX_GRAPHICA/Circle.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Map/Location/LocationRendererHelper.hpp>

#include <PAX_SAPIENTICA/GeographicInformation/PersonNameRepository.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Type/UnorderedMap.hpp>

namespace paxs {

    /// @brief 人物の肖像画と名前の描画を担当するクラス
    /// @brief Class responsible for rendering person portraits and names
    class PersonPortraitRenderer {
    public:
        /// @brief 人物の肖像画と名前を描画
        /// @brief Draw person portraits and names
        static void draw(
            const std::vector<PersonLocationList>& location_point_list_list,
            const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>& texture,
            const double jdn,
            const double map_view_width,
            const double map_view_height,
            const double map_view_center_x,
            const double map_view_center_y
        ) {
            const std::uint_least32_t ja_jp_language = MurMur3::calcHash("ja-JP");
            const std::uint_least32_t en_us_language = MurMur3::calcHash("en-US");

            for (std::size_t h = 0; h < location_point_list_list.size(); ++h) {
                const auto& person_location_list = location_point_list_list[h].person_location_list;

                auto& lll = location_point_list_list[h];
                // 時間の範囲外を除去
                if (lll.min_year > jdn) continue;
                if (lll.max_year < jdn) continue;

                // 人物の肖像画と名前を描画
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
                        drawPortraitOnly(
                            texture, lll, lli,
                            now_coordinate_x, now_coordinate_y,
                            map_view_width, map_view_height,
                            map_view_center_x, map_view_center_y
                        );
                        continue;
                    }

                    // 肖像画とテキストを描画
                    drawPortraitAndText(
                        texture, lll, lli,
                        now_coordinate_x, now_coordinate_y,
                        map_view_width, map_view_height,
                        map_view_center_x, map_view_center_y,
                        ja_jp_language, en_us_language
                    );
                }
            }
        }

    private:
        PersonPortraitRenderer() = default;

        /// @brief 肖像画のみを描画
        /// @brief Draw portrait only
        static void drawPortraitOnly(
            const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>& texture,
            const PersonLocationList& lll,
            const PersonLocationPoint& lli,
            double now_coordinate_x,
            double now_coordinate_y,
            double map_view_width,
            double map_view_height,
            double map_view_center_x,
            double map_view_center_y
        ) {
            // 描画位置
            const paxg::Vec2i draw_pos = LocationRendererHelper::toScreenPos(
                now_coordinate_x, now_coordinate_y,
                map_view_width, map_view_height,
                map_view_center_x, map_view_center_y
            );

            // エージェントアイコン描画
            if (LocationRendererHelper::drawAgentIcon(texture, lli.lpe, draw_pos)) {
                return;
            }

            // 通常のテクスチャ描画
            const int len = int(lli.overall_length / 2);
            const std::uint_least32_t place_tex = (lli.place_texture == 0) ? lll.place_texture : lli.place_texture;
            if (texture.find(place_tex) != texture.end()) {
                texture.at(place_tex).resizedDrawAt(len, draw_pos);
            }
        }

        /// @brief 肖像画とテキストを描画
        /// @brief Draw portrait and text
        static void drawPortraitAndText(
            const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>& texture,
            const PersonLocationList& lll,
            const PersonLocationPoint& lli,
            double now_coordinate_x,
            double now_coordinate_y,
            double map_view_width,
            double map_view_height,
            double map_view_center_x,
            double map_view_center_y,
            std::uint_least32_t ja_jp_language,
            std::uint_least32_t en_us_language
        ) {
            // 描画位置
            const paxg::Vec2i draw_pos = LocationRendererHelper::toScreenPos(
                now_coordinate_x, now_coordinate_y,
                map_view_width, map_view_height,
                map_view_center_x, map_view_center_y
            );

            const paxg::Vec2i draw_font_pos = paxg::Vec2i{
                draw_pos.x(), draw_pos.y() - 60
            };

            const std::uint_least32_t place_tex = (lli.place_texture == 0) ? lll.place_texture : lli.place_texture;
            // 描画
            if (texture.find(place_tex) != texture.end()) {
                texture.at(place_tex).resizedDrawAt(120, draw_pos);
            }

            // テキストを描画
            drawPersonNameText(lli, draw_font_pos, ja_jp_language, en_us_language);
        }

        /// @brief 人物名のテキストを描画
        /// @brief Draw person name text
        static void drawPersonNameText(
            const PersonLocationPoint& lli,
            const paxg::Vec2i& draw_font_pos,
            std::uint_least32_t ja_jp_language,
            std::uint_least32_t en_us_language
        ) {
            (void)ja_jp_language;  // 未使用警告を抑制
            (void)en_us_language;  // 未使用警告を抑制
            LocationRendererHelper::drawBilingualText(
                lli.place_name, draw_font_pos, "topCenter"
            );
        }
    };

}

#endif // !PAX_MAHOROBA_PERSON_PORTRAIT_RENDERER_HPP
