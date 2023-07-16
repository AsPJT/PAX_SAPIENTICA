/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_COLOR_HPP
#define PAX_GRAPHICA_COLOR_HPP

/*##########################################################################################

##########################################################################################*/

#ifdef PAXS_USING_SIV3D
#include <Siv3D/Color.hpp>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics/Color.hpp>
#endif

namespace paxg {

    struct Color {
#ifdef PAXS_USING_SIV3D
        s3d::Color color;
        Color(int r, int g, int b, int a) : color(r, g, b, a) {}
        s3d::Color() const { return color; }
#elif defined(PAXS_USING_SFML)
        sf::Color color;
        Color(int r, int g, int b, int a) : color(r, g, b, a) {}
        operator sf::Color() const { return color; }
#else
        int r, g, b, a;
        Color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}
#endif
    };

}

#endif // !PAX_GRAPHICA_COLOR_HPP