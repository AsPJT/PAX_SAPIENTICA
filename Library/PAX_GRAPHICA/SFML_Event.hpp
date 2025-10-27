/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SFML_EVENT_HPP
#define PAX_GRAPHICA_SFML_EVENT_HPP

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
        static SFML_Event instance;

        SFML_Event() = default;
        ~SFML_Event() = default;

        // Delete copy constructor and assignment operator
        SFML_Event(const SFML_Event&) = delete;
        SFML_Event& operator=(const SFML_Event&) = delete;

    public:
        // インスタンスを取得
        static SFML_Event* getInstance() {
            return &instance;
        }

#if defined(PAXS_USING_SFML)
        float wheel_delta = 0.0f;

        bool update(sf::RenderWindow& window) {
            wheel_delta = 0.0f;

            // SFML 3.0.0 の新しいポーリングメカニズム
            while (const auto& poll_event = window.pollEvent()) {
                if (poll_event->is<sf::Event::Closed>()) {
                    return false; // 終了シグナル
                }
                else if (const auto* mouseWheelScrolled = poll_event->getIf<sf::Event::MouseWheelScrolled>()) {
                    wheel_delta = -(mouseWheelScrolled->delta);
                }
                else if (const auto* resized = poll_event->getIf<sf::Event::Resized>()) {
                    // sf::FloatRect は position と size で構築（SFML 3.0 の新しい構造）
                    sf::FloatRect visibleAreaUpdate(
                        {0.f, 0.f},
                        {static_cast<float>(resized->size.x), static_cast<float>(resized->size.y)}
                    );
                    window.setView(sf::View(visibleAreaUpdate));
                }
            }
            return true;
        }

        // SFML 3.0.0 の handleEvents() サポート
        template<typename... EventHandlers>
        bool handleEvents(sf::RenderWindow& window, EventHandlers&&... handlers) {
            bool shouldContinue = true;
            wheel_delta = 0.0f;

            window.handleEvents([&](const sf::Event& event) {
                // クローズイベントの処理
                if (event.is<sf::Event::Closed>()) {
                    shouldContinue = false;
                    return;
                }

                // マウスホイールの処理
                if (const auto* mouseWheelScrolled = event.getIf<sf::Event::MouseWheelScrolled>()) {
                    wheel_delta = -(mouseWheelScrolled->delta);
                }

                // リサイズの処理
                if (const auto* resized = event.getIf<sf::Event::Resized>()) {
                    sf::FloatRect visibleAreaUpdate(
                        {0.f, 0.f},
                        {static_cast<float>(resized->size.x), static_cast<float>(resized->size.y)}
                    );
                    window.setView(sf::View(visibleAreaUpdate));
                }

                // ユーザー定義のハンドラを実行
                (handlers(event), ...);
            });

            return shouldContinue;
        }
#endif
    };

    // Static member definition
    SFML_Event SFML_Event::instance;

}

#endif // !PAX_GRAPHICA_SFML_EVENT_HPP
