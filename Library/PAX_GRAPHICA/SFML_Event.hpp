/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SFML_EVENT_HPP
#define PAX_GRAPHICA_SFML_EVENT_HPP

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
    class SFML_Event {
    private:
#if defined(PAXS_USING_SFML)
    sf::Event ev;
#endif
    public:
        // インスタンスを取得
        static SFML_Event* getInstance() {
            if (instance == nullptr) {
                instance = new SFML_Event();
            }
            return instance;
        }

#if defined(PAXS_USING_SFML)
        float wheel_delta = 0;
        bool update(sf::RenderWindow& window) {
            wheel_delta = 0;
            while (window.pollEvent(ev))
            {
                switch (ev.type) {
                case sf::Event::Closed:
                    return false;
                case sf::Event::MouseWheelMoved:
                    wheel_delta = ev.mouseWheel.delta;
                    break;
                case sf::Event::Resized:
                    sf::FloatRect visibleArea(0, 0, ev.size.width, ev.size.height);
                    window.setView(sf::View(visibleArea));
                    break;
                }
            }
            return true;
        }
        sf::Event& getEvent() {
            return ev;
        }
#endif

    private:
        static SFML_Event* instance;

#if defined(PAXS_USING_SFML)
        SFML_Event() {

        }
#else
        SFML_Event() = default;
#endif

        ~SFML_Event() {
            delete instance;
        }

    };

    SFML_Event* SFML_Event::instance = nullptr;

}

#endif // !PAX_GRAPHICA_SFML_EVENT_HPP
