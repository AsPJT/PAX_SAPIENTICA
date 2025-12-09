/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_TRIANGLE_HPP
#define PAX_GRAPHICA_TRIANGLE_HPP

#if defined(PAXS_USING_SIV3D)
#include <PAX_GRAPHICA/Siv3D/Triangle.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <PAX_GRAPHICA/DxLib/Triangle.hpp>
#elif defined(PAXS_USING_SFML)
#include <PAX_GRAPHICA/SFML/Triangle.hpp>
#else
#include <PAX_GRAPHICA/Null/Triangle.hpp>
#endif

#endif // PAX_GRAPHICA_TRIANGLE_HPP
