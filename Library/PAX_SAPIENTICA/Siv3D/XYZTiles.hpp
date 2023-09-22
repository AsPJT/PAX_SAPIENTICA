/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_XYZ_TILES_HPP
#define PAX_SAPIENTICA_SIV3D_XYZ_TILES_HPP

/*##########################################################################################

##########################################################################################*/

#include <cmath>
#include <string>

#include <PAX_SAPIENTICA/Siv3D/Init.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>

#include <PAX_GRAPHICA/Line.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxs {

    class XYZTile {
    public:
        // XYZ タイルの種別
        enum XYZTileFileName : int {
            Empty,
            Default,
            Original,
            Z_Original
        };

        // XYZ タイルの 1 つのセルのメルカトル座標を保持
        // 基本的に Z = 19 は無い
        using MapVec2 = Vector2<int>;
        using MapVec2D = Vector2<double>;

        std::vector<MapVec2D> pos_list{};
        // XYZ タイルの画像の情報を保持
        std::vector<paxg::Texture> texture_list{};

        XYZTile(const double map_view_width,
            const double map_view_height,
            const double map_view_center_x,
            const double map_view_center_y) {
            start_cell = MapVec2{
                int((((map_view_center_x - map_view_width / 2) + 180.0) / 360.0) * z_num),
                int(((360.0 - ((map_view_center_y + map_view_height / 2) + 180.0)) / 360.0) * z_num)
            };
            end_cell = MapVec2{
                int((((map_view_center_x + map_view_width / 2) + 180.0) / 360.0) * z_num),
                int(((360.0 - ((map_view_center_y - map_view_height / 2) + 180.0)) / 360.0) * z_num)
            };

            cell_num = MapVec2{
            (end_cell.x - start_cell.x),
            (end_cell.y - start_cell.y)
            };

            cell_all_num = (cell_num.x + 1) * (cell_num.y + 1);

            // 画面上の XYZ タイルのメルカトル座標を初期化
            pos_list.resize(cell_all_num);
            texture_list.resize(cell_all_num);
            for (std::size_t i = start_cell.y, k = 0; i <= end_cell.y; ++i) {
                for (std::size_t j = start_cell.x; j <= end_cell.x; ++j, ++k) {
                    pos_list[k] =
                        //pos_list[i * cell_num.x + j] =
                        MapVec2D{ j * 360.0 / z_num - 180.0,
                (360.0 - i * 360.0 / z_num) - 180.0 };
                }
            }
        }
        XYZTile()
            : start_cell(MapVec2{}), end_cell(MapVec2{}), cell_num(MapVec2{}), cell_all_num(0) {
        }
        // タイルを更新
        void update(const double map_view_width,
            const double map_view_height,
            const double map_view_center_x,
            const double map_view_center_y,
            const XYZTileFileName file_name_enum = XYZTileFileName::Original) {
            // 拡大率が変わった場合、拡大率にあわせて取得する地図の大きさを変える
            if (current_map_view_width != map_view_width) {
                if (default_z == 999) {
                    z = int(-std::log2(map_view_width) + 11.0);
                    magnification_z = z;
                    if (z < min_z) z = min_z;
                    if (z > max_z) z = max_z;
                }
                else {
                    z = default_z;
                    magnification_z = int(-std::log2(map_view_width) + 11.0);
                }
                z_num = int(std::pow(2, z));
                current_map_view_width = map_view_width;
            }
            // 拡大率が描画範囲外の場合はここで処理を終了
            if (magnification_z < draw_min_z) return;
            if (magnification_z > draw_max_z) return;

            // 画像を更新する必要があるか
            bool need_update = false;

            const MapVec2 new_start_cell{ int((((map_view_center_x - map_view_width / 2) + 180.0) / 360.0) * z_num), int(((360.0 - ((map_view_center_y + map_view_height / 2) + 180.0)) / 360.0) * z_num) };
            if (new_start_cell != start_cell) {
                start_cell = new_start_cell;
                need_update = true;
            }
            const MapVec2 new_end_cell = MapVec2{ int((((map_view_center_x + map_view_width / 2) + 180.0) / 360.0) * z_num), int(((360.0 - ((map_view_center_y - map_view_height / 2) + 180.0)) / 360.0) * z_num) };
            if (new_end_cell != end_cell) {
                end_cell = new_end_cell;
                need_update = true;
            }

            // もしタイルが更新されていたら更新処理
            if (!need_update) return;
            cell_num = MapVec2{
                (end_cell.x - start_cell.x),
                (end_cell.y - start_cell.y)
            };
            cell_all_num = (cell_num.x + 1) * (cell_num.y + 1);
            pos_list.resize(cell_all_num);
            texture_list.resize(cell_all_num);


            for (std::size_t i = start_cell.y, k = 0; i <= end_cell.y; ++i) {
                for (std::size_t j = start_cell.x; j <= end_cell.x; ++j, ++k) {
                    pos_list[k] =
                        MapVec2D{ j * 360.0 / z_num - 180.0,
                (360.0 - i * 360.0 / z_num) - 180.0 };
                }
            }

            // ファイルを保存
            for (std::size_t i = start_cell.y, k = 0; i <= end_cell.y; ++i) {
                for (std::size_t j = start_cell.x; j <= end_cell.x; ++j, ++k) {
                    std::string new_saveFilePath = "";
                    switch (file_name_enum) {
                    case XYZTileFileName::Original:
                        new_saveFilePath = (map_file_path_name + map_name
                            + std::string("_") + std::to_string(z)
                            + std::string("_") + std::to_string((j + z_num) % z_num)
                            + std::string("_") + std::to_string((i + z_num) % z_num)
                            + std::string(".png"));
                        break;
                    case XYZTileFileName::Z_Original:
                        new_saveFilePath = (map_file_path_name
                            + std::to_string(z) + std::string("/") + map_name
                            + std::string("_") + std::to_string(z)
                            + std::string("_") + std::to_string((j + z_num) % z_num)
                            + std::string("_") + std::to_string((i + z_num) % z_num)
                            + std::string(".png"));
                        break;
                    case XYZTileFileName::Default:
                        new_saveFilePath = (map_file_path_name
                            + std::to_string(z)
                            + std::string("/") + std::to_string((j + z_num) % z_num)
                            + std::string("/") + std::to_string((i + z_num) % z_num)
                            + std::string(".png"));
                        break;
                    default:
                        break;
                    }

                    // ファイルを同期ダウンロード
                    // ステータスコードが 200 (OK) なら
                    texture_list[k] = paxg::Texture{ new_saveFilePath };
                    if (!texture_list[k] &&
                        map_url_name.size() != 0) {
#if defined(PAXS_USING_SIV3D)
                        const s3d::URL new_url =
                            s3d::String(s3d::Unicode::FromUTF8(map_url_name))
                            + s3d::String(U"/") + s3d::ToString(z)
                            + s3d::String(U"/") + s3d::ToString((j + z_num) % z_num)
                            + s3d::String(U"/") + s3d::ToString((i + z_num) % z_num)
                            + s3d::String(U".png");
                        if (s3d::SimpleHTTP::Save(new_url, s3d::Unicode::FromUTF8(new_saveFilePath)).isOK()) {
                            texture_list[k] = paxg::Texture{ new_saveFilePath };
                        }
#endif
                    }
                }
            }
        }
        void drawXYZ(const double map_view_width, const double map_view_height, const double map_view_center_x, const double map_view_center_y)const {
            // 拡大率が描画範囲外の場合はここで処理を終了
            if (magnification_z < draw_min_z) return;
            if (magnification_z > draw_max_z) return;

            static paxg::Font tmp_font{16, "", 3};
            tmp_font.setOutline(0, 0.5, paxg::Color{ 255, 255, 255 });
            for (std::size_t i = start_cell.y, k = 0; i <= end_cell.y; ++i) {
                for (std::size_t j = start_cell.x; j <= end_cell.x; ++j, ++k) {
                    tmp_font.draw(
                        std::string("X:" + std::to_string(j) + "\nY:" + std::to_string(i) + "\nZ:" + std::to_string(z) + "\nL:" + std::to_string(static_cast<std::size_t>(40075016.68 / std::pow(2, z) * 10) / 10.0)),
                        paxg::Vec2i(static_cast<int>(10 + (pos_list[k].x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
                            static_cast<int>(5 + double(paxg::Window::height()) - ((pos_list[k].y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height()))))
                        , paxg::Color{ 0, 0, 0 }
                    );
                }
            }
        }
        void draw(const double map_view_width, const double map_view_height, const double map_view_center_x, const double map_view_center_y)const {

            // 拡大率が描画範囲外の場合はここで処理を終了
            if (magnification_z < draw_min_z) return;
            if (magnification_z > draw_max_z) return;

            for (std::size_t i = start_cell.y, k = 0; i <= end_cell.y; ++i) {
                for (std::size_t j = start_cell.x; j <= end_cell.x; ++j, ++k) {
                    if (texture_list[k]) {
                        texture_list[k].resizedDraw(
                            paxg::Vec2i(
                                static_cast<int>((360.0 / z_num) / map_view_width * double(paxg::Window::width()))
                                , static_cast<int>((360.0 / z_num) / map_view_height * double(paxg::Window::height()))
                            ),
                            paxg::Vec2i(
                                static_cast<int>((pos_list[k].x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
                            static_cast<int>(double(paxg::Window::height()) - ((pos_list[k].y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                            ));
                    }
                }
            }
        }
        void drawLine(const double map_view_width, const double map_view_height, const double map_view_center_x, const double map_view_center_y
            , const double thickness, const paxg::Color& color
        )const {

            // 拡大率が描画範囲外の場合はここで処理を終了
            if (magnification_z < draw_min_z) return;
            if (magnification_z > draw_max_z) return;

            double pos_x = (pos_list[0].x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width());
            double pos_y = double(paxg::Window::height()) - ((pos_list[0].y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height()));
            const double move_x = (360.0 / z_num) / map_view_width * double(paxg::Window::width());
            const double move_y = (360.0 / z_num) / map_view_height * double(paxg::Window::height());

            for (int i = start_cell.y; i <= end_cell.y; ++i, pos_y += move_y) {
                paxg::Line(
                    0, static_cast<float>(pos_y), static_cast<float>(paxg::Window::width()), static_cast<float>(pos_y)
                ).draw(thickness, color);
            }
            for (int j = start_cell.x; j <= end_cell.x; ++j, pos_x += move_x) {
                paxg::Line(
                    static_cast<float>(pos_x), 0, static_cast<float>(pos_x), static_cast<float>(paxg::Window::height())
                ).draw(thickness, color);
            }
        }
        // セル単位での枠の描画
        void drawLineCell(const double map_view_width, const double map_view_height, const double map_view_center_x, const double map_view_center_y
            , const double inner_thickness, const double outer_thickness, const paxg::Color& color
        )const {
            for (std::size_t i = start_cell.y, k = 0; i <= end_cell.y; ++i) {
                for (std::size_t j = start_cell.x; j <= end_cell.x; ++j, ++k) {
                    paxg::Rect(
                        static_cast<float>((pos_list[k].x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
                        static_cast<float>(double(paxg::Window::height()) - ((pos_list[k].y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height()))),
                        static_cast<float>((360.0 / z_num) / map_view_width * double(paxg::Window::width())),
                        static_cast<float>((360.0 / z_num) / map_view_height * double(paxg::Window::height()))
                    ).drawFrame(inner_thickness, outer_thickness, color);
                }
            }
        }
        MapVec2 getStartCell()const {
            return start_cell;
        }
        MapVec2 getEndCell()const {
            return end_cell;
        }
        int getDefaultZ()const {
            return default_z;
        }
        int getZ()const {
            return z;
        }
        int getZNum()const {
            return z_num;
        }
        void setDefaultZ(const int default_z_) {
            default_z = default_z_;
        }
        void setMinZ(const int min_z_) {
            min_z = min_z_;
        }
        void setMaxZ(const int max_z_) {
            max_z = max_z_;
        }
        void setDrawMinZ(const int min_z_) {
            draw_min_z = min_z_;
        }
        void setDrawMaxZ(const int max_z_) {
            draw_max_z = max_z_;
        }
        void setMapURL(const std::string& map_url_name_) {
            map_url_name = map_url_name_;
        }
        void setMapName(const std::string& map_name_) {
            map_name = map_name_;
        }
        void setMapFilePath(const std::string& map_file_path_name_) {
            map_file_path_name = map_file_path_name_;
        }
    private:
        // 固定された Z （ 999 の場合は固定なし ）
        int default_z = 999;
        // 最小 Z
        int min_z = 0;
        // 最大 Z
        int max_z = 25;
        // 画面の幅に最適な XYZ タイルの Z を格納
        int z = 2;
        // 画面上の Z の値
        int magnification_z = z;
        // 描画最小 Z
        int draw_min_z = 0;
        // 描画最大 Z
        int draw_max_z = 999;
        // 2 の z 乗
        int z_num = int(std::pow(2, z));
        // XYZ タイルの画面上の始点セル
        MapVec2 start_cell{};
        // XYZ タイルの画面上の終点セル
        MapVec2 end_cell{};
        // XYZ タイルの画面上のセルの数
        MapVec2 cell_num{};
        // XYZ タイルの画面上のセルの総数
        int cell_all_num{};

        std::string map_url_name = ""; // URL
        std::string map_name = "";
        std::string map_file_path_name = "";
        // 1フレーム前のマップの幅
        double current_map_view_width = 0;
    };
}

#endif // !PAX_SAPIENTICA_SIV3D_XYZ_TILES_HPP
