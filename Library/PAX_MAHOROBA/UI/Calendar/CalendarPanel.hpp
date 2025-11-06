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

#include <PAX_MAHOROBA/UI/Calendar/CalendarContent.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>
#include <PAX_MAHOROBA/UI/Calendar/TimeControlButtons.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief カレンダーパネル - 時間操作とカレンダー表示を統合管理
    /// @brief Calendar Panel - Integrates time control and calendar display with shared background
    class CalendarPanel : public IWidget {
    private:
        const paxs::FeatureVisibilityManager* visibility_manager_ptr = nullptr;

        TimeControlButtons time_control_widget_;
        CalendarContent calendar_widget_;
        const UILayout* ui_layout_;

    public:
        CalendarPanel(const UILayout& ui_layout)
            : ui_layout_(&ui_layout) {}

        /// @brief 初期化
        /// @brief Initialize
        void init(const paxs::FeatureVisibilityManager* visibility_manager) {
            visibility_manager_ptr = visibility_manager;
        }

        /// @brief カレンダー描画パラメータを設定
        /// @brief Set calendar rendering parameters
        void setCalendarParams(
            const paxs::Koyomi& koyomi
        ) {
            calendar_widget_.setRenderParams(koyomi, *ui_layout_);
        }

        void setTimeControlParams(paxs::Koyomi& koyomi) {
            if (!ui_layout_) return;
            time_control_widget_.setReferences(koyomi);
            time_control_widget_.setPos(paxg::Vec2i{ui_layout_->time_control_base_x, ui_layout_->koyomi_font_y + ui_layout_->time_control_base_y});
        }

        void render() const override {
            if (!isVisible() || !ui_layout_) return;

            // コンポーネントを描画
            time_control_widget_.render();
            calendar_widget_.render();
        }

        bool isVisible() const override {
            return visibility_manager_ptr->isVisible(MurMur3::calcHash("Calendar")) &&
                   visibility_manager_ptr->isVisible(MurMur3::calcHash("UI"));
        }

        /// @brief 時間操作ウィジェットの高さを取得
        /// @brief Get height of time control widget
        int getTimeControlHeight() const {
            return time_control_widget_.getHeight();
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            // 時間操作ウィジェットのマウス入力処理
            if (time_control_widget_.isHit(event.x, event.y)) {
                return time_control_widget_.handleEvent(event);
            }
            return EventHandlingResult::NotHandled();
        }

        bool isHit(int x, int y) const override {
            if (!isVisible() || !isEnabled()) return false;
            return time_control_widget_.isHit(x, y);
        }

        const char* getName() const override {
            return "CalendarPanel";
        }

        RenderLayer getLayer() const override {
            return RenderLayer::UIContent;
        }

        void setVisible(bool /*visible*/) override {}
        void setEnabled(bool /*enabled*/) override {}
        bool isEnabled() const override { return true; }
        void setPos(const paxg::Vec2i& /*pos*/) override {}
        paxg::Rect getRect() const override { return paxg::Rect{}; }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_CALENDAR_PANEL_HPP
