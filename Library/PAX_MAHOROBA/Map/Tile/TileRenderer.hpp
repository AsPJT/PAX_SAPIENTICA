/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_TILE_RENDERER_HPP
#define PAX_MAHOROBA_TILE_RENDERER_HPP

#include <vector>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Color/Background.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/UI/MenuBar.hpp>
#include <PAX_MAHOROBA/Map/Tile/XYZTiles.hpp>

#include <PAX_SAPIENTICA/Calendar/JulianDayNumber.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief タイル描画クラス (Presentation Layer)
    /// @brief 地図タイルの描画処理を担当
    class TileRenderer {
    public:
        TileRenderer() = default;

        /// @brief 地図の背景を描画
        void drawBackground() const {
            paxg::Rect{ 0, 0, static_cast<float>(paxg::Window::width()), static_cast<float>(paxg::Window::height()) }
                .draw(paxs::BackgroundColor::LightBlue);
        }

        /// @brief タイルを描画
        /// @param tiles タイルのリスト
        /// @param menu_bar メニューバー
        /// @param map_viewport マップビューポート
        /// @param jdn ユリウス日
        void drawTiles(
            const std::vector<XYZTile>& tiles,
            const MenuBar& menu_bar,
            const MapViewport& map_viewport,
            cal::JDN_F64 jdn
        ) const {
            const double map_viewport_width = map_viewport.getWidth();
            const double map_viewport_height = map_viewport.getHeight();
            const double map_viewport_center_x = map_viewport.getCenterX();
            const double map_viewport_center_y = map_viewport.getCenterY();
            const int date = static_cast<int>(jdn.cgetDay());

            const auto* map_pulldown = menu_bar.cgetPulldown(MurMur3::calcHash("map"));

            for (const auto& tile : tiles) {
                // メニューバーの可視性チェック
                if (map_pulldown && map_pulldown->getIsItemsKey(tile.getMenuBarMap()) != tile.getMenuBarMapBool()) {
                    continue;
                }

                // 描画タイプに応じて描画
                switch (tile.getDrawType()) {
                case paxs::MurMur3::calcHash("texture"): // 画像を描画
                    tile.draw(map_viewport_width, map_viewport_height, map_viewport_center_x, map_viewport_center_y, date);
                    break;
                case paxs::MurMur3::calcHash("texture_and_grid"): // 画像とグリッドを描画
                    tile.draw(map_viewport_width, map_viewport_height, map_viewport_center_x, map_viewport_center_y, date);
                    tile.drawLine(map_viewport_width, map_viewport_height, map_viewport_center_x, map_viewport_center_y, 0.8, paxg::Color{ 95, 99, 104 });
                    break;
                case paxs::MurMur3::calcHash("grid_and_string"): // 線と文字列を描画
                    tile.drawLine(map_viewport_width, map_viewport_height, map_viewport_center_x, map_viewport_center_y, 0.8, paxg::Color{ 95, 99, 104 });
                    tile.drawXYZ(map_viewport_width, map_viewport_height, map_viewport_center_x, map_viewport_center_y);
                    break;
                }
            }
        }
    };
}

#endif // !PAX_MAHOROBA_TILE_RENDERER_HPP
