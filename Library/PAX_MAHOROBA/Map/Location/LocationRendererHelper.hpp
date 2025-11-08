/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_LOCATION_RENDERER_HELPER_HPP
#define PAX_MAHOROBA_LOCATION_RENDERER_HELPER_HPP

#include <cstdint>
#include <cstring>

#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Type/UnorderedMap.hpp>

namespace paxs {

    /// @brief 地名・人名・集落レンダリングの共通ヘルパー
    /// @brief Common helper utilities for location, person name, and settlement rendering
    class LocationRendererHelper {
    public:
        /// @brief agent1/agent2アイコンを描画
        /// @brief Draw agent1/agent2 icons
        /// @param texture テクスチャマップ
        /// @param lpe ロケーションタイプ（agent1 or agent2のハッシュ値）
        /// @param draw_pos 描画位置
        /// @return true if agent icon was drawn, false otherwise
        static bool drawAgentIcon(
            const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>& texture,
            std::uint_least32_t lpe,
            const paxg::Vec2i& draw_pos
        ) {
            // エージェント1を描画
            if (lpe == MurMur3::calcHash("agent1")) {
                if (texture.find(MurMur3::calcHash("BlueCircle")) != texture.end()) {
                    texture.at(MurMur3::calcHash("BlueCircle")).resizedDrawAt(15, draw_pos);
                }
                return true;
            }
            // エージェント2を描画
            else if (lpe == MurMur3::calcHash("agent2")) {
                if (texture.find(MurMur3::calcHash("RedCircle")) != texture.end()) {
                    texture.at(MurMur3::calcHash("RedCircle")).resizedDrawAt(15, draw_pos);
                }
                return true;
            }
            return false;
        }

        /// @brief バイリンガルテキスト描画（日本語/英語）
        /// @brief Draw bilingual text (Japanese/English)
        /// @param place_name 言語別の名前マップ
        /// @param draw_pos 描画位置
        /// @param text_mode 描画モード ("topCenter", "at", "bottomCenter")
        /// @param outline_color アウトラインの色（デフォルト: 243, 243, 243）
        static void drawBilingualText(
            const paxs::UnorderedMap<std::uint_least32_t, std::string>& place_name,
            const paxg::Vec2i& draw_pos,
            const char* text_mode = "topCenter",
            const paxg::Color& outline_color = paxg::Color(243, 243, 243)
        ) {
            const std::uint_least32_t ja_jp = MurMur3::calcHash("ja-JP");
            const std::uint_least32_t en_us = MurMur3::calcHash("en-US");

            const bool has_ja = (place_name.find(ja_jp) != place_name.end());
            const bool has_en = (place_name.find(en_us) != place_name.end());

            paxg::Font* font = Fonts().getFont(FontProfiles::MAIN);
            paxg::Font* en_font = Fonts().getFont(FontProfiles::ENGLISH);

            // 日本語のみ
            if (has_ja && !has_en) {
                font->setOutline(0, 0.6, outline_color);
                drawTextByMode(font, place_name.at(ja_jp), draw_pos, paxg::Color(0, 0, 0), text_mode);
            }
            // 英語のみ
            else if (!has_ja && has_en) {
                // PlaceNameRendererは異なるoutline_colorを使用
                paxg::Color en_outline = (std::strcmp(text_mode, "at") == 0)
                    ? paxg::Color(243, 243, 243)
                    : outline_color;
                en_font->setOutline(0, 0.6, en_outline);
                drawTextByMode(en_font, place_name.at(en_us), draw_pos, paxg::Color(0, 0, 0), text_mode);
            }
            // 両方ある場合
            else if (has_ja && has_en) {
                // 英語を下に
                paxg::Color en_outline = (std::strcmp(text_mode, "at") == 0)
                    ? paxg::Color(243, 243, 243)
                    : outline_color;
                en_font->setOutline(0, 0.6, en_outline);
                en_font->drawBottomCenter(place_name.at(en_us), draw_pos, paxg::Color(0, 0, 0));
                // 日本語を上に
                font->setOutline(0, 0.6, outline_color);
                font->drawTopCenter(place_name.at(ja_jp), draw_pos, paxg::Color(0, 0, 0));
            }
        }

        /// @brief 座標をスクリーン座標に変換
        /// @brief Convert mercator coordinate to screen position
        /// @param coordinate_x メルカトル座標X
        /// @param coordinate_y メルカトル座標Y
        /// @param map_view_width マップビューの幅
        /// @param map_view_height マップビューの高さ
        /// @param map_view_center_x マップビューの中心X座標
        /// @param map_view_center_y マップビューの中心Y座標
        /// @return スクリーン座標
        static paxg::Vec2i toScreenPos(
            double coordinate_x,
            double coordinate_y,
            double map_view_width,
            double map_view_height,
            double map_view_center_x,
            double map_view_center_y
        ) {
            return paxg::Vec2i{
                static_cast<int>((coordinate_x - (map_view_center_x - map_view_width / 2))
                               / map_view_width * double(paxg::Window::width())),
                static_cast<int>(double(paxg::Window::height())
                               - ((coordinate_y - (map_view_center_y - map_view_height / 2))
                               / map_view_height * double(paxg::Window::height())))
            };
        }

        /// @brief 範囲内判定
        /// @brief Check if coordinate is within view bounds
        /// @param coordinate_x メルカトル座標X
        /// @param coordinate_y メルカトル座標Y
        /// @param map_view_width マップビューの幅
        /// @param map_view_height マップビューの高さ
        /// @param map_view_center_x マップビューの中心X座標
        /// @param map_view_center_y マップビューの中心Y座標
        /// @param margin_factor マージン係数（デフォルト: 1.6）
        /// @return 範囲内ならtrue
        static bool isInViewBounds(
            double coordinate_x,
            double coordinate_y,
            double map_view_width,
            double map_view_height,
            double map_view_center_x,
            double map_view_center_y,
            double margin_factor = 1.6
        ) {
            return !(coordinate_x < (map_view_center_x - map_view_width / margin_factor)
                  || coordinate_x > (map_view_center_x + map_view_width / margin_factor)
                  || coordinate_y < (map_view_center_y - map_view_height / margin_factor)
                  || coordinate_y > (map_view_center_y + map_view_height / margin_factor));
        }

    private:
        LocationRendererHelper() = default;

        /// @brief モードに応じてテキスト描画
        /// @brief Draw text according to specified mode
        static void drawTextByMode(
            paxg::Font* font,
            const std::string& text,
            const paxg::Vec2i& pos,
            const paxg::Color& color,
            const char* mode
        ) {
            if (std::strcmp(mode, "topCenter") == 0) {
                font->drawTopCenter(text, pos, color);
            } else if (std::strcmp(mode, "at") == 0) {
                font->drawAt(text, pos, color);
            } else if (std::strcmp(mode, "bottomCenter") == 0) {
                font->drawBottomCenter(text, pos, color);
            }
        }
    };

}

#endif // !PAX_MAHOROBA_LOCATION_RENDERER_HELPER_HPP
