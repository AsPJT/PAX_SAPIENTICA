/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_CIRCLE_HPP
#define PAX_GRAPHICA_CIRCLE_HPP

#if defined(PAXS_USING_SIV3D)
#include <PAX_GRAPHICA/Siv3D/Circle.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <PAX_GRAPHICA/DxLib/Circle.hpp>
#elif defined(PAXS_USING_SFML)
#include <PAX_GRAPHICA/SFML/Circle.hpp>
#else
#include <PAX_GRAPHICA/Null/Circle.hpp>
#endif

#endif // !PAX_GRAPHICA_CIRCLE_HPP
