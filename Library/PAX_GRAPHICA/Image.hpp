/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_IMAGE_HPP
#define PAX_GRAPHICA_IMAGE_HPP

#include <optional>
#include <vector>

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#include <PAX_GRAPHICA/String.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>

namespace paxg {

    struct Image
    {
#if defined(PAXS_USING_SIV3D)
        s3d::Image image;
        Image(const paxg::String& path) : image(path.string) {}
        constexpr operator s3d::Image() const { return image; }

        bool isValid() const { return !!image; }

#elif defined(PAXS_USING_SFML)
        sf::Image image;
        Image(const paxg::String& path) {
            if (!image.loadFromFile(path.string)) {
                PAXS_WARNING("Failed to load image: " + path.string);
            }
        }
        operator sf::Image() const { return image; }

        bool isValid() const { return image.getSize().x > 0 && image.getSize().y > 0; }

        // SFML 3.0.0: saveToMemory returns std::optional
        std::optional<std::vector<std::uint8_t>> saveToMemory(const std::string& format = "png") const {
            return image.saveToMemory(format);
        }

        // Save to file with error checking
        bool saveToFile(const std::string& filename) const {
            return image.saveToFile(filename);
        }

#else
        constexpr Image([[maybe_unused]] const paxg::String& path) {}
        constexpr bool isValid() const { return false; }
#endif

        // Common interface
        explicit operator bool() const { return isValid(); }
    };
}

#endif // !PAX_GRAPHICA_IMAGE_HPP
