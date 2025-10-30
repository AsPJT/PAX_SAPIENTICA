/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_TIME_CONTROL_PANEL_HPP
#define PAX_MAHOROBA_TIME_CONTROL_PANEL_HPP

#include <unordered_map>

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Calendar.hpp>

#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/TouchStateManager.hpp>

namespace paxs {

    // 時間操作パネル - 再生/一時停止/逆再生、時間移動アイコンを管理
    class TimeControlPanel {
    public:
        // 時間操作アイコンのサイズ
        int arrow_time_icon_size = 40; // 再生/停止/逆再生アイコンの大きさ
        int time_icon_size = 50;       // 時間移動アイコンの大きさ

        // アイコンの間隔
        int arrow_icon_move_x = int(arrow_time_icon_size * 1.2);
        int icon_move_x = int(time_icon_size * 1.1);
        int arrow_icon_move_y = int(arrow_time_icon_size * 1.1);
        int icon_move_y = int(time_icon_size * 1.1);

        // 時間操作パネルを更新・描画
        // base_x: パネル右端のX座標
        // base_y: パネル上端のY座標
        void update(
            int base_x,
            int base_y,
            const std::unordered_map<std::uint_least32_t, paxg::Texture>& texture_dictionary,
            paxs::TouchStateManager& tm,
            paxs::KoyomiSiv3D& koyomi_siv
        ) {
            int icon_start_x = base_x;
            int icon_start_y = 0;

            // 再生コントロール（逆再生/停止/再生）
            drawPlaybackControls(icon_start_x, base_y + icon_start_y, texture_dictionary, tm, koyomi_siv);
            icon_start_y += arrow_icon_move_y;
            icon_start_x = base_x;

            // 時間移動（過去へ: 日/月/年/10年/世紀/千年紀/万年紀）
            drawBackwardTimeControls(icon_start_x, base_y + icon_start_y, texture_dictionary, tm, koyomi_siv);
            icon_start_y += icon_move_y;
            icon_start_x = base_x;

            // 時間移動（未来へ: 日/月/年/10年/世紀/千年紀/万年紀）
            drawForwardTimeControls(icon_start_x, base_y + icon_start_y, texture_dictionary, tm, koyomi_siv);
        }

        // 時間操作パネルの高さを取得
        int getHeight() const {
            return arrow_time_icon_size + time_icon_size * 2;
        }

    private:
        // 再生/停止/逆再生ボタンを描画
        void drawPlaybackControls(
            int start_x,
            int y,
            const std::unordered_map<std::uint_least32_t, paxg::Texture>& texture_dictionary,
            paxs::TouchStateManager& tm,
            paxs::KoyomiSiv3D& koyomi_siv
        ) {
            int x = start_x;

            // 逆再生ボタン
            drawIconButton(x, y, arrow_time_icon_size, "texture_reverse_playback", texture_dictionary, tm,
                [&koyomi_siv]() {
                    koyomi_siv.move_forward_in_time = false;
                    koyomi_siv.go_back_in_time = true;
                });
            x -= arrow_icon_move_x;

            // 停止ボタン
            drawIconButton(x, y, arrow_time_icon_size, "texture_stop", texture_dictionary, tm,
                [&koyomi_siv]() {
                    koyomi_siv.move_forward_in_time = false;
                    koyomi_siv.go_back_in_time = false;
                });
            x -= arrow_icon_move_x;

            // 再生ボタン
            drawIconButton(x, y, arrow_time_icon_size, "texture_playback", texture_dictionary, tm,
                [&koyomi_siv]() {
                    koyomi_siv.move_forward_in_time = true;
                    koyomi_siv.go_back_in_time = false;
                });
        }

