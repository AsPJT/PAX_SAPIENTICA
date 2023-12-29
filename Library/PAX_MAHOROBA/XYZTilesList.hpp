/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_XYZ_TILES_LIST_HPP
#define PAX_MAHOROBA_XYZ_TILES_LIST_HPP

/*##########################################################################################

##########################################################################################*/

#include <string>
#include <unordered_map>
#include <vector>

#include <PAX_GRAPHICA/Key.hpp>

#include <PAX_MAHOROBA/Pulldown.hpp>
#include <PAX_MAHOROBA/XYZTiles.hpp>

#include <PAX_SAPIENTICA/MurMur3.hpp>


namespace paxs {

    class XYZTilesList {
    private:
        // 描画する XYZ タイルを管理
        std::vector<XYZTile> xyz_tile_list;

        // 項目の ID を返す
        std::size_t getMenuIndexMap(const std::unordered_map<std::uint_least32_t, std::size_t>& menu, const std::uint_least32_t& str_) {
            // Key が登録されていたら Key の中身（添え字）を返す
            return  (menu.find(str_) != menu.end()) ? menu.at(str_) :
                SIZE_MAX; // 登録されていない場合は最大値を返す
        }

    public:
        // XYZ Tiles を追加
        void add(const std::string& path, const std::string& str) {
            paxg::InputFile pifs(str, path);
            if (pifs.fail()) return;
            // 1 行目を読み込む
            if (!(pifs.getLine())) {
                return; // 何もない場合
            }
            // BOM を削除
            pifs.deleteBOM();
            // 1 行目を分割する
            std::unordered_map<std::uint_least32_t, std::size_t> menu = pifs.splitHashMapMurMur3('\t');

            //const std::size_t key_index = getMenuIndexMap(menu, MurMur3::calcHash("key"));
            //if (key_index == SIZE_MAX) return; // Key がないのはデータにならない
            const std::size_t menu_bar_index = getMenuIndexMap(menu, MurMur3::calcHash("menu_bar"));
            const std::size_t visible_menu_bar_index = getMenuIndexMap(menu, MurMur3::calcHash("visible_menu_bar"));
            const std::size_t texture_input_type_index = getMenuIndexMap(menu, MurMur3::calcHash("texture_input_type"));
            const std::size_t binary_input_type_index = getMenuIndexMap(menu, MurMur3::calcHash("binary_input_type"));
            const std::size_t binary_path_index = getMenuIndexMap(menu, MurMur3::calcHash("binary_path"));
            const std::size_t texture_path_index = getMenuIndexMap(menu, MurMur3::calcHash("texture_path"));
            const std::size_t texture_url_index = getMenuIndexMap(menu, MurMur3::calcHash("texture_url"));
            const std::size_t format_index = getMenuIndexMap(menu, MurMur3::calcHash("format"));
            const std::size_t draw_type_index = getMenuIndexMap(menu, MurMur3::calcHash("draw_type"));
            const std::size_t map_name_index = getMenuIndexMap(menu, MurMur3::calcHash("map_name"));
            const std::size_t min_z_index = getMenuIndexMap(menu, MurMur3::calcHash("min_z"));
            const std::size_t max_z_index = getMenuIndexMap(menu, MurMur3::calcHash("max_z"));
            const std::size_t draw_min_z_index = getMenuIndexMap(menu, MurMur3::calcHash("draw_min_z"));
            const std::size_t draw_max_z_index = getMenuIndexMap(menu, MurMur3::calcHash("draw_max_z"));

            // 1 行ずつ読み込み（区切りはタブ）
            while (pifs.getLine()) {
                std::vector<std::string> strvec = pifs.split('\t');

                // Key が空の場合は読み込まない
                //if (strvec[key_index].size() == 0) continue;

                // 描画の種類 例）画像 texture やグリッド grid など
                const bool menu_bar_map_bool = (visible_menu_bar_index >= strvec.size()) ? false :
                    ((strvec[visible_menu_bar_index].size() == 0) ? false :
                        (strvec[visible_menu_bar_index] == "1"));

                // 描画の種類 例）画像 texture やグリッド grid など
                const std::uint_least32_t draw_type = (draw_type_index >= strvec.size()) ? paxs::MurMur3::calcHash("texture") :
                    ((strvec[draw_type_index].size() == 0) ? paxs::MurMur3::calcHash("texture") :
                        paxs::MurMur3::calcHash(strvec[draw_type_index].size(), strvec[draw_type_index].c_str()));

                // メニューバーの Key
                const std::uint_least32_t menu_bar_map = (menu_bar_index >= strvec.size()) ? 0 : ((strvec[menu_bar_index].size() == 0) ? 0 :
                    paxs::MurMur3::calcHash(strvec[menu_bar_index].size(), strvec[menu_bar_index].c_str()));

                // 地図画像の読み込み種類
                const std::uint_least32_t texture_input_type = (texture_input_type_index >= strvec.size()) ? paxs::MurMur3::calcHash("asset_file") :
                    ((strvec[texture_input_type_index].size() == 0) ? paxs::MurMur3::calcHash("asset_file") :
                        paxs::MurMur3::calcHash(strvec[texture_input_type_index].size(), strvec[texture_input_type_index].c_str()));

                // 地図バイナリデータの読み込み種類
                const std::uint_least32_t binary_input_type = (binary_input_type_index >= strvec.size()) ? 0 :
                    ((strvec[binary_input_type_index].size() == 0) ? 0 :
                        paxs::MurMur3::calcHash(strvec[binary_input_type_index].size(), strvec[binary_input_type_index].c_str()));

                // フォルダ階層
                const std::string format = (format_index >= strvec.size()) ? "{z}/{x}/{y}" :
                    ((strvec[format_index].size() == 0) ? "{z}/{x}/{y}" : strvec[format_index]);

                // 地図バイナリデータのパス
                const std::string binary_path = (binary_path_index >= strvec.size()) ? "" :
                    ((strvec[binary_path_index].size() == 0) ? "" : strvec[binary_path_index]);

                // 地図画像のパス
                const std::string texture_path = (texture_path_index >= strvec.size()) ? "" :
                    ((strvec[texture_path_index].size() == 0) ? "" : strvec[texture_path_index]);

                // 地図画像の URL
                const std::string texture_url = (texture_url_index >= strvec.size()) ? "" :
                    ((strvec[texture_url_index].size() == 0) ? "" : strvec[texture_url_index]);

                // 地図の名前
                const std::string map_name = (map_name_index >= strvec.size()) ? "" :
                    ((strvec[map_name_index].size() == 0) ? "" : strvec[map_name_index]);

                // 読み込む地図の最小範囲
                const unsigned int min_z = (min_z_index >= strvec.size()) ?
                    99999999 : ((strvec[min_z_index].size() == 0) ?
                        99999999 : std::stoi(strvec[min_z_index]));
                // 読み込む地図の最大範囲
                const unsigned int max_z = (max_z_index >= strvec.size()) ?
                    99999999 : ((strvec[max_z_index].size() == 0) ?
                        99999999 : std::stoi(strvec[max_z_index]));
                // 可視化する地図の最小範囲
                const unsigned int draw_min_z = (draw_min_z_index >= strvec.size()) ?
                    99999999 : ((strvec[draw_min_z_index].size() == 0) ?
                        99999999 : std::stoi(strvec[draw_min_z_index]));
                // 可視化する地図の最大範囲
                const unsigned int draw_max_z = (draw_max_z_index >= strvec.size()) ?
                    99999999 : ((strvec[draw_max_z_index].size() == 0) ?
                        99999999 : std::stoi(strvec[draw_max_z_index]));

                XYZTile xyz_tile(menu_bar_map, menu_bar_map_bool,
                    texture_input_type, binary_input_type, &path, binary_path,
                    texture_path, format, draw_type, texture_url);
                if (map_name.size() != 0) xyz_tile.setMapName(map_name);
                if (min_z != 99999999) xyz_tile.setMinZ(min_z);
                if (max_z != 99999999) xyz_tile.setMaxZ(max_z);
                if (draw_min_z != 99999999) xyz_tile.setDrawMinZ(draw_min_z);
                if (draw_max_z != 99999999) xyz_tile.setDrawMaxZ(draw_max_z);
                xyz_tile_list.emplace_back(/*MurMur3::calcHash(strvec[key_index].size(), strvec[key_index].c_str()), */xyz_tile);
            }
        }


