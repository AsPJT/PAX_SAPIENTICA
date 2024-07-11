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
#include <PAX_SAPIENTICA/Simulation/JapanProvinces.hpp>

namespace paxs {

    /// @brief Lifespan data of the Kanakuma site.
    /// @brief 金隈遺跡の寿命データ
    class KanakumaLifeSpan {
    private:

        //// 金隈遺跡の寿命データ

        //std::uniform_int_distribution<> life_person_num{ 1, 136 }; // 金隈遺跡の寿命データによる人間の数
        //std::uniform_int_distribution<> life_child_num{ 1, 35 }; // 金隈遺跡の寿命データによる子供の数

        //std::uniform_int_distribution<> life_male_adult_num{ 1, 38 }; // 金隈遺跡の寿命データによる男性の数
        //std::uniform_int_distribution<> life_female_adult_num{ 1, 44 }; // 金隈遺跡の寿命データによる女性の数

        //std::uniform_int_distribution<> life_infant_exp_dist{ 1, 1 * SimulationConstants::getInstance()->steps_per_year }; // 乳児の寿命の乱数分布
        //std::uniform_int_distribution<> life_toddler_exp_dist{ 1 * SimulationConstants::getInstance()->steps_per_year + 1, 6 * SimulationConstants::getInstance()->steps_per_year }; // 幼児の寿命の乱数分布
        //std::uniform_int_distribution<> life_child_exp_dist{ 6 * SimulationConstants::getInstance()->steps_per_year + 1, 12 * SimulationConstants::getInstance()->steps_per_year }; // 小児の寿命の乱数分布
        //std::uniform_int_distribution<> life_young_exp_dist{ 12 * SimulationConstants::getInstance()->steps_per_year + 1, 20 * SimulationConstants::getInstance()->steps_per_year }; // 若年の寿命の乱数分布
        //std::uniform_int_distribution<> life_adult_exp_dist{ 20 * SimulationConstants::getInstance()->steps_per_year + 1, 40 * SimulationConstants::getInstance()->steps_per_year }; // 成年の寿命の乱数分布
        //std::uniform_int_distribution<> life_mature_exp_dist{ 40 * SimulationConstants::getInstance()->steps_per_year + 1, 60 * SimulationConstants::getInstance()->steps_per_year }; // 熟年の寿命の乱数分布
        //std::uniform_int_distribution<> life_older_exp_dist{ 60 * SimulationConstants::getInstance()->steps_per_year + 1, 80 * SimulationConstants::getInstance()->steps_per_year }; // 老年の寿命の乱数分布

    public:

        paxs::JapanProvinces* japan_provinces;
        KanakumaLifeSpan() = default;

        ///// @brief 英語未翻訳
        ///// @brief 寿命を決定する
        //AgeType setAdultLifeSpan(const bool is_male_, std::mt19937& gen) {
        //    // もし大人だったら
        //    if (!is_male_) { // 女性の場合
        //        const AgeType adult_type = static_cast<AgeType>(life_male_adult_num(gen));

        //        if (adult_type <= 14) { // 成年
        //            return static_cast<AgeType>(life_adult_exp_dist(gen));
        //        }
        //        else if (adult_type <= (14 + 26)) { // 熟年
        //            return static_cast<AgeType>(life_mature_exp_dist(gen));
        //        }
        //        // 老年
        //        return static_cast<AgeType>(life_older_exp_dist(gen));
        //    }
        //    else { // 男性の場合
        //        const AgeType adult_type = static_cast<AgeType>(life_female_adult_num(gen));

        //        if (adult_type <= 19) { // 成年
        //            return static_cast<AgeType>(life_adult_exp_dist(gen));
        //        }
        //        else if (adult_type <= (19 + 19)) { // 熟年
        //            return static_cast<AgeType>(life_mature_exp_dist(gen));
        //        }
        //    }
        //    // 老年
        //    return static_cast<AgeType>(life_older_exp_dist(gen));
        //}

        /// @brief 英語未翻訳
        /// @brief 寿命を決定する
        AgeType setLifeSpan(const bool is_farming_, const bool is_male_, std::mt19937& gen) {

            return static_cast<AgeType>(japan_provinces->getLifeSpan(is_farming_, !is_male_, gen) * SimulationConstants::getInstance()->steps_per_year
                + SimulationConstants::getInstance()->step_per_year_dist(gen));

            //if (life_person_num(gen) <= 37) { // もし子供だったら
            //    const AgeType child_type = static_cast<AgeType>(life_child_num(gen));

            //    if (child_type <= 6) { // 乳児
            //        return static_cast<AgeType>(life_infant_exp_dist(gen));
            //    }
            //    else if (child_type <= (6 + 22)) { // 幼児
            //        return static_cast<AgeType>(life_toddler_exp_dist(gen));
            //    }
            //    else if (child_type <= (6 + 22 + 5)) { // 小児
            //        return static_cast<AgeType>(life_child_exp_dist(gen));
            //    }
            //    // 若年
            //    return static_cast<AgeType>(life_young_exp_dist(gen));
            //}
            //// もし大人だったら
            //return setAdultLifeSpan(is_male_, gen);
        }

    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_KANAKUMA_LIFE_SPAN_HPP
