/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_SETTLEMENT_INPUT_HANDLER_HPP
#define PAX_MAHOROBA_MAP_SETTLEMENT_INPUT_HANDLER_HPP

#include <cstddef>

#include <PAX_GRAPHICA/Key.hpp>

#include <PAX_MAHOROBA/Input/IInputHandler.hpp>

#include <PAX_SAPIENTICA/EventSystem/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/EventSystem/EventBus.hpp>
#include <PAX_SAPIENTICA/InputStateManager.hpp>

namespace paxs {

    /// @brief 集落入力処理ハンドラー
    /// @brief Settlement input handler
    ///
    /// 集落の表示モード切替などの入力処理を担当します。
    /// キーボードイベント（1-6キー、L、Kキー）を処理するためIInputHandlerを継承します。
    class SettlementInputHandler : public IInputHandler {
    public:
        SettlementInputHandler() = default;

        EventHandlingResult handleEvent(const KeyboardEvent& event) override {
            (void)event;
            update();

            return EventHandlingResult::NotHandled();
        }

        /// @brief 現在の表示モードを取得
        std::size_t getSelectDraw() const { return select_draw_; }

        /// @brief グリッド線表示フラグを取得
        bool getIsLine() const { return is_line_; }

        /// @brief 移動矢印表示フラグを取得
        bool getIsArrow() const { return is_arrow_; }

    private:
        std::size_t select_draw_ = 1;  // 表示モード (1-6)
        bool is_line_ = false;          // グリッド線を表示するか
        bool is_arrow_ = true;          // 移動線（矢印）を表示するか

        // キー状態管理
        InputStateManager key_l_state_;   // Lキーの状態管理
        InputStateManager key_k_state_;   // Kキーの状態管理

        /// @brief 設定変更イベントを発行
        /// @brief Notify settlement display settings changed
        void notifyDisplayChanged() {
            paxs::EventBus::getInstance().publish(SettlementDisplayChangedEvent(
                select_draw_,
                is_line_,
                is_arrow_
            ));
        }

        /// @brief キーボード入力を処理してSettlementRendererの状態を更新（内部メソッド）
        /// @brief Process keyboard input and update SettlementRenderer state (internal method)
        void update() {
            bool changed = false;
            std::size_t new_select_draw = select_draw_;

            // 1-6キーで表示モードを切り替え（値が変わった場合のみ通知）
            if (paxg::Key(paxg::PAXG_KEY_1).isPressed()) { new_select_draw = 1; }
            else if (paxg::Key(paxg::PAXG_KEY_2).isPressed()) { new_select_draw = 2; }
            else if (paxg::Key(paxg::PAXG_KEY_3).isPressed()) { new_select_draw = 3; }
            else if (paxg::Key(paxg::PAXG_KEY_4).isPressed()) { new_select_draw = 4; }
            else if (paxg::Key(paxg::PAXG_KEY_5).isPressed()) { new_select_draw = 5; }
            else if (paxg::Key(paxg::PAXG_KEY_6).isPressed()) { new_select_draw = 6; }

            // 表示モードが変更された場合のみフラグを立てる
            if (new_select_draw != select_draw_) {
                select_draw_ = new_select_draw;
                changed = true;
            }

            // Lキーでグリッド線表示を切り替え（押された瞬間のみ）
            InputStateManager::State l_state = key_l_state_.update(paxg::Key(paxg::PAXG_KEY_L).isPressed());
            if (l_state == InputStateManager::State::Down) {
                is_line_ = !is_line_;
                changed = true;
            }

            // Kキーで移動矢印表示を切り替え（押された瞬間のみ）
            InputStateManager::State k_state = key_k_state_.update(paxg::Key(paxg::PAXG_KEY_K).isPressed());
            if (k_state == InputStateManager::State::Down) {
                is_arrow_ = !is_arrow_;
                changed = true;
            }

            // 設定が変更された場合はイベントを発行
            if (changed) {
                notifyDisplayChanged();
            }
        }

        RenderLayer getLayer() const override {
            return RenderLayer::MapContent;
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_SETTLEMENT_INPUT_HANDLER_HPP
