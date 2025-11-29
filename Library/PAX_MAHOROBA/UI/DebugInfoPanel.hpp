/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_DEBUG_INFO_PANEL_HPP
#define PAX_MAHOROBA_DEBUG_INFO_PANEL_HPP

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Map/Core/MapViewport.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>
#include <PAX_MAHOROBA/Rendering/UIComponent.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief デバッグ情報パネルを表示するクラス
    class DebugInfoPanel : public UIComponent {
    private:
        const paxs::FeatureVisibilityManager& visible_manager_;
        const MapViewport& map_viewport_;
        const UILayout& ui_layout_;
        const Koyomi& koyomi_;

        // Locales ドメインキー定数
        static constexpr std::uint_least32_t ui_map_domain_key = MurMur3::calcHash("Map");

        // デバッグ情報のテキストキー定数
        static constexpr std::uint_least32_t magnification_power_key = MurMur3::calcHash("magnification_power");
        static constexpr std::uint_least32_t mercator_longitude_key = MurMur3::calcHash("mercator_longitude");
        static constexpr std::uint_least32_t mercator_latitude_key = MurMur3::calcHash("mercator_latitude");
        static constexpr std::uint_least32_t latitude_key = MurMur3::calcHash("latitude");
        static constexpr std::uint_least32_t xyz_tiles_z_key = MurMur3::calcHash("xyz_tiles_z");

    public:
        DebugInfoPanel(const UILayout& ui_layout,
            const paxs::FeatureVisibilityManager& visible_manager,
            const MapViewport& map_viewport,
            const Koyomi& koyomi)
            : visible_manager_(visible_manager)
            , map_viewport_(map_viewport)
            , ui_layout_(ui_layout)
            , koyomi_(koyomi) {}

        void render() const override {
            if (!isVisible()) return;

            paxg::Font* font = Fonts().getFont(paxs::FontProfiles::koyomi);
            if (font == nullptr) return;

            font->setOutline(0, 0.6, paxg::Color(243, 243, 243));

            const int text_x = ui_layout_.debug_info_panel.x + 15; // パネル内の左端
            const int text_y = ui_layout_.debug_info_panel.y + 10; // パネル内の上端
            const Vector2<double> map_viewport_center = map_viewport_.getCenter();
            const int line_height = 25;

            int current_line = 0;

            // マップの拡大率
            const std::string* const mag_label_ptr = Fonts().getLocalesText(
                ui_map_domain_key,
                magnification_power_key
            );
            if (mag_label_ptr != nullptr) {
                font->draw(
                    *mag_label_ptr,
                    paxg::Vec2i(text_x, text_y + line_height * current_line++),
                    paxg::Color(0, 0, 0)
                );
            }
            font->draw(
                std::to_string(map_viewport_.getHeight()),
                paxg::Vec2i(text_x + 10, text_y + line_height * current_line++),
                paxg::Color(0, 0, 0)
            );

            // XYZ Tiles Z拡大率
            const int z_magnification = static_cast<int>(-std::log2(map_viewport_.getHeight()) + 12.5);
            const std::string* const xyz_label_ptr = Fonts().getLocalesText(
                ui_map_domain_key,
                xyz_tiles_z_key
            );
            if (xyz_label_ptr != nullptr) {
                font->draw(
                    *xyz_label_ptr,
                    paxg::Vec2i(text_x, text_y + line_height * current_line++),
                    paxg::Color(0, 0, 0)
                );
            }
            font->draw(
                std::to_string(z_magnification),
                paxg::Vec2i(text_x + 10, text_y + line_height * current_line++),
                paxg::Color(0, 0, 0)
            );

            // EPSG:3857(Webメルカトル) 経度
            const std::string* const merc_lon_label_ptr = Fonts().getLocalesText(
                ui_map_domain_key,
                mercator_longitude_key
            );
            if (merc_lon_label_ptr != nullptr) {
                font->draw(
                    *merc_lon_label_ptr,
                    paxg::Vec2i(text_x, text_y + line_height * current_line++),
                    paxg::Color(0, 0, 0)
                );
            }
            font->draw(
                std::to_string(map_viewport_center.x),
                paxg::Vec2i(text_x + 10, text_y + line_height * current_line++),
                paxg::Color(0, 0, 0)
            );

            // EPSG:3857(Webメルカトル) 緯度
            const std::string* const merc_lat_label_ptr = Fonts().getLocalesText(
                ui_map_domain_key,
                mercator_latitude_key
            );
            if (merc_lat_label_ptr != nullptr) {
                font->draw(
                    *merc_lat_label_ptr,
                    paxg::Vec2i(text_x, text_y + line_height * current_line++),
                    paxg::Color(0, 0, 0)
                );
            }
            font->draw(
                std::to_string(map_viewport_center.y),
                paxg::Vec2i(text_x + 10, text_y + line_height * current_line++),
                paxg::Color(0, 0, 0)
            );

            // 緯度（EPSG4326_WGS84）
            const std::string* const lat_label_ptr = Fonts().getLocalesText(
                ui_map_domain_key,
                latitude_key
            );
            if (lat_label_ptr != nullptr) {
                font->draw(
                    *lat_label_ptr,
                    paxg::Vec2i(text_x, text_y + line_height * current_line++),
                    paxg::Color(0, 0, 0)
                );
            }
            // EPSG:3857(Webメルカトル) 座標から EPSG:4326(WGS84) の緯度に変換
            const paxs::WebMercatorDeg merc_coord(paxs::Vector2<double>(map_viewport_center.x, map_viewport_center.y));
            font->draw(
                std::to_string(merc_coord.toEPSG4326_WGS84DegY()),
                paxg::Vec2i(text_x + 10, text_y + line_height * current_line++),
                paxg::Color(0, 0, 0)
            );

            // 大きな年号を描画
            if (!koyomi_.date_list.empty()) {
                // グレゴリオ暦の年を取得（date_list[1]がグレゴリオ暦）
                if (koyomi_.date_list.size() > 1) {
                    const int date_year = [&]() {
                        int year = 0;
                        std::visit([&](const auto& x) {
                            using T = std::decay_t<decltype(x)>;
                            if constexpr (paxs::cal::YearMonthDayDateType<T>) {
                                year = int(x.getYear());
                            }
                        }, koyomi_.date_list[1].date);
                        return year;
                    }();

                    if (date_year > 0) {
                        // 大きな年号フォントを取得（通常の3倍サイズ）
                        paxg::Font* big_year_font = Fonts().getFont(
                            static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_SIZE * 3),
                            static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_BUFFER_THICKNESS)
                        );
                        if (big_year_font != nullptr) {
                            big_year_font->setOutline(0, 0.6, paxg::Color(243, 243, 243));

                            // パネル内の右下に配置（右揃え）
                            const int big_year_x = ui_layout_.debug_info_panel.x + ui_layout_.debug_info_panel.width - 15;  // 右端から15px左
                            const int big_year_y = ui_layout_.debug_info_panel.y + ui_layout_.debug_info_panel.height - 25;  // パネル下部から25px上

                            big_year_font->drawBottomRight(
                                std::to_string(date_year),
                                paxg::Vec2i(big_year_x, big_year_y),
                                paxg::Color(0, 0, 0)
                            );
                        }
                    }
                }
            }
        }

        Rect<int> getRect() const override {
            return ui_layout_.debug_info_panel.getRect();
        }

        bool isVisible() const override {
            return visible_manager_.isVisible(ViewMenu::debug);
        }

        RenderLayer getLayer() const override { return RenderLayer::UIContent; }
        const char* getName() const override { return "DebugInfoPanel"; }
        void setPos(const Vector2<int>& /*pos*/) override {}
    };

}

#endif // !PAX_MAHOROBA_DEBUG_INFO_PANEL_HPP
