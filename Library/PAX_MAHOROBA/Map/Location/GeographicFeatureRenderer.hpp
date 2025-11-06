/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_GEOGRAPHIC_FEATURE_RENDERER_HPP
#define PAX_MAHOROBA_GEOGRAPHIC_FEATURE_RENDERER_HPP

#include <cstdint>
#include <vector>

#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Map/Location/LocationRendererHelper.hpp>

#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Map/LocationPoint.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Type/UnorderedMap.hpp>

namespace paxs {

    /// @brief 地理的特徴(地名とアイコン)の描画を担当するクラス
    class GeographicFeatureRenderer {
    public:
        GeographicFeatureRenderer() = default;

        /// @brief 地理的特徴(地名とアイコン)を描画
        void draw(
            const std::vector<LocationPointList>& location_point_list_list,
            const UnorderedMap<std::uint_least32_t, paxg::Texture>& texture,
            FeatureVisibilityManager& visible,
            const double jdn,
            const double map_view_width,
            const double map_view_height,
            const double map_view_center_x,
            const double map_view_center_y
        ) const {
            const std::uint_least32_t first_language = MurMur3::calcHash("ja-JP");
            const std::uint_least32_t second_language = MurMur3::calcHash("en-US");

            for (std::size_t h = 0; h < location_point_list_list.size(); ++h) {
                const auto& location_point_list = location_point_list_list[h].location_point_list;

                const auto& lll = location_point_list_list[h];
                // 空間の範囲外を除去
                if (lll.end_coordinate.x < (map_view_center_x - map_view_width / 1.6)
                    || lll.start_coordinate.x > (map_view_center_x + map_view_width / 1.6)
                    || lll.end_coordinate.y < (map_view_center_y - map_view_height / 1.6)
                    || lll.start_coordinate.y > (map_view_center_y + map_view_height / 1.6)) continue;
                // 時間の範囲外を除去
                if (lll.min_year > jdn) continue;
                if (lll.max_year < jdn) continue;

                if (!visible.isVisible(lll.lpe)) continue;

                // 地名を描画
                for (std::size_t i = 0; i < location_point_list.size(); ++i) {
                    const auto& lli = location_point_list[i];
                    // 空間の範囲外を除去
                    if (!LocationRendererHelper::isInViewBounds(
                        lli.coordinate.x, lli.coordinate.y,
                        map_view_width, map_view_height,
                        map_view_center_x, map_view_center_y)) continue;
                    // 時間の範囲外を除去
                    if (lli.min_year > jdn) continue;
                    if (lli.max_year < jdn) continue;

                    // 描画位置
                    const paxg::Vec2i draw_pos = LocationRendererHelper::toScreenPos(
                        lli.coordinate.x, lli.coordinate.y,
                        map_view_width, map_view_height,
                        map_view_center_x, map_view_center_y
                    );

                    // 範囲外の場合（アイコンのみ描画）
                    if (lli.min_view > map_view_height || lli.max_view < map_view_height) {
                        drawIconOnly(lli, lll, texture, draw_pos);
                        continue;
                    }

                    // 範囲内の場合（アイコン + テキスト描画）
                    drawIconAndText(lli, lll, texture, draw_pos, first_language, second_language);
                }
            }
        }

    private:
        // 描画定数
        static constexpr int TEXTURE_SPACING_HORIZONTAL = 4;  // テクスチャの水平間隔
        static constexpr int TEXTURE_SPACING_HORIZONTAL_ZOOMED = 6;  // ズーム時のテクスチャの水平間隔
        static constexpr int TEXTURE_SPACING_VERTICAL = 4;  // テクスチャの垂直間隔
        static constexpr std::uint_least16_t ZOOM_SPLIT_COUNT = 10;  // ズーム時の分割数

        /// @brief アイコンのみ描画（範囲外時）
        void drawIconOnly(
            const LocationPoint& lli,
            const LocationPointList& lll,
            const UnorderedMap<std::uint_least32_t, paxg::Texture>& texture,
            const paxg::Vec2i& draw_pos
        ) const {
            // エージェントアイコン描画
            if (LocationRendererHelper::drawAgentIcon(texture, lli.lpe, draw_pos)) {
                return;
            }

            // 通常のテクスチャ描画
            const std::uint_least32_t place_tex = (lli.place_texture == 0) ? lll.place_texture : lli.place_texture;
            if (texture.find(place_tex) != texture.end()) {
                const int len = int(lli.overall_length / 2 * lli.zoom);
                drawTextureMultiple(texture.at(place_tex), len, draw_pos, lli.x_size, lli.y_size, true);
            }
        }

        /// @brief アイコンとテキストを描画（範囲内時）
        void drawIconAndText(
            const LocationPoint& lli,
            const LocationPointList& lll,
            const UnorderedMap<std::uint_least32_t, paxg::Texture>& texture,
            const paxg::Vec2i& draw_pos,
            const std::uint_least32_t first_language,
            const std::uint_least32_t second_language
        ) const {
            const std::uint_least32_t place_tex = (lli.place_texture == 0) ? lll.place_texture : lli.place_texture;
            // 描画
            if (texture.find(place_tex) != texture.end()) {
                const int len = int(lli.overall_length / 2);
                drawTextureMultiple(texture.at(place_tex), len, draw_pos, lli.x_size, lli.y_size, false);
            }

            // テキスト描画
            drawPlaceNameText(lli, draw_pos, first_language, second_language);
        }

