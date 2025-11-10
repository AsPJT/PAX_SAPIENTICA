/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_GEOGRAPHIC_FEATURE_MANAGER_HPP
#define PAX_MAHOROBA_GEOGRAPHIC_FEATURE_MANAGER_HPP

#include <cstdint>
#include <string>
#include <vector>

#include <PAX_GRAPHICA/Texture.hpp>

#include <PAX_MAHOROBA/Map/Location/GeographicFeatureRenderer.hpp>
#include <PAX_MAHOROBA/Map/Location/MapCoordinateConverter.hpp>
#include <PAX_MAHOROBA/Map/Location/MapContentHitTester.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>
#include <PAX_SAPIENTICA/GeographicInformation/PlaceNameRepository.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/Map/LocationPoint.hpp>

namespace paxs {

    /// @brief GUI に描画する地理的特徴(地名とアイコン)の描画を管理するクラス
    /// @brief Class to manage geographic information for GUI rendering
    class GeographicFeatureManager : public IRenderable {
    public:
        GeographicFeatureManager() {
            std::string str = "";
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("MiniIcons"),
                [&](const std::string& path_) {str = path_; });
            if (str.size() == 0) return;

            if (!key_value_tsv.input(str, [&](const std::string& value_) { return paxg::Texture{ value_ }; })) {
                PAXS_ERROR("Failed to load texture KeyValueTSV: " + str);
            }
            // リポジトリに委譲してデータを読み込む
            repository_.loadPlaceNameList(
                [this](const std::string& file_path, double min_view, double max_view, int min_year, int max_year,
                       std::uint_least32_t lpe, std::uint_least32_t place_texture, double zoom) {
                    inputPlace(file_path, min_view, max_view, min_year, max_year, lpe, place_texture, zoom);
                }
            );
        }

        void render() const override {
            if (cached_visible_ == nullptr) return;

            // 描画処理をレンダラーに委譲
            GeographicFeatureRenderer::draw(
                location_point_list_list,
                key_value_tsv.get(),
                *cached_visible_,
                cached_jdn_,
                cached_map_view_width_,
                cached_map_view_height_,
                cached_map_view_center_x_,
                cached_map_view_center_y_
            );
        }

        /// @brief 描画パラメータを設定
        void setDrawParams(
            const paxs::FeatureVisibilityManager& visible,
            const double jdn,
            const double map_view_width, const double map_view_height,
            const double map_view_center_x, const double map_view_center_y
        ) {
            cached_visible_ = &visible;
            cached_jdn_ = jdn;
            cached_map_view_width_ = map_view_width;
            cached_map_view_height_ = map_view_height;
            cached_map_view_center_x_ = map_view_center_x;
            cached_map_view_center_y_ = map_view_center_y;
        }

        RenderLayer getLayer() const override {
            return RenderLayer::MapContent;
        }
        // TODO: isVisible() の実装を検討
        bool isVisible() const override { return true; }
        void setVisible(bool /*visible*/) override {}
        bool isHit(int mouse_x, int mouse_y, std::string& hit_place_name) const {
            if (cached_visible_ == nullptr) return false;

            const std::uint_least32_t ja_jp_language = MurMur3::calcHash("ja-JP");

            for (std::size_t h = 0; h < location_point_list_list.size(); ++h) {
                const auto& location_point_list = location_point_list_list[h].location_point_list;
                const auto& lll = location_point_list_list[h];

                // 時間の範囲外を除去
                if (lll.min_year > cached_jdn_) continue;
                if (lll.max_year < cached_jdn_) continue;

                if (!cached_visible_->isVisible(lll.lpe)) continue;

                for (std::size_t i = 0; i < location_point_list.size(); ++i) {
                    const auto& lli = location_point_list[i];

                    // 時間の範囲外を除去
                    if (lli.min_year > cached_jdn_) continue;
                    if (lli.max_year < cached_jdn_) continue;

                    // スクリーン座標に変換（経度ラップ処理付き）
                    const auto screen_positions = MapCoordinateConverter::toScreenPositions(
                        lli.coordinate.x, lli.coordinate.y,
                        cached_map_view_width_, cached_map_view_height_,
                        cached_map_view_center_x_, cached_map_view_center_y_
                    );

                    // ヒット判定（アイコンのサイズを基準）
                    const int hit_radius = static_cast<int>(lli.overall_length / 2 * lli.zoom);

                    // 3つのスクリーン座標でヒット判定
                    const bool is_hit = MapContentHitTester::testMultiplePositions(
                        mouse_x, mouse_y, screen_positions,
                        [hit_radius](int mx, int my, const paxg::Vec2i& pos) {
                            return MapContentHitTester::circleHitTest(mx, my, pos, hit_radius);
                        }
                    );

                    if (is_hit) {
                        // ヒット！地名を取得
                        if (lli.place_name.find(ja_jp_language) != lli.place_name.end()) {
                            hit_place_name = lli.place_name.at(ja_jp_language);
                        } else if (!lli.place_name.empty()) {
                            hit_place_name = lli.place_name.begin()->second;
                        }
                        return true;
                    }
                }
            }
            return false;
        }

    private:
        // 描画に必要なデータをキャッシュ（setDrawParams()で更新、render()で使用）
        const paxs::FeatureVisibilityManager* cached_visible_ = nullptr;
        double cached_jdn_ = 0.0;
        double cached_map_view_width_ = 0.0;
        double cached_map_view_height_ = 0.0;
        double cached_map_view_center_x_ = 0.0;
        double cached_map_view_center_y_ = 0.0;

        std::vector<LocationPointList> location_point_list_list{}; // 地理的特徴の一覧
        // アイコンのテクスチャ
        paxs::KeyValueTSV<paxg::Texture> key_value_tsv;
        // データ読み込みを担当
        PlaceNameRepository repository_;

        // 地理的特徴データを読み込み（リポジトリに委譲）
        void inputPlace(
            const std::string& str_,
            const double min_view_,
            const double max_view_,
            const int min_year_,
            const int max_year_,
            const std::uint_least32_t lpe_,
            const std::uint_least32_t place_texture_,
            const double zoom_
        ) {
            // リポジトリからLocationPointListを取得して追加
            LocationPointList loaded = repository_.loadPlaceFromFile(
                str_, min_view_, max_view_, min_year_, max_year_, lpe_, place_texture_, zoom_
            );

            // 空でない場合のみ追加
            if (loaded.location_point_list.size() > 0) {
                location_point_list_list.emplace_back(loaded);
            }
        }
    };
}

#endif // !PAX_MAHOROBA_GEOGRAPHIC_FEATURE_MANAGER_HPP
