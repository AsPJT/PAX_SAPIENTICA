/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_SIMULATION_CONST_HPP
#define PAX_SAPIENTICA_SIMULATION_SIMULATION_CONST_HPP

/*##########################################################################################

##########################################################################################*/

#include <cstdint>

namespace paxs {
    constexpr int pixel_size = 256; // 1画像あたりの縦横のピクセル数

    constexpr int unit_group_min = 11; // 単位集団の最小人数
    constexpr int unit_group_max = 40; // 単位集団の最大人数
    constexpr int basic_group_min = 41; // 基礎集団の最小人数
    constexpr int basic_group_max = 240; // 基礎集団の最大人数
    constexpr int composite_settlement_min = 82; // 複合集落の最小人数

    constexpr int steps_per_year = 12; // 1年あたりのステップ数

    // 結婚可能年齢：13歳以上60歳未満
    constexpr std::uint_least32_t marriageable_age_min = 13;
    constexpr std::uint_least32_t marriageable_age_max = 60;
    // 出産可能：15歳以上50歳未満
    constexpr std::uint_least32_t birthable_age_min = 15;
    constexpr std::uint_least32_t birthable_age_max = 50;
}

#endif // !PAX_SAPIENTICA_SIMULATION_SIMULATION_CONST_HPP
