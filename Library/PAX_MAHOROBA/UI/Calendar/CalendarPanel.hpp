/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_CALENDAR_PANEL_HPP
#define PAX_MAHOROBA_CALENDAR_PANEL_HPP

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/RoundRect.hpp>

#include <PAX_MAHOROBA/Input/Events.hpp>
#include <PAX_MAHOROBA/UI/Calendar/CalendarContent.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>
#include <PAX_MAHOROBA/UI/Calendar/TimeControlButtons.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/UI/PanelBackground.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief カレンダーパネル - 時間操作とカレンダー表示を統合管理
    /// @brief Calendar Panel - Integrates time control and calendar display with shared background
    class CalendarPanel : public IWidget {
    public:
        CalendarPanel(const UILayout& ui_layout)
            : ui_layout_(&ui_layout)
        {}

        /// @brief 初期化
        /// @brief Initialize
        /// @param fonts LanguageFontsへの参照
        void init(paxs::LanguageFonts& fonts, const paxs::FeatureVisibilityManager* visibility_manager) {
            calendar_widget_.init(fonts);
            visibility_manager_ptr = visibility_manager;
        }

        /// @brief テクスチャの参照を設定
        /// @brief Set texture references
        /// @param texture_dict アイコンテクスチャ辞書
        void setTextureDictionary(const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_dict) {
            texture_dictionary_ = &texture_dict;
        }

        /// @brief カレンダー描画パラメータを設定
        /// @brief Set calendar rendering parameters
        void setCalendarParams(
            const paxs::Koyomi& koyomi,
            const SelectLanguage& select_language,
            const paxs::Language& language_text
        ) {
            calendar_widget_.setRenderParams(koyomi, *ui_layout_, select_language, language_text);
        }

        /// @brief 時間操作ウィジェットの参照を設定
        /// @brief Set time control widget references
        void setTimeControlParams(paxs::Koyomi& koyomi) {
            if (!texture_dictionary_ || !ui_layout_) return;
            time_control_widget_.setReferences(*texture_dictionary_, koyomi);
            time_control_widget_.setPos(paxg::Vec2i{ui_layout_->time_control_base_x, ui_layout_->koyomi_font_y + ui_layout_->time_control_base_y});
        }

        /// @brief レンダリングレイヤーを取得
        /// @brief Get rendering layer
        RenderLayer getLayer() const override {
            return RenderLayer::UIContent;
        }

        void render() const override {
            if (!isVisible() || !ui_layout_) return;

            // コンポーネントを描画
            time_control_widget_.render();
            calendar_widget_.render();
        }

        /// @brief 時間操作ウィジェットとカレンダーウィジェットの可視性を設定
        /// @brief Set visibility of time control widget and calendar widget
        void setVisible(bool visible) override {
            (void)visible;
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

        paxg::Rect getRect() const override {
            return paxg::Rect{0, 0, 0, 0};
        }

        void setPos(const paxg::Vec2i& pos) override {
            (void)pos;
        }

        void setEnabled(bool enabled) override {
            enabled_ = enabled;
        }

        bool isEnabled() const override {
            return enabled_;
        }

        const char* getName() const override {
            return "CalendarPanel";
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (!isVisible() || !isEnabled()) {
                return EventHandlingResult::NotHandled();
            }
            // 時間操作ウィジェットのマウス入力処理
            if (time_control_widget_.isHit(event.x, event.y)) {
                return time_control_widget_.handleEvent(event);
            }
            return EventHandlingResult::NotHandled();
        }

        bool isHit(int x, int y) const override {
            if (!isVisible() || !isEnabled()) return false;
            // 時間操作ウィジェットのヒットテスト
            return time_control_widget_.isHit(x, y);
        }

    private:
        // IRenderable 状態
        bool enabled_ = true;

        const paxs::FeatureVisibilityManager* visibility_manager_ptr = nullptr;

        TimeControlButtons time_control_widget_;   // 時間操作ウィジェット
        CalendarContent calendar_widget_;          // カレンダー表示ウィジェット

        const UILayout* ui_layout_;
        const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>* texture_dictionary_ = nullptr;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_CALENDAR_PANEL_HPP