        // グリッド線を追加
        void addGridLine() {
            // XYZ タイルのグリッド線
            XYZTile xyz_tile(MurMur3::calcHash("menu_bar_map_line2"), false,
                paxs::MurMur3::calcHash("asset_file"), 0, nullptr, "", "",
                (""), paxs::MurMur3::calcHash("grid_and_string"), "");
            xyz_tile_list.emplace_back(/*MurMur3::calcHash("map_line3"), */xyz_tile);
        }

        // 地図の辞書を更新
        void update(const paxs::MenuBar& menu_bar, const MapView* const map_view, cal::JDN_F64 jdn) {
            if (map_view == nullptr) return; // null の場合は処理しない

            const double map_view_width = map_view->getWidth();
            const double map_view_height = map_view->getHeight();
            const double map_view_center_x = map_view->getCenterX();
            const double map_view_center_y = map_view->getCenterY();

            // 更新処理
            for (auto&& xyzi : xyz_tile_list) {
                if (xyzi.getMenuBarMap() != 0 && menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(xyzi.getMenuBarMap()) != xyzi.getMenuBarMapBool()) continue;
                xyzi.update(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
            }

            // 地図を表示する場合
            const int date = static_cast<int>(jdn.cgetDay());
            for (const auto& xyzi : xyz_tile_list) {
                if (menu_bar.cgetPulldown(MurMur3::calcHash("map")).getIsItemsKey(xyzi.getMenuBarMap()) != xyzi.getMenuBarMapBool()) continue;
                switch (xyzi.getDrawType()) {
                case paxs::MurMur3::calcHash("texture"): // 画像を描画
                    xyzi.draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
                    break;
                case paxs::MurMur3::calcHash("texture_and_grid"): // 画像とグリッドを描画
                    xyzi.draw(map_view_width, map_view_height, map_view_center_x, map_view_center_y, date);
                    xyzi.drawLine(map_view_width, map_view_height, map_view_center_x, map_view_center_y, 0.8, paxg::Color{ 95, 99, 104 });
                    break;
                case paxs::MurMur3::calcHash("grid_and_string"): // 線と文字列を描画
                    xyzi.drawLine(map_view_width, map_view_height, map_view_center_x, map_view_center_y, 0.8, paxg::Color{ 95, 99, 104 });
                    xyzi.drawXYZ(map_view_width, map_view_height, map_view_center_x, map_view_center_y);
                    break;
                }
            }
        }
    };
}

#endif // !PAX_MAHOROBA_XYZ_TILES_LIST_HPP
