/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_SIMULATION_SETTLEMENT_SIMULATION_PROVIDER_HPP
#define PAX_MAHOROBA_MAP_SIMULATION_SETTLEMENT_SIMULATION_PROVIDER_HPP

#ifdef PAXS_USING_SIMULATOR

#include <PAX_MAHOROBA/Map/Simulation/ISimulationProvider.hpp>
#include <PAX_MAHOROBA/Map/Simulation/SimulationManager.hpp>

namespace paxs {

    /// @brief Settlement シミュレーション機能プロバイダー
    /// @brief Settlement simulation provider
    ///
    /// PAXS_USING_SIMULATORが定義されている場合に使用される。
    /// MapContentSimulationManager をラップしてISimulationProviderインターフェースを提供。
    class SettlementSimulationProvider : public ISimulationProvider {
    private:
        MapContentSimulationManager simulation_manager_;

    public:
        explicit SettlementSimulationProvider(const AppStateManager& app_state_manager)
            : simulation_manager_(app_state_manager.getSimulationManager(), app_state_manager.getMapViewport()) {
        }

        ~SettlementSimulationProvider() override = default;

        // コピー・ムーブ禁止（観察ポインタとイベント購読を持つため）
        SettlementSimulationProvider(const SettlementSimulationProvider&) = delete;
        auto operator=(const SettlementSimulationProvider&)->SettlementSimulationProvider& = delete;
        SettlementSimulationProvider(SettlementSimulationProvider&&) = delete;
        auto operator=(SettlementSimulationProvider&&)->SettlementSimulationProvider& = delete;

        void subscribeToSimulationEvents() override {
            simulation_manager_.subscribeToSimulationEvents();
        }

        void updateSettlementData() override {
            simulation_manager_.updateSettlementData();
        }

        SettlementInputHandler* getSettlementInputHandler() override {
            return &simulation_manager_.getSettlementInputHandler();
        }

        const SettlementManager* getSettlementManager() const override {
            return &simulation_manager_.getSettlementManager();
        }

        bool isEnabled() const override {
            return true; // シミュレーション有効
        }

        void render() const override {
            simulation_manager_.getSettlementManager().render();
        }
    };

} // namespace paxs

#endif // PAXS_USING_SIMULATOR

#endif // !PAX_MAHOROBA_MAP_SIMULATION_SETTLEMENT_SIMULATION_PROVIDER_HPP
