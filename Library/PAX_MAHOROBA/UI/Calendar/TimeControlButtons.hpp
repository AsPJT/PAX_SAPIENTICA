/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_CALENDAR_TIME_CONTROL_WIDGET_HPP
#define PAX_MAHOROBA_CALENDAR_TIME_CONTROL_WIDGET_HPP

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Rendering/IWidget.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief 時間操作ウィジェット - 再生/一時停止/逆再生、時間移動アイコンを管理
    class TimeControlButtons : public IWidget {
    public:
        // 時間操作アイコンのサイズ
        const int arrow_time_icon_size = 40; // 再生/停止/逆再生アイコンの大きさ
        const int time_icon_size = 50;       // 時間移動アイコンの大きさ

        // アイコンの間隔x
        const int arrow_icon_move_x = int(arrow_time_icon_size * 1.2);
        const int arrow_icon_move_y = int(arrow_time_icon_size * 1.1);
        const int icon_move_x = int(time_icon_size * 1.1);
        const int icon_move_y = int(time_icon_size * 1.1);

        // 時間操作パネルの高さを取得
        int getHeight() const {
            return arrow_time_icon_size + time_icon_size * 2;
        }

    private:
        // どのボタンかを識別するID
        enum class ButtonId {
            None,

            // playback row
            PlaybackReverse,
            PlaybackStop,
            PlaybackForward,

            // backward row
            DayBackward,
            MonthBackward,
            YearBackward,
            Year10Backward,
            CenturyBackward,
            Century10Backward,
            Century100Backward,

            // forward row
            DayForward,
            MonthForward,
            YearForward,
            Year10Forward,
            CenturyForward,
            Century10Forward,
            Century100Forward,
        };

        bool enabled_ = true;
        paxg::Vec2i pos_{0, 0};

        paxs::KeyValueTSV<paxg::Texture> key_value_tsv;
        const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>* texture_dictionary_ = nullptr;
        paxs::Koyomi* koyomi_ = nullptr;

        // 状態更新（クリック判定などの入力処理）
        void updateState(const MouseEvent& event) {
            if (event.left_button_state != MouseButtonState::Released) return;

            // どのボタンが押されたかだけを判定
            ButtonId id = hitTestButton(event.x, event.y);
            if (id == ButtonId::None) return;

            // ボタンに応じた処理を実行
            executeButton(id);
        }

        // 全てのコントロールを描画
        void drawAllControls() const {
            int icon_start_x = pos_.x();
            int icon_start_y = pos_.y();

            // 再生コントロール
            drawPlaybackControlsOnly(icon_start_x, icon_start_y);
            icon_start_y += arrow_icon_move_y;

            // 過去へ
            drawBackwardTimeControlsOnly(icon_start_x, icon_start_y);
            icon_start_y += icon_move_y;

            // 未来へ
            drawForwardTimeControlsOnly(icon_start_x, icon_start_y);
        }

        ButtonId hitTestButton(int mx, int my) const {
            int icon_start_x = pos_.x();
            int icon_start_y = pos_.y();

            // 1. 再生コントロール行
            {
                int x = icon_start_x;
                if (isHitRect(mx, my, paxg::Window::width() - x, icon_start_y,
                              arrow_time_icon_size, arrow_time_icon_size)) {
                    return ButtonId::PlaybackReverse;
                }
                x -= arrow_icon_move_x;
                if (isHitRect(mx, my, paxg::Window::width() - x, icon_start_y,
                              arrow_time_icon_size, arrow_time_icon_size)) {
                    return ButtonId::PlaybackStop;
                }
                x -= arrow_icon_move_x;
                if (isHitRect(mx, my, paxg::Window::width() - x, icon_start_y,
                              arrow_time_icon_size, arrow_time_icon_size)) {
                    return ButtonId::PlaybackForward;
                }
            }
            icon_start_y += arrow_icon_move_y;

            // 2. 過去へ行
            {
                // 左に進むキーは7個
                static constexpr ButtonId ids[] = {
                    ButtonId::DayBackward,
                    ButtonId::MonthBackward,
                    ButtonId::YearBackward,
                    ButtonId::Year10Backward,
                    ButtonId::CenturyBackward,
                    ButtonId::Century10Backward,
                    ButtonId::Century100Backward,
                };
                int x = icon_start_x;
                for (ButtonId id : ids) {
                    if (isHitRect(mx, my, paxg::Window::width() - x, icon_start_y,
                                  time_icon_size, time_icon_size)) {
                        return id;
                    }
                    x -= icon_move_x;
                }
            }
            icon_start_y += icon_move_y;

            // 3. 未来へ行
            {
                static constexpr ButtonId ids[] = {
                    ButtonId::DayForward,
                    ButtonId::MonthForward,
                    ButtonId::YearForward,
                    ButtonId::Year10Forward,
                    ButtonId::CenturyForward,
                    ButtonId::Century10Forward,
                    ButtonId::Century100Forward,
                };
                int x = icon_start_x;
                for (ButtonId id : ids) {
                    if (isHitRect(mx, my, paxg::Window::width() - x, icon_start_y,
                                  time_icon_size, time_icon_size)) {
                        return id;
                    }
                    x -= icon_move_x;
                }
            }

            return ButtonId::None;
        }

        static bool isHitRect(int mx, int my, int rx, int ry, int w, int h) {
            return (mx >= rx && mx < rx + w && my >= ry && my < ry + h);
        }

        void executeButton(ButtonId id) {
            if (!koyomi_) return;

            // 再生系
            switch (id) {
            case ButtonId::PlaybackReverse:
                koyomi_->move_forward_in_time = false;
                koyomi_->go_back_in_time = true;
                return;
            case ButtonId::PlaybackStop:
                koyomi_->move_forward_in_time = false;
                koyomi_->go_back_in_time = false;
                return;
            case ButtonId::PlaybackForward:
                koyomi_->move_forward_in_time = true;
                koyomi_->go_back_in_time = false;
                return;
            default:
                break;
            }

            // 日付移動系
            auto add_days = [&](double d) {
                koyomi_->jdn.getDay() += d;
                koyomi_->calcDate();
            };

            constexpr double day   = 1.0;
            constexpr double month = (365.2422 / 12.0);
            constexpr double year  = 365.2422;

            switch (id) {
            // backward
            case ButtonId::DayBackward:        add_days(-day);         break;
            case ButtonId::MonthBackward:      add_days(-month);       break;
            case ButtonId::YearBackward:       add_days(-year);        break;
            case ButtonId::Year10Backward:     add_days(-year * 10);   break;
            case ButtonId::CenturyBackward:    add_days(-year * 100);  break;
            case ButtonId::Century10Backward:  add_days(-year * 1000); break;
            case ButtonId::Century100Backward: add_days(-year * 10000);break;

            // forward
            case ButtonId::DayForward:         add_days(+day);         break;
            case ButtonId::MonthForward:       add_days(+month);       break;
            case ButtonId::YearForward:        add_days(+year);        break;
            case ButtonId::Year10Forward:      add_days(+year * 10);   break;
            case ButtonId::CenturyForward:     add_days(+year * 100);  break;
            case ButtonId::Century10Forward:   add_days(+year * 1000); break;
            case ButtonId::Century100Forward:  add_days(+year * 10000);break;

            case ButtonId::None:
            default:
                break;
            }
        }

        // 再生コントロールの描画のみ
        void drawPlaybackControlsOnly(int start_x, int y) const {
            int x = start_x;

            paxg::Vec2i pos1(paxg::Window::width() - x, y);
            texture_dictionary_->at(MurMur3::calcHash("texture_reverse_playback"))
                .resizedDraw(arrow_time_icon_size, pos1);
            x -= arrow_icon_move_x;

            paxg::Vec2i pos2(paxg::Window::width() - x, y);
            texture_dictionary_->at(MurMur3::calcHash("texture_stop"))
                .resizedDraw(arrow_time_icon_size, pos2);
            x -= arrow_icon_move_x;

            paxg::Vec2i pos3(paxg::Window::width() - x, y);
            texture_dictionary_->at(MurMur3::calcHash("texture_playback"))
                .resizedDraw(arrow_time_icon_size, pos3);
        }

        // 過去への時間移動の描画のみ
        void drawBackwardTimeControlsOnly(int start_x, int y) const {
            const char* texture_keys[] = {
                "texture_d_l", "texture_m_l", "texture_y_l", "texture_10y_l",
                "texture_c_l", "texture_10c_l", "texture_100c_l"
            };
            int x = start_x;
            for (const char* key : texture_keys) {
                paxg::Vec2i pos(paxg::Window::width() - x, y);
                texture_dictionary_->at(MurMur3::calcHash(key))
                    .resizedDraw(time_icon_size, pos);
                x -= icon_move_x;
            }
        }

        // 未来への時間移動の描画のみ
        void drawForwardTimeControlsOnly(int start_x, int y) const {
            const char* texture_keys[] = {
                "texture_d_r", "texture_m_r", "texture_y_r", "texture_10y_r",
                "texture_c_r", "texture_10c_r", "texture_100c_r"
            };
            int x = start_x;
            for (const char* key : texture_keys) {
                paxg::Vec2i pos(paxg::Window::width() - x, y);
                texture_dictionary_->at(MurMur3::calcHash(key))
                    .resizedDraw(time_icon_size, pos);
                x -= icon_move_x;
            }
        }

    public:
        TimeControlButtons() {
            // 暦の時間操作のアイコン
            if (!key_value_tsv.input(paxs::AppConfig::getInstance()->getRootPath() + "Data/MenuIcon/MenuIcons.tsv", [&](const std::string& value_) { return paxg::Texture{ value_ }; })) {
                PAXS_ERROR("Failed to load texture KeyValueTSV: Data/MenuIcon/MenuIcons.tsv");
            }
            texture_dictionary_ = &key_value_tsv.get();
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (!enabled_ || !texture_dictionary_ || !koyomi_) {
                return EventHandlingResult::NotHandled();
            }
            updateState(event);
            return EventHandlingResult::Handled();
        }

        void setReferences(paxs::Koyomi& koyomi) {
            koyomi_ = &koyomi;
        }

        bool isHit(int x, int y) const override {
            if (!enabled_ || !texture_dictionary_ || !koyomi_) return false;
            return hitTestButton(x, y) != ButtonId::None;
        }

        void render() const override {
            if (!enabled_ || !texture_dictionary_ || !koyomi_) return;
            drawAllControls();
        }

        paxg::Rect getRect() const override {
            return paxg::Rect{
                static_cast<float>(pos_.x()),
                static_cast<float>(pos_.y()),
                static_cast<float>(icon_move_x * 7), // 7つのアイコン
                static_cast<float>(getHeight())
            };
        }

        RenderLayer getLayer() const override {
            return RenderLayer::UIContent;
        }
        void setVisible(bool /*visible*/) override {}
        bool isVisible() const override { return true; }
        bool isEnabled() const override { return enabled_; }
        void setEnabled(bool enabled) override { enabled_ = enabled; }
        void setPos(const paxg::Vec2i& pos) override { pos_ = pos; }
        const char* getName() const override { return "TimeControlPanel"; }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_TIME_CONTROL_WIDGET_HPP