        /// @brief テクスチャを複数描画
        void drawTextureMultiple(
            const paxg::Texture& tex,
            const int len,
            const paxg::Vec2i& draw_pos,
            const std::uint_least16_t x_size,
            const std::uint_least16_t y_size,
            const bool is_zoomed
        ) const {
            if (x_size <= 1) {
                if (y_size <= 1) {
                    tex.resizedDrawAt(len, draw_pos);
                }
                else {
                    for (std::uint_least16_t iy = 0; iy < y_size; ++iy) {
                        tex.resizedDrawAt(len, paxg::Vec2i{ draw_pos.x(), draw_pos.y() + static_cast<int>(iy) * TEXTURE_SPACING_VERTICAL });
                    }
                }
            }
            else {
                if (is_zoomed) {
                    // ズーム時は分割表示
                    constexpr std::uint_least16_t split_count = ZOOM_SPLIT_COUNT;
                    if (y_size <= 1) {
                        for (std::uint_least16_t ix = 0, ixx = 0, iyy = 0; ix < x_size; ++ix, ++ixx) {
                            if (ix != 0 && ix % split_count == 0) {
                                ixx = 0;
                                ++iyy;
                            }
                            tex.resizedDrawAt(len, paxg::Vec2i{ draw_pos.x() + static_cast<int>(ixx) * TEXTURE_SPACING_HORIZONTAL_ZOOMED, draw_pos.y() + static_cast<int>(iyy) * TEXTURE_SPACING_VERTICAL });
                        }
                    }
                    else {
                        for (std::uint_least16_t iy = 0; iy < y_size; ++iy) {
                            for (std::uint_least16_t ix = 0; ix < x_size; ++ix) {
                                tex.resizedDrawAt(len, paxg::Vec2i{ draw_pos.x() + static_cast<int>(ix) * TEXTURE_SPACING_HORIZONTAL, draw_pos.y() + static_cast<int>(iy) * TEXTURE_SPACING_VERTICAL });
                            }
                        }
                    }
                }
                else {
                    // 通常時
                    if (y_size <= 1) {
                        for (std::uint_least16_t ix = 0; ix < x_size; ++ix) {
                            tex.resizedDrawAt(len, paxg::Vec2i{ draw_pos.x() + static_cast<int>(ix) * TEXTURE_SPACING_HORIZONTAL, draw_pos.y() });
                        }
                    }
                    else {
                        for (std::uint_least16_t iy = 0; iy < y_size; ++iy) {
                            for (std::uint_least16_t ix = 0; ix < x_size; ++ix) {
                                tex.resizedDrawAt(len, paxg::Vec2i{ draw_pos.x() + static_cast<int>(ix) * TEXTURE_SPACING_HORIZONTAL, draw_pos.y() + static_cast<int>(iy) * TEXTURE_SPACING_VERTICAL });
                            }
                        }
                    }
                }
            }
        }

        /// @brief 地名のテキストを描画
        void drawPlaceNameText(
            const LocationPoint& lli,
            const paxg::Vec2i& draw_pos,
            const std::uint_least32_t first_language,
            const std::uint_least32_t second_language
        ) const {
            paxg::Font* font = Fonts().getFont(FontProfiles::MAIN);
            paxg::Font* en_font = Fonts().getFont(FontProfiles::ENGLISH);

            // 英語名がない場合
            if (lli.place_name.find(second_language) == lli.place_name.end()) {
                // 名前を描画
                if (lli.place_name.find(first_language) != lli.place_name.end()) {
                    font->setOutline(0, 0.6, paxg::Color(240, 245, 250));
                    font->drawAt(lli.place_name.at(first_language), draw_pos, paxg::Color(0, 0, 0));
                }
            }
            // 日本語名がない場合
            else if (lli.place_name.find(first_language) == lli.place_name.end()) {
                // 名前を描画
                if (lli.place_name.find(second_language) != lli.place_name.end()) {
                    en_font->setOutline(0, 0.6, paxg::Color(240, 245, 250));
                    en_font->drawAt(lli.place_name.at(second_language), draw_pos, paxg::Color(0, 0, 0));
                }
            }
            // 英語名がある場合
            else {
                // 名前（英語）を描画
                en_font->setOutline(0, 0.6, paxg::Color(240, 245, 250));
                en_font->drawBottomCenter(lli.place_name.at(second_language), draw_pos, paxg::Color(0, 0, 0));
                // 名前を描画
                if (lli.place_name.find(first_language) != lli.place_name.end()) {
                    font->setOutline(0, 0.6, paxg::Color(240, 245, 250));
                    font->drawTopCenter(lli.place_name.at(first_language), draw_pos, paxg::Color(0, 0, 0));
                }
            }
        }
    };

}

#endif // !PAX_MAHOROBA_GEOGRAPHIC_FEATURE_RENDERER_HPP
