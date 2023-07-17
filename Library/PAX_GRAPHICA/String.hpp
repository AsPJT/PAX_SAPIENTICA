/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_STRING_HPP
#define PAX_GRAPHICA_STRING_HPP

/*##########################################################################################

##########################################################################################*/

#ifdef PAXS_USING_SIV3D
#include <Siv3D.hpp>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

namespace paxg {

    struct String
    {
#ifdef PAXS_USING_SIV3D
        s3d::String string;
        String(const s3d::String& string) : string(string) {}
        String(const std::string& string) : string(string.c_str()) {}
        operator s3d::String() const { return string; }
// #elif defined(PAXS_USING_SFML)
//         sf::String string;
//         String(const sf::String& string) : string(string) {}
//         String(const std::string& string) : string(string.c_str()) {}
//         operator sf::String() const { return string; }
#else
        std::string string;
        String(const std::string& string) : string(string) {}
        operator std::string() const { return string; }
#endif
    };
}

#endif // !PAX_GRAPHICA_STRING_HPP