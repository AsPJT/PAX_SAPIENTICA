/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_DEBUG_INFO_PANEL_HPP
#define PAX_MAHOROBA_DEBUG_INFO_PANEL_HPP

#include <array>
#include <cmath>
#include <string>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/RoundRect.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/UI/UILayout.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/Rendering/LanguageFonts.hpp>
#include <PAX_MAHOROBA/Rendering/ShadowRenderer.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_SAPIENTICA/Simulation/Simulator.hpp>
#endif

namespace paxs {

    /// @brief デバッグ情報パネルを表示するクラス
    /// @brief Debug information panel class
    class DebugInfoPanel : public IWidget {
    private:
        paxs::LanguageFonts* language_fonts_ptr = nullptr;
        const paxs::Language* language_text_ptr = nullptr;
        const SelectLanguage* select_language_ptr = nullptr;
        const MapViewport* map_viewport_ptr = nullptr;
        const paxs::FeatureVisibilityManager* visible_manager_ptr = nullptr;

        bool enabled_ = true;
        const UILayout* ui_layout_ = nullptr;

    public:
        DebugInfoPanel() = default;
        DebugInfoPanel(const UILayout& ui_layout, const paxs::FeatureVisibilityManager* visible_manager)
            : ui_layout_(&ui_layout), visible_manager_ptr(visible_manager) {}

        /// @brief 初期化
        void init(paxs::LanguageFonts* fonts,
            const paxs::Language* language_text,
            const SelectLanguage* select_language,
            const MapViewport* map_viewport) {
            language_fonts_ptr = fonts;
            language_text_ptr = language_text;
            select_language_ptr = select_language;
            map_viewport_ptr = map_viewport;
        }

        const char* getName() const override {
            return "DebugInfoPanel";
        }

        /// @brief レンダリングレイヤーを取得（背景レイヤー）
        /// @brief Get rendering layer (background layer)
        RenderLayer getLayer() const override {
            return RenderLayer::UIContent;
        }

        // 可視性の設定・取得
        void setVisible(bool visible) override {
            (void)visible;
        }
        bool isVisible() const override { return visible_manager_ptr->isVisible(MurMur3::calcHash("UI")) && visible_manager_ptr->isVisible(MurMur3::calcHash("Debug")); }

        /// @brief マップ情報とシミュレーション統計を描画
        /// @brief Render map information and simulation statistics
        void renderMapAndSimulationInfo(
#ifdef PAXS_USING_SIMULATOR
            const std::unique_ptr<paxs::SettlementSimulator>& simulator,
#endif
            const paxs::Koyomi* koyomi = nullptr,
            bool is_simulator_active = false
        ) const {
            if (!isVisible()) return;

            // フォントを取得
            paxg::Font* font = language_fonts_ptr->getAndAdd(
                select_language_ptr->cgetKey(),
                static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_SIZE),
                static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_BUFFER_THICKNESS)
            );
            if (font == nullptr) return;

            font->setOutline(0, 0.6, paxg::Color(255, 255, 255));

            const int text_x = ui_layout_->debug_info_panel.x + 15; // パネル内の左端
            const int text_y = ui_layout_->debug_info_panel.y + 15; // パネル内の上端
            constexpr int line_height = 25;
            int current_line = 0;

            // タイトル
            font->draw(
                (select_language_ptr->cgetKey() == MurMur3::calcHash("ja-JP")) ?
                    reinterpret_cast<const char*>(u8"デバッグ情報") : "Debug Info",
                paxg::Vec2i(text_x, text_y + line_height * current_line++),
                paxg::Color(0, 0, 0)
            );

            // マップの拡大率
            font->draw(
                (select_language_ptr->cgetKey() == MurMur3::calcHash("ja-JP")) ?
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
            const std::string* const xyz_label_ptr = language_text_ptr->getStringPtr(
                MurMur3::calcHash("debug_xyz_tiles_z"),
                select_language_ptr->cgetKey()
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

#ifdef PAXS_USING_SIMULATOR
            if (simulator != nullptr) {
                // 人口数
                font->draw(
                    (select_language_ptr->cgetKey() == MurMur3::calcHash("ja-JP")) ?
                        reinterpret_cast<const char*>(u8"人口: ") : "Population: ",
                    paxg::Vec2i(text_x, text_y + line_height * current_line),
                    paxg::Color(0, 0, 0)
                );
                font->draw(
                    std::to_string(simulator->cgetPopulationNum()),
                    paxg::Vec2i(text_x + 130, text_y + line_height * current_line++),
                    paxg::Color(0, 0, 0)
                );

                // 集落数
                font->draw(
                    (select_language_ptr->cgetKey() == MurMur3::calcHash("ja-JP")) ?
                        reinterpret_cast<const char*>(u8"集落: ") : "Settlements: ",
                    paxg::Vec2i(text_x, text_y + line_height * current_line),
                    paxg::Color(0, 0, 0)
                );
                font->draw(
                    std::to_string(simulator->cgetSettlement()),
                    paxg::Vec2i(text_x + 130, text_y + line_height * current_line++),
                    paxg::Color(0, 0, 0)
                );
            }
#endif

            // 大きな年号を描画（シミュレーション非アクティブ時のみ）
            if (koyomi != nullptr && !is_simulator_active && !koyomi->date_list.empty()) {
                // グレゴリオ暦の年を取得（date_list[1]がグレゴリオ暦）
                if (koyomi->date_list.size() > 1) {
                    const int date_year = [&]() {
                        int year = 0;
                        std::visit([&](const auto& x) {
                            year = int(x.cgetYear());
                        }, koyomi->date_list[1].date);
                        return year;
                    }();

                    if (date_year > 0) {
                        // 大きな年号フォントを取得（通常の3倍サイズ）
                        paxg::Font* big_year_font = language_fonts_ptr->getAndAdd(
                            select_language_ptr->cgetKey(),
                            static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_SIZE * 3),
                            static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_BUFFER_THICKNESS)
                        );
                        if (big_year_font != nullptr) {
                            big_year_font->setOutline(0, 0.6, paxg::Color(255, 255, 255));

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

    public:
        // IWidget インターフェースの実装
        EventHandlingResult handleEvent(const MouseEvent& event) override {
            // DebugInfoPanelは入力処理を行わない
            (void)event;
            return EventHandlingResult::NotHandled();
        }

        void render() const override {
            // TODO: 描画処理
            // renderMapAndSimulationInfo();
        }

        paxg::Rect getRect() const override {
            return paxg::Rect{
                static_cast<float>(ui_layout_->debug_info_panel.x),
                static_cast<float>(ui_layout_->debug_info_panel.y),
                static_cast<float>(ui_layout_->debug_info_panel.width),
                static_cast<float>(ui_layout_->debug_info_panel.height)
            };
        }

        void setPos(const paxg::Vec2i& /*pos*/) override {}

        bool isHit(int x, int y) const override {
            if (!isVisible() || !isEnabled()) return false;
            return (x >= ui_layout_->debug_info_panel.x && x < ui_layout_->debug_info_panel.x + ui_layout_->debug_info_panel.width &&
                y >= ui_layout_->debug_info_panel.y && y < ui_layout_->debug_info_panel.y + ui_layout_->debug_info_panel.height);
            // TODO: child
        }

        void setEnabled(bool enabled) override { enabled_ = enabled; }
        bool isEnabled() const override { return enabled_; }
    };

}

#endif // !PAX_MAHOROBA_DEBUG_INFO_PANEL_HPP
