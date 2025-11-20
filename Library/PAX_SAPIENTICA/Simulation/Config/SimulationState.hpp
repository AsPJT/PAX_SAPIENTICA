/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2025 As Project
    [Production]    2023-2025 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_CONFIG_SIMULATION_STATE_HPP
#define PAX_SAPIENTICA_SIMULATION_CONFIG_SIMULATION_STATE_HPP

#include <cstdint>

namespace paxs {
    /// @brief シミュレーション状態
    /// @brief Simulation State
    enum class SimulationState : std::uint8_t {
        Uninitialized,  // 初期化前 / Before initialization
        Stopped,        // 停止中 / Stopped
        Playing         // 再生中 / Playing
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_SIMULATION_CONFIG_SIMULATION_STATE_HPP
