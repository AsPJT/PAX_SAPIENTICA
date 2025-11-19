/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_SIMULATION_PANEL_HPP
#define PAX_MAHOROBA_UI_SIMULATION_PANEL_HPP

#include <vector>

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/UI/Pulldown.hpp>
#include <PAX_MAHOROBA/UI/Simulation/SimulationControlButton.hpp>
#include <PAX_MAHOROBA/UI/Simulation/SimulationStatsWidget.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>

#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationState.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>
#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/System/InputFile.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    class SimulationPanel : public IWidget {
    private:
        const paxs::FeatureVisibilityManager& visibility_manager_;
        AppStateManager& app_state_manager_;
        const UILayout& ui_layout_;

        std::vector<std::uint_least32_t> simulation_key;
        std::vector<std::string> simulation_model_name;

        paxs::Pulldown simulation_pulldown;
        SimulationControlButtons control_buttons_;
#ifdef PAXS_USING_SIMULATOR
        SimulationStatsWidget stats_widget_;
#endif

        // イベントから同期された状態 / State synchronized from events
        paxs::SimulationState simulation_state_ = paxs::SimulationState::Uninitialized;

        /// @brief イベント購読を設定
        void subscribeToEvents() {
            // 言語変更イベントを購読してプルダウンのレイアウトを更新
            EventBus::getInstance().subscribe<LanguageChangedEvent>(
                [this](const LanguageChangedEvent&) {
                    simulation_pulldown.updateLayout();
                }
            );

            // SimulationStateChangedイベントを購読
            EventBus::getInstance().subscribe<SimulationStateChangedEvent>(
                [this](const SimulationStateChangedEvent& event) {
                    simulation_state_ = event.new_state;
                    // SimulationControlButtonsに状態を通知
                    control_buttons_.setSimulationState(simulation_state_);
                }
            );
        }

        void onControlButtonClicked(SimulationControlButton::Id id) {
#ifdef PAXS_USING_SIMULATOR
            const std::string model_name = simulation_model_name[simulation_pulldown.getIndex()];

            switch (id) {
            case SimulationControlButton::Id::Initialize: {
                // シミュレーション非同期初期化を開始
                app_state_manager_.executeSimulationInitializeAsync(model_name);
                break;
            }
            case SimulationControlButton::Id::Stop: {
                // 停止コマンドを発行
                app_state_manager_.executeSimulationStop();
                break;
            }
            case SimulationControlButton::Id::ReloadInputData: {
                // シミュレーション入力データ再読み込みコマンドを発行
                app_state_manager_.executeReloadInputData(model_name);
                break;
            }
            case SimulationControlButton::Id::InitHumanData: {
                // 人間データ初期化コマンドを発行
                app_state_manager_.executeInitHumanData(model_name);
                break;
            }
            case SimulationControlButton::Id::Clear: {
                // シミュレーションをクリア（初期化前の状態に戻す）
                app_state_manager_.executeSimulationClear();
                break;
            }
            case SimulationControlButton::Id::Play: {
                // 再生コマンドを発行（読み込み済みチェックはAppStateManagerで行う）
                const int iterations = SimulationConstants::getInstance(model_name).num_iterations;
                app_state_manager_.executeSimulationPlay(iterations);
                break;
            }
            case SimulationControlButton::Id::Step: {
                // 1ステップ実行コマンドを発行
                app_state_manager_.executeSimulationStep(1);
                break;
            }
            case SimulationControlButton::Id::None:
            default:
                break;
            }
#endif
        }

        /// @brief コールバックを設定
        void setupCallback() {
            control_buttons_.setOnClick([this](SimulationControlButton::Id id) {
                this->onControlButtonClicked(id);
            });
        }

    public:
        // コンストラクタ
        SimulationPanel(
            const paxs::FeatureVisibilityManager& visibility_manager,
            AppStateManager& app_state_manager,
            const UILayout& ui_layout
        ) : visibility_manager_(visibility_manager),
            app_state_manager_(app_state_manager),
            ui_layout_(ui_layout),
#ifdef PAXS_USING_SIMULATOR
            stats_widget_(app_state_manager.getSimulationManager()),