        // 過去への時間移動ボタンを描画
        void drawBackwardTimeControls(
            int start_x,
            int y,
            const std::unordered_map<std::uint_least32_t, paxg::Texture>& texture_dictionary,
            paxs::TouchStateManager& tm,
            paxs::KoyomiSiv3D& koyomi_siv
        ) {
            int x = start_x;

            // 日
            drawTimeButton(x, y, "texture_d_l", -1, texture_dictionary, tm, koyomi_siv);
            x -= icon_move_x;

            // 月
            drawTimeButton(x, y, "texture_m_l", -(365.2422 / 12.0), texture_dictionary, tm, koyomi_siv);
            x -= icon_move_x;

            // 年
            drawTimeButton(x, y, "texture_y_l", -365.2422, texture_dictionary, tm, koyomi_siv);
            x -= icon_move_x;

            // 10年
            drawTimeButton(x, y, "texture_10y_l", -(365.2422 * 10), texture_dictionary, tm, koyomi_siv);
            x -= icon_move_x;

            // 世紀（100年）
            drawTimeButton(x, y, "texture_c_l", -(365.2422 * 100), texture_dictionary, tm, koyomi_siv);
            x -= icon_move_x;

            // 千年紀（1000年）
            drawTimeButton(x, y, "texture_10c_l", -(365.2422 * 1000), texture_dictionary, tm, koyomi_siv);
            x -= icon_move_x;

            // 万年紀（10000年）
            drawTimeButton(x, y, "texture_100c_l", -(365.2422 * 10000), texture_dictionary, tm, koyomi_siv);
        }

        // 未来への時間移動ボタンを描画
        void drawForwardTimeControls(
            int start_x,
            int y,
            const std::unordered_map<std::uint_least32_t, paxg::Texture>& texture_dictionary,
            paxs::TouchStateManager& tm,
            paxs::KoyomiSiv3D& koyomi_siv
        ) {
            int x = start_x;

            // 日
            drawTimeButton(x, y, "texture_d_r", 1, texture_dictionary, tm, koyomi_siv);
            x -= icon_move_x;

            // 月
            drawTimeButton(x, y, "texture_m_r", (365.2422 / 12.0), texture_dictionary, tm, koyomi_siv);
            x -= icon_move_x;

            // 年
            drawTimeButton(x, y, "texture_y_r", 365.2422, texture_dictionary, tm, koyomi_siv);
            x -= icon_move_x;

            // 10年
            drawTimeButton(x, y, "texture_10y_r", (365.2422 * 10), texture_dictionary, tm, koyomi_siv);
            x -= icon_move_x;

            // 世紀（100年）
            drawTimeButton(x, y, "texture_c_r", (365.2422 * 100), texture_dictionary, tm, koyomi_siv);
            x -= icon_move_x;

            // 千年紀（1000年）
            drawTimeButton(x, y, "texture_10c_r", (365.2422 * 1000), texture_dictionary, tm, koyomi_siv);
            x -= icon_move_x;

            // 万年紀（10000年）
            drawTimeButton(x, y, "texture_100c_r", (365.2422 * 10000), texture_dictionary, tm, koyomi_siv);
        }

        // アイコンボタンを描画（汎用）
        template<typename Callback>
        void drawIconButton(
            int x,
            int y,
            int size,
            const char* texture_key,
            const std::unordered_map<std::uint_least32_t, paxg::Texture>& texture_dictionary,
            paxs::TouchStateManager& tm,
            Callback&& callback
        ) {
            paxg::Vec2i pos(paxg::Window::width() - x, y);
            texture_dictionary.at(MurMur3::calcHash(texture_key)).resizedDraw(size, pos);

            if (tm.get(paxg::Rect{ pos, paxg::Vec2i(size, size) }.leftClicked())) {
                callback();
            }
        }

        // 時間移動ボタンを描画
        void drawTimeButton(
            int x,
            int y,
            const char* texture_key,
            double day_delta,
            const std::unordered_map<std::uint_least32_t, paxg::Texture>& texture_dictionary,
            paxs::TouchStateManager& tm,
            paxs::KoyomiSiv3D& koyomi_siv
        ) {
            paxg::Vec2i pos(paxg::Window::width() - x, y);
            texture_dictionary.at(MurMur3::calcHash(texture_key)).resizedDraw(time_icon_size, pos);

            if (tm.get(paxg::Rect{ pos, paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                koyomi_siv.jdn.getDay() += day_delta;
                koyomi_siv.calcDate();
            }
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_TIME_CONTROL_PANEL_HPP
