/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_KANAKUMA_LIFE_SPAN_HPP
#define PAX_SAPIENTICA_SIMULATION_KANAKUMA_LIFE_SPAN_HPP

#include <random>

#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>

namespace paxs {

    /// @brief Lifespan data of the Kanakuma site.
    /// @brief 金隈遺跡の寿命データ
    class KanakumaLifeSpan {
    public:
        KanakumaLifeSpan() = default;
        /// @brief 英語未翻訳
        /// @brief 寿命を決定する
        AgeType setLifeSpan(const bool is_farming_, const bool is_male_, std::mt19937& gen) {
            return static_cast<AgeType>(SimulationConstants::getInstance()->getLifeSpan(is_farming_, !is_male_, gen) * SimulationConstants::getInstance()->steps_per_year
                + SimulationConstants::getInstance()->step_per_year_dist(gen));
        }

    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_KANAKUMA_LIFE_SPAN_HPP
