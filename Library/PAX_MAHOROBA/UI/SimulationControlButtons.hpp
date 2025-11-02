/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0.  https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_SIMULATION_CONTROL_BUTTONS_HPP
#define PAX_MAHOROBA_UI_SIMULATION_CONTROL_BUTTONS_HPP

#include <cstdint>
#include <memory>

#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>
#include <PAX_SAPIENTICA/UnorderedMap.hpp>

namespace paxs {

    /// @brief シミュレーション制御ボタンコンポーネント
    /// @brief Simulation control buttons component
    class SimulationControlButtons {
    public:
        SimulationControlButtons() = default;

        /// @brief シミュレーション制御ボタンを描画
        /// @brief Draw simulation control buttons
        /// @param simulator シミュレータのユニークポインタ
        /// @param koyomi 暦情報
        /// @param texture_dictionary テクスチャ辞書
        /// @param debug_start_y 描画開始Y座標
        void draw(
            const std::unique_ptr<paxs::SettlementSimulator>* simulator_ptr,
            const paxs::Koyomi* koyomi,
            const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_dictionary,
            int debug_start_y
        ) const {
            if (!simulator_ptr || !koyomi) return;

            constexpr int time_icon_size = 40;

            // シミュレーションが初期化されていない場合
            if (simulator_ptr->get() == nullptr) {
                // 地形データ読み込みボタン
                texture_dictionary.at(MurMur3::calcHash("texture_load_geographic_data2")).resizedDraw(
                    time_icon_size, paxg::Vec2i(paxg::Window::width() - 360, debug_start_y));
            }
            // シミュレーションが初期化されている場合
            else {
                // シミュレーションが再生中の場合
                if (koyomi->is_agent_update) {
                    // 停止ボタン
                    texture_dictionary.at(MurMur3::calcHash("texture_stop")).resizedDraw(
                        time_icon_size, paxg::Vec2i(paxg::Window::width() - 300, debug_start_y));
                }
                // シミュレーションが停止中の場合
                else {
                    // シミュレーション入力データ初期化ボタン
                    texture_dictionary.at(MurMur3::calcHash("texture_reload")).resizedDraw(
                        time_icon_size, paxg::Vec2i(paxg::Window::width() - 420, debug_start_y + 60));

                    // 人間データ初期化ボタン (Simulation Init)
                    texture_dictionary.at(MurMur3::calcHash("texture_load_agent_data2")).resizedDraw(
                        time_icon_size, paxg::Vec2i(paxg::Window::width() - 420, debug_start_y));

                    // 地形データ削除ボタン
                    texture_dictionary.at(MurMur3::calcHash("texture_delete_geographic_data")).resizedDraw(
                        time_icon_size, paxg::Vec2i(paxg::Window::width() - 360, debug_start_y));

                    // 再生ボタン
                    texture_dictionary.at(MurMur3::calcHash("texture_playback")).resizedDraw(
                        time_icon_size, paxg::Vec2i(paxg::Window::width() - 300, debug_start_y));

                    // 1ステップ実行ボタン
                    texture_dictionary.at(MurMur3::calcHash("texture_1step")).resizedDraw(
                        time_icon_size, paxg::Vec2i(paxg::Window::width() - 240, debug_start_y));
                }
            }
        }
    };

}

#endif // !PAX_MAHOROBA_UI_SIMULATION_CONTROL_BUTTONS_HPP
