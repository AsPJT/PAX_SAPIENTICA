/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_CALENDAR_PANEL_HPP
#define PAX_MAHOROBA_CALENDAR_PANEL_HPP

#include <PAX_GRAPHICA/Rect.hpp>

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/UI/Calendar/CalendarContent.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>
#include <PAX_MAHOROBA/UI/Calendar/TimeControlButton.hpp>
#include <PAX_MAHOROBA/Rendering/InteractiveUIComponent.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>

namespace paxs {

    /// @brief カレンダーパネル - 時間操作とカレンダー表示を統合管理
    /// @brief Calendar Panel - Integrates time control and calendar display with shared background
    class CalendarPanel : public InteractiveUIComponent {
    private:
        const paxs::FeatureVisibilityManager& visibility_manager_;

        TimeControlButtons time_control_widget_;
        CalendarContent calendar_content;

    public:
        CalendarPanel(
            const UILayout& ui_layout
            , const paxs::FeatureVisibilityManager& visibility_manager
            , const AppStateManager& app_state_manager)
            : visibility_manager_(visibility_manager)
            , time_control_widget_(ui_layout, app_state_manager)
            , calendar_content(ui_layout, app_state_manager.getKoyomi()) {}

        /// @brief ボタンレイアウトを更新（UILayoutが変更された時に呼ぶ）
        void updateButtonLayout() {
            time_control_widget_.layoutButtons();
        }

        void render() const override {
            if (!isVisible()) return;

            time_control_widget_.render();
            calendar_content.render();
        }

        /// @brief 時間操作ウィジェットの高さを取得
        /// @brief Get height of time control widget
        int getTimeControlHeight() const {
            return time_control_widget_.getHeight();
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            return time_control_widget_.handleEvent(event);
        }

        bool isHit(const paxs::Vector2<int>& pos) const override {
            if (!isVisible()) return false;
            return time_control_widget_.isHit(paxs::Vector2<int>(pos.x, pos.y));
        }

        bool isVisible() const override {
            return visibility_manager_.isVisible(ViewMenu::calendar);
        }
        const char* getName() const override { return "CalendarPanel"; }
        RenderLayer getLayer() const override { return RenderLayer::UIContent; }
        void setPos(const Vector2<int>& /*pos*/) override {}
        Rect<int> getRect() const override { return {0, 0, 0, 0}; }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_CALENDAR_PANEL_HPP
