/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_TILE_RENDERER_HPP
#define PAX_MAHOROBA_TILE_RENDERER_HPP

#include <string>
#include <vector>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Line.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Rendering/BackgroundColor.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Map/Tile/XYZTile.hpp>

#include <PAX_SAPIENTICA/Calendar/JulianDayNumber.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>

namespace paxs {

    /// @brief 地図タイル描画クラス
    class TileRenderer {
    public:
        /// @brief 地図の背景を描画
        static void drawBackground() {
            paxg::Rect{ 0, 0, static_cast<float>(paxg::Window::width()), static_cast<float>(paxg::Window::height()) }
                .draw(paxs::BackgroundColor::LightBlue);
        }

        /// @brief タイルを描画
        /// @param tiles タイルのリスト
        /// @param visible 可視性管理
        /// @param map_viewport マップビューポート
        /// @param jdn ユリウス日
        static void drawTiles(
            const std::vector<XYZTile>& tiles,
            const FeatureVisibilityManager& visible,
            const MapViewport& map_viewport,
            cal::JDN_F64 jdn
        ) {
            const double map_viewport_width = map_viewport.getWidth();
            const double map_viewport_height = map_viewport.getHeight();
            const double map_viewport_center_x = map_viewport.getCenterX();
            const double map_viewport_center_y = map_viewport.getCenterY();
            const int date = static_cast<int>(jdn.cgetDay());

            for (const auto& tile : tiles) {
                // 可視性チェック
                if (tile.getMenuBarMap() != 0 && visible.isVisible(tile.getMenuBarMap()) != tile.getMenuBarMapBool()) {
                    continue;
                }

                // 描画タイプに応じて描画
                switch (tile.getDrawType()) {
                case paxs::MurMur3::calcHash("texture"): // 画像を描画
                    drawTileTextures(tile, map_viewport_width, map_viewport_height, map_viewport_center_x, map_viewport_center_y, date);
                    break;
                case paxs::MurMur3::calcHash("texture_and_grid"): // 画像とグリッドを描画
                    drawTileTextures(tile, map_viewport_width, map_viewport_height, map_viewport_center_x, map_viewport_center_y, date);
                    drawTileGridLines(tile, map_viewport_width, map_viewport_height, map_viewport_center_x, map_viewport_center_y, 0.8, paxg::Color{ 95, 99, 104 });
                    break;
                case paxs::MurMur3::calcHash("grid_and_string"): // 線と文字列を描画
                    drawTileGridLines(tile, map_viewport_width, map_viewport_height, map_viewport_center_x, map_viewport_center_y, 0.8, paxg::Color{ 95, 99, 104 });
                    drawTileDebugInfo(tile, map_viewport_width, map_viewport_height, map_viewport_center_x, map_viewport_center_y);
                    break;
                }
            }
        }

    private:
        TileRenderer() = default;

