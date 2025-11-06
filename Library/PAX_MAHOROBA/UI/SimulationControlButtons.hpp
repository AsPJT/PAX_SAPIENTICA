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

namespace paxs {

    /// @brief シミュレーション制御ボタンコンポーネント
    class SimulationControlButtons : public IWidget {
    private:
        paxs::KeyValueTSV<paxg::Texture> key_value_tsv_;

        // 外部参照
        std::unique_ptr<paxs::SettlementSimulator>* simulator_ptr_ = nullptr;
        paxs::Koyomi* koyomi_ = nullptr;
        int debug_start_y_ = 0;

        bool visible_ = true;
        bool enabled_ = true;

        // ボタンサイズ
        static constexpr int TIME_ICON_SIZE = 40;

        // 右端からの固定オフセット
        static constexpr int X_LOAD_OR_DELETE  = 360; // 地形データ読み込み/削除
        static constexpr int X_INIT_OR_RELOAD  = 420; // 人間データ初期化/リロード
        static constexpr int X_PLAY_OR_STOP    = 300; // 再生/停止
        static constexpr int X_STEP            = 240; // 1ステップ
        static constexpr int RELOAD_OFFSET_Y   = 60;  // リロードだけ+60

    public:
        // どのボタンかを識別するID
        enum class ButtonId {
            None,
            // 未初期化のとき
            LoadGeographicData,
            // 初期化済 & 再生中
            Stop,
            // 初期化済 & 停止中
            ReloadInputData,
            InitHumanData,
            DeleteGeographicData,
            Play,
            Step,
        };

        using ClickCallback = std::function<void(ButtonId)>;

        void setOnClick(ClickCallback cb) {
            on_click_ = std::move(cb);
        }

        /// @brief コンストラクタ（テクスチャを読み込む）
        SimulationControlButtons() {
            if (!key_value_tsv_.input(
                    paxs::AppConfig::getInstance()->getRootPath() + "Data/MenuIcon/MenuIcons.tsv",
                    [](const std::string& value_) { return paxg::Texture{ value_ }; })) {
                PAXS_ERROR("Failed to load texture KeyValueTSV: Data/MenuIcon/MenuIcons.tsv");
            }
        }

        /// @brief 外部参照を設定
        void setReferences(
            std::unique_ptr<paxs::SettlementSimulator>* simulator_ptr,
            paxs::Koyomi* koyomi,
            int debug_start_y
        ) {
            simulator_ptr_ = simulator_ptr;
            koyomi_ = koyomi;
            debug_start_y_ = debug_start_y;
        }

        const char* getName() const override {
            return "SimulationControlButtons";
        }

        RenderLayer getLayer() const override {
            return RenderLayer::UIContent;
        }

        void setEnabled(bool enabled) override {
            enabled_ = enabled;
        }

        bool isEnabled() const override {
            return enabled_;
        }

        paxg::Rect getRect() const override {
            // 固定配置なのでダミーでOK
            return paxg::Rect{0.f, 0.f, 0.f, 0.f};
        }

        void setPos(const paxg::Vec2i& /*pos*/) override {
            // 位置は固定なので無視
        }

        void setVisible(bool visible) override {
            visible_ = visible;
        }

        bool isVisible() const override {
            return visible_;
        }

        /// @brief ヒットテスト
        bool isHit(int x, int y) const override {
            if (!visible_ || !enabled_) return false;
            if (!simulator_ptr_ || !koyomi_) return false;

            return hitTestButton(x, y) != ButtonId::None;
        }

        /// @brief マウス入力処理（実際の処理はSimulationPanelに委譲）
        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (!visible_ || !enabled_) return EventHandlingResult::NotHandled();
            if (!simulator_ptr_ || !koyomi_) return EventHandlingResult::NotHandled();

            // どのボタンかだけここで判断
            const ButtonId id = hitTestButton(event.x, event.y);
            if (id == ButtonId::None) {
                return EventHandlingResult::NotHandled();
            }

            if (event.left_button_state == MouseButtonState::Pressed) {
                if (on_click_) {
                    on_click_(id);
                }
                return EventHandlingResult::Handled();
            } else {
                return EventHandlingResult::Handled();
            }

            return EventHandlingResult::NotHandled();
        }

