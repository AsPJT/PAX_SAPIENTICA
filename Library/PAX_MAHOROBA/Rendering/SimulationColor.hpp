/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_RENDERING_SIMULATION_COLOR_HPP
#define PAX_MAHOROBA_RENDERING_SIMULATION_COLOR_HPP

#include <array>

#include <PAX_GRAPHICA/Color.hpp>

namespace paxs {
    namespace SimulationColor {
        constexpr std::array settlement_colors = {
            paxg::Color( 45,  87, 154), // 青
            paxg::Color( 36,  92, 156),
            paxg::Color( 27,  96, 158),
            paxg::Color( 18, 101, 161),
            paxg::Color(  9, 105, 163),
            paxg::Color(  0, 110, 165),
            paxg::Color(  0, 111, 164),
            paxg::Color(  0, 112, 163),
            paxg::Color(  0, 112, 161),
            paxg::Color(  0, 113, 160),
            paxg::Color(  0, 114, 159),
            paxg::Color(  0, 115, 158),
            paxg::Color(  0, 117, 157),
            paxg::Color(  0, 118, 156),
            paxg::Color(  0, 120, 155),
            paxg::Color(  0, 121, 154),
            paxg::Color(  0, 125, 148),
            paxg::Color(  0, 130, 143),
            paxg::Color(  0, 134, 137),
            paxg::Color(  0, 139, 132),
            paxg::Color(  0, 143, 126),
            paxg::Color(  0, 141, 115),
            paxg::Color(  0, 138, 104),
            paxg::Color(  0, 136,  94),
            paxg::Color(  0, 133,  83),
            paxg::Color(  0, 131,  72), // 緑
            paxg::Color(  6, 136,  60),
            paxg::Color( 13, 141,  48),
            paxg::Color( 19, 145,  37),
            paxg::Color( 26, 150,  25),
            paxg::Color( 32, 155,  13),
            paxg::Color( 55, 159,  16),
            paxg::Color( 78, 164,  19),
            paxg::Color(101, 168,  23),
            paxg::Color(124, 173,  26),
            paxg::Color(147, 177,  29),
            paxg::Color(156, 182,  26),
            paxg::Color(166, 187,  24),
            paxg::Color(175, 193,  21),
            paxg::Color(185, 198,  19),
            paxg::Color(194, 203,  16),
            paxg::Color(201, 204,  13),
            paxg::Color(208, 205,  10),
            paxg::Color(216, 205,   6),
            paxg::Color(223, 206,   3),
            paxg::Color(230, 207,   0), // 黄
            paxg::Color(228, 202,   1),
            paxg::Color(226, 197,   2),
            paxg::Color(225, 191,   3),
            paxg::Color(221, 181,   5),
            paxg::Color(223, 186,   4),
            paxg::Color(219, 176,   6),
            paxg::Color(218, 171,   7),
            paxg::Color(216, 165,   8),
            paxg::Color(215, 160,   9),
            paxg::Color(213, 155,  10),
            paxg::Color(210, 138,  12),
            paxg::Color(206, 122,  15),
            paxg::Color(203, 105,  17),
            paxg::Color(199,  89,  20),
            paxg::Color(196,  72,  22),
            paxg::Color(193,  64,  28),
            paxg::Color(189,  56,  34),
            paxg::Color(186,  48,  40),
            paxg::Color(182,  40,  46), // 赤
            paxg::Color(179,  32,  52),
            paxg::Color(178,  31,  57),
            paxg::Color(176,  30,  62),
            paxg::Color(175,  29,  68),
            paxg::Color(173,  28,  73),
            paxg::Color(172,  27,  78),
            paxg::Color(170,  26,  83),
            paxg::Color(169,  25,  88),
            paxg::Color(167,  24,  92),
            paxg::Color(166,  23,  97),
            paxg::Color(164,  22, 102),
            paxg::Color(131,  18,  82),
            paxg::Color( 98,  13,  61),
            paxg::Color( 66,   9,  41),
            paxg::Color( 33,   4,  20),
            paxg::Color(  0,   0,   0),
            paxg::Color(255, 255, 255) // 無効
        };

        /// @brief 集落の色を取得
        /// @param index 集落のインデックス
        /// @return 色
        constexpr paxg::Color getSettlementColor(const std::size_t index) {
            if (index >= settlement_colors.size()) {
                return settlement_colors.back();
            }
            return settlement_colors[index];
        }
    }

}

#endif // PAX_MAHOROBA_RENDERING_SIMULATION_COLOR_HPP
