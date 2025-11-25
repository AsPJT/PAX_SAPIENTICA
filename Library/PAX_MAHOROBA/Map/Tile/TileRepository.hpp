/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_TILE_REPOSITORY_HPP
#define PAX_MAHOROBA_TILE_REPOSITORY_HPP

#include <string>
#include <vector>

#include <PAX_MAHOROBA/Map/Tile/XYZTile.hpp>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Key/MenuBarKeys.hpp>
#include <PAX_SAPIENTICA/System/InputFile.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>
#include <PAX_SAPIENTICA/Utility/StringUtils.hpp>

namespace paxs {

    /// @brief タイルデータリポジトリ (Infrastructure Layer)
    /// @brief ファイルからタイルデータを読み込む
    class TileRepository {
    private:
        // 項目の ID を返す
        static std::size_t getMenuIndexMap(const paxs::UnorderedMap<std::uint_least32_t, std::size_t>& menu, const std::uint_least32_t& str_) {
            // Key が登録されていたら Key の中身（添え字）を返す
            const auto iterator = menu.find(str_);
            return iterator != menu.end() ? iterator->second :
                SIZE_MAX; // 登録されていない場合は最大値を返す
        }

    public:
        TileRepository() = default;

        /// @brief ファイルからXYZタイルを読み込む
        /// @param relative_path 相対パス
        /// @return 読み込んだタイルのリスト
        std::vector<XYZTile> loadFromFile(const std::string& relative_path) const {
            std::vector<XYZTile> tiles;

            paxs::InputFile input_file(relative_path);
            if (input_file.fail()) {
                PAXS_ERROR(relative_path + " could not be opened.");
                return tiles;
            }

            // 1 行目を読み込む
            if (!(input_file.getLine())) {
                PAXS_ERROR("The first line (header) of " + relative_path + " could not be read.");
                return tiles; // 何もない場合
            }

            // BOM を削除
            input_file.deleteBOM();

            // 1 行目を分割する
            paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu = input_file.splitHashMapMurMur3('\t');

            // カラムインデックスを取得
            const std::size_t menu_bar_index = getMenuIndexMap(menu, MurMur3::calcHash("menu_bar"));
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
            while (input_file.getLine()) {
                std::vector<std::string> strvec = input_file.split('\t');

                // TODO: 必要か確認
                const bool menu_bar_map_bool = true;

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
                        99999999 : static_cast<unsigned int>(StringUtils::safeStoi(strvec[min_z_index], 99999999, true)));

                // 読み込む地図の最大範囲
                const unsigned int max_z = (max_z_index >= strvec.size()) ?
                    99999999 : ((strvec[max_z_index].size() == 0) ?
                        99999999 : static_cast<unsigned int>(StringUtils::safeStoi(strvec[max_z_index], 99999999, true)));

                // 表示する最小ズームレベル
                const unsigned int draw_min_z = (draw_min_z_index >= strvec.size()) ?
                    99999999 : ((strvec[draw_min_z_index].size() == 0) ?
                        99999999 : static_cast<unsigned int>(StringUtils::safeStoi(strvec[draw_min_z_index], 99999999, true)));

                // 表示する最大ズームレベル
                const unsigned int draw_max_z = (draw_max_z_index >= strvec.size()) ?
                    99999999 : ((strvec[draw_max_z_index].size() == 0) ?
                        99999999 : static_cast<unsigned int>(StringUtils::safeStoi(strvec[draw_max_z_index], 99999999, true)));

                XYZTile xyz_tile(menu_bar_map, menu_bar_map_bool,
                    texture_input_type, binary_input_type, binary_path,
                    texture_path, format, draw_type, texture_url);

                if (map_name.size() != 0) xyz_tile.setMapName(map_name);
                if (min_z != 99999999) xyz_tile.setMinZ(min_z);
                if (max_z != 99999999) xyz_tile.setMaxZ(max_z);
                if (draw_min_z != 99999999) xyz_tile.setDrawMinZ(draw_min_z);
                if (draw_max_z != 99999999) xyz_tile.setDrawMaxZ(draw_max_z);

                tiles.emplace_back(xyz_tile);
            }

            return tiles;
        }

        /// @brief グリッド線タイルを作成
        /// @return グリッド線タイル
        XYZTile createGridLineTile() const {
            XYZTile xyz_tile(static_cast<std::uint_least32_t>(paxs::MapLayersMenu::line1), true,
                paxs::MurMur3::calcHash("asset_file"), 0, "", "",
                (""), paxs::MurMur3::calcHash("grid_and_string"), "");
            return xyz_tile;
        }
    };
}

#endif // !PAX_MAHOROBA_TILE_REPOSITORY_HPP
