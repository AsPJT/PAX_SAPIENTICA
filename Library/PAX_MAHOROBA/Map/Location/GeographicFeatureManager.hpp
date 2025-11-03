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

#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Texture.hpp>

#include <PAX_MAHOROBA/Map/Location/GeographicFeatureRenderer.hpp>
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
        GeographicFeatureManager() = default;

        // 地理的特徴データを追加
        void add() {
            // リポジトリに委譲してデータを読み込む
            repository_.loadPlaceNameList(
                [this](const std::string& file_path, double min_view, double max_view, int min_year, int max_year,
                       std::uint_least32_t lpe, std::uint_least32_t place_texture, double zoom) {
                    inputPlace(file_path, min_view, max_view, min_year, max_year, lpe, place_texture, zoom);
                }
            );
        }

        void init() {
            std::string str = "";
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("MiniIcons"),
                [&](const std::string& path_) {str = path_; });
            if (str.size() == 0) return;

            if (!key_value_tsv.input(str, [&](const std::string& value_) { return paxg::Texture{ value_ }; })) {
                PAXS_ERROR("Failed to load texture KeyValueTSV: " + str);
            }
        }

        // IRenderable の実装
        // IRenderable implementation

        /// @brief レンダリング処理
        /// @brief Render
        void render() override {
            if (!visible_) return;
            if (cached_visible_ == nullptr) return;
            if (cached_font_ == nullptr || cached_en_font_ == nullptr || cached_pin_font_ == nullptr) return;

            // 描画処理をレンダラーに委譲
            renderer_.draw(
                location_point_list_list,
                key_value_tsv.get(),
                *cached_visible_,
                cached_jdn_,
                cached_map_view_width_,
                cached_map_view_height_,
                cached_map_view_center_x_,
                cached_map_view_center_y_,
                *cached_font_,
                *cached_en_font_,
                *cached_pin_font_
            );
        }

        /// @brief レンダリングレイヤーを取得
        /// @brief Get rendering layer
        RenderLayer getLayer() const override {
            return RenderLayer::MapContent;
        }

        /// @brief 可視性を取得
        /// @brief Get visibility
        bool isVisible() const override {
            return visible_;
        }

        /// @brief 可視性を設定
        /// @brief Set visibility
        void setVisible(bool visible) override {
            visible_ = visible;
        }

        /// @brief 描画パラメータを設定（MapContentManager から呼び出される）
        /// @brief Set drawing parameters (called from MapContentManager)
        void setDrawParams(
            paxs::FeatureVisibilityManager& visible,
            const double jdn,
            const double map_view_width, const double map_view_height,
            const double map_view_center_x, const double map_view_center_y,
            paxg::Font& font, paxg::Font& en_font, paxg::Font& pin_font
        ) {
            cached_visible_ = &visible;
            cached_jdn_ = jdn;
            cached_map_view_width_ = map_view_width;
            cached_map_view_height_ = map_view_height;
            cached_map_view_center_x_ = map_view_center_x;
            cached_map_view_center_y_ = map_view_center_y;
            cached_font_ = &font;
            cached_en_font_ = &en_font;
            cached_pin_font_ = &pin_font;
        }

    private:
        // 可視性管理
        bool visible_ = true;

        // 描画に必要なデータをキャッシュ（setDrawParams()で更新、render()で使用）
        paxs::FeatureVisibilityManager* cached_visible_ = nullptr;
        double cached_jdn_ = 0.0;
        double cached_map_view_width_ = 0.0;
        double cached_map_view_height_ = 0.0;
        double cached_map_view_center_x_ = 0.0;
        double cached_map_view_center_y_ = 0.0;
        paxg::Font* cached_font_ = nullptr;
        paxg::Font* cached_en_font_ = nullptr;
        paxg::Font* cached_pin_font_ = nullptr;

        std::vector<LocationPointList> location_point_list_list{}; // 地理的特徴の一覧
        // アイコンのテクスチャ
        paxs::KeyValueTSV<paxg::Texture> key_value_tsv;
        // 描画処理を担当
        GeographicFeatureRenderer renderer_;
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
