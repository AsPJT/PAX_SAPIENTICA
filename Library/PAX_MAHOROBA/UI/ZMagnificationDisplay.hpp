/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0.  https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_Z_MAGNIFICATION_DISPLAY_HPP
#define PAX_MAHOROBA_UI_Z_MAGNIFICATION_DISPLAY_HPP

#include <cmath>
#include <cstdint>
#include <string>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Rendering/LanguageFonts.hpp>

#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief Z拡大率表示コンポーネント
    /// @brief Z magnification display component
    class ZMagnificationDisplay {
    public:
        ZMagnificationDisplay() = default;

        /// @brief Z拡大率を描画
        /// @brief Draw Z magnification
        /// @param map_viewport マップビューポート
        /// @param language_fonts 言語フォント
        /// @param select_language 選択言語
        /// @param language_text 言語テキスト
        /// @param debug_start_y 描画開始Y座標
        void draw(
            const MapViewport* map_viewport,
            LanguageFonts* language_fonts,
            const SelectLanguage* select_language,
            const paxs::Language* language_text,
            int debug_start_y
        ) const {
            if (!map_viewport || !language_fonts || !select_language || !language_text) return;

            // XYZ Tile Z magnification の計算
            const int magnification_z = static_cast<int>(-std::log2(map_viewport->getHeight()) + 12.5);

            // フォントサイズを設定
            constexpr int font_size = 22;
            constexpr int font_buffer_thickness = 2;

            paxg::Font* font = language_fonts->getAndAdd(select_language->cgetKey(),
                static_cast<std::uint_least8_t>(font_size),
                static_cast<std::uint_least8_t>(font_buffer_thickness));
            if (font == nullptr) return;

            // ラベルテキストを取得
            const std::string* label_text_ptr = language_text->getStringPtr(
                MurMur3::calcHash("debug_xyz_tiles_z"),
                select_language->cgetKey()
            );
            if (label_text_ptr == nullptr) return;

            // Z拡大率のラベルを描画
            font->setOutline(0, 0.6, paxg::Color(255, 255, 255));
            font->drawTopRight(*label_text_ptr,
                paxg::Vec2i(paxg::Window::width() - 40, debug_start_y), paxg::Color(0, 0, 0));

            // Z拡大率の値を描画
            font->drawTopRight(std::to_string(magnification_z),
                paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 30), paxg::Color(0, 0, 0));
        }
    };

}

#endif // !PAX_MAHOROBA_UI_Z_MAGNIFICATION_DISPLAY_HPP
