/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0.  https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_CALENDAR_TIME_CONTROL_BUTTON_HPP
#define PAX_MAHOROBA_CALENDAR_TIME_CONTROL_BUTTON_HPP

#include <vector>

#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/Core/ApplicationEvents.hpp>
#include <PAX_MAHOROBA/UI/Widget/IconButton.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief 時間操作ボタン
    /// @brief Time control button
    class TimeControlButton : public IconButton {
    public:
        enum class Id {
            // 再生コントロール / Playback controls
            PlaybackReverse,
            PlaybackStop,
            PlaybackForward,

            // 過去への移動 / Backward navigation
            DayBackward,
            MonthBackward,
            YearBackward,
            Year10Backward,
            CenturyBackward,
            Century10Backward,
            Century100Backward,

            // 未来への移動 / Forward navigation
            DayForward,
            MonthForward,
            YearForward,
            Year10Forward,
            CenturyForward,
            Century10Forward,
            Century100Forward,

            None,
        };

        static constexpr std::array<std::pair<const char*, std::uint_least32_t>, 17> button_data = {{
            // Playback controls
            {"TimeControlReverseButton", MurMur3::calcHash("texture_reverse_playback")},
            {"TimeControlStopButton", MurMur3::calcHash("texture_stop")},
            {"TimeControlForwardButton", MurMur3::calcHash("texture_playback")},

            // Backward navigation
            {"TimeControlDayBackwardButton", MurMur3::calcHash("texture_d_l")},
            {"TimeControlMonthBackwardButton", MurMur3::calcHash("texture_m_l")},
            {"TimeControlYearBackwardButton", MurMur3::calcHash("texture_y_l")},
            {"TimeControl10YearBackwardButton", MurMur3::calcHash("texture_10y_l")},
            {"TimeControlCenturyBackwardButton", MurMur3::calcHash("texture_c_l")},
            {"TimeControl10CenturyBackwardButton", MurMur3::calcHash("texture_10c_l")},
            {"TimeControl100CenturyBackwardButton", MurMur3::calcHash("texture_100c_l")},

            // Forward navigation
            {"TimeControlDayForwardButton", MurMur3::calcHash("texture_d_r")},
            {"TimeControlMonthForwardButton", MurMur3::calcHash("texture_m_r")},
            {"TimeControlYearForwardButton", MurMur3::calcHash("texture_y_r")},
            {"TimeControl10YearForwardButton", MurMur3::calcHash("texture_10y_r")},
            {"TimeControlCenturyForwardButton", MurMur3::calcHash("texture_c_r")},
            {"TimeControl10CenturyForwardButton", MurMur3::calcHash("texture_10c_r")},
            {"TimeControl100CenturyForwardButton", MurMur3::calcHash("texture_100c_r")},
        }};

        using ClickCallback = std::function<void(Id)>;

        TimeControlButton(Id id)
            : IconButton(
                button_data.at(static_cast<std::size_t>(id)).first,
                button_data.at(static_cast<std::size_t>(id)).second
            ), id_(id) {}

        RenderLayer getLayer() const override { return RenderLayer::UIContent; }

        void setOnClick(ClickCallback cb) {
            on_click_ = std::move(cb);
        }

        Id getId() const { return id_; }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (event.left_button_state == MouseButtonState::Pressed) {
                if (on_click_) {
                    on_click_(id_);
                }
            }
            return EventHandlingResult::Handled();
        }

        void placeFromRight(int offset_from_right, int y, int size) {
            const int x = paxg::Window::width() - offset_from_right;
            setPos(paxg::Vec2i{ x, y });
            setSize(paxg::Vec2i{ size, size });
        }

    private:
        Id id_ = Id::None;
        ClickCallback on_click_;
    };

    /// @brief 時間操作ボタン群の管理クラス
    /// @brief Time control buttons manager class
    class TimeControlButtons : public IWidget {
    public:
        using ClickCallback = std::function<void(TimeControlButton::Id)>;

        // アイコンのサイズ定数
        static constexpr int ARROW_TIME_ICON_SIZE = 40; // 再生/停止/逆再生アイコンの大きさ
        static constexpr int TIME_ICON_SIZE = 50;       // 時間移動アイコンの大きさ

        // アイコンの間隔
        static constexpr int ARROW_ICON_MOVE_X = static_cast<int>(ARROW_TIME_ICON_SIZE * 1.2);
        static constexpr int ARROW_ICON_MOVE_Y = static_cast<int>(ARROW_TIME_ICON_SIZE * 1.1);
        static constexpr int ICON_MOVE_X = static_cast<int>(TIME_ICON_SIZE * 1.1);
        static constexpr int ICON_MOVE_Y = static_cast<int>(TIME_ICON_SIZE * 1.1);

        TimeControlButtons() {
            buildButtons();
        }

        /// @brief 時間操作パネルの高さを取得
        /// @brief Get height of time control panel
        int getHeight() const {
            return ARROW_TIME_ICON_SIZE + TIME_ICON_SIZE * 2;
        }

        const char* getName() const override { return "TimeControlButtons"; }
        RenderLayer getLayer() const override { return RenderLayer::UIContent; }

        void setReferences(const paxs::Koyomi& koyomi) {
            koyomi_ = &koyomi;
        }

        void setAppStateManager(AppStateManager* app_state_manager) {
            app_state_manager_ = app_state_manager;
        }

        void setOnClick() {
            // 各ボタンにコールバックを直接渡す
            for (auto& btn : buttons_) {
                btn.setOnClick([this](TimeControlButton::Id id) {
                    executeButtonAction(id);
                });
            }
        }

        void render() const override {
            if (!visible_ || !koyomi_) return;
            for (const auto& btn : buttons_) {
                btn.render();
            }
        }

        bool isHit(int x, int y) const override {
            if (!visible_ || !enabled_ || !koyomi_) return false;
            for (const auto& btn : buttons_) {
                if (btn.isHit(x, y)) {
                    return true;
                }
            }
            return false;
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            for (auto& btn : buttons_) {
                if (btn.isHit(event.x, event.y)) {
                    return btn.handleEvent(event);
                }
            }
            return EventHandlingResult::NotHandled();
        }

        paxg::Rect getRect() const override {
            return paxg::Rect{
                static_cast<float>(pos_.x()),
                static_cast<float>(pos_.y()),
                static_cast<float>(ICON_MOVE_X * 7), // 7つのアイコン
                static_cast<float>(getHeight())
            };
        }

        void setPos(const paxg::Vec2i& pos) override {
            pos_ = pos;
            layoutButtons();
        }

        bool isEnabled() const override { return enabled_; }
        bool isVisible() const override { return visible_; }
        void setEnabled(bool enabled) override { enabled_ = enabled; }
        void setVisible(bool visible) override { visible_ = visible; }

    private:
        std::vector<TimeControlButton> buttons_;
        const paxs::Koyomi* koyomi_ = nullptr;
        AppStateManager* app_state_manager_ = nullptr;
        paxg::Vec2i pos_{0, 0};
        bool visible_ = true;
        bool enabled_ = true;
        ClickCallback on_click_;

        void buildButtons() {
            using Id = TimeControlButton::Id;

            // 再生コントロール
            buttons_.emplace_back(Id::PlaybackReverse);
            buttons_.emplace_back(Id::PlaybackStop);
            buttons_.emplace_back(Id::PlaybackForward);

            // 過去への移動
            buttons_.emplace_back(Id::DayBackward);
            buttons_.emplace_back(Id::MonthBackward);
            buttons_.emplace_back(Id::YearBackward);
            buttons_.emplace_back(Id::Year10Backward);
            buttons_.emplace_back(Id::CenturyBackward);
            buttons_.emplace_back(Id::Century10Backward);
            buttons_.emplace_back(Id::Century100Backward);

            // 未来への移動
            buttons_.emplace_back(Id::DayForward);
            buttons_.emplace_back(Id::MonthForward);
            buttons_.emplace_back(Id::YearForward);
            buttons_.emplace_back(Id::Year10Forward);
            buttons_.emplace_back(Id::CenturyForward);
            buttons_.emplace_back(Id::Century10Forward);
            buttons_.emplace_back(Id::Century100Forward);

            layoutButtons();
            setOnClick();
        }

        /// @brief ボタンタイプを判定
        /// @brief Determine button type
        enum class ButtonType { Playback, Backward, Forward, None };

        static ButtonType getButtonType(TimeControlButton::Id id) {
            using Id = TimeControlButton::Id;
            if (id >= Id::PlaybackReverse && id <= Id::PlaybackForward) return ButtonType::Playback;
            if (id >= Id::DayBackward && id <= Id::Century100Backward) return ButtonType::Backward;
            if (id >= Id::DayForward && id <= Id::Century100Forward) return ButtonType::Forward;
            return ButtonType::None;
        }

        void layoutButtons() {
            const int offset_from_right_base = pos_.x();
            int current_y = pos_.y();

            // 各行のレイアウト設定 (type, icon_size, move_x, move_y)
            struct RowLayout { ButtonType type; int icon_size; int move_x; int move_y; };
            const RowLayout rows[] = {
                {ButtonType::Playback, ARROW_TIME_ICON_SIZE, ARROW_ICON_MOVE_X, ARROW_ICON_MOVE_Y},
                {ButtonType::Backward, TIME_ICON_SIZE, ICON_MOVE_X, ICON_MOVE_Y},
                {ButtonType::Forward, TIME_ICON_SIZE, ICON_MOVE_X, 0}
            };

            for (const auto& row : rows) {
                int offset_from_right = offset_from_right_base;
                for (auto& btn : buttons_) {
                    if (getButtonType(btn.getId()) == row.type) {
                        btn.placeFromRight(offset_from_right, current_y, row.icon_size);
                        offset_from_right -= row.move_x;
                    }
                }
                current_y += row.move_y;
            }
        }

        /// @brief ボタンアクションを実行
        /// @brief Execute button action
        void executeButtonAction(TimeControlButton::Id id) {
            if (!app_state_manager_) return;

            using Id = TimeControlButton::Id;
            using Action = TimePlaybackControlEvent::Action;

            // 再生コントロール
            if (id == Id::PlaybackReverse) {
                app_state_manager_->executeTimePlaybackControl(Action::Reverse);
            } else if (id == Id::PlaybackStop) {
                app_state_manager_->executeTimePlaybackControl(Action::Stop);
            } else if (id == Id::PlaybackForward) {
                app_state_manager_->executeTimePlaybackControl(Action::Forward);
            } else {
                // 日付移動系
                constexpr double day   = 1.0;
                constexpr double month = 365.2422 / 12.0;
                constexpr double year  = 365.2422;

                // ボタンIDから日数を計算
                const double days_map[] = {
                    -day, -month, -year, -year * 10, -year * 100, -year * 1000, -year * 10000,  // Backward
                    +day, +month, +year, +year * 10, +year * 100, +year * 1000, +year * 10000   // Forward
                };

                const int index = static_cast<int>(id) - static_cast<int>(Id::DayBackward);
                if (index >= 0 && index < 14) {
                    app_state_manager_->executeDateNavigation(days_map[index]);
                }
            }
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_CALENDAR_TIME_CONTROL_BUTTON_HPP
