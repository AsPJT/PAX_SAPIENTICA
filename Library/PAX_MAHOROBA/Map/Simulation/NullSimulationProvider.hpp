/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_SIMULATION_NULL_SIMULATION_PROVIDER_HPP
#define PAX_MAHOROBA_MAP_SIMULATION_NULL_SIMULATION_PROVIDER_HPP

#include <PAX_MAHOROBA/Map/Simulation/ISimulationProvider.hpp>

namespace paxs {

    /// @brief Null Object パターンによるシミュレーション無効時の実装
    /// @brief Null Object pattern implementation for disabled simulation
    ///
    /// PAXS_USING_SIMULATORが未定義の場合に使用される。
    /// すべてのメソッドは何もしない（No-op）。
    class NullSimulationProvider : public ISimulationProvider {
    public:
        NullSimulationProvider() = default;
        ~NullSimulationProvider() override = default;

        // コピー・ムーブ可能（状態を持たないため）
        NullSimulationProvider(const NullSimulationProvider&) = default;
        auto operator=(const NullSimulationProvider&)->NullSimulationProvider& = default;
        NullSimulationProvider(NullSimulationProvider&&) = default;
        auto operator=(NullSimulationProvider&&)->NullSimulationProvider& = default;

        void subscribeToSimulationEvents() override {
            // No-op: シミュレーション無効時は何もしない
        }

        void updateSettlementData() override {
            // No-op: シミュレーション無効時は何もしない
        }

        SettlementInputHandler* getSettlementInputHandler() override {
            return nullptr; // シミュレーション無効時はnullptr
        }

        const SettlementManager* getSettlementManager() const override {
            return nullptr; // シミュレーション無効時はnullptr
        }

        bool isEnabled() const override {
            return false; // シミュレーション無効
        }

        void render() const override {
            // No-op: シミュレーション無効時は何も描画しない
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_SIMULATION_NULL_SIMULATION_PROVIDER_HPP
