#include <SFML/Graphics.hpp>

int main()
{
    // ウィンドウの作成
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    // 四角形の作成
    sf::RectangleShape rectangle(sf::Vector2f(100.f, 100.f));
    rectangle.setFillColor(sf::Color::Red);
    rectangle.setPosition(350.f, 250.f);

    // ゲームループ
    while (window.isOpen())
    {
        // イベントの処理
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 描画
        window.clear();
        window.draw(rectangle);
        window.display();
    }

    return 0;
}
