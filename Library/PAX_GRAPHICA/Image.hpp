/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_IMAGE_HPP
#define PAX_GRAPHICA_IMAGE_HPP

/*##########################################################################################

##########################################################################################*/

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#endif

#include <PAX_GRAPHICA/String.hpp>

namespace paxg {

    struct Image
    {
#if defined(PAXS_USING_SIV3D)
        s3d::Image image;
        Image(const paxg::String& path) : image(path.string) {}
        constexpr operator s3d::Image() const { return image; }
#elif defined(PAXS_USING_SFML)
        sf::Image image;
        Image(const paxg::String& path) { image.loadFromFile(path); }
        operator sf::Image() const { return image; }
#else
        constexpr Image([[maybe_unused]] const paxg::String& path) {}
#endif
    };
}

#endif // !PAX_GRAPHICA_IMAGE_HPP
