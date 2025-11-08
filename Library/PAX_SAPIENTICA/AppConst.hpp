/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_APP_CONST_HPP
#define PAX_SAPIENTICA_APP_CONST_HPP

#include <PAX_SAPIENTICA/Type/Vector2.hpp>

namespace paxs {
    namespace AppConst {
        // 画面の最小サイズ
        constexpr paxs::Vector2<int> min_window_size{ 1280, 720 };
        // デフォルトの画面サイズ
        constexpr paxs::Vector2<int> default_window_size{ 1280, 720 };
        // Full HD
        constexpr paxs::Vector2<int> full_hd_window_size{ 1920, 1080 };
        // 画面の最大サイズ
        constexpr paxs::Vector2<int> max_window_size{ 3840, 2160 };
    }
}

#endif // PAX_SAPIENTICA_APP_CONST_HPP
