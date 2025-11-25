/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SFML_CIRCLE_HPP
#define PAX_GRAPHICA_SFML_CIRCLE_HPP

#ifdef PAXS_USING_SFML
#include <SFML/Graphics.hpp>
#endif

namespace paxg {

    // 実行時定数
    class SFML_Circle {
    private:
        static SFML_Circle instance;

#if defined(PAXS_USING_SFML)
        SFML_Circle() {}
#else
        SFML_Circle() = default;
#endif
        ~SFML_Circle() = default;

    public:
        // Delete copy and move operations
        SFML_Circle(const SFML_Circle&) = delete;
        SFML_Circle& operator=(const SFML_Circle&) = delete;
        SFML_Circle(SFML_Circle&&) = delete;
        SFML_Circle& operator=(SFML_Circle&&) = delete;

#if defined(PAXS_USING_SFML)
        sf::CircleShape circle;
#endif

        // インスタンスを取得
        static SFML_Circle* getInstance() {
            return &instance;
        }
    };

    // Static member definition
    SFML_Circle SFML_Circle::instance;

}

#endif // !PAX_GRAPHICA_SFML_CIRCLE_HPP
