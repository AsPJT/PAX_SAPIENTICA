/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0.  https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_SIMULATION_CONTROL_BUTTONS_HPP
#define PAX_MAHOROBA_UI_SIMULATION_CONTROL_BUTTONS_HPP

#include <PAX_MAHOROBA/UI/Widget/IconButton.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>

namespace paxs {

    /// @brief シミュレーション制御ボタン
    class SimulationControlButton : public IconButton {
    public:
        enum class Id {
            LoadGeographicData, // 未初期化のとき
            Stop, // 初期化済 & 再生中
            ReloadInputData, // 初期化済 & 停止中
            InitHumanData,
            DeleteGeographicData,
            Play,
            Step,
            None,
        };
        static constexpr std::array<std::pair<const char*, std::uint_least32_t>, 7> button_data = {{
            {"SimulationLoadGeographicDataButton", MurMur3::calcHash("texture_load_geographic_data2")},
            {"SimulationStopButton", MurMur3::calcHash("texture_stop")},
            {"SimulationReloadInputDataButton", MurMur3::calcHash("texture_reload")},
            {"SimulationInitHumanDataButton", MurMur3::calcHash("texture_load_agent_data2")},
            {"SimulationDeleteGeographicDataButton", MurMur3::calcHash("texture_delete_geographic_data")},
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
            if (!isVisible() || !isEnabled()) {
                return EventHandlingResult::NotHandled();
            }
            // 左クリックが押されたら
            if (event.left_button_state == MouseButtonState::Pressed) {
                const auto r = getRect();
                if (r.contains(event.x, event.y)) {
                    if (on_click_) {
                        on_click_(id_);
                    }
                    return EventHandlingResult::Handled();
                }
            }
            return EventHandlingResult::NotHandled();
        }

        void placeFromRight(int offset_from_right, int y, int size) {
            const int x = paxg::Window::width() - offset_from_right;
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

        void setReferences(
            std::unique_ptr<paxs::SettlementSimulator>* simulator_ptr,
            paxs::Koyomi* koyomi,
            int debug_start_y
        ) {
            simulator_ptr_ = simulator_ptr;
            koyomi_ = koyomi;
            debug_start_y_ = debug_start_y;

            layoutButtons();
        }

        void setOnClick(ClickCallback cb) {
            on_click_ = std::move(cb);
            // 各ボタンにも同じコールバックを渡す
            for (auto& btn : buttons_) {
                btn.setOnClick(on_click_);
            }
        }

        void render() const override {
            if (!visible_) return;

            if (simulator_ptr_->get() == nullptr) {
               buttons_[static_cast<std::size_t>(SimulationControlButton::Id::LoadGeographicData)].render();
               return;
            }
            for (const auto& btn : buttons_) {
                if (btn.getId() == SimulationControlButton::Id::LoadGeographicData) {
                    // 読み込みボタンは非表示
                    continue;
                }
                btn.render();
            }
        }

        bool isHit(int x, int y) const override {
            if (!isVisible() || !isEnabled()) {
                return false;
            }
            if (simulator_ptr_->get() == nullptr) {
                return buttons_[static_cast<std::size_t>(SimulationControlButton::Id::LoadGeographicData)].isHit(x, y);
            }
            for (const auto& btn : buttons_) {
                if (btn.getId() == SimulationControlButton::Id::LoadGeographicData) {
                    // 読み込みボタンは非表示
                    continue;
                }
                if (btn.isHit(x, y)) {
                    return true;
                }
            }
            return false;
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (simulator_ptr_->get() == nullptr) {
                return buttons_[static_cast<std::size_t>(SimulationControlButton::Id::LoadGeographicData)].handleEvent(event);
            }
            for (auto& btn : buttons_) {
                if (btn.getId() == SimulationControlButton::Id::LoadGeographicData) {
                    // 読み込みボタンは非表示
                    continue;
                }
                EventHandlingResult r = btn.handleEvent(event);
                if (r.handled) {
                    return r;
                }
            }
            return EventHandlingResult::NotHandled();
        }

        paxg::Rect getRect() const override { return {}; }
        void setPos(const paxg::Vec2i& /*pos*/) override {}
        bool isEnabled() const override { return enabled_; }
        bool isVisible() const override { return visible_; }
        void setEnabled(bool enabled) override { enabled_ = enabled; }
        void setVisible(bool visible) override { visible_ = visible; }

    private:
        std::vector<SimulationControlButton> buttons_;

        std::unique_ptr<paxs::SettlementSimulator>* simulator_ptr_ = nullptr;
        paxs::Koyomi* koyomi_ = nullptr;
        int debug_start_y_ = 0;

        bool visible_ = true;
        bool enabled_ = true;

        static constexpr int TIME_ICON_SIZE = 40;

        // 右端からの固定オフセット
        static constexpr int X_LOAD_OR_DELETE = 360; // 地形データ読み込み/削除
        static constexpr int X_INIT_OR_RELOAD = 420; // 人間データ初期化/リロード
        static constexpr int X_PLAY_OR_STOP   = 300; // 再生/停止
        static constexpr int X_STEP           = 240; // 1ステップ
        static constexpr int RELOAD_OFFSET_Y  = 60;  // リロードだけ+60

        ClickCallback on_click_;

    private:
        void buildButtons() {
            buttons_.emplace_back(SimulationControlButton::Id::LoadGeographicData);
            buttons_.emplace_back(SimulationControlButton::Id::Stop);
            buttons_.emplace_back(SimulationControlButton::Id::ReloadInputData);
            buttons_.emplace_back(SimulationControlButton::Id::InitHumanData);
            buttons_.emplace_back(SimulationControlButton::Id::DeleteGeographicData);
            buttons_.emplace_back(SimulationControlButton::Id::Play);
            buttons_.emplace_back(SimulationControlButton::Id::Step);

            if (on_click_) {
                for (auto& btn : buttons_) {
                    btn.setOnClick(on_click_);
                }
            }

            layoutButtons();
        }

        void layoutButtons() {
            // 基本のY位置
            const int base_y = debug_start_y_;

            for (auto& btn : buttons_) {
                switch (btn.getId()) {
                case SimulationControlButton::Id::LoadGeographicData:
                    btn.placeFromRight(X_LOAD_OR_DELETE, base_y, TIME_ICON_SIZE);
                    break;
                case SimulationControlButton::Id::DeleteGeographicData:
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
    };
} // namespace paxs


#endif // !PAX_MAHOROBA_UI_SIMULATION_CONTROL_BUTTONS_HPP
