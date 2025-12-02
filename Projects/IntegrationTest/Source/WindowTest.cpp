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
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_SAPIENTICA/Utility/LoggerIntegration.hpp>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <libgen.h>
#include <unistd.h>
#endif

int main() {
#ifdef __APPLE__
    // macOS: 実行ファイルのディレクトリにカレントディレクトリを変更
    char exe_path[1024];
    uint32_t size = sizeof(exe_path);
    if (_NSGetExecutablePath(exe_path, &size) == 0) {
        // 実行ファイルのディレクトリを取得
        char* dir = dirname(exe_path);
        chdir(dir);
    }
#endif

    paxg::Rect rect(0, 0, 100, 100);
    paxg::Circle circle(100, 100, 50);
    paxg::String path("Projects/IntegrationTest/Data/sample.png");
    paxg::Texture texture(path);
    paxg::Window::Init(800, 600, "PAX SAPIENTICA Library");
    while (paxg::Window::update()) {
        rect.draw();
        circle.draw();
        texture.drawAt(paxg::Vec2i(200, 200));
    }
}