        /// @brief 描画処理
        void render() const override {
            if (!visible_) return;
            if (!simulator_ptr_ || !koyomi_) return;

            const auto& texture_dictionary = key_value_tsv_.get();
            const auto width = paxg::Window::width();
            const int base_y = debug_start_y_;

            // シミュレーションが初期化されていない場合
            if (simulator_ptr_->get() == nullptr) {
                // 地形データ読み込みボタン
                texture_dictionary.at(MurMur3::calcHash("texture_load_geographic_data2")).resizedDraw(
                    TIME_ICON_SIZE,
                    paxg::Vec2i(width - X_LOAD_OR_DELETE, base_y)
                );
                return;
            }

            // シミュレーションが初期化されている場合
            if (koyomi_->is_agent_update) {
                // 再生中 → 停止ボタン
                texture_dictionary.at(MurMur3::calcHash("texture_stop")).resizedDraw(
                    TIME_ICON_SIZE,
                    paxg::Vec2i(width - X_PLAY_OR_STOP, base_y)
                );
            } else {
                // 停止中 → 複数ボタン表示
                texture_dictionary.at(MurMur3::calcHash("texture_reload")).resizedDraw(
                    TIME_ICON_SIZE,
                    paxg::Vec2i(width - X_INIT_OR_RELOAD, base_y + RELOAD_OFFSET_Y)
                );

                texture_dictionary.at(MurMur3::calcHash("texture_load_agent_data2")).resizedDraw(
                    TIME_ICON_SIZE,
                    paxg::Vec2i(width - X_INIT_OR_RELOAD, base_y)
                );

                texture_dictionary.at(MurMur3::calcHash("texture_delete_geographic_data")).resizedDraw(
                    TIME_ICON_SIZE,
                    paxg::Vec2i(width - X_LOAD_OR_DELETE, base_y)
                );

                texture_dictionary.at(MurMur3::calcHash("texture_playback")).resizedDraw(
                    TIME_ICON_SIZE,
                    paxg::Vec2i(width - X_PLAY_OR_STOP, base_y)
                );

                texture_dictionary.at(MurMur3::calcHash("texture_1step")).resizedDraw(
                    TIME_ICON_SIZE,
                    paxg::Vec2i(width - X_STEP, base_y)
                );
            }
        }

    private:
        // クリック時コールバック
        ClickCallback on_click_;

        ButtonId hitTestButton(int mx, int my) const {
            const auto width = paxg::Window::width();
            const int base_y = debug_start_y_;

            // 未初期化の場合：地形データ読み込みだけ出てる
            if (simulator_ptr_->get() == nullptr) {
                if (hitRect(mx, my, width - X_LOAD_OR_DELETE, base_y, TIME_ICON_SIZE, TIME_ICON_SIZE)) {
                    return ButtonId::LoadGeographicData;
                }
                return ButtonId::None;
            }

            // 初期化済み
            if (koyomi_->is_agent_update) {
                // 再生中 → 停止だけ
                if (hitRect(mx, my, width - X_PLAY_OR_STOP, base_y, TIME_ICON_SIZE, TIME_ICON_SIZE)) {
                    return ButtonId::Stop;
                }
                return ButtonId::None;
            }

            // 停止中 → 複数ボタン
            if (hitRect(mx, my, width - X_INIT_OR_RELOAD, base_y + RELOAD_OFFSET_Y, TIME_ICON_SIZE, TIME_ICON_SIZE)) {
                return ButtonId::ReloadInputData;
            }
            if (hitRect(mx, my, width - X_INIT_OR_RELOAD, base_y, TIME_ICON_SIZE, TIME_ICON_SIZE)) {
                return ButtonId::InitHumanData;
            }
            if (hitRect(mx, my, width - X_LOAD_OR_DELETE, base_y, TIME_ICON_SIZE, TIME_ICON_SIZE)) {
                return ButtonId::DeleteGeographicData;
            }
            if (hitRect(mx, my, width - X_PLAY_OR_STOP, base_y, TIME_ICON_SIZE, TIME_ICON_SIZE)) {
                return ButtonId::Play;
            }
            if (hitRect(mx, my, width - X_STEP, base_y, TIME_ICON_SIZE, TIME_ICON_SIZE)) {
                return ButtonId::Step;
            }

            return ButtonId::None;
        }

        static constexpr bool hitRect(int mx, int my, int rx, int ry, int w, int h) {
            return (mx >= rx && mx < rx + w && my >= ry && my < ry + h);
        }
    };

} // namespace paxs


#endif // !PAX_MAHOROBA_UI_SIMULATION_CONTROL_BUTTONS_HPP
