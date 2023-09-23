/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_IDRAWABLE_HPP
#define PAX_GRAPHICA_IDRAWABLE_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_GRAPHICA/Vec2.hpp>

namespace paxg {

    struct IDrawable {

        virtual ~IDrawable() {}

        virtual void draw() const = 0;

        virtual void drawAt(const Vec2f& pos) const = 0;
        virtual void drawAt(const Vec2i& pos) const = 0;
    };

}

#endif // !PAX_GRAPHICA_IDRAWABLE_HPP