#endif
            simulation_pulldown(
                paxs::MurMur3::calcHash("SimulationModels"),
                paxg::Vec2i{3000, 0},
                paxs::PulldownDisplayType::SelectedValue,
                false
            )
        {
            const std::string models_path = paxs::AppConfig::getInstance().getSettingPath(MurMur3::calcHash("SimulationModels"));
            paxs::InputFile models_tsv(models_path);
            if (models_tsv.fail()) {
                PAXS_WARNING("Failed to read Models TXT file: " + models_path);
                simulation_model_name.emplace_back("Sample");
                simulation_key.emplace_back(MurMur3::calcHash("Sample"));
            } else {
                if (!(models_tsv.getLine())) {
                    simulation_model_name.emplace_back("Sample");
                    simulation_key.emplace_back(MurMur3::calcHash("Sample"));
                } else {
                    models_tsv.deleteBOM();
                    simulation_model_name.emplace_back(models_tsv.pline);
                    simulation_key.emplace_back(MurMur3::calcHash(models_tsv.pline.c_str()));
                    while (models_tsv.getLine()) {
                        simulation_model_name.emplace_back(models_tsv.pline);
                        simulation_key.emplace_back(MurMur3::calcHash(models_tsv.pline.c_str()));
                    }
                }
            }

            simulation_pulldown.setItemsKey(simulation_key);

            setupCallback();

            // イベント購読を設定
            subscribeToEvents();
        }

        void render() const override {
            if (!isVisible()) return;

            control_buttons_.render();

            if (simulation_state_ == paxs::SimulationState::Uninitialized) {
                simulation_pulldown.render();
            }
#ifdef PAXS_USING_SIMULATOR
            else {
                stats_widget_.render();
            }
#endif
        }

        // イベント処理
        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (!isVisible()) {
                return EventHandlingResult::NotHandled();
            }

            // まずプルダウンを処理
            if (simulation_pulldown.isHit(event.pos)) {
                if (event.left_button_state == MouseButtonState::Released) {
                    return simulation_pulldown.handleEvent(event);
                }
                return EventHandlingResult::Handled();
            }

            // 次にボタンを処理
            if (control_buttons_.isHit(event.pos)) {
                return control_buttons_.handleEvent(event);
            }

            return EventHandlingResult::NotHandled();
        }

        bool isHit(const paxs::Vector2<int>& pos) const override {
            if (!isVisible()) return false;
            if (simulation_pulldown.isHit(paxs::Vector2<int>(pos.x, pos.y))) return true;
            if (control_buttons_.isHit(paxs::Vector2<int>(pos.x, pos.y))) return true;
            return false;
        }

        /// @brief レイアウトを再計算
        void calculateLayout() {
#ifdef PAXS_USING_SIMULATOR
            // プルダウンの位置をUILayoutから取得
            simulation_pulldown.setPos(Vector2<int>{
                static_cast<int>(paxg::Window::width() - simulation_pulldown.getRect().width() - ui_layout_.simulation_pulldown_right_margin),
                ui_layout_.simulation_pulldown_y_position
            });

            // ボタンレイアウトをUILayoutの情報で更新
            control_buttons_.setButtonsBaseY(ui_layout_.simulation_buttons_base_y);

            // 統計ウィジェットの位置
            stats_widget_.setPos(Vector2<int>{
                static_cast<int>(paxg::Window::width() - 250),
                ui_layout_.simulation_pulldown_y_position
            });
#endif
        }

        /// @brief SimulationControlButtonsへのアクセス
        SimulationControlButtons& getControlButtons() { return control_buttons_; }

        bool isVisible() const override {
            return visibility_manager_.isVisible(ViewMenu::simulation);
        }
        const char* getName() const override { return "SimulationPanel"; }
        RenderLayer getLayer() const override { return RenderLayer::UIContent; }
        Rect<int> getRect() const override { return {0, 0, 0, 0}; }
        void setPos(const Vector2<int>& /*pos*/) override {}
    };
}

#endif // !PAX_MAHOROBA_UI_SIMULATION_PANEL_HPP
