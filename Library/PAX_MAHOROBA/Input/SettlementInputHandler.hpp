/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0.  https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_SETTLEMENT_INPUT_HANDLER_HPP
#define PAX_MAHOROBA_MAP_SETTLEMENT_INPUT_HANDLER_HPP

#include <cstddef>

#include <PAX_GRAPHICA/Key.hpp>

#include <PAX_MAHOROBA/Core/ApplicationEvents.hpp>
#include <PAX_MAHOROBA/Core/EventBus.hpp>
#include <PAX_MAHOROBA/Input/IEventHandler.hpp>

namespace paxs {

    /// @brief 集落入力処理ハンドラー
    /// @brief Settlement input handler
    ///
    /// 集落の表示モード切替などの入力処理を担当します。
    /// キーボードイベント（1-6キー、L、Kキー）を処理するためIEventHandlerを継承します。
    class SettlementInputHandler : public IEventHandler {
    public:
        SettlementInputHandler() = default;

        /// @brief EventBusを設定
        /// @brief Set EventBus
        void setEventBus(EventBus* event_bus) {
            event_bus_ = event_bus;
        }

        // IEventHandler の実装
        // IEventHandler implementation

        /// @brief キーボードイベント処理
        /// @brief Handle keyboard event
        /// @param event キーボードイベント / Keyboard event
        /// @return イベント処理結果 / Event handling result
        EventHandlingResult handleEvent(const KeyboardEvent& event) override {
            (void)event;
            if (!visible_ || !enabled_) return EventHandlingResult::NotHandled();

            // 既存のupdate()ロジックを呼び出す
            update();

            // 入力を消費しない（背後のハンドラーにも伝播させる）
            return EventHandlingResult::NotHandled();
        }

        void setVisible(bool visible) { visible_ = visible; }
        bool isVisible() const { return visible_; }
        bool isEnabled() const override { return enabled_; }

        /// @brief 現在の表示モードを取得
        /// @brief Get current display mode
        std::size_t getSelectDraw() const { return select_draw_; }

        /// @brief グリッド線表示フラグを取得
        /// @brief Get grid line display flag
        bool getIsLine() const { return is_line_; }

        /// @brief 移動矢印表示フラグを取得
        /// @brief Get arrow display flag
        bool getIsArrow() const { return is_arrow_; }

    private:
        std::size_t select_draw_ = 1;  // 表示モード (1-6)
        bool is_line_ = false;          // グリッド線を表示するか
        bool is_arrow_ = true;          // 移動線（矢印）を表示するか
        bool visible_ = true;
        bool enabled_ = true;
        EventBus* event_bus_ = nullptr;

        /// @brief 設定変更イベントを発行
        /// @brief Notify settlement display settings changed
        void notifyDisplayChanged() {
            if (event_bus_ != nullptr) {
                event_bus_->publish(SettlementDisplayChangedEvent(
                    select_draw_,
                    is_line_,
                    is_arrow_
                ));
            }
        }

        /// @brief キーボード入力を処理してSettlementRendererの状態を更新（内部メソッド）
        /// @brief Process keyboard input and update SettlementRenderer state (internal method)
        void update() {
            bool changed = false;

            // 1-6キーで表示モードを切り替え
            if (Key(PAXG_KEY_1).isPressed()) { select_draw_ = 1; changed = true; }
            else if (Key(PAXG_KEY_2).isPressed()) { select_draw_ = 2; changed = true; }
            else if (Key(PAXG_KEY_3).isPressed()) { select_draw_ = 3; changed = true; }
            else if (Key(PAXG_KEY_4).isPressed()) { select_draw_ = 4; changed = true; }
            else if (Key(PAXG_KEY_5).isPressed()) { select_draw_ = 5; changed = true; }
            else if (Key(PAXG_KEY_6).isPressed()) { select_draw_ = 6; changed = true; }

            // Lキーでグリッド線表示を切り替え
            else if (Key(PAXG_KEY_L).isPressed()) { is_line_ = !is_line_; changed = true; }

            // Kキーで移動矢印表示を切り替え
            else if (Key(PAXG_KEY_K).isPressed()) { is_arrow_ = !is_arrow_; changed = true; }

            // 設定が変更された場合はイベントを発行
            if (changed) {
                notifyDisplayChanged();
            }
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_SETTLEMENT_INPUT_HANDLER_HPP
