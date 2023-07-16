/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_VEC2_HPP
#define PAX_GRAPHICA_VEC2_HPP

/*##########################################################################################

##########################################################################################*/

#ifdef PAXS_USING_SIV3D
#include <Siv3D/Vector2D.hpp>
#elif defined(PAXS_USING_SFML)
#include <SFML/System/Vector2.hpp>
#endif

namespace paxg {

    struct Vec2 {
#ifdef PAXS_USING_SIV3D
        s3d::Vec2 vec2;
        Vec2(int x, int y) : vec2(x, y) {}
        Vec2(const s3d::Vec2& vec2) : vec2(vec2) {}
        operator s3d::Vec2() const { return vec2; }
#elif defined(PAXS_USING_SFML)
        sf::Vector2i vec2;
        Vec2(int x, int y) : vec2(x, y) {}
        Vec2(const sf::Vector2i& vec2) : vec2(vec2) {}
        operator sf::Vector2i() const { return vec2; }
#else
        int x, y;
        Vec2(int x, int y) : x(x), y(y) {}
#endif
    };
}

#endif // !PAX_GRAPHICA_VEC2_HPP