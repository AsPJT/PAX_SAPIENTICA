/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <SFML/Graphics.hpp>

int main()
{
    // ウィンドウの作成
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML window");

    // 四角形の作成
    sf::RectangleShape rectangle(sf::Vector2f(100.f, 100.f));
    rectangle.setFillColor(sf::Color::Red);
    rectangle.setPosition({ 350.f, 250.f });

    // ゲームループ
    while (const auto& event_ = window.pollEvent())
    {
        // イベントの処理
        if (event_->is<sf::Event::Closed>())
        {
            window.close();
        }

        // 描画
        window.clear();
        window.draw(rectangle);
        window.display();
    }

    return 0;
}
