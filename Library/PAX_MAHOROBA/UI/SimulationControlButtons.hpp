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

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Input/Events.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>
#include <PAX_SAPIENTICA/UnorderedMap.hpp>

namespace paxs {

    /// @brief シミュレーション制御ボタンコンポーネント
    /// @brief Simulation control buttons component
    class SimulationControlButtons : public IWidget {
    private:
        paxs::KeyValueTSV<paxg::Texture> key_value_tsv_;

        // 外部参照（SimulationPanelから設定）
        std::unique_ptr<paxs::SettlementSimulator>* simulator_ptr_ = nullptr;
        paxs::Koyomi* koyomi_ = nullptr;
        int debug_start_y_ = 0;

        bool visible_ = true;
        bool enabled_ = true;

        static constexpr int TIME_ICON_SIZE = 40;

    public:
        /// @brief コンストラクタ（テクスチャを読み込む）
        /// @brief Constructor (load textures)
        SimulationControlButtons() {
            if (!key_value_tsv_.input(paxs::AppConfig::getInstance()->getRootPath() + "Data/MenuIcon/MenuIcons.tsv",
                [&](const std::string& value_) { return paxg::Texture{ value_ }; })) {
                PAXS_ERROR("Failed to load texture KeyValueTSV: Data/MenuIcon/MenuIcons.tsv");
            }
        }

        /// @brief 外部参照を設定
        /// @brief Set external references
        void setReferences(
            std::unique_ptr<paxs::SettlementSimulator>* simulator_ptr,
            paxs::Koyomi* koyomi,
            int debug_start_y
        ) {
            simulator_ptr_ = simulator_ptr;
            koyomi_ = koyomi;
            debug_start_y_ = debug_start_y;
        }

        // IWidget インターフェース実装

        const char* getName() const override {
            return "SimulationControlButtons";
        }

        RenderLayer getLayer() const override {
            return RenderLayer::UIContent;
        }

        bool isAvailable() const override {
            return true;
        }

        void setEnabled(bool enabled) override {
            enabled_ = enabled;
        }

        bool isEnabled() const override {
            return enabled_;
        }

        paxg::Rect getRect() const override {
            // ダミーの矩形（実際のhitTestは個別ボタンで判定）
            return paxg::Rect{0, 0, 0, 0};
        }

        void setPos(const paxg::Vec2i& /*pos*/) override {
            // ボタンは画面右端に固定配置なので位置設定は無視
        }

        void setVisible(bool visible) override {
            visible_ = visible;
        }

        bool isVisible() const override {
            return visible_;
        }

        /// @brief ヒットテスト（状態に応じたボタン位置を個別判定）
        /// @brief Hit test (individual button positions based on state)
        bool hitTest(int x, int y) const override {
            if (!visible_ || !enabled_) return false;
            if (!simulator_ptr_ || !koyomi_) return false;

            // シミュレーションが初期化されていない場合
            if (simulator_ptr_->get() == nullptr) {
                // 地形データ読み込みボタン (Window.width - 360, debug_start_y, 40x40)
                float btn_x = static_cast<float>(paxg::Window::width() - 360);
                float btn_y = static_cast<float>(debug_start_y_);
                if (x >= btn_x && x < btn_x + TIME_ICON_SIZE &&
                    y >= btn_y && y < btn_y + TIME_ICON_SIZE) {
                    return true;
                }
            }
            // シミュレーションが初期化されている場合
            else {
                // 再生中の場合
                if (koyomi_->is_agent_update) {
                    // 停止ボタン (Window.width - 300, debug_start_y, 40x40)
                    float btn_x = static_cast<float>(paxg::Window::width() - 300);
                    float btn_y = static_cast<float>(debug_start_y_);
                    if (x >= btn_x && x < btn_x + TIME_ICON_SIZE &&
                        y >= btn_y && y < btn_y + TIME_ICON_SIZE) {
                        return true;
                    }
                }
                // 停止中の場合
                else {
                    // リロードボタン (Window.width - 420, debug_start_y + 60, 40x40)
                    float reload_x = static_cast<float>(paxg::Window::width() - 420);
                    float reload_y = static_cast<float>(debug_start_y_ + 60);
                    if (x >= reload_x && x < reload_x + TIME_ICON_SIZE &&
                        y >= reload_y && y < reload_y + TIME_ICON_SIZE) {
                        return true;
                    }

                    // 人間データ初期化ボタン (Window.width - 420, debug_start_y, 40x40)
                    float init_x = static_cast<float>(paxg::Window::width() - 420);
                    float init_y = static_cast<float>(debug_start_y_);
                    if (x >= init_x && x < init_x + TIME_ICON_SIZE &&
                        y >= init_y && y < init_y + TIME_ICON_SIZE) {
                        return true;
                    }

                    // 地形データ削除ボタン (Window.width - 360, debug_start_y, 40x40)
                    float delete_x = static_cast<float>(paxg::Window::width() - 360);
                    float delete_y = static_cast<float>(debug_start_y_);
                    if (x >= delete_x && x < delete_x + TIME_ICON_SIZE &&
                        y >= delete_y && y < delete_y + TIME_ICON_SIZE) {
                        return true;
                    }

                    // 再生ボタン (Window.width - 300, debug_start_y, 40x40)
                    float play_x = static_cast<float>(paxg::Window::width() - 300);
                    float play_y = static_cast<float>(debug_start_y_);
                    if (x >= play_x && x < play_x + TIME_ICON_SIZE &&
                        y >= play_y && y < play_y + TIME_ICON_SIZE) {
                        return true;
                    }

                    // 1ステップボタン (Window.width - 240, debug_start_y, 40x40)
                    float step_x = static_cast<float>(paxg::Window::width() - 240);
                    float step_y = static_cast<float>(debug_start_y_);
                    if (x >= step_x && x < step_x + TIME_ICON_SIZE &&
                        y >= step_y && y < step_y + TIME_ICON_SIZE) {
                        return true;
                    }
                }
            }

            return false;
        }

