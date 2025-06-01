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
    public:
        // インスタンスを取得
        static SFML_Event* getInstance() {
            if (instance == nullptr) {
                instance = new SFML_Event();
            }
            return instance;
        }

#if defined(PAXS_USING_SFML)
        float wheel_delta = 0.0f;
        bool update(sf::RenderWindow& window) {
            wheel_delta = 0.0f;
            sf::FloatRect visibleArea;

            // 新しいポーリングメカニズム
            while (const auto& poll_event = window.pollEvent()) {
                if (poll_event->is<sf::Event::Closed>()) return false; // 終了シグナル
                else if (const auto* mouseWheelScrolled = poll_event->getIf<sf::Event::MouseWheelScrolled>()) {
                    wheel_delta = -(mouseWheelScrolled->delta); // 特定のイベント構造体からdeltaにアクセス
                }
                else if (const auto* resized = poll_event->getIf<sf::Event::Resized>()) {
                    // resized->size (sf::Vector2u型) から新しい幅と高さをアクセス
                    // sf::FloatRectのコンストラクタは {位置}, {サイズ} を取る
                    sf::FloatRect visibleAreaUpdate({ 0.f, 0.f },
                        { static_cast<float>(resized->size.x), static_cast<float>(resized->size.y) });
                    // sf::View(sf::FloatRect) コンストラクタは引き続き利用可能
                    window.setView(sf::View(visibleAreaUpdate));
                }
            }
            return true;
        }
#endif

    private:
        static SFML_Event* instance;
        SFML_Event() = default;
        ~SFML_Event() {
            delete instance;
        }

    };

    SFML_Event* SFML_Event::instance = nullptr;

}

#endif // !PAX_GRAPHICA_SFML_EVENT_HPP
