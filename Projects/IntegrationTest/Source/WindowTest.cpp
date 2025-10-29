/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <PAX_GRAPHICA/Circle.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/String.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Window.hpp>

int main() {
    paxg::Rect rect(0, 0, 100, 100);
    // paxg::Circle circle(100, 100, 50);
    paxg::String path("../data/sample.png");
    paxg::Texture texture(path);
    paxg::Window::Init(800, 600, "PAX SAPIENTICA Library");
    while (paxg::Window::update()) {
        paxg::Window::clear();
        rect.draw();
        // circle.draw();
        // texture.drawAt({200, 200});
        paxg::Window::display();
    }
}
