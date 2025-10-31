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

namespace paxs {

    /// @brief SettlementRendererの入力処理を担当するクラス（UI層）
    /// @brief Handles input processing for SettlementRenderer (UI layer)
    class SettlementInputHandler {
    private:
        std::size_t select_draw_ = 1;  // 表示モード (1-6)
        bool is_line_ = false;          // グリッド線を表示するか
        bool is_arrow_ = true;          // 移動線（矢印）を表示するか

    public:
        SettlementInputHandler() = default;

        /// @brief キーボード入力を処理してSettlementRendererの状態を更新
        /// @brief Process keyboard input and update SettlementRenderer state
        void update() {
            // 1-6キーで表示モードを切り替え
            if (Key(PAXG_KEY_1).isPressed()) select_draw_ = 1;
            else if (Key(PAXG_KEY_2).isPressed()) select_draw_ = 2;
            else if (Key(PAXG_KEY_3).isPressed()) select_draw_ = 3;
            else if (Key(PAXG_KEY_4).isPressed()) select_draw_ = 4;
            else if (Key(PAXG_KEY_5).isPressed()) select_draw_ = 5;
            else if (Key(PAXG_KEY_6).isPressed()) select_draw_ = 6;

            // Lキーでグリッド線表示を切り替え
            else if (Key(PAXG_KEY_L).isPressed()) is_line_ = !is_line_;

            // Kキーで移動矢印表示を切り替え
            else if (Key(PAXG_KEY_K).isPressed()) is_arrow_ = !is_arrow_;
        }

        /// @brief 現在の表示モードを取得
        /// @brief Get current display mode
        std::size_t getSelectDraw() const { return select_draw_; }

        /// @brief グリッド線表示フラグを取得
        /// @brief Get grid line display flag
        bool getIsLine() const { return is_line_; }

        /// @brief 移動矢印表示フラグを取得
        /// @brief Get arrow display flag
        bool getIsArrow() const { return is_arrow_; }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_SETTLEMENT_INPUT_HANDLER_HPP
