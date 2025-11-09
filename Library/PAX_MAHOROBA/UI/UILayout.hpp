/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_LAYOUT_HPP
#define PAX_MAHOROBA_UI_LAYOUT_HPP

#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxs {

    /// @brief パネルのレイアウト情報
    /// @brief Panel layout information
    struct PanelLayout {
        // パネルの境界
        int x = 0;
        int y = 0;
        int width = 0;
        int height = 0;

        // コンテンツ領域（パディング考慮後）
        int content_x = 0;
        int content_y = 0;
        int content_width = 0;
        int content_height = 0;

        // パディング
        int padding_top = 15;
        int padding_bottom = 15;
        int padding_left = 15;
        int padding_right = 15;

        /// @brief コンテンツ領域を計算
        /// @brief Calculate content area
        void calculateContentArea() {
            content_x = x + padding_left;
            content_y = y + padding_top;
            content_width = width - padding_left - padding_right;
            content_height = height - padding_top - padding_bottom;
        }

        paxg::Rect getRect() const {
            return paxg::Rect{
                static_cast<float>(x),
                static_cast<float>(y),
                static_cast<float>(width),
                static_cast<float>(height)
            };
        }
    };

    // UIのレイアウト情報
    // パネルの位置とサイズを計算・保持
    struct UILayout {
        // 各パネルのレイアウト
        PanelLayout calendar_panel;
        PanelLayout simulation_panel;
        PanelLayout debug_info_panel;
        PanelLayout settlement_status_panel;

        // カレンダーテキストの位置
        const int koyomi_font_y = paxg::FontConfig::PULLDOWN_FONT_SIZE + 43;
        const int koyomi_font_en_y = paxg::FontConfig::PULLDOWN_FONT_SIZE + 43;
        int koyomi_font_x = 0;
        int koyomi_font_en_x = 0;

        // パネルの位置とサイズ
        int rect_start_x = 0;
        int rect_len_x = 0;
        int koyomi_height = 0;

        // 時間操作パネルの位置
        int time_control_base_x = 0;
        int time_control_base_y = 0;
        int time_control_height = 0;

        // シミュレーション情報パネルの位置
        int next_rect_start_y = 0;
        int next_rect_end_y = 0;

        // デバッグ情報パネルの位置
        const int debug_info_panel_x = 10;
        const int debug_info_panel_y = 10;
        const int debug_info_panel_width = 300;
        const int debug_info_panel_height = 250;

        // シミュレーション情報パネルの位置
        const int simulation_info_panel_x = 40;
        const int simulation_info_panel_y = 80;
        const int simulation_info_panel_width = 300; // テキスト幅 + パディング
        const int simulation_info_panel_height = 60; // テキスト高さ + パディング

        // 集落パラメータパネルの位置
        const int settlement_status_panel_x = 40;
        const int settlement_status_panel_y = 80;
        const int settlement_status_panel_width = 300; // テキスト幅 + パディング
        const int settlement_status_panel_height = 60; // テキスト高さ + パディ

        UILayout() {
            settlement_status_panel.x = settlement_status_panel_x;
            settlement_status_panel.y = settlement_status_panel_y;
            settlement_status_panel.width = settlement_status_panel_width;
            settlement_status_panel.height = settlement_status_panel_height;
            settlement_status_panel.calculateContentArea();
        }

        // レイアウトを計算
        void calculate(
            std::size_t date_list_size,
            int time_control_panel_height
        ) {
            // 暦の位置
            koyomi_font_x = paxg::Window::width() - 270;
            koyomi_font_en_x = koyomi_font_x - 20;
            rect_start_x = koyomi_font_en_x - 165;
            rect_len_x = paxg::Window::width() - rect_start_x - 15;

            koyomi_height = static_cast<int>(date_list_size) * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3); // 暦の縦の幅

            // 時間操作パネルの位置とサイズ
            time_control_base_x = rect_len_x - 10;
            time_control_base_y = koyomi_height + 40;
            time_control_height = time_control_panel_height;

            next_rect_start_y = time_control_base_y + time_control_height + 50;
            next_rect_end_y = 280;

            // CalendarPanelのレイアウト
            calendar_panel.x = rect_start_x;
            calendar_panel.y = koyomi_font_y - 15;
            calendar_panel.width = rect_len_x;
            calendar_panel.height = next_rect_start_y;
            calendar_panel.calculateContentArea();

            // SimulationPanelのレイアウト
            const int simulation_start_y = koyomi_font_y + next_rect_start_y - 5;
            simulation_panel.x = rect_start_x;
            simulation_panel.y = simulation_start_y;
            simulation_panel.width = rect_len_x;
            simulation_panel.height = next_rect_end_y;
            simulation_panel.calculateContentArea();

            // DebugInfoPanelのレイアウト
            debug_info_panel.x = debug_info_panel_x;
            debug_info_panel.y = paxg::Window::height() - debug_info_panel_height - debug_info_panel_y;
            debug_info_panel.width = debug_info_panel_width;
            debug_info_panel.height = debug_info_panel_height;
            debug_info_panel.calculateContentArea();
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_LAYOUT_HPP