        /// @brief タイルのテクスチャを描画
        static void drawTileTextures(
            const XYZTile& tile,
            const double map_view_width,
            const double map_view_height,
            const double map_view_center_x,
            const double map_view_center_y,
            const int date
        ) {
            // 拡大率が描画範囲外の場合は終了
            const unsigned int magnification_z = tile.getMagnificationZ();
            if (magnification_z < tile.getDrawMinZ()) return;
            if (magnification_z > tile.getDrawMaxZ()) return;

            // 描画する期間外の場合は終了
            if (tile.getMinDate() != 99999999 && tile.getMinDate() > date) return;
            if (tile.getMaxDate() != 99999999 && tile.getMaxDate() < date) return;

            const unsigned int z = tile.getZ();
            const unsigned int z_num = tile.getZNum();
            const Vector2<int> start_cell = tile.getStartCell();
            const Vector2<int> end_cell = tile.getEndCell();

            paxg::Vec2f tile_size = paxg::Vec2f(
                static_cast<float>((360.0 / z_num) / map_view_width * static_cast<double>(paxg::Window::width())),
                static_cast<float>((360.0 / z_num) / map_view_height * static_cast<double>(paxg::Window::height()))
            );

            for (int i = start_cell.y; i <= end_cell.y; ++i) {
                for (int j = start_cell.x; j <= end_cell.x; ++j) {
                    const auto* texture = tile.getTextureAt(z, (i + z_num) & (z_num - 1), (j + z_num) & (z_num - 1));
                    if (texture) {
                        const Vector2<double> map_pos = Vector2<double>{
                            j * 360.0 / z_num - 180.0,
                            (360.0 - i * 360.0 / z_num) - 180.0
                        };
                        texture->resizedDraw(tile_size,
                            paxg::Vec2f(
                                static_cast<float>((map_pos.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
                                static_cast<float>(double(paxg::Window::height()) - ((map_pos.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                            ));
                    }
                }
            }
        }

        /// @brief タイルのグリッド線を描画
        static void drawTileGridLines(
            const XYZTile& tile,
            const double map_view_width,
            const double map_view_height,
            const double map_view_center_x,
            const double map_view_center_y,
            const double thickness,
            const paxg::Color& color
        ) {
            // 拡大率が描画範囲外の場合は終了
            const unsigned int magnification_z = tile.getMagnificationZ();
            if (magnification_z < tile.getDrawMinZ()) return;
            if (magnification_z > tile.getDrawMaxZ()) return;

            const unsigned int z_num = tile.getZNum();
            const Vector2<int> start_cell = tile.getStartCell();
            const Vector2<int> end_cell = tile.getEndCell();

            const Vector2<double> map_start_pos = Vector2<double>{
                start_cell.x * 360.0 / z_num - 180.0,
                (360.0 - start_cell.y * 360.0 / z_num) - 180.0
            };

            double pos_x = (map_start_pos.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width());
            double pos_y = double(paxg::Window::height()) - ((map_start_pos.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height()));
            const double move_x = (360.0 / z_num) / map_view_width * double(paxg::Window::width());
            const double move_y = (360.0 / z_num) / map_view_height * double(paxg::Window::height());

            // 水平線を描画
            for (int i = start_cell.y; i <= end_cell.y; ++i, pos_y += move_y) {
                paxg::Line(
                    0, static_cast<float>(pos_y),
                    static_cast<float>(paxg::Window::width()), static_cast<float>(pos_y)
                ).draw(thickness, color);
            }

            // 垂直線を描画
            for (int j = start_cell.x; j <= end_cell.x; ++j, pos_x += move_x) {
                paxg::Line(
                    static_cast<float>(pos_x), 0,
                    static_cast<float>(pos_x), static_cast<float>(paxg::Window::height())
                ).draw(thickness, color);
            }
        }

        /// @brief タイルのデバッグ情報を描画
        static void drawTileDebugInfo(
            const XYZTile& tile,
            const double map_view_width,
            const double map_view_height,
            const double map_view_center_x,
            const double map_view_center_y
        ) {
            static paxg::Font tmp_font{ 16, "", 3 };
            tmp_font.setOutline(0, 0.5, paxg::Color{ 255, 255, 255 });

            const unsigned int z = tile.getZ();
            const unsigned int z_num = tile.getZNum();
            const Vector2<int> start_cell = tile.getStartCell();
            const Vector2<int> end_cell = tile.getEndCell();

            for (int i = start_cell.y; i <= end_cell.y; ++i) {
                const std::size_t i2 = (i + z_num) & (z_num - 1);
                for (int j = start_cell.x; j <= end_cell.x; ++j) {
                    const std::size_t j2 = (j + z_num) & (z_num - 1);
                    const Vector2<double> map_pos = Vector2<double>{
                        j * 360.0 / z_num - 180.0,
                        (360.0 - i * 360.0 / z_num) - 180.0
                    };
                    tmp_font.draw(
                        std::string("X:" + std::to_string(j2) + "\nY:" + std::to_string(i2) +
                                   "\nZ:" + std::to_string(z) +
                                   "\nL:" + std::to_string(static_cast<std::size_t>(40075016.68 / (1 << z)))),
                        paxg::Vec2i(
                            static_cast<int>(10 + (map_pos.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
                            static_cast<int>(5 + double(paxg::Window::height()) - ((map_pos.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                        ),
                        paxg::Color{ 0, 0, 0 }
                    );
                }
            }
        }
    };
}

#endif // !PAX_MAHOROBA_TILE_RENDERER_HPP
