/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_CALENDAR_UI_LAYOUT_HPP
#define PAX_MAHOROBA_CALENDAR_UI_LAYOUT_HPP

#include <PAX_GRAPHICA/Window.hpp>

namespace paxs {

    // カレンダーUIのレイアウト情報
    // パネルの位置とサイズを計算・保持
    struct CalendarUILayout {
        // カレンダーテキストの位置
        int koyomi_font_y;
        int koyomi_font_en_y;
        int koyomi_font_x;
        int koyomi_font_en_x;

        // パネルの位置とサイズ
        int rect_start_x;
        int rect_len_x;
        int koyomi_height;

        // 時間操作パネルの位置
        int time_control_base_x;
        int time_control_base_y;
        int time_control_height;

        // デバッグ情報パネルの位置
        int next_rect_start_y;
        int next_rect_end_y;

        // レイアウトを計算
        // pulldown_font_size: プルダウンのフォントサイズ
        // koyomi_font_size: カレンダーのフォントサイズ
        // date_list_size: 表示する暦の数
        // time_control_panel_height: 時間操作パネルの高さ
        void calculate(
            int pulldown_font_size,
            int koyomi_font_size,
            std::size_t date_list_size,
            int time_control_panel_height
        ) {
            // 暦の位置
            koyomi_font_y = pulldown_font_size + 43;
            koyomi_font_en_y = pulldown_font_size + 43;

            koyomi_font_x = paxg::Window::width() - 270;
            koyomi_font_en_x = koyomi_font_x - 20;
            rect_start_x = koyomi_font_en_x - 165;
            rect_len_x = paxg::Window::width() - rect_start_x - 15;

            koyomi_height = static_cast<int>(date_list_size) * (koyomi_font_size * 4 / 3); // 暦の縦の幅

            // 時間操作パネルの位置とサイズ
            time_control_base_x = rect_len_x - 10;
            time_control_base_y = koyomi_height + 40;
            time_control_height = time_control_panel_height;

            next_rect_start_y = time_control_base_y + time_control_height + 50;
            next_rect_end_y = 280;
        }

        // デバッグ情報の開始Y座標を取得
        int getDebugStartY() const {
            return koyomi_font_y + next_rect_start_y + 10;
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_CALENDAR_UI_LAYOUT_HPP
