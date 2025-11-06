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
#include <cstdint>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/UI/Pulldown.hpp>
#include <PAX_MAHOROBA/UI/SimulationControlButtons.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/InputFile.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>

namespace paxs {

    class SimulationPanel : public IWidget {
    private:
        const paxs::FeatureVisibilityManager* visibility_manager_ptr = nullptr;
        std::unique_ptr<paxs::SettlementSimulator>* simulator_ptr_ = nullptr;
        paxs::Koyomi* koyomi_ = nullptr;

        const int pulldown_y = 600; // プルダウンのY座標

        int m_remaining_iterations = 0;
        bool enabled_ = true;

        mutable paxs::Pulldown simulation_pulldown;
        SimulationControlButtons control_buttons_;

        void simulationInit() const {
            if (!simulator_ptr_ || !koyomi_) return;
            const std::string model_name = simulation_model_name[simulation_pulldown.getIndex()];

            (*simulator_ptr_)->init();
            koyomi_->steps.setDay(0);
            koyomi_->jdn.setDay(static_cast<double>(
                SimulationConstants::getInstance(model_name)->start_julian_day));
            koyomi_->calcDate();
            koyomi_->is_agent_update = false;
            koyomi_->move_forward_in_time = false;
            koyomi_->go_back_in_time = false;
        }

        void onControlButtonClicked(SimulationControlButtons::ButtonId id) {
            if (!simulator_ptr_ || !koyomi_) return;

            const std::string model_name = simulation_model_name[simulation_pulldown.getIndex()];

            // よく使うパスを先に作る（必要な場合だけ使う）
            auto make_paths = [&]() {
                std::string map_list_path = "Data/Simulations/" + model_name + "/MapList.tsv";
                std::string japan_provinces_path = "Data/Simulations/" + model_name;
                AppConfig::getInstance()->calcDataSettingsNotPath(
                    MurMur3::calcHash("SimulationXYZTiles"),
                    [&](const std::string& path_) { map_list_path = path_; });
                AppConfig::getInstance()->calcDataSettingsNotPath(
                    MurMur3::calcHash("SimulationProvincesPath"),
                    [&](const std::string& path_) { japan_provinces_path = path_; });
                paxs::StringExtensions::replace(map_list_path, "Sample", model_name);
                paxs::StringExtensions::replace(japan_provinces_path, "Sample", model_name);
                return std::pair{std::move(map_list_path), std::move(japan_provinces_path)};
            };

            switch (id) {
            case SimulationControlButtons::ButtonId::LoadGeographicData: {
                // 元の「未初期化時に地形データ読み込みを押したとき」の処理
                auto [map_list_path, japan_provinces_path] = make_paths();
                SimulationConstants::getInstance(model_name)->init(model_name);

#ifdef PAXS_USING_SIV3D
                static bool is_console_open = false;
                if (!is_console_open) {
                    s3d::detail::Console_impl{}.open();
                    is_console_open = true;
                }
#endif
                std::random_device seed_gen;
                *simulator_ptr_ = std::make_unique<paxs::SettlementSimulator>(
                    map_list_path, japan_provinces_path, seed_gen());
                simulationInit();
                break;
            }
            case SimulationControlButtons::ButtonId::Stop: {
                // 再生中 → 停止
                koyomi_->is_agent_update = false;
                koyomi_->move_forward_in_time = false;
                koyomi_->go_back_in_time = false;
                break;
            }
            case SimulationControlButtons::ButtonId::ReloadInputData: {
                // シミュレーション入力データ初期化
                SimulationConstants::getInstance(model_name)->init(model_name);
                break;
            }
            case SimulationControlButtons::ButtonId::InitHumanData: {
                // 人間データ初期化
                simulationInit();
                koyomi_->steps.setDay(0);
                koyomi_->calcDate();
                break;
            }
            case SimulationControlButtons::ButtonId::DeleteGeographicData: {
                // 地形データ削除
                simulator_ptr_->reset();
                koyomi_->steps.setDay(0);
                koyomi_->calcDate();
                break;
            }
            case SimulationControlButtons::ButtonId::Play: {
                // 再生
                if (simulator_ptr_->get() == nullptr) {
                    // 読み込み前なら何もしない
                    break;
                }
                koyomi_->is_agent_update = true;
                m_remaining_iterations = SimulationConstants::getInstance(model_name)->num_iterations;
                koyomi_->move_forward_in_time = true;
                koyomi_->go_back_in_time = false;
                break;
            }
            case SimulationControlButtons::ButtonId::Step: {
                // 1ステップ実行
                if (simulator_ptr_->get() == nullptr) break;
                simulator_ptr_->get()->step();
                koyomi_->steps.getDay()++;
                koyomi_->calcDate();
                koyomi_->move_forward_in_time = false;
                koyomi_->go_back_in_time = false;
                break;
            }
            case SimulationControlButtons::ButtonId::None:
            default:
                break;
            }
        }

