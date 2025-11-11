/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_SIMULATION_PANEL_HPP
#define PAX_MAHOROBA_UI_SIMULATION_PANEL_HPP

#include <cmath>
#include <memory>
#include <random>
#include <vector>

#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/Core/ApplicationEvents.hpp>
#include <PAX_MAHOROBA/Core/EventBus.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/UI/Pulldown.hpp>
#include <PAX_MAHOROBA/UI/Simulation/SimulationControlButton.hpp>
#include <PAX_MAHOROBA/UI/Simulation/SimulationStatsWidget.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/InputFile.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationState.hpp>

namespace paxs {

    class SimulationPanel : public IWidget {
    private:
        const paxs::FeatureVisibilityManager* visibility_manager_ptr = nullptr;
        AppStateManager* app_state_manager_ = nullptr;

        // UI配置定数
        static constexpr int pulldown_y_position = 600;
        static constexpr int pulldown_right_margin = 200;

        paxs::Pulldown simulation_pulldown;
        SimulationControlButtons control_buttons_;
        SimulationStatsWidget stats_widget_;

        // イベントから同期された状態 / State synchronized from events
        paxs::SimulationState simulation_state_ = paxs::SimulationState::Uninitialized;

        /// @brief イベント購読を設定
        /// @brief Subscribe to events
        void subscribeToEvents() {
            // 言語変更イベントを購読してプルダウンの言語を更新
            EventBus::getInstance().subscribe<LanguageChangedEvent>(
                [this](const LanguageChangedEvent&) {
                    simulation_pulldown.updateLanguage();
                    calculateLayout();
                }
            );

            // ウィンドウリサイズイベントを購読してレイアウトを再計算
            EventBus::getInstance().subscribe<WindowResizedEvent>(
                [this](const WindowResizedEvent&) {
                    calculateLayout();
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

        /// @brief レイアウトを再計算
        void calculateLayout() {
#ifdef PAXS_USING_SIMULATOR
            // プルダウンの位置を更新
            simulation_pulldown.setPos(paxg::Vec2i{
                static_cast<int>(paxg::Window::width() - simulation_pulldown.getRect().w() - pulldown_right_margin),
                pulldown_y_position
            });

            // ボタンのレイアウトも更新
            control_buttons_.layoutButtons();

            stats_widget_.setPos(paxg::Vec2i{
                static_cast<int>(paxg::Window::width() - 250),
                pulldown_y_position
            });
#endif
        }

        void simulationInit() const {
#ifdef PAXS_USING_SIMULATOR
            if (!app_state_manager_) return;
            const std::string model_name = simulation_model_name[simulation_pulldown.getIndex()];

            // AppStateManagerを通じてシミュレーション初期化コマンドを発行
            app_state_manager_->executeSimulationInit(model_name);
#endif
        }

        void onControlButtonClicked(SimulationControlButton::Id id) {
#ifdef PAXS_USING_SIMULATOR
            if (!app_state_manager_) return;

            const std::string model_name = simulation_model_name[simulation_pulldown.getIndex()];

            switch (id) {
            case SimulationControlButton::Id::Initialize: {
#ifdef PAXS_USING_SIV3D
                // Siv3Dコンソールを開く（初回のみ）
                static bool is_console_open = false;
                if (!is_console_open) {
                    s3d::detail::Console_impl{}.open();
                    is_console_open = true;
                }
#endif
                // シミュレーション初期化コマンドを発行
                std::random_device seed_gen;
                app_state_manager_->executeSimulationInitialize(model_name, seed_gen());
                break;
            }
            case SimulationControlButton::Id::Stop: {
                // 停止コマンドを発行
                app_state_manager_->executeSimulationStop();
                break;
            }
            case SimulationControlButton::Id::ReloadInputData: {
                // シミュレーション入力データ再読み込みコマンドを発行
                app_state_manager_->executeReloadInputData(model_name);
                break;
            }
            case SimulationControlButton::Id::InitHumanData: {
                // 人間データ初期化コマンドを発行
                app_state_manager_->executeInitHumanData(model_name);
                break;
            }
            case SimulationControlButton::Id::Clear: {
                // シミュレーションをクリア（初期化前の状態に戻す）
                app_state_manager_->executeSimulationClear();
                break;
            }
            case SimulationControlButton::Id::Play: {
                // 再生コマンドを発行（読み込み済みチェックはAppStateManagerで行う）
                const int iterations = SimulationConstants::getInstance(model_name)->num_iterations;
                app_state_manager_->executeSimulationPlay(iterations);
                break;
            }
            case SimulationControlButton::Id::Step: {
                // 1ステップ実行コマンドを発行
                app_state_manager_->executeSimulationStep(1);
                break;
            }
            case SimulationControlButton::Id::None:
            default:
                break;
            }
#endif
        }

    public:
        std::vector<std::uint_least32_t> simulation_key;
        std::vector<std::string> simulation_model_name;

        // コンストラクタ
        SimulationPanel(
            const paxs::FeatureVisibilityManager* visibility_manager,
            AppStateManager& app_state_manager
        ) : visibility_manager_ptr(visibility_manager),
            app_state_manager_(&app_state_manager),
            simulation_pulldown(
                paxs::LanguageDomain::SIMULATION,
                paxg::Vec2i{3000, 0},
                paxs::PulldownDisplayType::SelectedValue,
                false
            )
        {

            // Models.txt 読み込み
            const std::string models_path = "Data/Simulations/Models.txt";
            paxs::InputFile models_tsv(AppConfig::getInstance()->getRootPath() + models_path);
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

            // イベント購読を設定
            subscribeToEvents();
        }

        /// @brief SimulationControlButtonsへのアクセス
        SimulationControlButtons& getControlButtons() { return control_buttons_; }

        /// @brief コールバックを設定
        void setupCallback() {
            control_buttons_.setOnClick([this](SimulationControlButton::Id id) {
                this->onControlButtonClicked(id);
            });

#ifdef PAXS_USING_SIMULATOR
            // SimulationStatsWidgetにSimulationManagerを設定
            if (app_state_manager_) {
                stats_widget_.setSimulationManager(&app_state_manager_->getSimulationManager());
            }
#endif

            // 初期レイアウト計算
            calculateLayout();
        }

        void render() const override {
            if (!isVisible()) return;

            control_buttons_.render();

            if (simulation_state_ == paxs::SimulationState::Uninitialized) {
                simulation_pulldown.render();
            } else {
                stats_widget_.render();
            }
        }

        // イベント処理
        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (!isVisible()) {
                return EventHandlingResult::NotHandled();
            }

            // まずプルダウンを処理
            if (simulation_pulldown.isHit(event.x, event.y)) {
                if (event.left_button_state == MouseButtonState::Released) {
                    return simulation_pulldown.handleEvent(event);
                } else {
                    return EventHandlingResult::Handled();
                }
            }

            // 次にボタンを処理
            if (control_buttons_.isHit(event.x, event.y)) {
                return control_buttons_.handleEvent(event);
            }

            return EventHandlingResult::NotHandled();
        }

        bool isHit(int x, int y) const override {
            if (!isVisible()) return false;
            if (simulation_pulldown.isHit(x, y)) return true;
            if (control_buttons_.isHit(x, y)) return true;
            return false;
        }

        bool isVisible() const override {
            return visibility_manager_ptr->isVisible(ViewMenu::simulation);
        }
        const char* getName() const override { return "SimulationPanel"; }
        RenderLayer getLayer() const override { return RenderLayer::UIContent; }
        paxg::Rect getRect() const override { return paxg::Rect{};}
        void setPos(const paxg::Vec2i& /*pos*/) override {}
    };
}

#endif // !PAX_MAHOROBA_UI_SIMULATION_PANEL_HPP
