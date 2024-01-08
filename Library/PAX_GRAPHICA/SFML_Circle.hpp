/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SFML_CIRCLE_HPP
#define PAX_GRAPHICA_SFML_CIRCLE_HPP

/*##########################################################################################

##########################################################################################*/

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/IDrawable.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxg {

    // 実行時定数
    class SFML_Circle {
    public:
#if defined(PAXS_USING_SFML)
        sf::CircleShape circle;
#endif
    public:
        // インスタンスを取得
        static SFML_Circle* getInstance() {
            if (instance == nullptr) {
                instance = new SFML_Circle();
            }
            return instance;
        }

#if defined(PAXS_USING_SFML)

#endif

    private:
        static SFML_Circle* instance;

#if defined(PAXS_USING_SFML)
        SFML_Circle() {

        }
#else
        SFML_Circle() = default;
#endif

        ~SFML_Circle() {
            delete instance;
        }

    };

    SFML_Circle* SFML_Circle::instance = nullptr;

}

#endif // !PAX_GRAPHICA_SFML_CIRCLE_HPP
