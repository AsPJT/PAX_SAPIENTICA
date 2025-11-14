/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_SIMULATION_CONTROL_BUTTONS_HPP
#define PAX_MAHOROBA_UI_SIMULATION_CONTROL_BUTTONS_HPP

#include <PAX_MAHOROBA/UI/Widget/IconButton.hpp>

#include <PAX_SAPIENTICA/Simulation/Config/SimulationState.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief シミュレーション制御ボタン
    class SimulationControlButton : public IconButton {
    public:
        enum class Id {
            Initialize, // 未初期化のとき
            Stop, // 初期化済 & 再生中
            ReloadInputData, // 初期化済 & 停止中
            InitHumanData,
            Clear, // シミュレーションを初期化前の状態に戻す
            Play,
            Step,
            None,
        };
        static constexpr std::array<std::pair<const char*, std::uint_least32_t>, 7> button_data = {{
            {"SimulationInitializeButton", MurMur3::calcHash("texture_load_geographic_data2")},
            {"SimulationStopButton", MurMur3::calcHash("texture_stop")},
            {"SimulationReloadInputDataButton", MurMur3::calcHash("texture_reload")},
            {"SimulationInitHumanDataButton", MurMur3::calcHash("texture_load_agent_data2")},
            {"SimulationClearButton", MurMur3::calcHash("texture_delete_geographic_data")},
            {"SimulationPlayButton", MurMur3::calcHash("texture_playback")},
            {"SimulationStepButton", MurMur3::calcHash("texture_1step")},
        }};
        using ClickCallback = std::function<void(Id)>;

        SimulationControlButton(Id id)
            : IconButton(button_data.at(static_cast<std::size_t>(id)).first, button_data.at(static_cast<std::size_t>(id)).second), id_(id) {}

        RenderLayer getLayer() const override { return RenderLayer::MenuBar; }

        void setOnClick(ClickCallback cb) {
            on_click_ = std::move(cb);
        }

        SimulationControlButton::Id getId() const { return id_; }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            // 左クリックが押されたら
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

        void setButtonPos(int x, int y, int size) {
            setPos(paxg::Vec2i{ x, y });
            setSize(paxg::Vec2i{ size, size });
        }
    private:
        Id id_ = Id::None;
        ClickCallback on_click_;
    };

    class SimulationControlButtons : public IWidget {
    public:
        using ClickCallback = std::function<void(SimulationControlButton::Id)>;

        SimulationControlButtons() {
            buildButtons();
        }

        const char* getName() const override { return "SimulationControlButtons"; }
        RenderLayer getLayer() const override { return RenderLayer::MenuBar; }

        void setButtonsBaseY(int buttons_base_y) {
            buttons_base_y_ = buttons_base_y;

            layoutButtons();
        }

        /// @brief シミュレーション状態を設定
        void setSimulationState(SimulationState state) {
            simulation_state_ = state;
        }

        void setOnClick(ClickCallback cb) {
            on_click_ = std::move(cb);
            // 各ボタンにも同じコールバックを渡す
            for (auto& btn : buttons_) {
                btn.setOnClick(on_click_);
            }
        }

        void render() const override {
            if (simulation_state_ == SimulationState::Uninitialized) {
               buttons_[static_cast<std::size_t>(SimulationControlButton::Id::Initialize)].render();
               return;
            }
            if (simulation_state_ == SimulationState::Playing) {
                // 再生中 → 停止ボタンのみ表示
                buttons_[static_cast<std::size_t>(SimulationControlButton::Id::Stop)].render();
                return;
            }
            // 停止中 → 読み込みボタンと停止ボタン以外を表示
            for (const auto& btn : buttons_) {
                if (btn.getId() == SimulationControlButton::Id::Initialize || btn.getId() == SimulationControlButton::Id::Stop) {
                    continue;
                }
                btn.render();
            }
        }

        bool isHit(int x, int y) const override {
            if (!isVisible()) {
                return false;
            }
            if (simulation_state_ == SimulationState::Uninitialized) {
                return buttons_[static_cast<std::size_t>(SimulationControlButton::Id::Initialize)].isHit(x, y);
            }
            if (simulation_state_ == SimulationState::Playing) {
                // 再生中 → 停止ボタンのみ有効
                return buttons_[static_cast<std::size_t>(SimulationControlButton::Id::Stop)].isHit(x, y);
            }
            // 停止中
            for (const auto& btn : buttons_) {
                if (btn.getId() == SimulationControlButton::Id::Initialize || btn.getId() == SimulationControlButton::Id::Stop) {
                    continue;
                }
                if (btn.isHit(x, y)) {
                    return true;
                }
            }
            return false;
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (simulation_state_ == SimulationState::Uninitialized) {
                return buttons_[static_cast<std::size_t>(SimulationControlButton::Id::Initialize)].handleEvent(event);
            }
            if (simulation_state_ == SimulationState::Playing) {
                // 再生中 → 停止ボタンのみ有効
                return buttons_[static_cast<std::size_t>(SimulationControlButton::Id::Stop)].handleEvent(event);
            }
            // 停止中
            for (auto& btn : buttons_) {
                if (btn.getId() == SimulationControlButton::Id::Initialize || btn.getId() == SimulationControlButton::Id::Stop) {
                    continue;
                }
                if (btn.isHit(event.x, event.y)) {
                    return btn.handleEvent(event);
                }
            }
            return EventHandlingResult::NotHandled();
        }

        void layoutButtons() {
            // 基本のY位置
            const int base_y = buttons_base_y_;

            for (auto& btn : buttons_) {
                switch (btn.getId()) {
                case SimulationControlButton::Id::Initialize:
                    btn.placeFromRight(X_LOAD_OR_DELETE, base_y, TIME_ICON_SIZE);
                    break;
                case SimulationControlButton::Id::Clear:
                    // 同じ位置に置いておいて、表示のON/OFFは外部状態で切る想定でもOK
                    btn.placeFromRight(X_LOAD_OR_DELETE, base_y, TIME_ICON_SIZE);
                    break;
                case SimulationControlButton::Id::InitHumanData:
                    btn.placeFromRight(X_INIT_OR_RELOAD, base_y, TIME_ICON_SIZE);
                    break;
                case SimulationControlButton::Id::ReloadInputData:
                    // リロードだけちょっと下に
                    btn.placeFromRight(X_INIT_OR_RELOAD, base_y + RELOAD_OFFSET_Y, TIME_ICON_SIZE);
                    break;
                case SimulationControlButton::Id::Play:
                    btn.placeFromRight(X_PLAY_OR_STOP, base_y, TIME_ICON_SIZE);
                    break;
                case SimulationControlButton::Id::Stop:
                    btn.placeFromRight(X_PLAY_OR_STOP, base_y, TIME_ICON_SIZE);
                    break;
                case SimulationControlButton::Id::Step:
                    btn.placeFromRight(X_STEP, base_y, TIME_ICON_SIZE);
                    break;
                default:
                    break;
                }
            }
        }

        paxg::Rect getRect() const override { return {}; }
        void setPos(const paxg::Vec2i& /*pos*/) override {}
        bool isVisible() const override { return true; }

    private:
        std::vector<SimulationControlButton> buttons_;

        // ボタン配置の基準Y座標 / Base Y coordinate for button layout
        int buttons_base_y_ = 0;

        // SimulationPanelから設定される状態 / State set from SimulationPanel
        SimulationState simulation_state_ = SimulationState::Uninitialized;

        static constexpr int TIME_ICON_SIZE = 40;

        // 右端からの固定オフセット
        static constexpr int X_LOAD_OR_DELETE = 360; // 地形データ読み込み/削除
        static constexpr int X_INIT_OR_RELOAD = 420; // 人間データ初期化/リロード
        static constexpr int X_PLAY_OR_STOP   = 300; // 再生/停止
        static constexpr int X_STEP           = 240; // 1ステップ
        static constexpr int RELOAD_OFFSET_Y  = 60;  // リロードだけ+60

        ClickCallback on_click_;

        void buildButtons() {
            buttons_.emplace_back(SimulationControlButton::Id::Initialize);
            buttons_.emplace_back(SimulationControlButton::Id::Stop);
            buttons_.emplace_back(SimulationControlButton::Id::ReloadInputData);
            buttons_.emplace_back(SimulationControlButton::Id::InitHumanData);
            buttons_.emplace_back(SimulationControlButton::Id::Clear);
            buttons_.emplace_back(SimulationControlButton::Id::Play);
            buttons_.emplace_back(SimulationControlButton::Id::Step);

            if (on_click_) {
                for (auto& btn : buttons_) {
                    btn.setOnClick(on_click_);
                }
            }

            layoutButtons();
        }
    };
} // namespace paxs


#endif // !PAX_MAHOROBA_UI_SIMULATION_CONTROL_BUTTONS_HPP
