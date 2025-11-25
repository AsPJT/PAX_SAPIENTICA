/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_SIMULATION_I_SIMULATION_PROVIDER_HPP
#define PAX_MAHOROBA_MAP_SIMULATION_I_SIMULATION_PROVIDER_HPP

#include <functional>

namespace paxs {

    // Forward declarations
    class SettlementInputHandler;
    class SettlementManager;

    /// @brief シミュレーション機能プロバイダーインターフェース
    /// @brief Simulation provider interface
    class ISimulationProvider {
    public:
        virtual ~ISimulationProvider() = default;

        /// @brief シミュレーション関連イベントを購読
        /// @brief Subscribe to simulation events
        virtual void subscribeToSimulationEvents() = 0;

        /// @brief Settlementデータを更新
        /// @brief Update settlement data
        virtual void updateSettlementData() = 0;

        /// @brief SettlementInputHandler への参照を取得
        /// @brief Get SettlementInputHandler reference
        /// @return SettlementInputHandler* (nullptrの場合あり)
        virtual SettlementInputHandler* getSettlementInputHandler() = 0;

        /// @brief SettlementManager への参照を取得
        /// @brief Get SettlementManager reference
        /// @return SettlementManager* (nullptrの場合あり)
        virtual const SettlementManager* getSettlementManager() const = 0;

        /// @brief シミュレーション機能が有効かどうか
        /// @brief Check if simulation is enabled
        virtual bool isEnabled() const = 0;

        /// @brief シミュレーション描画を実行
        /// @brief Render simulation
        virtual void render() const = 0;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_SIMULATION_I_SIMULATION_PROVIDER_HPP
