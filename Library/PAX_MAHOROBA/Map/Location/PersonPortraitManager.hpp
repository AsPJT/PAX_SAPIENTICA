/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0.  https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_PERSON_PORTRAIT_MANAGER_HPP
#define PAX_MAHOROBA_PERSON_PORTRAIT_MANAGER_HPP

#include <cstdint>
#include <string>
#include <vector>

#include <PAX_GRAPHICA/Texture.hpp>

#include <PAX_MAHOROBA/Map/Location/MapCoordinateConverter.hpp>
#include <PAX_MAHOROBA/Map/Location/MapContentHitTester.hpp>
#include <PAX_MAHOROBA/Map/Location/PersonPortraitRenderer.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>
#include <PAX_SAPIENTICA/GeographicInformation/PersonNameRepository.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief 人物の肖像画と名前の描画を管理するクラス
    /// @brief Class to manage person portraits and names rendering
    class PersonPortraitManager : public IRenderable {
    public:
        PersonPortraitManager() {
            std::string str = "";
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("Portraits"),
                [&](const std::string& path_) {str = path_; });
            if (str.size() == 0) return;

            if (!key_value_tsv.input(str, [&](const std::string& value_) { return paxg::Texture{ value_ }; })) {
                PAXS_ERROR("Failed to load texture KeyValueTSV: " + str);
            }
            // リポジトリに委譲してデータを読み込む
            repository_.loadPersonNameList(
                [this](const std::string& file_path, double min_view, double max_view,
                       int min_year, int max_year, std::uint_least32_t lpe,
                       std::uint_least32_t place_texture) {
                    inputPersonData(file_path, min_view, max_view, min_year, max_year, lpe, place_texture);
                }
            );
        }

        RenderLayer getLayer() const override {
            return RenderLayer::MapContent;
        }
        bool isVisible() const override {
            return visible_;
        }
        void setVisible(bool visible) override {
            visible_ = visible;
        }

        /// @brief 描画パラメータを設定
        void setDrawParams(
            const double jdn,
            const double map_view_width, const double map_view_height,
            const double map_view_center_x, const double map_view_center_y
        ) {
            cached_jdn_ = jdn;
            cached_map_view_width_ = map_view_width;
            cached_map_view_height_ = map_view_height;
            cached_map_view_center_x_ = map_view_center_x;
            cached_map_view_center_y_ = map_view_center_y;
        }

        void render() const override {
            if (!visible_) return;

            PersonPortraitRenderer::draw(location_point_list_list, key_value_tsv.get(), cached_jdn_,
                cached_map_view_width_, cached_map_view_height_,
                cached_map_view_center_x_, cached_map_view_center_y_);
        }

        bool isHit(int mouse_x, int mouse_y, std::string& hit_person_name) const {
            if (!visible_) return false;

            const std::uint_least32_t ja_jp_language = MurMur3::calcHash("ja-JP");

            for (std::size_t h = 0; h < location_point_list_list.size(); ++h) {
                const auto& person_location_list = location_point_list_list[h].person_location_list;
                const auto& lll = location_point_list_list[h];

                // 時間の範囲外を除去
                if (lll.min_year > cached_jdn_) continue;
                if (lll.max_year < cached_jdn_) continue;

                for (std::size_t i = 0; i < person_location_list.size(); ++i) {
                    const auto& lli = person_location_list[i];

                    // 時間の範囲外を除去
                    if (lli.min_year > cached_jdn_) continue;
                    if (lli.max_year < cached_jdn_) continue;

                    // 時間補間座標の計算
                    const auto interpolated_pos = MapCoordinateConverter::interpolatePosition(
                        lli.start_coordinate, lli.end_coordinate,
                        cached_jdn_, lli.min_year, lli.max_year
                    );

                    // スクリーン座標に変換（経度ラップ処理付き）
                    const auto screen_positions = MapCoordinateConverter::toScreenPositions(
                        interpolated_pos.x, interpolated_pos.y,
                        cached_map_view_width_, cached_map_view_height_,
                        cached_map_view_center_x_, cached_map_view_center_y_
                    );

                    // ヒット判定（肖像画のサイズは120または overall_length/2）
                    const int hit_radius = (lli.min_view > cached_map_view_width_ || lli.max_view < cached_map_view_width_)
                        ? static_cast<int>(lli.overall_length / 2)
                        : 60;  // 120 / 2

                    // 3つのスクリーン座標でヒット判定
                    const bool is_hit = MapContentHitTester::testMultiplePositions(
                        mouse_x, mouse_y, screen_positions,
                        [hit_radius](int mx, int my, const paxg::Vec2i& pos) {
                            return MapContentHitTester::circleHitTest(mx, my, pos, hit_radius);
                        }
                    );

                    if (is_hit) {
                        // ヒット！人物名を取得
                        if (lli.place_name.find(ja_jp_language) != lli.place_name.end()) {
                            hit_person_name = lli.place_name.at(ja_jp_language);
                        } else if (!lli.place_name.empty()) {
                            hit_person_name = lli.place_name.begin()->second;
                        }
                        return true;
                    }
                }
            }
            return false;
        }

    private:
        // 可視性管理
        bool visible_ = true;

        // 描画に必要なデータをキャッシュ(setDrawParams()で更新、render()で使用)
        double cached_jdn_ = 0.0;
        double cached_map_view_width_ = 0.0;
        double cached_map_view_height_ = 0.0;
        double cached_map_view_center_x_ = 0.0;
        double cached_map_view_center_y_ = 0.0;
        std::vector<PersonLocationList> location_point_list_list{}; // 人物の一覧
        // アイコンのテクスチャ
        paxs::KeyValueTSV<paxg::Texture> key_value_tsv;
        PersonNameRepository repository_; // データ読み込みを担当

        /// @brief 人物データを読み込み
        /// @brief Load person data
        void inputPersonData(
            const std::string& str_,
            const double min_view_,  // 可視化する地図の最小範囲
            const double max_view_,  // 可視化する地図の最大範囲
            const int min_year_,  // 可視化する時代(古い年～)
            const int max_year_,  // 可視化する時代(～新しい年)
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

#endif // !PAX_MAHOROBA_PERSON_PORTRAIT_MANAGER_HPP