    public:
        // モデルリスト
        std::vector<std::uint_least32_t> simulation_key;
        std::vector<std::string> simulation_model_name;

        // コンストラクタ
        SimulationPanel(
            const paxs::FeatureVisibilityManager* visibility_manager
        ) : visibility_manager_ptr(visibility_manager),
            simulation_pulldown(
                static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_SIZE),
                static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
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
        }

        // TODO: 移行
        void updateSimulationAuto() {
            if (!simulator_ptr_ || !koyomi_ || simulator_ptr_->get() == nullptr) return;

            const std::string model_name = simulation_model_name[simulation_pulldown.getIndex()];
            const auto* constants = SimulationConstants::getInstance(model_name);
            const int total_steps = constants->total_steps;

            // 規定ステップ数に達したかチェック
            if (total_steps > 0 &&
                koyomi_->steps.getDay() >= static_cast<std::size_t>(total_steps)) {

                // 残り実行回数を減らす
                m_remaining_iterations--;

                if (m_remaining_iterations > 0) {
                    // まだ実行回数が残っている場合、シミュレーションを初期化して自動で再開
                    simulationInit();
                    koyomi_->is_agent_update = true;
                    koyomi_->move_forward_in_time = true;
                } else {
                    // 全ての実行が終了した場合、シミュレーションを停止
                    koyomi_->is_agent_update = false;
                    koyomi_->move_forward_in_time = false;
                    m_remaining_iterations = 0;
                }
            }
        }

        // 外部参照のセット
        void setReferences(
            std::unique_ptr<paxs::SettlementSimulator>& simulator,
            paxs::Koyomi& koyomi,
            int debug_start_y
        ) {
            simulator_ptr_ = &simulator;
            koyomi_ = &koyomi;

            // ボタン側にも参照を渡す
            control_buttons_.setReferences(simulator_ptr_, koyomi_, debug_start_y);
            // ここで「押されたときの処理」を紐づける
            control_buttons_.setOnClick([this](SimulationControlButtons::ButtonId id) {
                this->onControlButtonClicked(id);
            });
        }

        // 描画
        void render() const override {
            if (!isVisible() || !simulator_ptr_ || !koyomi_) return;

            drawPulldown();
            control_buttons_.render();
        }

        void drawPulldown() const {
            simulation_pulldown.setPos(paxg::Vec2i{
                static_cast<int>(paxg::Window::width() - simulation_pulldown.getRect().w() - 200),
                pulldown_y
            });
            simulation_pulldown.updateLanguage();
            if (simulator_ptr_->get() == nullptr) {
                simulation_pulldown.render();
            }
        }

        // イベント処理
        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (!isVisible() || !isEnabled()) {
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

        bool isVisible() const override {
            return visibility_manager_ptr->isVisible(MurMur3::calcHash("Simulation"));
        }

        bool isHit(int x, int y) const override {
            if (!isVisible() || !isEnabled()) return false;
            if (simulation_pulldown.isHit(x, y)) return true;
            if (control_buttons_.isHit(x, y)) return true;
            return false;
        }

        RenderLayer getLayer() const override { return RenderLayer::UIContent; }

        const char* getName() const override {
            return "SimulationPanel";
        }

        paxg::Rect getRect() const override { return paxg::Rect{};}
        void setVisible(bool /*visible*/) override {}
        void setPos(const paxg::Vec2i& /*pos*/) override {}
        void setEnabled(bool enabled) override { enabled_ = enabled; }
        bool isEnabled() const override { return enabled_; }
    };
}

#endif // !PAX_MAHOROBA_UI_SIMULATION_PANEL_HPP
