/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_PERSON_NAME_MANAGER_HPP
#define PAX_MAHOROBA_PERSON_NAME_MANAGER_HPP

#include <cstdint>
#include <string>
#include <vector>

#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Texture.hpp>

#include <PAX_MAHOROBA/Map/Location/PersonNameRenderer.hpp>
#include <PAX_SAPIENTICA/GeographicInformation/PersonNameRepository.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief GUI に描画する地物の情報を管理するクラス (Application Layer)
    class PersonNameManager {
    public:
        PersonNameManager() = default;

        /// @brief 地物を追加
        void add() {
            repository_.loadPersonNameList(
                [this](const std::string& file_path, double min_view, double max_view,
                       int min_year, int max_year, std::uint_least32_t lpe,
                       std::uint_least32_t place_texture) {
                    inputPlace(file_path, min_view, max_view, min_year, max_year, lpe, place_texture);
                }
            );
        }

        /// @brief 初期化
        void init() {
            std::string str = "";
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("Portraits"),
                [&](const std::string& path_) {str = path_; });
            if (str.size() == 0) return;

            key_value_tsv.input(str, [&](const std::string& value_) { return paxg::Texture{ value_ }; });
        }

        /// @brief 描画
        void draw(const double jdn,
            const double map_view_width, const double map_view_height, const double map_view_center_x, const double map_view_center_y,
            paxg::Font& font, paxg::Font& en_font, paxg::Font& pin_font) {
            renderer_.draw(location_point_list_list, key_value_tsv.get(), jdn,
                map_view_width, map_view_height, map_view_center_x, map_view_center_y,
                font, en_font, pin_font);
        }

    private:
        std::vector<PersonLocationList> location_point_list_list{}; // 地物の一覧
        // アイコンのテクスチャ
        paxs::KeyValueTSV<paxg::Texture> key_value_tsv;
        PersonNameRenderer renderer_; // 描画処理を担当
        PersonNameRepository repository_; // データ読み込みを担当

        /// @brief 地名を読み込み
        void inputPlace(
            const std::string& str_,
            const double min_view_,  // 可視化する地図の最小範囲
            const double max_view_,  // 可視化する地図の最大範囲
            const int min_year_,  // 可視化する時代（古い年～）
            const int max_year_,  // 可視化する時代（～新しい年）
            const std::uint_least32_t lpe_,  // 対象となる地物の種別
            const std::uint_least32_t place_texture_ // 出典
        ) {
            PersonLocationList loaded = repository_.loadPersonFromFile(
                str_, min_view_, max_view_, min_year_, max_year_, lpe_, place_texture_
            );
            if (loaded.person_location_list.size() > 0) {
                location_point_list_list.emplace_back(loaded);
            }
        }
    };

}

#endif // !PAX_MAHOROBA_PERSON_NAME_MANAGER_HPP
