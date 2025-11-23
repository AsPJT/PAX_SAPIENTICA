/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_SIMULATION_MANAGER_HPP
#define PAX_MAHOROBA_MAP_SIMULATION_MANAGER_HPP

#ifdef PAXS_USING_SIMULATOR

#include <PAX_MAHOROBA/Input/SettlementInputHandler.hpp>
#include <PAX_MAHOROBA/Map/Core/MapViewport.hpp>
#include <PAX_MAHOROBA/Map/Simulation/SettlementManager.hpp>

#include <PAX_SAPIENTICA/Simulation/Manager/SimulationManager.hpp>
#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>

namespace paxs {

    /// @brief 地図コンテンツシミュレーション管理クラス
    /// @brief Map content simulation manager class
    ///
    /// 責務:
    /// - settlement_manager_とsettlement_input_handler_の管理
    /// - updateSettlementData()の実行
    /// - シミュレーション関連イベントの購読
    class MapContentSimulationManager {
    private:
        const paxs::SimulationManager& simulation_manager_;
        const MapViewport& map_viewport_;

        SettlementManager settlement_manager_{}; ///< 集落管理
        SettlementInputHandler settlement_input_handler_; ///< 集落入力処理

    public:
        MapContentSimulationManager(const paxs::SimulationManager& simulation_manager, const MapViewport& map_viewport)
            : simulation_manager_(simulation_manager)
            , map_viewport_(map_viewport) {
        }

        ~MapContentSimulationManager() = default;

        // コピー・ムーブ禁止（観察ポインタとイベント購読を持つため）
        MapContentSimulationManager(const MapContentSimulationManager&) = delete;
        auto operator=(const MapContentSimulationManager&)->MapContentSimulationManager & = delete;
        MapContentSimulationManager(MapContentSimulationManager&&) = delete;
        auto operator=(MapContentSimulationManager&&)->MapContentSimulationManager & = delete;

        /// @brief Settlementデータのみ更新
        /// @brief Update settlement data only
        void updateSettlementData() {
            // SettlementManager に描画パラメータを設定
            if (simulation_manager_.isActive()) {
                settlement_manager_.setDrawParams(
                    simulation_manager_.getSettlementGrids(),
                    simulation_manager_.getMarriagePositions(),
                    simulation_manager_.getBronzeShareList(),
                    map_viewport_.getSize(),
                    map_viewport_.getCenter(),
                    settlement_input_handler_.getSelectDraw(),
                    settlement_input_handler_.getIsLine(),
                    settlement_input_handler_.getIsArrow()
                );
            }
        }

        /// @brief シミュレーション関連イベントを購読
        /// @brief Subscribe to simulation events
        void subscribeToSimulationEvents() {
            paxs::EventBus& event_bus = paxs::EventBus::getInstance();

            // シミュレーション状態変更イベントの購読（初期化完了検知）
            event_bus.subscribe<SimulationStateChangedEvent>(
                [this](const SimulationStateChangedEvent& event) {
                    // 停止状態になった時（初期化完了時）に集落データを更新
                    if (event.new_state == SimulationState::Stopped) {
                        updateSettlementData();
                    }
                    else if (event.new_state == SimulationState::Uninitialized) {
                        // クリアされた場合はキャッシュをクリアして無効な参照を防ぐ
                        settlement_manager_.clearCache();
                    }
                }
            );

            // シミュレーションステップ実行イベントの購読
            // Settlementデータのみ更新（シミュレーション進行時）
            event_bus.subscribe<SimulationStepExecutedEvent>(
                [this](const SimulationStepExecutedEvent& event) {
                    (void)event;
                    updateSettlementData();
                }
            );

            // 集落表示設定変更イベントの購読
            // Settlement表示設定（select_draw, is_line, is_arrow）変更時
            event_bus.subscribe<SettlementDisplayChangedEvent>(
                [this](const SettlementDisplayChangedEvent& event) {
                    (void)event;
                    updateSettlementData();
                }
            );
        }

        /// @brief SettlementInputHandler への参照を取得（AppComponentManager での登録用）
        SettlementInputHandler& getSettlementInputHandler() {
            return settlement_input_handler_;
        }

        /// @brief SettlementManager への参照を取得（描画用）
        const SettlementManager& getSettlementManager() const {
            return settlement_manager_;
        }
    };
}

#endif // PAXS_USING_SIMULATOR

#endif // !PAX_MAHOROBA_MAP_SIMULATION_MANAGER_HPP
