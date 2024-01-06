/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_KANAKUMA_LIFE_SPAN_HPP
#define PAX_SAPIENTICA_SIMULATION_KANAKUMA_LIFE_SPAN_HPP

/*##########################################################################################

##########################################################################################*/

#include <random>

#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>

namespace paxs {

    /// @brief Lifespan data of the Kanakuma site.
    /// @brief 金隈遺跡の寿命データ
    class KanakumaLifeSpan {
    private:

        // 金隈遺跡の寿命データ

        std::uniform_int_distribution<> life_person_num{ 1, 136 }; // 金隈遺跡の寿命データによる人間の数
        std::uniform_int_distribution<> life_child_num{ 1, 35 }; // 金隈遺跡の寿命データによる子供の数

        std::uniform_int_distribution<> life_male_adult_num{ 1, 38 }; // 金隈遺跡の寿命データによる男性の数
        std::uniform_int_distribution<> life_female_adult_num{ 1, 44 }; // 金隈遺跡の寿命データによる女性の数

        std::uniform_int_distribution<> life_infant_exp_dist{ 1, 1 * SimulationConstants::getInstance()->steps_per_year }; // 乳児の寿命の乱数分布
        std::uniform_int_distribution<> life_toddler_exp_dist{ 1 * SimulationConstants::getInstance()->steps_per_year + 1, 6 * SimulationConstants::getInstance()->steps_per_year }; // 幼児の寿命の乱数分布
        std::uniform_int_distribution<> life_child_exp_dist{ 6 * SimulationConstants::getInstance()->steps_per_year + 1, 12 * SimulationConstants::getInstance()->steps_per_year }; // 小児の寿命の乱数分布
        std::uniform_int_distribution<> life_young_exp_dist{ 12 * SimulationConstants::getInstance()->steps_per_year + 1, 20 * SimulationConstants::getInstance()->steps_per_year }; // 若年の寿命の乱数分布
        std::uniform_int_distribution<> life_adult_exp_dist{ 20 * SimulationConstants::getInstance()->steps_per_year + 1, 40 * SimulationConstants::getInstance()->steps_per_year }; // 成年の寿命の乱数分布
        std::uniform_int_distribution<> life_mature_exp_dist{ 40 * SimulationConstants::getInstance()->steps_per_year + 1, 60 * SimulationConstants::getInstance()->steps_per_year }; // 熟年の寿命の乱数分布
        std::uniform_int_distribution<> life_older_exp_dist{ 60 * SimulationConstants::getInstance()->steps_per_year + 1, 80 * SimulationConstants::getInstance()->steps_per_year }; // 老年の寿命の乱数分布

    public:

        /// @brief 英語未翻訳
        /// @brief 寿命を決定する
        std::uint_least32_t setAdultLifeSpan(const std::uint_least8_t gender_, std::mt19937& gen) {
            // もし大人だったら
            if (gender_ == 0) { // 女性の場合
                const std::uint_least32_t adult_type = static_cast<std::uint_least32_t>(life_male_adult_num(gen));

                if (adult_type <= 14) { // 成年
                    return static_cast<std::uint_least32_t>(life_adult_exp_dist(gen));
                }
                else if (adult_type <= (14 + 26)) { // 熟年
                    return static_cast<std::uint_least32_t>(life_mature_exp_dist(gen));
                }
                // 老年
                return static_cast<std::uint_least32_t>(life_older_exp_dist(gen));
            }
            else if (gender_ == 1) { // 男性の場合
                const std::uint_least32_t adult_type = static_cast<std::uint_least32_t>(life_female_adult_num(gen));

                if (adult_type <= 19) { // 成年
                    return static_cast<std::uint_least32_t>(life_adult_exp_dist(gen));
                }
                else if (adult_type <= (19 + 19)) { // 熟年
                    return static_cast<std::uint_least32_t>(life_mature_exp_dist(gen));
                }
            }
            // 老年
            return static_cast<std::uint_least32_t>(life_older_exp_dist(gen));
        }

        /// @brief 英語未翻訳
        /// @brief 寿命を決定する
        std::uint_least32_t setLifeSpan(const std::uint_least8_t gender_, std::mt19937& gen) {

            if (life_person_num(gen) <= 37) { // もし子供だったら
                const std::uint_least32_t child_type = static_cast<std::uint_least32_t>(life_child_num(gen));

                if (child_type <= 6) { // 乳児
                    return static_cast<std::uint_least32_t>(life_infant_exp_dist(gen));
                }
                else if (child_type <= (6 + 22)) { // 幼児
                    return static_cast<std::uint_least32_t>(life_toddler_exp_dist(gen));
                }
                else if (child_type <= (6 + 22 + 5)) { // 小児
                    return static_cast<std::uint_least32_t>(life_child_exp_dist(gen));
                }
                // 若年
                return static_cast<std::uint_least32_t>(life_young_exp_dist(gen));
            }
            // もし大人だったら
            return setAdultLifeSpan(gender_, gen);
        }

    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_KANAKUMA_LIFE_SPAN_HPP
