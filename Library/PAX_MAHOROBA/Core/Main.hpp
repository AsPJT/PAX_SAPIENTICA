/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAIN_HPP
#define PAX_MAHOROBA_MAIN_HPP

#include <PAX_SAPIENTICA/Core/Platform.hpp>

#if defined(PAXS_USING_DXLIB) && defined(PAXS_PLATFORM_ANDROID)
// シミュレーションを使用しない
#else
#define PAXS_USING_SIMULATOR
#endif

#include <PAX_MAHOROBA/Core/Application.hpp>

namespace paxs {

/// @brief メイン関数
void startMain() {
    Application app;
    app.initialize();
    app.run();
}

} // namespace paxs

#endif // !PAX_MAHOROBA_MAIN_HPP
