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

#include <PAX_MAHOROBA/UI/UILayout.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief デバッグ情報パネルを表示するクラス
    class DebugInfoPanel : public IWidget {
    private:
        const MapViewport* map_viewport_ptr = nullptr;
        const paxs::FeatureVisibilityManager* visible_manager_ptr = nullptr;
        const UILayout* ui_layout_ = nullptr;
        Koyomi* koyomi_ = nullptr;

        // TODO: 表示
        std::size_t map_viewport_width_str_index = MurMur3::calcHash(25, "debug_magnification_power");
        std::size_t map_viewport_center_x_str_index = MurMur3::calcHash(24, "debug_mercator_longitude");
        std::size_t map_viewport_center_y_str_index = MurMur3::calcHash(23, "debug_mercator_latitude");
        std::size_t map_viewport_center_lat_str_index = MurMur3::calcHash(14, "debug_latitude");

    public:
        DebugInfoPanel(const UILayout& ui_layout,
            const paxs::FeatureVisibilityManager* visible_manager,
            const MapViewport* map_viewport
        ) : ui_layout_(&ui_layout), visible_manager_ptr(visible_manager), map_viewport_ptr(map_viewport) {}

        const char* getName() const override {
            return "DebugInfoPanel";
        }

        /// @brief レンダリングレイヤーを取得（背景レイヤー）
        /// @brief Get rendering layer (background layer)
        RenderLayer getLayer() const override {
            return RenderLayer::UIContent;
        }

        bool isVisible() const override {
            return visible_manager_ptr->isVisible(FeatureVisibilityManager::View::Debug);
        }


    public:
        // IWidget インターフェースの実装
        EventHandlingResult handleEvent(const MouseEvent& event) override {
            // DebugInfoPanelは入力処理を行わない
            (void)event;
            return EventHandlingResult::NotHandled();
        }

        void render() const override {
            if (!isVisible()) return;

            paxg::Font* font = Fonts().getFont(paxs::FontProfiles::KOYOMI);
            if (font == nullptr) return;

            font->setOutline(0, 0.6, paxg::Color(243, 243, 243));

            const int text_x = ui_layout_->debug_info_panel.x + 15; // パネル内の左端
            const int text_y = ui_layout_->debug_info_panel.y + 15; // パネル内の上端
            const int line_height = 25;

            int current_line = 0;

            // タイトル
            font->draw(
                (Fonts().getSelectedLanguage().cgetKey() == MurMur3::calcHash("ja-JP")) ?
                    reinterpret_cast<const char*>(u8"デバッグ情報") : "Debug Info",
                paxg::Vec2i(text_x, text_y + line_height * current_line++),
                paxg::Color(0, 0, 0)
            );

            // マップの拡大率
            font->draw(
                (Fonts().getSelectedLanguage().cgetKey() == MurMur3::calcHash("ja-JP")) ?
                    reinterpret_cast<const char*>(u8"拡大率: ") : "Zoom: ",
                paxg::Vec2i(text_x, text_y + line_height * current_line),
                paxg::Color(0, 0, 0)
            );
            font->draw(
                std::to_string(map_viewport_ptr->getHeight()),
                paxg::Vec2i(text_x + 100, text_y + line_height * current_line++),
                paxg::Color(0, 0, 0)
            );

            // XYZ Tiles Z拡大率
            const int z_magnification = static_cast<int>(-std::log2(map_viewport_ptr->getHeight()) + 12.5);
            const std::string* const xyz_label_ptr = Fonts().getText(
                MurMur3::calcHash("debug_xyz_tiles_z"),
                LanguageDomain::UI
            );
            if (xyz_label_ptr != nullptr) {
                font->draw(
                    (*xyz_label_ptr + ":").c_str(),
                    paxg::Vec2i(text_x, text_y + line_height * current_line),
                    paxg::Color(0, 0, 0)
                );
            }
            font->draw(
                std::to_string(z_magnification),
                paxg::Vec2i(text_x + 130, text_y + line_height * current_line++),
                paxg::Color(0, 0, 0)
            );

            // 大きな年号を描画
            if (koyomi_ != nullptr && !koyomi_->date_list.empty()) {
                // グレゴリオ暦の年を取得（date_list[1]がグレゴリオ暦）
                if (koyomi_->date_list.size() > 1) {
                    const int date_year = [&]() {
                        int year = 0;
                        std::visit([&](const auto& x) {
                            year = int(x.cgetYear());
                        }, koyomi_->date_list[1].date);
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

                            // パネル内の下部に配置
                            const int big_year_x = text_x;
                            const int big_year_y = ui_layout_->debug_info_panel.y + ui_layout_->debug_info_panel.height - 80;  // パネル下部から80px上

                            big_year_font->draw(
                                std::to_string(date_year),
                                paxg::Vec2i(big_year_x, big_year_y),
                                paxg::Color(0, 0, 0)
                            );
                        }
                    }
                }
            }
        }

        paxg::Rect getRect() const override {
            return ui_layout_->debug_info_panel.getRect();
        }

        bool isHit(int x, int y) const override {
            if (!isVisible() || !isEnabled()) return false;
            (void)x; (void)y;
            return false;
            // TODO: child
        }

        void setVisible(bool /*visible*/) override {}
        void setEnabled(bool /*enabled*/) override {}
        bool isEnabled() const override { return true; }
        void setPos(const paxg::Vec2i& /*pos*/) override {}
    };

}

#endif // !PAX_MAHOROBA_DEBUG_INFO_PANEL_HPP
