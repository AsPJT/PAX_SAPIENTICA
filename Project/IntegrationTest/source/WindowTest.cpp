/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#define PAXS_USING_SFML

#include <PAX_GRAPHICA/Graphics.hpp>

int main() {
    paxg::Rect rect(0, 0, 100, 100);
    paxg::Window::Init(800, 600, "PAX SAPIENTICA Library");
    while (paxg::Window::update()) {
        paxg::Window::clear();
        paxg::Window::drawRect(rect);
        paxg::Window::display();
    }
}