        /// @brief マウス入力処理（実際の処理はSimulationPanelに委譲）
        /// @brief Handle mouse input (actual handling delegated to SimulationPanel)
        EventHandlingResult handleMouseInput(const MouseEvent& event) override {
            if (!visible_ || !enabled_) return EventHandlingResult::NotHandled();
            if (!simulator_ptr_ || !koyomi_) return EventHandlingResult::NotHandled();

            // hitTest内のボタンエリアでのクリックをHandledとして返す
            // 実際のボタン処理はSimulationPanel.updateSimulation()で行われる
            if (hitTest(event.x, event.y)) {
                if (event.left_button_state == MouseButtonState::Pressed ||
                    event.left_button_state == MouseButtonState::Held ||
                    event.left_button_state == MouseButtonState::Released) {
                    return EventHandlingResult::Handled();
                }
            }

            return EventHandlingResult::NotHandled();
        }

        /// @brief 描画処理
        /// @brief Rendering
        void render() const override {
            if (!visible_) return;
            if (!simulator_ptr_ || !koyomi_) return;

            const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_dictionary = key_value_tsv_.get();

            // シミュレーションが初期化されていない場合
            if (simulator_ptr_->get() == nullptr) {
                // 地形データ読み込みボタン
                texture_dictionary.at(MurMur3::calcHash("texture_load_geographic_data2")).resizedDraw(
                    TIME_ICON_SIZE, paxg::Vec2i(paxg::Window::width() - 360, debug_start_y_));
            }
            // シミュレーションが初期化されている場合
            else {
                // シミュレーションが再生中の場合
                if (koyomi_->is_agent_update) {
                    // 停止ボタン
                    texture_dictionary.at(MurMur3::calcHash("texture_stop")).resizedDraw(
                        TIME_ICON_SIZE, paxg::Vec2i(paxg::Window::width() - 300, debug_start_y_));
                }
                // シミュレーションが停止中の場合
                else {
                    // シミュレーション入力データ初期化ボタン
                    texture_dictionary.at(MurMur3::calcHash("texture_reload")).resizedDraw(
                        TIME_ICON_SIZE, paxg::Vec2i(paxg::Window::width() - 420, debug_start_y_ + 60));

                    // 人間データ初期化ボタン (Simulation Init)
                    texture_dictionary.at(MurMur3::calcHash("texture_load_agent_data2")).resizedDraw(
                        TIME_ICON_SIZE, paxg::Vec2i(paxg::Window::width() - 420, debug_start_y_));

                    // 地形データ削除ボタン
                    texture_dictionary.at(MurMur3::calcHash("texture_delete_geographic_data")).resizedDraw(
                        TIME_ICON_SIZE, paxg::Vec2i(paxg::Window::width() - 360, debug_start_y_));

                    // 再生ボタン
                    texture_dictionary.at(MurMur3::calcHash("texture_playback")).resizedDraw(
                        TIME_ICON_SIZE, paxg::Vec2i(paxg::Window::width() - 300, debug_start_y_));

                    // 1ステップ実行ボタン
                    texture_dictionary.at(MurMur3::calcHash("texture_1step")).resizedDraw(
                        TIME_ICON_SIZE, paxg::Vec2i(paxg::Window::width() - 240, debug_start_y_));
                }
            }
        }

    };

}

#endif // !PAX_MAHOROBA_UI_SIMULATION_CONTROL_BUTTONS_HPP
